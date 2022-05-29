#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdarg>
#include <iterator>
#include <string>
#include <regex>
#include <numeric>
#include <cmath>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstring>

#include "../gradinglib/gradinglib.hpp"
#include "../stats_functions.hpp"
#include <limits>

namespace tdgrading {

using namespace testlib;

/**************************** Tests - DO NOT TOUCH! ****************************/

const bool defaultPrint = true;
const std::string defaultPath = "./tests/";
const double defaultEps = 0.001;

double hash (double vector[], int length) 
{
    double sum  = 0;
    for (int i = 0; i < length; i++)
        sum += vector[i];
    return sum;
} 

double **unpack (double values[], std::vector<int> &scenario, int size, int rows, int columns) 
{
    double **matrix = new double*[size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
        if (i < rows) {
            for (int j = 0; j < columns; j++)
                matrix[i][j] = values[i*columns + j];
            for (int j = columns; j < size; j++)
                matrix[i][j] = 1;
        } else {
            for (int j = 0; j < size; j++)
                matrix[i][j] = 1;
        }
    }

    return matrix;
}

void cleanup (double **matrix, int rows)
{
    for (int i = 0; i < rows; i++)
        delete[] matrix[i];
    delete matrix;
}

double **matrixFromArray (double array[], int rows, int columns) 
{
    double **matrix = new double*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new double[columns];
        for (int j = 0; j < columns; j++)
            matrix[i][j] = array[i*columns + j];
    }
    return matrix;
}

std::ostream &printArray (std::ostream &str, double values[], int length, std::string prefix = "") {
  str << prefix << "Length: " << length << std::endl;

  str << prefix;
  for (int i = 0; i < length; i++)
    str << values[i] << " ";

  return str << std::endl;
}

void loadScenarii (std::ifstream &is, int nb_scenarii, int size_scenario, 
                  std::vector<std::vector<int> > &scenarii, std::vector<double> &results) {
  assert(is.is_open());

  for (int i = 0; i < nb_scenarii; i++) {
    for (int j = 0; j < size_scenario; j++) 
      is >> scenarii[i][j];

    is >> results[i];
  }
} 

void loadArray (std::ifstream &is, int length, int max, double values[]) {
  assert(is.is_open());
  for (int i = 0; i < length; i++) {
    is >> values[i]; 
  }
  for (int i = length; i < max; i++) {
    values[i] = 0;
  }
}

bool loadTests(std::string filename, int &nb_scenarii, int &size_scenario,
               std::vector<std::vector<int> > &scenarii,
               std::vector<double> &results, int &length, double *&values) {
  std::ifstream is(filename);
  if (!is.is_open())
    return false;

  int shift_index;
  is >> nb_scenarii >> size_scenario >> shift_index;

  scenarii.clear();
  scenarii.resize(nb_scenarii);
  for (int i = 0; i < nb_scenarii; i++)
    scenarii[i].resize(size_scenario);

  results.clear();
  results.resize(nb_scenarii);
  loadScenarii(is, nb_scenarii, size_scenario, scenarii, results);

  int maxShift = 0;
  if (shift_index >= 0) 
    for (int i = 0; i < nb_scenarii; i++)
      if (scenarii[i][shift_index] > maxShift)
        maxShift = scenarii[i][shift_index];

  is >> length;
  values = new double [length + maxShift];
  loadArray(is, length, length + maxShift, values);

  is.close();
  return true;
}

const int lengthIndex = 0;

bool testScenarii (double (*func)(double[], std::vector<int>&), std::string &filename, double eps, bool print, std::ostream &out = std::cout) {
  int nb_scenarii;
  int size_scenario;
  std::vector<double> results;
  int length;
  double *values;
  std::vector<std::vector<int> > scenarii;
  
  if (!loadTests(filename, nb_scenarii, size_scenario, scenarii, results, length, values)) {
    out << "\tProblem loading tests from the file " << filename << std::endl;
    return false;
  }

  if (print) {
    out << "\tRead " << nb_scenarii << " scenarii of size " << size_scenario 
              << " from the file " << filename << std::endl
              << "\tWill test on the following array" << std::endl;
    printArray(out, values, length, "\t");
  }

  bool found = false;
  for (int i = 0; i < nb_scenarii; i++) {
    double stat = func(values, scenarii[i]);
    if (fabs(stat - results[i]) < eps) {
      if (print) {
        out << std::endl << "\tFound scenario ( ";
        for (int j = 0; j < size_scenario; j++)
          out << scenarii[i][j] << " ";

        out << ") matching the computed value " << stat
                  << std::endl;
      }
      
      found = true;
      break;
    }
  }

  delete[] values;
  return found;
}

bool testScenariiMsg (double (*func)(double[], std::vector<int>&), std::string scenarii[], int nb, bool needAll, 
                      std::string &path, double eps, bool print, std::ostream &out = std::cout) {
  bool found = false;
  bool all = true;

  if (print)
    out << std::endl << "\tWill test on " << (nb/2) << " scenario file(s) now..." << std::endl;

  // If one found, keep looking only if need to satisfy all
  for (int i = 0; (!found || needAll) && i < nb; i += 2) {
    std::string filename = path + scenarii[i];
    std::string message = scenarii[i+1];

    if (print)
      out << std::endl;

    found = testScenarii(func, filename, eps, print, out);
    all = all && found;
    if (found && print && !message.empty())
        std::cerr << "\t" << message << std::endl;
  }

  return needAll ? all : found;
}

bool testFunction (double (*func)(double[], std::vector<int>&),
                   std::string bad[], int nbBad, 
                   std::string good[], int nbGood,
                   std::string &path, double eps, bool print,
		   std::ostream &out = std::cout) 
{
  if (print)
    out << std::endl;

  // For the sake of clarity in the calls below
  bool needAll = true; 
  bool needOne = false;

  if (testScenariiMsg(func, bad, nbBad, needOne, path, eps, print, out)) {
    return false;
  }

  if (testScenariiMsg(func, good, nbGood, needAll, path, eps, print, out)) {
    return true;
  }

  out << "\tYou have either made a mistake we haven't thought of or several mistakes." << std::endl;
  return false;
}


/**************************** Concrete tests ****************************/



double mockComputeMean (double values[], std::vector<int> &scenario) {
  return computeMean(values, scenario[0]);
}

bool testComputeMean (std::string path, double eps, bool print, std::ostream &out = std::cout) {
  out << "Testing the function computeMean()...\t\t\t\t";

  std::string bad[] = {
    "computeMeanShift.txt", "Check the loop bounds: the indices of an array of size n go from 0 to n-1!",
    "computeMeanLength.txt", "Check the divisor: is it equal to the number of values?",
    "computeMeanOW.txt", "Check the accumulator in the loop: did you add values or did you overwrite it?"
  };
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeMean0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeMean, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

double mockComputeVariance (double values[], std::vector<int> &scenario) {
  return computeVariance(values, scenario[0]);
}

bool testComputeVariance (std::string path, double eps, bool print, std::ostream &out = std::cout) {
  out << "Testing the function computeVariance()...\t\t\t";

  std::string bad[] = {
    "computeVarianceShift1.txt", "Check the loop bounds: the indices of an array of size n go from 0 to n-1!",
    "computeVarianceShift2.txt", "Check the loop bounds: the indices of an array of size n go from 0 to n-1!",
    "computeVarianceLength1.txt", "Check the divisor: is it equal to the number of values?",
    "computeVarianceLength2.txt", "Check the divisor: is it equal to the number of values?",
    "computeVarianceOW1.txt", "Check the accumulator in the loop: did you add values or did you overwrite it?",
    "computeVarianceOW2.txt", "Check the accumulator in the loop: did you add values or did you overwrite it?"
  };
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeVariance01.txt", "",
    "computeVariance02.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeVariance, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

double mockComputeSampleVariance (double values[], std::vector<int> &scenario) {
  return computeSampleVariance(values, scenario[0]);
}

bool testComputeSampleVariance (std::string path, double eps, bool print, std::ostream &out = std::cout) {
  out << "Testing the function computeSampleVariance()...\t\t\t";

  std::string bad[] = {
    "computeSampleVariance1.txt", "Check the denominator in the definition of the (unbiased) sample variance",
  };
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeSampleVariance0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeSampleVariance, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

double mockComputeStandardDeviation (double values[], std::vector<int> &scenario) {
  return computeStandardDeviation(values, scenario[0]);
}

bool testComputeStandardDeviation (std::string path, double eps, bool print, std::ostream &out = std::cout) {
  out << "Testing the function computeStandardDeviation()...\t\t";

  std::string bad[] = {
    "computeStandardDeviation1.txt", "Did you take the square root correctly?",
    "computeStandardDeviation2.txt", "Did you use pow(..., 1/2) to take the square root? If so, recall that integer division gives 1/2 = 0.",
  };
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeStandardDeviation0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeStandardDeviation, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

double mockComputeSampleStandardDeviation (double values[], std::vector<int> &scenario) {
  return computeSampleStandardDeviation(values, scenario[0]);
}

bool testComputeSampleStandardDeviation (std::string path, double eps, bool print, std::ostream &out = std::cout) {
  out << "Testing the function computeSampleStandardDeviation()...\t";

  std::string bad[] = {
    "computeSampleStandardDeviation1.txt", "Did you take the square root correctly?",
    "computeStandardDeviation2.txt", "Did you use pow(..., 1/2) to take the square root? If so, recall that integer division gives 1/2 = 0.",
  };
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeSampleStandardDeviation0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeSampleStandardDeviation, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

int findSize (double values[], int length, double &eps) {
    for (int i = length - 1; i >= 0; i--)
        if (fabs(values[i]) > eps)
            return i + 1;

    return 0;
}

double mockGetRow (double values[], std::vector<int> &scenario) {
    int rows = scenario[0];
    int columns = scenario[1];
    int size = scenario[2];
    int dStart = scenario[3];
    int dEnd = scenario[4];
    double eps = 1.0/pow(10, scenario[5]);

    double **matrix = unpack(values, scenario, size, rows, columns);
    double row[size];

    double mHash = 1;
    for (int i = 0; i < rows; i++) {
        memset(row, 0, size*sizeof(double));
        getRow(matrix, columns, i, row);
        mHash *= hash(row, findSize(row, size, eps));
    }

    cleanup(matrix, size);
        
    return mHash;
}

double mockGetColumn (double values[], std::vector<int> &scenario) {
    int rows = scenario[0];
    int columns = scenario[1];
    int size = scenario[2];
    int dStart = scenario[3];
    int dEnd = scenario[4];
    double eps = 1.0/pow(10, scenario[5]);

    double **matrix = unpack(values, scenario, size, rows, columns);
    double column[size];

    double mHash = 1;
    for (int i = 0; i < columns; i++) {
        memset(column, 0, size*sizeof(double));
        getColumn(matrix, rows, i, column);
        mHash *= hash(column, findSize(column, size, eps));
    }

    cleanup(matrix, size);
        
    return mHash;
}

bool testGetRow(std::string path, double eps, bool print, std::ostream &out = std::cout) {
  out << "Testing the function getRow()...\t\t\t\t";

  std::string bad[] = {
    "getRowShift.txt", "Check the loop bounds: the indices of an array of size n go from 0 to n-1!",
    "getRowSwitch.txt", "Check the order of indices: did you invert columns and rows?",
    "getRowErase.txt", "Did you use the 'index' parameter on the wrong side of the assignment?",
};
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "getRow0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockGetRow, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

bool testGetColumn(std::string path, double eps, bool print, std::ostream &out = std::cout) {
  out << "Testing the function getColumn()...\t\t\t\t";

  std::string bad[] = {
    "getColumnShift.txt", "Check the loop bounds: the indices of an array of size n go from 0 to n-1!",
    "getColumnSwitch.txt", "Check the order of indices: did you invert columns and rows?",
    "getColumnErase.txt", "Did you use the 'index' parameter on the wrong side of the assignment?",
  };
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "getColumn0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockGetColumn, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

double mockComputeCovariance (double values[], std::vector<int> &scenario) {
  int length = scenario[0];
  return computeCovariance(values, values + length, length);
}

double mockComputeCorrelation (double values[], std::vector<int> &scenario) {
  int length = scenario[0];
  return computeCorrelation(values, values + length, length);
}

bool testComputeCovariance(std::string path, double eps, bool print, std::ostream &out = std::cout)
{
  out << "Testing the function computeCovariance()...\t";

  std::string bad[] = {
    "computeCovarianceShift.txt", "Check the loop bounds: the indices of an array of size n go from 0 to n-1!",
    "computeCovarianceLength.txt", "Check the divisor: is it equal to the number of values?",
    "computeCovarianceOW.txt", "Check the accumulator in the loop: did you add values or did you overwrite it?",
};
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeCovariance0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeCovariance, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

bool testComputeCorrelation(std::string path, double eps, bool print, std::ostream &out = std::cout){
  out << "Testing the function computeCorrelation()...\t";

  std::string bad[] = {
    "computeCorrelation1.txt", "Did you devide by both standard deviations? Check the / and * operations and parentheses.",
  };
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeCorrelation0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeCorrelation, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

int findSizeArray (double values[], int length, double &eps) {
    for (int i = length - 1; i >= 0; i--)
        if (fabs(values[i]) > eps)
            return i + 1;

    return 0;
}

int findSize (double **matrix, int side, double &eps) {
    int sizeH = 0;
    int sizeV = 0;
    
    for (int i = 0; i < side; i++)
        for (int j = side - 1; j >= 0; j--)
            if (fabs(matrix[i][j]) > eps) {
                sizeH = (sizeH > j + 1) ? sizeH : j + 1;
                break;
            }

    for (int i = 0; i < side; i++)
        for (int j = side - 1; j >= 0; j--)
            if (fabs(matrix[j][i]) > eps) {
                sizeV = (sizeV > j + 1) ? sizeV : j + 1;
                break;
            }

    return sizeH * (side + 1) + sizeV;
}

double mockComputeCovarianceMatrix (double values[], std::vector<int> &scenario) {
    int rows = scenario[0];
    int columns = scenario[1];
    int bound1 = scenario[2];
    int bound2 = scenario[3];
    double eps = 1.0/pow(10, scenario[7]);

    int maxBound = (bound1 > bound2) ? bound1 : bound2;
    int max = (rows > columns ? rows : columns);
    max = (max > maxBound) ? max : maxBound;

    double **matrix = unpack(values, scenario, max, rows, columns);

    for (int i = 0; i < max; i++)
      for (int j = 0; j < max; j++)
        if (i >= rows || j >= columns)
          matrix[i][j] = i + j;

    double **computed = computeCovarianceMatrix(matrix, rows, columns);

    // int temp = findSize(computed, columns, eps);
    // int trueRows = temp % (columns + 1);
    // int trueColumns = temp / (columns + 1);
    int trueRows = columns;
    int trueColumns = columns;

    double prod = 1;
    for (int i = 0; i < trueRows; i++) {
        double sum = 0;
        for (int j = 0; j < trueColumns; j++)
            sum += computed[i][j];
        if (fabs(sum) > eps)
            prod *= sum;
    }

    cleanup(matrix, max);
    cleanup(computed, columns);

    return prod;
}

double mockComputeCorrelationMatrix (double values[], std::vector<int> &scenario) {
    int rows = scenario[0];
    int columns = scenario[1];
    int bound1 = scenario[2];
    int bound2 = scenario[3];
    double eps = 1.0/pow(10, scenario[7]);

    int maxBound = (bound1 > bound2) ? bound1 : bound2;
    int max = (rows > columns ? rows : columns);
    max = (max > maxBound) ? max : maxBound;

    double **matrix = unpack(values, scenario, max, rows, columns);

    for (int i = 0; i < max; i++)
      for (int j = 0; j < max; j++)
        if (i >= rows || j >= columns)
          matrix[i][j] = i + j;

    double **computed = computeCorrelationMatrix(matrix, rows, columns);

    int temp = findSize(computed, columns, eps);
    int trueRows = temp % (columns + 1);
    int trueColumns = temp / (columns + 1);

    double prod = 1;
    for (int i = 0; i < trueRows; i++) {
        double sum = 0;
        for (int j = 0; j < trueColumns; j++)
            sum += computed[i][j];
        if (sum != 0)
            prod *= sum;
    }

    cleanup(matrix, max);
    cleanup(computed, columns);

    return prod;
}

bool testComputeCovarianceMatrix(std::string path, double eps, bool print, std::ostream &out = std::cout){
  out << "Testing the function computeCovarianceMatrix()...\t\t\t\t";

  std::string bad[] = {
    "computeCovarianceMatrixShift.txt", "Check the loop bounds: the indices of an array of size n go from 0 to n-1!",
    "computeCovarianceMatrixColumn.txt", "Do you pass the right length to getColumn()?",
    "computeCovarianceMatrixFlip.txt", "Did you forget to fill out the symmetric part of the covariance matrix?",
    "computeCovarianceMatrixDiagonal.txt", "Did you set the values at the diagonal to 1 instead of the variance?",
};
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeCovarianceMatrix0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeCovarianceMatrix, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

bool testComputeCorrelationMatrix(std::string path, double eps, bool print, std::ostream &out = std::cout){
  out << "Testing the function computeCorrelationMatrix()...\t\t\t\t";

  std::string bad[] = {
    "computeCorrelationMatrixShift.txt", "Check the loop bounds: the indices of an array of size n go from 0 to n-1!",
    "computeCorrelationMatrixColumn.txt", "Do you pass the right length to getColumn()?",
    "computeCorrelationMatrixFlip.txt", "Did you forget to fill out the symmetric part of the correlation matrix?",
    "computeCorrelationMatrixDiagonal.txt", "Did you set the values at the diagonal to variance instead of 1?",
  };
  int nbBad = sizeof(bad)/sizeof(bad[0]);

  std::string good[] = {
    "computeCorrelationMatrix0.txt", ""
  };
  int nbGood = sizeof(good)/sizeof(good[0]);

  bool result = testFunction(mockComputeCorrelationMatrix, bad, nbBad, good, nbGood, path, eps, print, out);
  return result;
}

int test_ex1(std::ostream &out, const std::string test_name) {
	std::string entity_name = "stats";
	start_test_suite(out, test_name);

	std::vector<int> res = {
		test_eq(out, "computeMean_test_status", testComputeMean(defaultPath, defaultEps, defaultPrint, out), true),
		test_eq(out, "computeVariance_test_status", testComputeVariance(defaultPath, defaultEps, defaultPrint, out), true),
		test_eq(out, "computeSampleVariance_test_status", testComputeSampleVariance(defaultPath, defaultEps, defaultPrint, out), true),
		test_eq(out, "computeStandardDeviation_test_status", testComputeStandardDeviation(defaultPath, defaultEps, defaultPrint, out), true),
		test_eq(out, "computeSampleStandardDeviation_test_status", testComputeSampleStandardDeviation(defaultPath, defaultEps, defaultPrint, out), true)
	};

	return end_test_suite(out, test_name, std::accumulate(res.begin(), res.end(), 0), res.size());
}

int test_ex2(std::ostream &out, const std::string test_name) {
	std::string entity_name = "stats";
	start_test_suite(out, test_name);

	std::vector<int> res = {
		test_eq(out, "getRow_test_status", testGetRow(defaultPath, defaultEps, defaultPrint, out), true),
		test_eq(out, "getColumn_test_status", testGetColumn(defaultPath, defaultEps, defaultPrint, out), true)
	};

	return end_test_suite(out, test_name, std::accumulate(res.begin(), res.end(), 0), res.size());
}

int test_ex3(std::ostream &out, const std::string test_name) {
	std::string entity_name = "stats";
	start_test_suite(out, test_name);

	std::vector<int> res = {
		test_eq(out, "computeCovariance_test_status", testComputeCovariance(defaultPath, defaultEps, defaultPrint, out), true),
		test_eq(out, "computeCorrelation_test_status", testComputeCorrelation(defaultPath, defaultEps, defaultPrint, out), true)
	};

	return end_test_suite(out, test_name, std::accumulate(res.begin(), res.end(), 0), res.size());
}

int test_ex4(std::ostream &out, const std::string test_name) {
	std::string entity_name = "stats";
	start_test_suite(out, test_name);

	std::vector<int> res = {
		test_eq(out, "computeCovarianceMatrix_test_status", testComputeCovarianceMatrix(defaultPath, defaultEps, defaultPrint, out), true),
		test_eq(out, "computeCorrelationMatrix_test_status", testComputeCorrelationMatrix(defaultPath, defaultEps, defaultPrint, out), true)
	};

	return end_test_suite(out, test_name, std::accumulate(res.begin(), res.end(), 0), res.size());
}

int grading(std::ostream &out, const int test_case_number)
{
	/**

	  Annotations used for the autograder.

	  [START-AUTOGRADER-ANNOTATION]
	  {
	  "total" : 4,
	  "names" : [
	  "stats_functions.cpp::test_ex1", 
	  "stats_functions.cpp::test_ex2", 
	  "stats_functions.cpp::test_ex3", 
	  "stats_functions.cpp::test_ex4"
	  ],
	  "points" : [25, 25, 25, 25]
	  }
	  [END-AUTOGRADER-ANNOTATION]
	  */

	int const total_test_cases = 4;
	std::string const test_names[total_test_cases] = {"Test_ex_one", "Test_ex_two", "Test_ex_three", "Test_ex_four"};
	int const points[total_test_cases] = {25, 25, 25, 25};
	int (*test_functions[total_test_cases]) (std::ostream &, const std::string) = {
		test_ex1, test_ex2, test_ex3, test_ex4
	};

	return run_grading(out, test_case_number, total_test_cases,
			test_names, points,
			test_functions);
}

} // End of namepsace tdgrading

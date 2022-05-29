#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <string>
#include <vector>

namespace tdgrading {
bool testFunction (double (*func)(double[], std::vector<int>&),
                   std::string bad[], int nbBad, 
                   std::string good[], int nbGood,
                   std::string &path, double eps, bool print,
		   std::ostream &out = std::cout);

const bool defaultPrint = true;
const std::string defaultPath = "./tests/";
const double defaultEps = 0.001;

double hash (double vector[], int length);
double **unpack (double values[], std::vector<int> &scenario, int size, int rows, int columns);
void cleanup (double **matrix, int rows);
double **matrixFromArray (double array[], int rows, int columns);

double mockComputeMean (double values[], std::vector<int> &scenario);
bool testComputeMean (std::string path, double eps, bool print, std::ostream &out = std::cout);
double mockComputeVariance (double values[], std::vector<int> &scenario);
bool testComputeVariance (std::string path, double eps, bool print, std::ostream &out = std::cout);
double mockComputeSampleVariance (double values[], std::vector<int> &scenario);
bool testComputeSampleVariance (std::string path, double eps, bool print, std::ostream &out = std::cout);
double mockComputeStandardDeviation (double values[], std::vector<int> &scenario);
bool testComputeStandardDeviation (std::string path, double eps, bool print, std::ostream &out = std::cout);
double mockComputeSampleStandardDeviation (double values[], std::vector<int> &scenario);
bool testComputeSampleStandardDeviation (std::string path, double eps, bool print, std::ostream &out = std::cout);

int findSize (double values[], int length, double &eps);
double mockGetRow (double values[], std::vector<int> &scenario);
double mockGetColumn (double values[], std::vector<int> &scenario);
bool testGetRow(std::string path, double eps, bool print, std::ostream &out = std::cout);
bool testGetColumn(std::string path, double eps, bool print, std::ostream &out = std::cout);

double mockComputeCovariance (double values[], std::vector<int> &scenario);
double mockComputeCorrelation (double values[], std::vector<int> &scenario);
bool testComputeCovariance(std::string path, double eps, bool print, std::ostream &out = std::cout);
bool testComputeCorrelation(std::string path, double eps, bool print, std::ostream &out = std::cout);

int findSizeArray (double values[], int length, double &eps);
int findSize (double **matrix, int side, double &eps);
double mockComputeCovarianceMatrix (double values[], std::vector<int> &scenario);
double mockComputeCorrelationMatrix (double values[], std::vector<int> &scenario);
bool testComputeCovarianceMatrix(std::string path, double eps, bool print, std::ostream &out = std::cout);
bool testComputeCorrelationMatrix(std::string path, double eps, bool print, std::ostream &out = std::cout);

int grading(std::ostream &out, const int test_case);
}

#endif // TEST_HPP

#include <Eigen/Dense>
#include <Eigen/Core>
#include "Dataset.hpp"
#include "Regression.hpp"

#ifndef LINEARREGRESSION_HPP
#define LINEARREGRESSION_HPP
/**
  The LinearRegression class inherits from the Regression class, stores the coefficient and provides a bunch of specific methods.
*/
class LinearRegression : public Regression {
private:
    /**
      The linear regression coefficient.
    */
	Eigen::VectorXd* m_beta;
public:
    /**
      The linear regression method fits a linear regression coefficient to col_regr using the provided Dataset. It calls setCoefficients under the hood.
     @param dataset a pointer to a dataset
     @param m_col_regr the integer of the column index of Y
    */
	LinearRegression(Dataset* dataset, int col_regr);
    /**
      The destructor (frees m_beta).
    */
  ~LinearRegression();

  /**
    A function to construct from the data the matrix X needed by LinearRegression.
  */
	Eigen::MatrixXd ConstructMatrix();

  /**
    A function to construct the vector y needed by LinearRegression.
  */
  Eigen::VectorXd ConstructY() ;

  /**
    The setter method of the private attribute m_beta which is called by LinearRegression.
    It should use the functions ConstructMatrix and ConstructY.
  */
	void SetCoefficients();
  
  /**
      The getter method of the private attribute m_beta.
    */
	const Eigen::VectorXd* GetCoefficients() const;
  
    /**
      Prints the contents of the private attribute m_beta.
    */
	void ShowCoefficients() const;
  /**
      Prints the contents of the private attribute m_beta in a line.
    */
	void PrintRawCoefficients() const;
    /**
      The SumsOfSquares method calculates the ESS, RSS and TSS that will be initialized, passed by reference and thereafter printed by test_linear.
    */
	void SumsOfSquares( Dataset* dataset, double& ess, double& rss, double& tss ) const;
    /**
      The estimate method outputs the predicted Y for a given point x.
     @param x the point for which to estimate Y.
    */
	double Estimate( const Eigen::VectorXd & x ) const;
};

#endif //LINEARREGRESSION_HPP

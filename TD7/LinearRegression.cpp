#include "LinearRegression.hpp"
#include "Dataset.hpp"
#include "Regression.hpp"
#include<iostream>
#include<cassert>


LinearRegression::LinearRegression( Dataset* dataset, int col_regr ) 
: Regression(dataset, col_regr) {
	m_beta = NULL;
	SetCoefficients();
}

LinearRegression::~LinearRegression() {
	if (m_beta != NULL) {
		m_beta->resize(0);
		delete m_beta;
	}
}


Eigen::MatrixXd LinearRegression::ConstructMatrix() {
	// TODO Exercise 1

	int n = m_dataset->GetNbrSamples(), d = m_dataset->GetDim();
	Eigen::MatrixXd x(n, d);
	for(int i = 0; i<n; i++){
		x(i,0) = 1;
		int ix = 1; // the index of x row
		for(int j = 0; j<d; j++){
			if(j!=m_col_regr){
				x(i,ix) = m_dataset->GetInstance(i)[j];
				ix += 1;
			}
		}
	}
	return x;
}

Eigen::VectorXd LinearRegression::ConstructY() {
	// TODO Exercise 1

	int n = m_dataset->GetNbrSamples();
	Eigen::VectorXd y(n);
	for(int i = 0; i<n; i++){
		y(i) = m_dataset->GetInstance(i)[m_col_regr];
	}
	return y;
}

void LinearRegression::SetCoefficients() {
	// TODO Exercise 2

	int d = m_dataset->GetDim();
	m_beta = new Eigen::VectorXd(d);
	Eigen::MatrixXd X = ConstructMatrix();
	Eigen::VectorXd y = ConstructY();
	*m_beta = (X.transpose() * X).ldlt().solve(X.transpose() * y);	
}

const Eigen::VectorXd* LinearRegression::GetCoefficients() const {
	if (!m_beta) {
		std::cout<<"Coefficients have not been allocated."<<std::endl;
		return NULL;
	}
	return m_beta;
}

void LinearRegression::ShowCoefficients() const {
	if (!m_beta) {
		std::cout<<"Coefficients have not been allocated."<<std::endl;
		return;
	}
	
	if (m_beta->size() != m_dataset->GetDim()) {  // ( beta_0 beta_1 ... beta_{d} )
		std::cout<< "Warning, unexpected size of coefficients vector: " << m_beta->size() << std::endl;
	}
	
	std::cout<< "beta = (";
	for (int i=0; i<m_beta->size(); i++) {
		std::cout<< " " << (*m_beta)[i];
	}
	std::cout << " )"<<std::endl;
}

void LinearRegression::PrintRawCoefficients() const {
	std::cout<< "{ ";
	for (int i=0; i<m_beta->size()-1; i++) {
		std::cout<< (*m_beta)[i]  << ", ";
	}
	std::cout << (*m_beta)[m_beta->size()-1];
	std::cout << " }"<<std::endl;
}

void LinearRegression::SumsOfSquares( Dataset* dataset, double& ess, double& rss, double& tss ) const {
	assert(dataset->GetDim()==m_dataset->GetDim());
	// TODO Exercise 4

	int n = dataset->GetNbrSamples(), d = dataset->GetDim();
	Eigen::MatrixXd x(n, d);
	Eigen::VectorXd y(n);
	for(int i = 0; i<n; i++){
		x(i,0) = 1;
		y(i) = dataset->GetInstance(i)[m_col_regr];
		int ix = 1; // the index of x row
		for(int j = 0; j<d; j++){
			if(j!=m_col_regr){
				x(i,ix) = dataset->GetInstance(i)[j];
				ix += 1;
			}
		}
	}
	Eigen::VectorXd prediction = x*(*m_beta);
	Eigen::VectorXd y_mean = Eigen::VectorXd::Ones(n) * y.mean(); 
	ess = (prediction - y_mean).dot(prediction - y_mean);
	tss = (y - y_mean).dot(y - y_mean);
	rss = (prediction - y).dot(prediction - y);

}

double LinearRegression::Estimate( const Eigen::VectorXd & x ) const {
	// TODO Exercise 3
	Eigen::VectorXd xx(m_dataset->GetDim());
	xx(0) = 1;
	for(int i = 1; i<m_dataset->GetDim(); i++){
		xx(i) = x(i-1);
	}
	return xx.dot(*m_beta);
}

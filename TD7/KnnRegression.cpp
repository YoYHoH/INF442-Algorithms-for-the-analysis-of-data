
#include "KnnRegression.hpp"
#include<iostream>
#include <ANN/ANN.h>


KnnRegression::KnnRegression(int k, Dataset* dataset, int col_regr)
: Regression(dataset, col_regr) {
	m_k = k;
 	// TODO Exercise 5
	this->m_dataPts = annAllocPts(dataset->GetNbrSamples(), dataset->GetDim()-1);
    for(int i = 0; i<dataset->GetNbrSamples(); i++){
        int j = 0;
        for(j; j<dataset->GetDim(); j++){
            if(j==col_regr){
                continue;
            }
            if(j<col_regr){
                m_dataPts[i][j] = dataset->GetInstance(i)[j];
            }
            if(j>col_regr){
                m_dataPts[i][j-1] = dataset->GetInstance(i)[j];
            }
        }
    }
    this->m_kdTree = new ANNkd_tree(m_dataPts, dataset->GetNbrSamples(), dataset->GetDim()-1);
}

KnnRegression::~KnnRegression() {
	// TODO Exercise 5
	delete m_kdTree;
    delete[] m_dataPts;
	annClose();
}

double KnnRegression::Estimate(const Eigen::VectorXd & x) const {
	assert(x.size()==m_dataset->GetDim()-1);
	// TODO Exercise 6
	ANNidxArray nn_idx = new ANNidx[m_k];
    ANNdistArray dists = new ANNdist[m_k];
	ANNpoint x_ = new ANNcoord[x.size()];
	for(int i = 0; i < x.size(); i++) {
		x_[i] = x(i);
	}
    m_kdTree -> annkSearch(x_, m_k, nn_idx, dists);
    double p = 0;
    for(int i = 0; i<m_k; i++){
        p += m_dataset->GetInstance(nn_idx[i])[m_col_regr];
    }
    p = p/m_k;
    return p;
}

int KnnRegression::GetK() const {
	return m_k;
}

ANNkd_tree* KnnRegression::GetKdTree() const {
	return m_kdTree;
}

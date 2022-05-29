
#include "KnnClassification.hpp"
#include <iostream>
#include <ANN/ANN.h>

 
KnnClassification::KnnClassification(int k, Dataset* dataset, int col_class)
: Classification(dataset, col_class) {
    // TODO Exercise 1
    this->m_k = k;
    this->m_dataPts = annAllocPts(dataset->getNbrSamples(), dataset->getDim()-1);
    for(int i = 0; i<dataset->getNbrSamples(); i++){
        int j = 0;
        for(j; j<dataset->getDim(); j++){
            if(j==col_class){
                continue;
            }
            if(j<col_class){
                m_dataPts[i][j] = dataset->getInstance(i)[j];
            }
            if(j>col_class){
                m_dataPts[i][j-1] = dataset->getInstance(i)[j];
            }
        }
    }
    this->m_kdTree = new ANNkd_tree(m_dataPts, dataset->getNbrSamples(), dataset->getDim()-1);
}

KnnClassification::~KnnClassification() {
    // TODO Exercise 1
    delete m_kdTree;
    delete[] m_dataPts;
    annClose();
}

int KnnClassification::Estimate(const ANNpoint & x, double threshold) const {
    // TODO Exercise 2
    ANNidxArray nn_idx = new ANNidx[m_k];
    ANNdistArray dists = new ANNdist[m_k];
    m_kdTree -> annkSearch(x, m_k, nn_idx, dists);
    double p = 0;
    for(int i = 0; i<m_k; i++){
        p += m_dataset->getInstance(nn_idx[i])[m_col_class];
    }
    p = p/m_k;
    double y = (p > threshold)? 1 : 0;
    // std::cout << y << std::endl;
     return y;
}

int KnnClassification::getK() const {
    return m_k;
}

ANNkd_tree* KnnClassification::getKdTree() {
    return m_kdTree;
}

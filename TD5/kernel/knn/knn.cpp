#include <point.hpp>
#include <cloud.hpp>
#include <kernel.hpp>
#include <knn.hpp>

// TODO 2.2.1: implement density for knn kernel
knn::knn(cloud *data_, int k_, double V_) : kernel(data_){
    k = k_;
    V = V_;
}

double knn::density(point &p) const {
    return k /( 2 * data->get_n() * V * data->k_dist_knn(p, k) );
}

double knn::volume() const {
    return V;
}

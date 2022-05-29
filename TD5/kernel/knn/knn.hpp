#pragma once

#include "../../point/point.hpp"
#include "../../cloud/cloud.hpp"
#include "../kernel.hpp"

/**
	The knn class implements the knn kernel.
*/
class knn : public kernel {

public:
    knn(cloud *data_, int k_, double V_);
    virtual ~knn() {}
    double volume() const;
    virtual double density(point &p) const;

protected:
/**
	The number of neighbors to consider.
*/
    int k;
/**
	The volume of the unit ball.
*/
   double V;
};

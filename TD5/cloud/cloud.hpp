#pragma once

#include <fstream>

#include <point.hpp>

/**
	The cloud class stores a collection of points.
*/
class cloud {
/**
	The number of points in the cloud.
*/
	int n;
/**
	The maximum possible number of points.
*/
	int nmax;
/**
	The points.
*/
	point *points;

public:
	cloud(int _d, int _nmax);
	~cloud();

/**
	Getter for n.
*/
	int get_n() const;
/**
	Getter for point.
*/
	point& get_point(int i);

/**
	Add a point to the cloud.
*/
	void add_point(point &p);
/**
	Load points from file.
*/
	void load(std::ifstream &is);

/**
	The minimum value of points for a particular coordinate
	@param m (int) coordinate to search for min.
*/
	double min_in_coord(int m);
/**
	The maximum value of points for a particular coordinate
	@param m (int) coordinate to search for maximum.
*/
	double max_in_coord(int m);
/**
	The standard deviation
*/
	double standard_deviation();
/**
	Finds the distance to the k-th nearest neighbor.
	@param p (point&) a query point.
	@param k (int) number of k nearest neighbors.
*/
	double k_dist_knn(point &p, int k) const;
/**
	Return k nearest neighbors.
	@param p (point&) a query point.
	@param k (int) number of k nearest neighbors.
*/
	point* knn(point &p, int k);
/**
	Performs one iteration of meanshift.
	@param k (int) number of k nearest neighbors.
*/
	point* shift(int k);
/**
	Performs meanshift in a loop and prints stuff.
	@param n_steps (int) number of iterations.
	@param k (int) number of k nearest neighbors.
	@param verbose (bool) whether to print stuff.
*/
	void meanshift(int n_steps, int k, bool verbose = false);
};

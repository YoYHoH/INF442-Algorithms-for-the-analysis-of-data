
#include <algorithm>  // for sort
#include <cassert>    // for assertions
#include <cfloat>     // for DBL_MAX
#include <cmath>      // for math operations like sqrt, log, etc
#include <cstdlib>    // for rand, srand
#include <ctime>      // for clock
#include <fstream>    // for ifstream
#include <iostream>   // for cout

#include "retrieval.hpp"

using std::cout;
using std::endl;

/*****************************************************
 * TD 2: K-Dimensional Tree (kd-tree)                *
 *****************************************************/



void print_point(point p, int dim) {
    std::cout << "[ ";
    for (int i = 0; i < dim; ++i) {
        std::cout << p[i] << " ";
    }
    std::cout << "]" << std::endl;
}

void pure_print(point p, int dim) {
    std::cout << p[0];
    for (int j = 1; j < dim; j++)
        std::cout << " " << p[j];
    std::cout << "\n";
}


/**
 * This function computes the Euclidean distance between two points
 *
 * @param p  the first point
 * @param q the second point
 * @param dim the dimension of the space where the points live
 * @return the distance between p and q, i.e., the length of the segment pq
 */
double dist(point p, point q, int dim) {
    // Exercise 1
    double distance = 0.0;
    for(int i = 0; i<dim; i++){
        distance = distance + std::pow(p[i]-q[i], 2);
    }
    distance = std::sqrt(distance);
    return distance;
}

/**
 * This function for a given query point q  returns the index of the point
 * in an array of points P that is closest to q using the linear scan algorithm.
 *
 * @param q the query point
 * @param dim the dimension of the space where the points live
 * @param P a set of points
 * @param n the number of points in P
 * @return the index of the point in p that is closest to q
 */
int linear_scan(point q, int dim, point *P, int n) {
    // Exercise 2
    int idx = -1;  // It will contain the index of the closest point
    double dis = dist(q, *P, dim);
    idx = 0 ;
    for(int i = 1; i<n; i++){
        if(dist(q, *(P+i), dim) < dis){
            dis = dist(q, *(P+i), dim);
            idx = i;
        }
    }

    return idx;
}

/**
 * This function computes the median of all the c coordinates 
 * of an subarray P of n points that is P[start] .. P[end - 1]
 *
 * @param P a set of points
 * @param start the starting index
 * @param end the last index; the element P[end] is not considered
 * @return the median of the c coordinate
 */
double compute_median(point *P, int start, int end, int c) {
    // Exercise 3
    double median = 0.0;
    int n = end - start + 1;
    double* pp = new double[n];
    for(int i = 0; i<n; i++){
        pp[i] = *(*(P+start+i)+c);
    }
    std::sort(pp, pp+n-1);
    median = *(pp+(n-1)/2);
    // delete[] pp;
    return median;
}

/**
 * Partitions the the array P wrt to its median value along a coordinate
 *
 * @param P a set of points (an array)
 * @param start the starting index
 * @param end the last index; the element P[end] is not considered
 * @param c the coordinate that we will consider the median
 * @return the index of the median value
 */
int partition(point *P, int start, int end, int c) {
    // Exercise 4
    double m = compute_median(P, start, end, c);
    int idx = -1;  // this is where we store the index of the median
    idx = (end - start)/2 + start;

    int i = start;
    int j = end-1;
    point pp;
    while(i<j){
        if(P[i][c]==m){
            if(P[i][c]==m){
                for(int k = 1; k<(end-start); k++){
                    if(P[i+k][c]==m){
                        continue;
                    }
                    else{
                        pp = P[i];
                        P[i] = P[i+k];
                        P[i+k] = pp;
                        break;
                    }
                }
            }
        }
        else if(P[i][c]<m){
            i++;
        }
        else{
            pp = P[i];
            P[i]=P[j];
			P[j]=pp;
			j--;
        }
        
    }
    

    return idx;
}

/**
 * Creates a leaf node in the kd-tree
 *
 * @param val the value of the leaf node
 * @return a leaf node that contains val
 */
node *create_node(int _idx) {
    // Exercise 5

    node *nd=new node;
    nd->idx=_idx;
    nd->left=NULL;
    nd->right=NULL;

    return nd;
}

/**
 * Creates a internal node in the kd-tree
 *
 * @param idx the value of the leaf node
 * @return an internal node in the kd-tree that contains val
 */
node *create_node(int _c, double _m, int _idx,
                  node *_left, node *_right) {  
    // Exercise 5

    node *nd=new node;
    nd->c=_c;
    nd->idx=_idx;
    nd->left=_left;
    nd->right=_right;
    nd->m=_m;

    return nd;
}

node* build(point *P, int start, int end, int c, int dim) {
    // builds tree for sub-cloud P[start -> end-1]
    assert(end - start >= 0);
    if (debug)
        std::cout << "start=" << start << ", end=" << end << ", c=" << c
                  << std::endl;
    if (end - start == 0)  // no data point left to process
        return NULL;
    else if (end - start == 1)  // leaf node
        return create_node(start);
    else {  // internal node
        if (debug) {
            std::cout << "array:\n";
            for (int i = start; i < end; i++)
                print_point(P[i], dim);
            // std::cout << P[i] << ((i==end-1)?"\n":" ");
        }
        // compute partition
        // rearrange subarray (less-than-median first, more-than-median last)
        int p = partition(P, start, end, c);
        if (p == -1) { return NULL; }
        double m = P[p][c];
        // prepare for recursive calls
        int cc = (c + 1) % dim;  // next coordinate
        return create_node(c, m, p, build(P, start, p, cc, dim),
                           build(P, p + 1, end, cc, dim));
    }
}

/**
 *  Defeatist search in a kd-tree
 *
 * @param n the roots of the kd-tree
 * @param q the query point
 * @param dim the dimension of the points
 * @param P a pointer to an array of points
 * @param res the distance of q to its NN in P
 * @param nnp the index of the NN of q in P
 * @return a leaf node that contains val
 */
void defeatist_search(node *n, point q, int dim, point *P, double &res, int &nnp) {
    // Exercise 6
    double b;
  
	if (n != NULL) {
        b = std::min(res, dist (q, P[n->idx], dim));
    	if(b<res){
    		nnp=n->idx;
    		res=b;
		}
		
        if (n->left != NULL || n->right != NULL)  // internal node
            defeatist_search ( (q[n->c] <= n->m) ? n->left : n->right, q, dim, P, res,nnp);
  	}   
}

/**
 *  Backtracking search in a kd-tree
 *
 * @param n the roots of the kd-tree
 * @param q the query point
 * @param dim the dimension of the points
 * @param P a pointer to an array of points
 * @param res the distance of q to its NN in P
 * @param nnp the index of the NN of q in P
 * @return a leaf node that contains val
 */
void backtracking_search(node *n, point q, int dim, point *P, double &res, int &nnp) {
    // Exercise 7
    if (n != NULL)
    {
        // res = std::min(res, dist(q, P[n->idx], dim));
        double dis = dist(q, P[n->idx], dim);
        if (dis < res)
        {
            res = dis;
            nnp = n->idx;
        }

        if (n->left != NULL || n->right != NULL)
        {                              // internal node
            if (q[n->c] - res <= n->m) // ball intersects left side of H
                backtracking_search(n->left, q, dim, P, res, nnp);
            if (q[n->c] + res > n->m) // ball intersects right side of H
                backtracking_search(n->right, q, dim, P, res, nnp);
        }
    }
}

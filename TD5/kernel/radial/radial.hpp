#pragma once

#include <point.hpp>
#include <cloud.hpp>
#include <kernel.hpp>

/** 
	The radial class is an abstract class inheriting from the kernel class which will be the basis of the flat and gaussian classes.
*/
class radial : public kernel
{
protected:
    /**
      The bandwith parameter.
    */
	double bandwidth;

public:
	/**
		Radial class constructor, uses kernel constructor and sets bandwith attribute
	@param data_ (cloud*) its cloud of points.
	@param bandwidth_ (double) the bandiwth of the radial kernel.
	*/
	radial(cloud *data_, double bandwidth_);

	/**
		Density function relying on pure virtual functions volume and profile below
	@param p (&point) the value of the density function at point p.
	*/
	double density(point &p) const;

	/**
		Virtual function volume implemented in flat and gaussian as
		the volume of the unit ball weighed with the kernel
	*/
	virtual double volume() const = 0;
	/**
		Virtual function profile implemented in flat and gaussian as
		the kernel profile function evaluated at the given real number t
	@param t (double) the value at which the profile is evaluated.
	*/
	virtual double profile(double t) const = 0;
};

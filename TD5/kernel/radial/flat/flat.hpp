#pragma once

#include <cloud.hpp>
#include <radial.hpp>

/** 
	The flat class inherits from the radial class itself inheriting from the kernel class.
*/
class flat : public radial
{
private:

public:
	/**
		Flat class constructor, uses radial constructor => no implementation needed in flat.cpp
	@param data_ (cloud*) its cloud of points.
	@param bandwidth_ (double) the bandiwth of the flat kernel.
	*/
	flat(cloud *data_, double bandwidth_) : radial(data_, bandwidth_) {}

	/**
		Volume of the unit ball weighed with the flat kernel (see exercise sheet)
	*/
	double volume() const;

	/**
		Profile of the flat kernel evaluated at t
	@param t (double) the value at which the profile is evaluated.
	*/
	double profile(double t) const;
};

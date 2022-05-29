#pragma once

#include <cloud.hpp>
#include <radial.hpp>

class gaussian : public radial
{
private:


public:
	gaussian(cloud *data_, double bandwidth_) : radial(data_, bandwidth_) {}

	double volume() const;
	double profile(double t) const;

	void guess_bandwidth();
};

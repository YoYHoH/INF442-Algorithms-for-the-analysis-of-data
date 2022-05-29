#pragma once

#include <point.hpp>
#include <cloud.hpp>

/**
	The kernel abstract class.
*/
class kernel
{
protected:
	cloud *data;

public:
	kernel(cloud *data_);

	virtual double density(point &p) const = 0;
};

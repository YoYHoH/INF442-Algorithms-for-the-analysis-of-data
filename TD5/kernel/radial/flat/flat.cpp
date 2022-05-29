#include <cmath> // for pow, atan, should you need them

#include <point.hpp>
#include <flat.hpp>

// TODO 2.1.1: implement volume and profile
// HINT: pi = std::atan(1) * 4.0
double flat::volume() const
{
	double d = data->get_point(0).get_dim();
	return pow(atan(1)*4, d/2) / std::tgamma(d/2 + 1);
}

double flat::profile(double t) const
{
	return (t>1) ? 0 : 1;
}

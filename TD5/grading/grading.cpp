#include <numeric>

#include <gradinglib.hpp>
#include <point.hpp>
#include <cloud.hpp>
#include <kernel.hpp>
#include <radial.hpp>
#include <flat.hpp>
#include <gaussian.hpp>
#include <knn.hpp>

namespace tdgrading {

using namespace testlib;
using namespace std;

class radtest : public radial
{
private:


public:
	radtest(cloud *data_, double bandwidth_) : radial(data_, bandwidth_) {}

	double volume() const
	{
		return data->get_n();
	}

	double profile(double t) const
	{
		return t;
	}
};

int test_radial(std::ostream &out, const std::string test_name) {
    std::string entity_name = "radial";
    start_test_suite(out, test_name);
    std::vector<int> res;
	const double eps = 0.1;

	cloud c(2, 5);
	point p;

	p.coords[0] = 1.0;
	p.coords[1] = 2.0;
	c.add_point(p);
	p.coords[0] = 3.0;
	p.coords[1] = 4.0;
	c.add_point(p);
	p.coords[0] = 0.0;
	p.coords[1] = -1.0;
	c.add_point(p);
	p.coords[0] = 3.0;
	p.coords[1] = 7.0;
	c.add_point(p);
	p.coords[0] = 10.0;
	p.coords[1] = -2.0;
	c.add_point(p);

	radtest ker(&c, 0.42);

	p.coords[0] = 0.0;
	p.coords[1] = 0.0;
    res.push_back(test_eq_approx(out, "density", ker.density(p), 248.096, eps));

	p.coords[0] = 42.0;
	p.coords[1] = -1.0;
    res.push_back(test_eq_approx(out, "density", ker.density(p), 9782.45, eps));

	p.coords[0] = -3.0;
	p.coords[1] = 1.0;
    res.push_back(test_eq_approx(out, "density", ker.density(p), 417.779, eps));

    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());
}

int test_flat(std::ostream &out, const std::string test_name) {
    std::string entity_name = "flat";
    start_test_suite(out, test_name);
    std::vector<int> res;
    const double eps = 0.1;

	cloud c(7, 5);

	flat ker(&c, 5.00);
    res.push_back(test_eq_approx(out, "volume", ker.volume(), 4.72477, eps));
    res.push_back(test_eq_approx(out, "profile", ker.profile(0.5), 1.0, eps));
    res.push_back(test_eq_approx(out, "profile", ker.profile(1.5), 0.0, eps));
    res.push_back(test_eq_approx(out, "profile", ker.profile(2.5), 0.0, eps));

    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());

}

int test_gaussian(std::ostream &out, const std::string test_name) {
    std::string entity_name = "gaussian";
    start_test_suite(out, test_name);
    std::vector<int> res;
	const double eps = 0.01;

	cloud c(7, 5);
	point p;
	p.coords[0] = 1.0;
	p.coords[1] = 2.0;
	p.coords[2] = 3.0;
	p.coords[3] = 4.0;
	p.coords[4] = 5.0;
	p.coords[5] = 6.0;
	p.coords[6] = 7.0;
	c.add_point(p);
	p.coords[0] = 0.0;
	p.coords[1] = -2.0;
	p.coords[2] = 3.0;
	p.coords[3] = 11.0;
	p.coords[4] = -5.0;
	p.coords[5] = 6.0;
	p.coords[6] = 3.0;
	c.add_point(p);

	gaussian ker(&c, 5.00);

    res.push_back(test_eq_approx(out, "volume", ker.volume() / 621.77, 1.0, eps));
    res.push_back(test_eq_approx(out, "profile", ker.profile(0.5) / 0.778801, 1.0, eps));
    res.push_back(test_eq_approx(out, "profile", ker.profile(1.5) / 0.472367, 1.0, eps));
    res.push_back(test_eq_approx(out, "profile", ker.profile(2.5) / 0.286505, 1.0, eps));

	p.coords[0] = 0;
	p.coords[1] = 0;
	p.coords[2] = 0;
	p.coords[3] = 0;
	p.coords[4] = 0;
	p.coords[5] = 0;
	p.coords[6] = 0;

    res.push_back(test_eq_approx(out, "density", ker.density(p) / 7.99962e-10, 1.0, eps));

	ker.guess_bandwidth();
    res.push_back(test_eq_approx(out, "density", ker.density(p) / 1.32198e-10, 1.0, eps));
    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());
}

int test_cloud(std::ostream &out, const std::string test_name) {
    std::string entity_name = "k_dist_knn";
    start_test_suite(out, test_name);
    std::vector<int> res;
	const double eps = 0.001;

	cloud c(2, 2);
	point p;

	p.coords[0] = 0.0;
	p.coords[1] = 0.0;
	c.add_point(p);
	p.coords[0] = 1.0;
	p.coords[1] = 1.0;
	c.add_point(p);

	p.coords[0] = 0.0;
	p.coords[1] = 1.0;
    res.push_back(test_eq_approx(out, "k_dist_knn", c.k_dist_knn(p, 1), 1.0, eps));
	p.coords[0] = 0.0;
	p.coords[1] = 0.5;
    res.push_back(test_eq_approx(out, "k_dist_knn", c.k_dist_knn(p, 1), 0.5, eps));
	p.coords[0] = 0.5;
	p.coords[1] = 0.5;
    res.push_back(test_eq_approx(out, "k_dist_knn", c.k_dist_knn(p, 1), 0.707, eps));

    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());
}

int test_knn(std::ostream &out, const std::string test_name) {
    std::string entity_name = "knn";
    start_test_suite(out, test_name);
    std::vector<int> res;
	const double eps = 0.001;

	cloud c(2, 5);
	point p;

	p.coords[0] = 1.0;
	p.coords[1] = 2.0;
	c.add_point(p);
	p.coords[0] = 3.0;
	p.coords[1] = 4.0;
	c.add_point(p);
	p.coords[0] = 0.0;
	p.coords[1] = -1.0;
	c.add_point(p);
	p.coords[0] = 3.0;
	p.coords[1] = 7.0;
	c.add_point(p);
	p.coords[0] = 10.0;
	p.coords[1] = -2.0;
	c.add_point(p);

	knn ker(&c, 3, 2.0);

	p.coords[0] = 0.0;
	p.coords[1] = 0.0;
    res.push_back(test_eq_approx(out, "density", ker.density(p), 0.03, eps));

	p.coords[0] = 42.0;
	p.coords[1] = -1.0;
    res.push_back(test_eq_approx(out, "density", ker.density(p), 0.0037677, eps));

	p.coords[0] = -3.0;
	p.coords[1] = 1.0;
    res.push_back(test_eq_approx(out, "density", ker.density(p), 0.0223607, eps));

    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());
}

int test_meanshift(std::ostream &out, const std::string test_name) {
    std::string entity_name = "meanshift";
    start_test_suite(out, test_name);
    std::vector<int> res;
    const double eps = 0.01;

	std::ifstream is("csv/normal.csv");

	cloud c(2, 130000);
	c.load(is);
	c.meanshift(1, 3);
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(0).coords[0], -0.742035, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(0).coords[1], 1.09605, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(1).coords[0], 0.025417, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(1).coords[1], 0.1229, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(2).coords[0], 0.845654, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(2).coords[1], -0.97052, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(3).coords[0], 0.04139, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(3).coords[1], 0.921662, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(4).coords[0], 0.00173367, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(4).coords[1], -0.848124, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(5).coords[0], 0.095338, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(5).coords[1], 1.4491, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(6).coords[0], 2.13119, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(6).coords[1], -0.484897, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(7).coords[0], -0.302607, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(7).coords[1], 0.184375, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(8).coords[0], 1.26503, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(8).coords[1], -0.312559, eps));

	res.push_back(test_eq_approx(out, "meanshift", c.get_point(9).coords[0], -0.467785, eps));
	res.push_back(test_eq_approx(out, "meanshift", c.get_point(9).coords[1], 0.744896, eps));

    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());

}

//-----------------------------------------------------------------------------

int grading(std::ostream &out, const int test_case_number)
{
/**

Annotations used for the autograder.

[START-AUTOGRADER-ANNOTATION]
{
  "total" : 6,
  "names" : [
      "radial.cpp::test_radial",
      "flat.cpp::test_flat",
      "gaussian.cpp::test_gaussian",
      "cloud.cpp::test_knn",
      "knn.cpp::test_knn",
      "meanshift.cpp::test_meanshift"
  ],
  "points" : [10, 15, 15, 20, 20, 20]
}
[END-AUTOGRADER-ANNOTATION]
*/

    int const total_test_cases = 6;
    std::string const test_names[total_test_cases] = {"test_radial", "test_flat", "test_gaussian", "test_cloud", "test_knn", "test_meanshift"};
    int const points[total_test_cases] = {10, 15, 15, 20, 20, 20};
    int (*test_functions[total_test_cases]) (std::ostream &, const std::string) = {
        test_radial, test_flat, test_gaussian, test_cloud, test_knn, test_meanshift
    };

    return run_grading(out, test_case_number, total_test_cases,
                       test_names, points,
                       test_functions);
}

} // End of namepsace tdgrading

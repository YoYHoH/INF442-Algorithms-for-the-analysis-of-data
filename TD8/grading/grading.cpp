#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdarg>
#include <iterator>
#include <string>
#include <regex>
#include <numeric>
#include <cmath>
#include <fstream>
#include <random>
#include <limits>

#include "../gradinglib/gradinglib.hpp"
#include "../libsvm-3.23/eval.h"
#include "../libsvm-3.23/svm.h"
#include "../libsvm-3.23/svm.cpp"

namespace tdgrading {

using namespace testlib;
using namespace std;

const double deps = 0.001;
const std::string default_path = "./grading/tests/";    

double rel_error(double a, double b) {
    return fabs(a - b) / fabs(a);
}

template <typename T, typename... Arguments>
bool test_rel_error(std::ostream &out,
                      const std::string &function_name,
                      T result,
                      T expected,
                      T delta,
                      const Arguments&... args) 
{
    bool success = (rel_error(result, expected) <= delta);
    
    out << (success ? "[SUCCESS] " : "[FAILURE] ");

    print_tested_function(out, function_name, args...);

    out << ": got " << result
        << " expected " << expected << "  The relative error should be in [-" << delta << "," << delta << "]";
    out << std::endl;

    return success;
}

std::string exec(const char* in) {
    std::array<char, 128> buffer;
    std::string out;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(in, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        out += buffer.data();
    }
    return out;
}

int svm_train(std::ostream &out, const std::string test_name) 
{
    std::string entity_name = "Test svm-train.c";
    start_test_suite(out, test_name);
    std::vector<int> res; 

    const char* command = "./libsvm-3.23/svm-train ./csv/mail_train.svm ./csv/mail_svm.model_default";
    std::vector<std::string> expected;
    expected.push_back("optimization finished");
    expected.push_back("#iter = 1102");
    expected.push_back("obj = -1099");
    expected.push_back("Total nSV = 1622");
    res.push_back(test_in_output(out, entity_name, exec, command, expected));

    const char* command_cv = "./libsvm-3.23/svm-train -v 2 ./csv/mail_train.svm ./csv/mail_svm.model_default";
    std::vector<std::string> expected_cv;
    expected_cv.push_back("Precision = 96");
    expected_cv.push_back("Recall = 79");
    expected_cv.push_back("Accuracy = 92");
    expected_cv.push_back("Cross Validation = 92");
    res.push_back(test_in_output(out, entity_name, exec, command_cv, expected_cv));

    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());
}

int svm_predict(std::ostream &out, const std::string test_name) 
{
    std::string entity_name = "Test svm-predict.c";
    start_test_suite(out, test_name);
    std::vector<int> res; 

    const char* command_train = "./libsvm-3.23/svm-train ./csv/mail_train.svm ./csv/mail_svm.model_default";
    exec(command_train);
    const char* command_test = "./libsvm-3.23/svm-predict ./csv/mail_test.svm ./csv/mail_svm.model_default ./csv/mail_svm.output_default";
    std::vector<std::string> expected_test;
    expected_test.push_back("Precision = 97");
    expected_test.push_back("Recall = 86");
    expected_test.push_back("F-score = 0.91");
    expected_test.push_back("Accuracy = 95");

    res.push_back(test_in_output(out, entity_name, exec, command_test, expected_test));
    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());
}

int svm_kernel(std::ostream &out, const std::string test_name) 
{
    std::string entity_name = "Test new kernel";
    start_test_suite(out, test_name);
    std::vector<int> res; 

    const char* command_train = "./libsvm-3.23/svm-train -r 1 -t 5 ./csv/mail_train.svm ./csv/mail_svm.model_default";
    exec(command_train);
    std::vector<std::string> expected;
    expected.push_back("optimization finished");
    expected.push_back("#iter = 6354");
    expected.push_back("obj = -998");
    expected.push_back("Total nSV = 3734");
    res.push_back(test_in_output(out, entity_name, exec, command_train, expected));

    const char* command_test = "./libsvm-3.23/svm-predict ./csv/mail_test.svm ./csv/mail_svm.model_default ./csv/mail_svm.output_default";
    std::vector<std::string> expected_test;
    expected_test.push_back("Precision = 100");
    expected_test.push_back("Recall = 57");
    expected_test.push_back("F-score = 0.73");
    expected_test.push_back("Accuracy = 87");
    res.push_back(test_in_output(out, entity_name, exec, command_test, expected_test));
    return end_test_suite(out, test_name, accumulate(res.begin(), res.end(), 0), res.size());
}

int grading(std::ostream &out, const int test_case_number)
{
/**

Annotations used for the autograder.

[START-AUTOGRADER-ANNOTATION]
{
  "total" : 3,
  "names" : [
        "svm-predict.c::svm_predict",
        "svm-train.c::svm_train",
        "svm.cpp::svm_kernel"
        ],
  "points" : [10, 10, 10]
}
[END-AUTOGRADER-ANNOTATION]
*/

    int const total_test_cases = 3;
    std::string const test_names[total_test_cases] = {"svm_predict", "svm_train", "svm_kernel"};
    int const points[total_test_cases] = {10, 10, 10};
    int (*test_functions[total_test_cases]) (std::ostream &, const std::string) = {
      svm_predict, svm_train, svm_kernel
    };

    return run_grading(out, test_case_number, total_test_cases,
                       test_names, points,
                       test_functions);
}

} // End of namepsace tdgrading

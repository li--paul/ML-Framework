// Unit test file for linear_regression

#include <iostream>
#include <string>
#include <cfloat>
#include <ctime>
#include <cstdlib>
#include <cstdio>
using namespace std;

#include "../../linear_regression/linear_regression_nd/lr.hpp"
#include "../../linear_regression/linear_regression_nd/lr_utils.hpp"

#define ERR_OUT(stmt) if(stmt) { cerr<<"Error out - "<<__FILE__<<" - "<<__LINE__<<endl;  return false; }

/* Utility */
vector<vector<double> > rand_vector_vector_double(int width, int height) {
    assert(width > 0);
    assert(height > 0);
    
    srand(time(NULL));

    vector<vector<double> > ret_vec;
    for(int row = 0; row < height; row++) {
        ret_vec.push_back(vector<double> ());
        for(int col = 0; col < width; col++) {
            ret_vec[row].push_back(rand() % 100);
        }
    }
    return ret_vec;
}

vector<double> rand_vector_double(int width) {
    assert(width > 0);

    srand(time(NULL));

    vector<double> ret_vec;
    for(int col = 0; col < width; col++) {
        ret_vec.push_back(rand() % 100);
    }
    return ret_vec;
}

/* 
 * Test the hypothesis function;
 * Return true if all tests passed;
 * Return false if atleast one failed;
 */
bool hypothesis_test() {
    // Error cases 
    ERR_OUT(hypothesis(vector<double> (), vector<double> ()) != DBL_MAX);
    ERR_OUT(hypothesis(vector<double> (), rand_vector_double(3)) != DBL_MAX);
    ERR_OUT(hypothesis(rand_vector_double(3), vector<double>()) != DBL_MAX);
    ERR_OUT(hypothesis(rand_vector_double(3), rand_vector_double(2)) != DBL_MAX);
    ERR_OUT(hypothesis(rand_vector_double(3), rand_vector_double(3)) != DBL_MAX);

    // Success case
    ERR_OUT(hypothesis(rand_vector_double(2), rand_vector_double(3)) == DBL_MAX);

    // Custom test case
    vector<double> feature_vector;
    vector<double> w;

    feature_vector.push_back(20); // x0
    feature_vector.push_back(30); // x1
    w.push_back(-1); // w0
    w.push_back(2);  // w1
    w.push_back(44); // bias
    
    ERR_OUT(hypothesis(feature_vector, w) != 84);
    
    return true;
} 

/*
 * Test linear regression cost function
 * Return true if all tests pass
 * Return false if atleast one fails
 */
int cost_function_test() {

    ERR_OUT(cost_function(vector<vector<double> > (),      vector<double> (),     vector<double> ()) != DBL_MAX);
    ERR_OUT(cost_function(vector<vector<double> > (),      vector<double> (),     rand_vector_double(3)) != DBL_MAX);
    ERR_OUT(cost_function(vector<vector<double> > (),      rand_vector_double(2), vector<double> ()) != DBL_MAX);
    ERR_OUT(cost_function(rand_vector_vector_double(3, 3), vector<double> (),     vector<double> ()) != DBL_MAX);
    ERR_OUT(cost_function(rand_vector_vector_double(3, 3), rand_vector_double(3), vector<double> ()) != DBL_MAX);
    ERR_OUT(cost_function(rand_vector_vector_double(3, 3), vector<double> (),     rand_vector_double(4)) != DBL_MAX);
    ERR_OUT(cost_function(vector<vector<double> > (),      rand_vector_double(2), rand_vector_double(3)) != DBL_MAX);
    ERR_OUT(cost_function(rand_vector_vector_double(3, 3), rand_vector_double(2), vector<double> ()) != DBL_MAX);
    ERR_OUT(cost_function(rand_vector_vector_double(3, 3), vector<double> (),     rand_vector_double(2)) != DBL_MAX);
    ERR_OUT(cost_function(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(32)) != DBL_MAX);
    ERR_OUT(cost_function(rand_vector_vector_double(3, 3), rand_vector_double(1), rand_vector_double(4)) != DBL_MAX);

    /* Jagged features */
    vector<vector<double> > jagged;
    jagged = rand_vector_vector_double(5, 2);
    jagged.push_back(rand_vector_double(4));
    jagged.push_back(rand_vector_double(2));
    jagged.push_back(rand_vector_double(5));

    ERR_OUT(cost_function(jagged, rand_vector_double(5), rand_vector_double(6)) != DBL_MAX);
    ERR_OUT(cost_function(jagged, rand_vector_double(3), rand_vector_double(4)) != DBL_MAX);

    /* Random success test case */
    ERR_OUT(cost_function(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(4)) == DBL_MAX);

    /* Custom success test case */
    vector<vector<double> > f;
    vector<double> y;
    vector<double> w;

    f.resize(3);
    f[0].push_back(1); f[0].push_back(2); f[0].push_back(3);
    f[1].push_back(4); f[1].push_back(5); f[1].push_back(6);
    f[2].push_back(7); f[2].push_back(8); f[2].push_back(9);

    y.push_back(33); y.push_back(3); y.push_back(4);

    w.push_back(2); w.push_back(3); w.push_back(4); w.push_back(10);

    ERR_OUT(cost_function(f, y, w) != 9325);

    return true;
}

/*
 * Test cost_function_weight_slope().
 * Return true if all test passed.
 * Return false if atleast one test failed.
 */
bool cost_function_weight_slope_test() {

    /* Jagged features */
    vector<vector<double> > jagged;
    jagged = rand_vector_vector_double(5, 2);
    jagged.push_back(rand_vector_double(4));
    jagged.push_back(rand_vector_double(2));
    jagged.push_back(rand_vector_double(5));

    /* Custom test case */
    vector<vector<double> > f;
    vector<double> y;
    vector<double> w;

    f.resize(3);
    f[0].push_back(1); f[0].push_back(2); f[0].push_back(3);
    f[1].push_back(4); f[1].push_back(5); f[1].push_back(6);
    f[2].push_back(7); f[2].push_back(8); f[2].push_back(9);

    y.push_back(33); y.push_back(3); y.push_back(4);

    w.push_back(2); w.push_back(3); w.push_back(4); w.push_back(10);

    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      vector<double> (),     vector<double> (),      1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      vector<double> (),     rand_vector_double(3),  1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      rand_vector_double(2), vector<double> (),      1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     vector<double> (),      1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), vector<double> (),      1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     rand_vector_double(4),  1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      rand_vector_double(2), rand_vector_double(3),  1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(2), vector<double> (),      1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     rand_vector_double(2),  1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(32), 1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(1), rand_vector_double(4),  1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_double(5), rand_vector_double(6),  1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_double(3), rand_vector_double(4),  1) != DBL_MAX);

    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      vector<double> (),     vector<double> (),      -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      vector<double> (),     rand_vector_double(3),  -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      rand_vector_double(2), vector<double> (),      -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     vector<double> (),      -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), vector<double> (),      -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     rand_vector_double(4),  -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      rand_vector_double(2), rand_vector_double(3),  -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(2), vector<double> (),      -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     rand_vector_double(2),  -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(32), -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(1), rand_vector_double(4),  -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_double(5), rand_vector_double(6),  -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_double(3), rand_vector_double(4),  -1) != DBL_MAX);

    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      vector<double> (),     vector<double> (),      100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      vector<double> (),     rand_vector_double(3),  100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      rand_vector_double(2), vector<double> (),      100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     vector<double> (),      100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), vector<double> (),      100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     rand_vector_double(4),  100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<double> > (),      rand_vector_double(2), rand_vector_double(3),  100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(2), vector<double> (),      100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), vector<double> (),     rand_vector_double(2),  100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(32), 100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(1), rand_vector_double(4),  100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_double(5), rand_vector_double(6),  100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_double(3), rand_vector_double(4),  100) != DBL_MAX);

    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(4), -1) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(4), 100) != DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(4), 4) != DBL_MAX);

    /* Success cases */
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(4), 0) == DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(4), 1) == DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(4), 2) == DBL_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_double(3, 3), rand_vector_double(3), rand_vector_double(4), 3) == DBL_MAX);

    /* Custom sucess case */
    ERR_OUT((int)cost_function_weight_slope(f, y, w, 3) != (int)((double)131/(double)f.size()) );
    ERR_OUT((int)cost_function_weight_slope(f, y, w, 1) != (int)((double)904/(double)f.size()) );
    ERR_OUT((int)cost_function_weight_slope(f, y, w, 2) != (int)((double)1035/(double)f.size()) );
    ERR_OUT((int)cost_function_weight_slope(f, y, w, 0) != (int)((double)773/(double)f.size()) );

    return true;
}

/*
 * Utility used in lr_core to test
 * perform_linear_regression with csv file input
 * Return true if test passed.
 * Return false if test failed.
 */
bool lr_core_csv_tests(string fname) {
    lr_input regression_input;
    weights regression_weights;
    weights result_weights;
    ERR_OUT(construct_input(regression_input, fname) != 0);

    /* Enough epochs to converge */
    regression_input.epoch = 5000000;

    double ret_val = perform_linear_regression(regression_input, result_weights, regression_weights);
    assert(ret_val == 0);

    ERR_OUT(cost_function(regression_input.features, regression_input.y, result_weights.w) > (double)0.1);
    return true;
}

/*
 * Test perform_linear_regression()
 * Return true if all tests passed.
 * Return false if atleast one test failed.
 */
bool lr_core_test() {
    
    weights rw;
    weights iw;
    /* Jagged features */
    vector<vector<double> > jagged;
    jagged = rand_vector_vector_double(5, 2);
    jagged.push_back(rand_vector_double(4));
    jagged.push_back(rand_vector_double(2));
    jagged.push_back(rand_vector_double(5));
    /* Custom test case */
    vector<vector<double> > f;
    vector<double> y;
    vector<double> w;

    f.resize(3);
    f[0].push_back(1); f[0].push_back(2); f[0].push_back(3);
    f[1].push_back(4); f[1].push_back(5); f[1].push_back(6);
    f[2].push_back(7); f[2].push_back(8); f[2].push_back(9);
    y.push_back(33); y.push_back(3); y.push_back(4);
    w.push_back(2); w.push_back(3); w.push_back(4); w.push_back(10);

    ERR_OUT(perform_linear_regression(lr_input(vector<vector<double> > (),      vector<double> ()),     rw, iw) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_double(3, 3), vector<double> ()),     rw, iw) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(vector<vector<double> > (),      rand_vector_double(4)), rw, iw) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(vector<vector<double> > (),      vector<double> ()),     rw, weights(34)) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_double(3, 3), vector<double> ()),     rw, weights(4)) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(vector<vector<double> > (),      rand_vector_double(4)), rw, weights(4)) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_double(3, 3), rand_vector_double(3)), rw, weights(66)) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_double(3, 3), rand_vector_double(3)), rw, weights(3)) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_double(3, 3), rand_vector_double(3)), rw, weights(2)) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_double(3, 3), rand_vector_double(4)), rw, weights(4)) != DBL_MAX);
    ERR_OUT(perform_linear_regression(lr_input(jagged,                         rand_vector_double(5)), rw, weights(6)) != DBL_MAX);

    /* Success cases */
    lr_input ip(f, y, 0.00001, 10);
    ERR_OUT(perform_linear_regression(ip, rw, weights(w)) != 0);
    ERR_OUT(rw.w.size() != f[0].size() + 1);
    double cost_before = cost_function(f, y, w);
    double cost_after = cost_function(f, y, rw.w);
    ERR_OUT(cost_after >= cost_before);
    ERR_OUT(perform_linear_regression(ip, rw, iw) != 0);
    ERR_OUT(rw.w.size() != f[0].size() + 1);

    /* Test with various csv files */
    /* Takes a very long time - Print updates */
   // ERR_OUT(lr_core_csv_tests("./test_45.csv") == false);
   // fprintf(stderr, "Update -    test_45.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_45_up.csv") == false);
   // fprintf(stderr, "Update -    test_45_up.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_45_down.csv") == false);
   // fprintf(stderr, "Update -    test_45_down.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_45_right.csv") == false);
   // fprintf(stderr, "Update -    test_45_right.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_45_left.csv") == false);
   // fprintf(stderr, "Update -    test_45_left.csv complete ...");

   // ERR_OUT(lr_core_csv_tests("./test_135.csv") == false);
   // fprintf(stderr, "Update -    test_135.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_135_up.csv") == false);
   // fprintf(stderr, "Update -    test_135_up.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_135_down.csv") == false);
   // fprintf(stderr, "Update -    test_135_down.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_135_right.csv") == false);
   // fprintf(stderr, "Update -    test_135_right.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_135_left.csv") == false);
   // fprintf(stderr, "Update -    test_135_left.csv complete ...");

   // ERR_OUT(lr_core_csv_tests("./test_225.csv") == false);
   // fprintf(stderr, "Update -    test_225.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_225_up.csv") == false);
   // fprintf(stderr, "Update -    test_225_up.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_225_down.csv") == false);
   // fprintf(stderr, "Update -    test_225_down.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_225_right.csv") == false);
   // fprintf(stderr, "Update -    test_225_right.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_225_left.csv") == false);
   // fprintf(stderr, "Update -    test_225_left.csv complete ...");

   // ERR_OUT(lr_core_csv_tests("./test_315.csv") == false);
   // fprintf(stderr, "Update -    test_315.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_315_up.csv") == false);
   // fprintf(stderr, "Update -    test_315_up.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_315_down.csv") == false);
   // fprintf(stderr, "Update -    test_315_down.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_315_right.csv") == false);
   // fprintf(stderr, "Update -    test_315_right.csv complete ...");
   // ERR_OUT(lr_core_csv_tests("./test_315_left.csv") == false);
   // fprintf(stderr, "Update -    test_315_left.csv complete ...");

    return true;
}

/*
 * Test construct_input function
 * Return true if all passed;
 * Return false if atleast one failed;
 */
bool construct_input_test() {
    lr_input ip;

    ERR_OUT(construct_input(ip, "no-file.txt") != -1);
    ERR_OUT(construct_input(ip, "./csv_fail_1.txt") != -1);
    ERR_OUT(construct_input(ip, "./csv_fail_2.txt") != -1);
    ERR_OUT(construct_input(ip, "./csv_fail_3.txt") != -1);
    ERR_OUT(construct_input(ip, "./csv_fail_4.txt") != -1);
    ERR_OUT(construct_input(ip, "./csv_fail_5.txt") != -1);

    /* Success cases */
    ERR_OUT(construct_input(ip, "./csv_success_1.txt") == -1);
    ERR_OUT(construct_input(ip, "./csv_success_2.txt") == -1);

    return true;
}

/*
 * Test construct_weights function
 * Return true if all passed
 * Return false if atleast one failed
 */
bool construct_weights_test() {
    weights w;

    ERR_OUT(construct_weights(w, "./no-file.txt") != -1);
    ERR_OUT(construct_weights(w, "./csv_weights_fail_1.txt") != -1);
    ERR_OUT(construct_weights(w, "./csv_weights_fail_2.txt") != -1);

    /* Success case */
    ERR_OUT(construct_weights(w, "./csv_weights_success_1.txt") == -1);
    ERR_OUT(construct_weights(w, "./csv_weights_success_2.txt") == -1);

    return true;
}

void print_result(bool hypothesis_success, 
                  bool cost_function_success,
                  bool cost_function_weight_slope_success,
                  bool lr_core_success,
                  bool construct_input_success,
                  bool construct_weights_success) {

    string hypothesis_test_str = hypothesis_success ? "Success" : "Fail";
    string cost_function_test_str = cost_function_success? "Success" : "Fail";
    string cost_function_weight_slope_test_str = cost_function_weight_slope_success? "Success" : "Fail";
    string lr_core_test_str = lr_core_success? "Success" : "Fail";
    string construct_input_test_str = construct_input_success ? "Success" : "Fail";
    string construct_weights_test_str = construct_weights_success ? "Success" : "Fail";

    cout<<"hypthesis()                  - "<<hypothesis_test_str<<endl;
    cout<<"cost_function()              - "<<cost_function_test_str<<endl;
    cout<<"cost_function_weight_slope() - "<<cost_function_weight_slope_test_str<<endl;
    cout<<"perform_linear_regression()  - "<<lr_core_test_str<<endl; 
    cout<<"construct_input()            - "<<construct_input_test_str<<endl;
    cout<<"construct_weights()          - "<<construct_weights_test_str<<endl;
}

int main() {
    /* Print result */
    print_result(hypothesis_test(),
                 cost_function_test(),
                 cost_function_weight_slope_test(),
                 lr_core_test(),
                 construct_input_test(),
                 construct_weights_test());
    return 0;
}

// Unit test file for linear_regression

#include <iostream>
#include <string>
#include <cfloat>
#include <ctime>
#include <cstdlib>
using namespace std;

#include "../../linear_regression/linear_regression_nd/lr.hpp"
#include "../../linear_regression/linear_regression_nd/lr_utils.hpp"

#define ERR_OUT(stmt) if(stmt) { cerr<<"Error out - "<<__FILE__<<" - "<<__LINE__<<endl;  return false; }

/* Utility */
vector<vector<float> > rand_vector_vector_float(int width, int height) {
    assert(width > 0);
    assert(height > 0);
    
    srand(time(NULL));

    vector<vector<float> > ret_vec;
    for(int row = 0; row < height; row++) {
        ret_vec.push_back(vector<float> ());
        for(int col = 0; col < width; col++) {
            ret_vec[row].push_back(rand() % 100);
        }
    }
    return ret_vec;
}

vector<float> rand_vector_float(int width) {
    assert(width > 0);

    srand(time(NULL));

    vector<float> ret_vec;
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
    ERR_OUT(hypothesis(vector<float> (), vector<float> ()) != FLT_MAX);
    ERR_OUT(hypothesis(vector<float> (), rand_vector_float(3)) != FLT_MAX);
    ERR_OUT(hypothesis(rand_vector_float(3), vector<float>()) != FLT_MAX);
    ERR_OUT(hypothesis(rand_vector_float(3), rand_vector_float(2)) != FLT_MAX);
    ERR_OUT(hypothesis(rand_vector_float(3), rand_vector_float(3)) != FLT_MAX);

    // Success case
    ERR_OUT(hypothesis(rand_vector_float(2), rand_vector_float(3)) == FLT_MAX);

    // Custom test case
    vector<float> feature_vector;
    vector<float> w;

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

    ERR_OUT(cost_function(vector<vector<float> > (),      vector<float> (),     vector<float> ()) != FLT_MAX);
    ERR_OUT(cost_function(vector<vector<float> > (),      vector<float> (),     rand_vector_float(3)) != FLT_MAX);
    ERR_OUT(cost_function(vector<vector<float> > (),      rand_vector_float(2), vector<float> ()) != FLT_MAX);
    ERR_OUT(cost_function(rand_vector_vector_float(3, 3), vector<float> (),     vector<float> ()) != FLT_MAX);
    ERR_OUT(cost_function(rand_vector_vector_float(3, 3), rand_vector_float(3), vector<float> ()) != FLT_MAX);
    ERR_OUT(cost_function(rand_vector_vector_float(3, 3), vector<float> (),     rand_vector_float(4)) != FLT_MAX);
    ERR_OUT(cost_function(vector<vector<float> > (),      rand_vector_float(2), rand_vector_float(3)) != FLT_MAX);
    ERR_OUT(cost_function(rand_vector_vector_float(3, 3), rand_vector_float(2), vector<float> ()) != FLT_MAX);
    ERR_OUT(cost_function(rand_vector_vector_float(3, 3), vector<float> (),     rand_vector_float(2)) != FLT_MAX);
    ERR_OUT(cost_function(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(32)) != FLT_MAX);
    ERR_OUT(cost_function(rand_vector_vector_float(3, 3), rand_vector_float(1), rand_vector_float(4)) != FLT_MAX);

    /* Jagged features */
    vector<vector<float> > jagged;
    jagged = rand_vector_vector_float(5, 2);
    jagged.push_back(rand_vector_float(4));
    jagged.push_back(rand_vector_float(2));
    jagged.push_back(rand_vector_float(5));

    ERR_OUT(cost_function(jagged, rand_vector_float(5), rand_vector_float(6)) != FLT_MAX);
    ERR_OUT(cost_function(jagged, rand_vector_float(3), rand_vector_float(4)) != FLT_MAX);

    /* Random success test case */
    ERR_OUT(cost_function(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(4)) == FLT_MAX);

    /* Custom success test case */
    vector<vector<float> > f;
    vector<float> y;
    vector<float> w;

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
    vector<vector<float> > jagged;
    jagged = rand_vector_vector_float(5, 2);
    jagged.push_back(rand_vector_float(4));
    jagged.push_back(rand_vector_float(2));
    jagged.push_back(rand_vector_float(5));

    /* Custom test case */
    vector<vector<float> > f;
    vector<float> y;
    vector<float> w;

    f.resize(3);
    f[0].push_back(1); f[0].push_back(2); f[0].push_back(3);
    f[1].push_back(4); f[1].push_back(5); f[1].push_back(6);
    f[2].push_back(7); f[2].push_back(8); f[2].push_back(9);

    y.push_back(33); y.push_back(3); y.push_back(4);

    w.push_back(2); w.push_back(3); w.push_back(4); w.push_back(10);

    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      vector<float> (),     vector<float> (),      1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      vector<float> (),     rand_vector_float(3),  1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      rand_vector_float(2), vector<float> (),      1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     vector<float> (),      1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), vector<float> (),      1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     rand_vector_float(4),  1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      rand_vector_float(2), rand_vector_float(3),  1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(2), vector<float> (),      1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     rand_vector_float(2),  1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(32), 1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(1), rand_vector_float(4),  1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_float(5), rand_vector_float(6),  1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_float(3), rand_vector_float(4),  1) != FLT_MAX);

    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      vector<float> (),     vector<float> (),      -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      vector<float> (),     rand_vector_float(3),  -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      rand_vector_float(2), vector<float> (),      -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     vector<float> (),      -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), vector<float> (),      -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     rand_vector_float(4),  -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      rand_vector_float(2), rand_vector_float(3),  -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(2), vector<float> (),      -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     rand_vector_float(2),  -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(32), -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(1), rand_vector_float(4),  -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_float(5), rand_vector_float(6),  -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_float(3), rand_vector_float(4),  -1) != FLT_MAX);

    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      vector<float> (),     vector<float> (),      100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      vector<float> (),     rand_vector_float(3),  100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      rand_vector_float(2), vector<float> (),      100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     vector<float> (),      100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), vector<float> (),      100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     rand_vector_float(4),  100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(vector<vector<float> > (),      rand_vector_float(2), rand_vector_float(3),  100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(2), vector<float> (),      100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), vector<float> (),     rand_vector_float(2),  100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(32), 100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(1), rand_vector_float(4),  100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_float(5), rand_vector_float(6),  100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(jagged,                         rand_vector_float(3), rand_vector_float(4),  100) != FLT_MAX);

    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(4), -1) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(4), 100) != FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(4), 4) != FLT_MAX);

    /* Success cases */
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(4), 0) == FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(4), 1) == FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(4), 2) == FLT_MAX);
    ERR_OUT(cost_function_weight_slope(rand_vector_vector_float(3, 3), rand_vector_float(3), rand_vector_float(4), 3) == FLT_MAX);

    /* Custom sucess case */
    ERR_OUT((int)cost_function_weight_slope(f, y, w, 3) != (int)((float)131/(float)f.size()) );
    ERR_OUT((int)cost_function_weight_slope(f, y, w, 1) != (int)((float)904/(float)f.size()) );
    ERR_OUT((int)cost_function_weight_slope(f, y, w, 2) != (int)((float)1035/(float)f.size()) );
    ERR_OUT((int)cost_function_weight_slope(f, y, w, 0) != (int)((float)773/(float)f.size()) );

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
    vector<vector<float> > jagged;
    jagged = rand_vector_vector_float(5, 2);
    jagged.push_back(rand_vector_float(4));
    jagged.push_back(rand_vector_float(2));
    jagged.push_back(rand_vector_float(5));
    /* Custom test case */
    vector<vector<float> > f;
    vector<float> y;
    vector<float> w;

    f.resize(3);
    f[0].push_back(1); f[0].push_back(2); f[0].push_back(3);
    f[1].push_back(4); f[1].push_back(5); f[1].push_back(6);
    f[2].push_back(7); f[2].push_back(8); f[2].push_back(9);
    y.push_back(33); y.push_back(3); y.push_back(4);
    w.push_back(2); w.push_back(3); w.push_back(4); w.push_back(10);

    ERR_OUT(perform_linear_regression(lr_input(vector<vector<float> > (),      vector<float> ()),     rw, iw) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_float(3, 3), vector<float> ()),     rw, iw) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(vector<vector<float> > (),      rand_vector_float(4)), rw, iw) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(vector<vector<float> > (),      vector<float> ()),     rw, weights(34)) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_float(3, 3), vector<float> ()),     rw, weights(4)) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(vector<vector<float> > (),      rand_vector_float(4)), rw, weights(4)) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_float(3, 3), rand_vector_float(3)), rw, weights(66)) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_float(3, 3), rand_vector_float(3)), rw, weights(3)) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_float(3, 3), rand_vector_float(3)), rw, weights(2)) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(rand_vector_vector_float(3, 3), rand_vector_float(4)), rw, weights(4)) != FLT_MAX);
    ERR_OUT(perform_linear_regression(lr_input(jagged,                         rand_vector_float(5)), rw, weights(6)) != FLT_MAX);

    /* Success cases */
    lr_input ip(f, y, 0.00001, 10);
    ERR_OUT(perform_linear_regression(ip, rw, weights(w)) != 0);
    ERR_OUT(rw.w.size() != f[0].size() + 1);
    float cost_before = cost_function(f, y, w);
    float cost_after = cost_function(f, y, rw.w);
    ERR_OUT(cost_after >= cost_before);

    ERR_OUT(perform_linear_regression(ip, rw, iw) != 0);
    ERR_OUT(rw.w.size() != f[0].size() + 1);


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
    //ERR_OUT(construct_weights(w, "./csv_weights_fail_1.txt") != -1);
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

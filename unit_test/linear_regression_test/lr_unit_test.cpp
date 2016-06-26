// Unit test file for linear_regression

#include <iostream>
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

int main() {
    bool hypothesis_test_success;

    /* Unit test hypothesis function */
    hypothesis_test_success = hypothesis_test();


    cout<<"hypothesis test - "<<hypothesis_test_success<<endl;

    return 0;
}

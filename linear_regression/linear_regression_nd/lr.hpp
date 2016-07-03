#ifndef __LR_HPP__
#define __LR_HPP__

/* This file declares the functions to use
 * linear regression code;
 */

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cassert>
using namespace std;

typedef struct weights {
    /* Vector of weights; The offset/bias
     * is the very last value
     */
    vector<float> w;

    weights() {
        w.clear();
    }

    weights(int n) {
        /* Random */
        srand(time(NULL));
        for(int wt_iter = 0; wt_iter < n; wt_iter++) {
           w.push_back(rand() % 100);
        }
    }
    weights(vector<float> w_) {
        w = w_;
    }

    bool is_empty() const {
        if(w.size() == 0) { return true; }
        else              { return false; }
    }

    void clear_weights() {
        w.clear();
    }
}weights; 

typedef struct lr_input {
    /* feature points - x0, x1, x2 ... 
     * This is a vector of feature vectors
     */
    vector<vector<float> > features;
    /* Actual values - y */
    vector<float> y;

    /* Learning parameters */
    float learning_rate;
    int epoch;
    
    lr_input() {
        /* def values */
        learning_rate = 0.00001;
        epoch = 10000;
    }
    
    lr_input(vector<vector<float> > features_, vector<float> y_, float learning_rate_, int epoch_) {
        /* Make copies of the inputs vectors */
        features = features_;
        y = y_;
        learning_rate = learning_rate_;
        epoch = epoch_;
    }

    lr_input(vector<vector<float> > features_, vector<float> y_) {
        /* Make copies of the input vectors */
        features = features_;
        y = y_;
        learning_rate = 0.00001;
        epoch = 10000;
    }
}lr_input;

/*
 * Construct lr_input from a given csv file;
 * Arguments :
 *      ip        - Result object that is filled
 *      filename  - Name of the csv file
 * Returns 0 for success
 * Returns -1 for failure
 */
int construct_input(lr_input &ip, const string filename); 

/*
 * Construct weights from a given csv file;
 * Arguments :
 *      w        - Result object that is filled
 *      filename - Name of the csv file
 * Returns 0 for success
 * Returns -1 for success
 */
int construct_weights(weights &w, const string filename);

/*
 * Perform linear regression for the given data points;
 * Arguments : 
 *      input        - Contains valid feature vectors and expected outputs
 *      result       - The output weights are filled here; The function
 *                     clears the contents of result weights before use
 *      init_weights - Initialization weights; if vector in init_weights
 *                     is empty; then weights are randomly initialized 
 * Return FLT_MAX incase of error;
 * Return 0 incase of success;
 */
float perform_linear_regression(const lr_input &input, weights &result, const weights &init_weights);

#endif // __LR_HPP__

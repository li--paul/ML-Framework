#ifndef __LR_HPP__
#define __LR_HPP__

/* This file declares the functions to use
 * linear regression code;
 */

#include <iostream>
#include <vector>
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
    weights(int n, vector<float> w_) {
        assert(w_.size() == n);
        w = w_;
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
        features = features_;
        y = y_;
        learning_rate = learning_rate_;
        epoch = epoch_;
    }

    lr_input(vector<vector<float> > features_, vector<float> y_) {
        features = features_;
        y = y_;
        learning_rate = 0.00001;
        epoch = 10000;
    }
}lr_input;

/*
 * Perform linear regression for the given data points;
 * Return FLT_MAX incase of error;
 * Return 0 incase of success;
 */
float perform_linear_regression(const lr_input &input, weights &result, const weights &init_weights);

#endif // __LR_HPP__

#ifndef __LR_UTILS__
#define __LR_UTILS__

/* 
 * This file declares the utility
 * functions to be defined in lr.cpp
 */
#include <iostream>
#include <vector>
using namespace std;

/*
 * Given features and weights, evaluate the hypothesis function.
 * Return FLT_MAX in case of error.
 * Any other value is considered success (hypothesis result).
 */
float hypothesis(const vector<float> &feature_vector, const vector<float> &w);


/* 
 * Calculate the cost of using current weights.
 * Return FLT_MAX in case of error.
 * Any other value is considered success (Cost).
 */
float cost_function(const vector<vector<float> > &features,
                    const vector<float> &y,
                    const vector<float> &w);

/* 
 * Calculate the slope of cost function w.r.t. a particular weight.
 * Return FLT_MAX in case of error.
 * Any other value is considered a Success (slope).
 */
float cost_function_weight_slope(   const vector<vector<float> > &features,
                                    const vector<float> &y,
                                    const vector<float> &w,
                                    const int weight_index);
        

#endif // __LR_UTILS__

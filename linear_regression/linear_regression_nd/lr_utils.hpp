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
 * TODO : Make the return value -1
 * Given features and weights, evaluate the hypothesis function.
 * Return DBL_MAX in case of error.
 * Any other value is considered success (hypothesis result).
 */
double hypothesis(const vector<double> &feature_vector, const vector<double> &w);


/* 
 * TODO : Make the return value -1 incase of error!
 * Calculate the cost of using current weights.
 * Return DBL_MAX in case of error.
 * Any other value is considered success (Cost).
 */
double cost_function(const vector<vector<double> > &features,
                    const vector<double> &y,
                    const vector<double> &w);

/* 
 * TODO : Do something about its return value
 * Calculate the slope of cost function w.r.t. a particular weight.
 * Return DBL_MAX in case of error.
 * Any other value is considered a Success (slope).
 */
double cost_function_weight_slope(   const vector<vector<double> > &features,
                                    const vector<double> &y,
                                    const vector<double> &w,
                                    const int weight_index);
        

#endif // __LR_UTILS__

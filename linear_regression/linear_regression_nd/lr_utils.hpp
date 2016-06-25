#ifndef __LR_UTILS__
#define __LR_UTILS__

/* 
 * This file declares the utility
 * functions to be defined in lr.cpp
 */
#include <iostream>
#include <vector>
using namespace std;

/* Given feature and weights, evaluate the hypothesis function */
float hypothesis(const vector<float> &feature_vector, const vector<float> &w);

/* Calculate the cost of using current weights */ 
float cost_function(const vector<vector<float> > &features,
                    const vector<float> &y,
                    const vector<float> &w);

/* Calculate the slope of cost function w.r.t.
 * a particular weight.
 */
float cost_function_weight_slope(   const vector<vector<float> > &features,
                                    const vector<float> &y,
                                    const vector<float> &w,
                                    const int weight_index);
        

#endif // __LR_UTILS__

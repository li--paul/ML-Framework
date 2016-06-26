
/* Defines functions in lr.hpp */

#include "lr.hpp"
#include "lr_utils.hpp"
#include <cassert>
#include <cfloat>

#define DEBUG
#define ERR_OUT(stmt) if(stmt) { cerr<<"Error out - "<<__FILE__<<" - "<<__LINE__<<endl;  return FLT_MAX; }

/*
 * Given features and weights, evaluate the hypothesis function.
 * Return FLT_MAX in case of error.
 * Any other value is considered success (hypothesis result).
 */
float hypothesis(const vector<float> &feature_vector, const vector<float> &w) {
    /* Weights vector will have one extra element (bias) */
    ERR_OUT(feature_vector.size() != w.size() - 1);

    float output = 0;
    for(unsigned int feature_iter = 0; feature_iter < feature_vector.size(); feature_iter++) {
        output += feature_vector[feature_iter] * w[feature_iter];
    }
    output += w[w.size() - 1];
    return output;
} 

/* 
 * Calculate the cost of using current weights.
 * Return FLT_MAX in case of error.
 * Any other value is considered success (Cost).
 */
float cost_function(const vector<vector<float> > &features,
                    const vector<float> &y,
                    const vector<float> &w) {
    ERR_OUT(features[0].size() != w.size() - 1);
    ERR_OUT(features.size() != y.size());
    int n = features.size();
    float cost = 0;

    /*
     * Note : n is the number of rows in dataset
     * Note : hypothesis h(x0, x1, ... xk) = w0 * x0 + w1 * x1 ... + wk * xk + bias
     * Cost function : summation over all inputs( (h(x0, x1 ...xk) - y) * (h(x0, x1 ...xk) - y) )
     */
    for(int iter = 0; iter < n; iter++) {
        assert(features[iter].size == w.size() - 1);

        float hypothesis_result = hypothesis(features[iter], w);
        assert(hypothesis_result != FLT_MAX);
        cost += (hypothesis_result - y[iter]) *
                (hypothesis_result - y[iter]); 
    } 
    
    return cost;
}

/* 
 * Calculate the slope of cost function w.r.t. a particular weight.
 * Return FLT_MAX in case of error.
 * Any other value is considered a Success (slope).
 */
float cost_function_weight_slope(   const vector<vector<float> > &features,
                                    const vector<float> &y,
                                    const vector<float> &w,
                                    const int weight_index) {
    ERR_OUT(features[0].size() != w.size() - 1);
    ERR_OUT(features.size() != y.size());
    ERR_OUT(weight_index >= w.size());
    ERR_OUT(weight_index < 0);

    /*
     * Note : n is the number of input points
     * Note : hypothesis h(x0, x1, ... xk) = w0 * x0 + w1 * x1 ... + wk * xk + bias
     * Cost function derivative w.r.t. w(i) (i <= k) : [ (1/n) * summation over all inputs( (h(x0, x1, ... xk)  - y) *  xi) ]
     * Cost function derivative w.r.t. bias : [ (1/n) * summation over all inputs(h(x0, x1, ... xk)  - y) ]
     *
     */
    int n = features.size();
    int this_weight_is_not_bias = ((unsigned long)weight_index < w.size() - 1);
    float slope = 0;

    for(int iter = 0; iter < n; iter++) {
        assert(features[iter].size == w.size() - 1);

        float hypothesis_result = hypothesis(features[iter], w);
        assert(hypothesis_result != FLT_MAX);
        slope += (hypothesis_result - y[iter]) * 
                 (features[iter][weight_index] * this_weight_is_not_bias); 
    } 
    
    return (slope * ((float)1 / (float)(n)));
}

/*
 * Perform linear regression for the given data points;
 * Return FLT_MAX incase of error;
 * Return 0 incase of success;
 */
float perform_linear_regression(  const lr_input &input,
                                  weights &result_weights,
                                  const weights &init_weights) {
    /* Make sure feature's x and y dim are not empty */
    ERR_OUT(input.features.size() == 0);
    ERR_OUT(input.features[0].size() == 0);
    ERR_OUT(input.features.size() != input.y.size());
    /* Check all feature vectors are of same length */
    int num_features = input.features[0].size();
    for(unsigned int feature_iter = 0; feature_iter < input.features.size(); feature_iter++) {
        ERR_OUT(input.features[feature_iter].size() != num_features);
    }

    /* If the w in init_weights is empty !! then the user
     * did not pass any initial weights.
     */
    if(init_weights.w.size() == 0) {
        /* Initialize Random weights */
        result_weights = weights(num_features + 1);
    } else {
        result_weights = init_weights;
    }

    /* Use references from here on */
    vector<float> &w = result_weights.w;
    const vector<float> &y = input.y;
    const vector<vector<float> > &features = input.features;
    
    int iter = 0;
    while(iter < input.epoch) {
        vector<float> weight_slopes;
        for(unsigned int weight_iter = 0; weight_iter < w.size(); weight_iter++) {
            float slope = cost_function_weight_slope( features, y, w, weight_iter);
            assert(slope != FLT_MAX);
            slope *= input.learning_rate;
            weight_slopes.push_back(slope);
        }
        assert(weight_slopes.size() == w.size());
        /* Update weights */
        for(unsigned int weight_iter = 0; weight_iter < w.size(); weight_iter++) {
            w[iter] -= weight_slopes[iter];
        }

        iter++;

#ifdef DEBUG
        cerr<<"epoch - "<<iter<<" | "<< "cost - "<<cost_function(features, y, w)<<endl;
#endif
    }
    
    return 0;
}

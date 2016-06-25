
/* Defines functions in lr.hpp */

#include "lr.hpp"
#include <cassert>
#define DEBUG

float hypothesis(const vector<float> &feature_vector, const vector<float> &w) {
    /* Weights vector will have one extra element (bias) */
    assert(feature_vector.size() == w.size() - 1);
    float output = 0;
    for(unsigned int feature_iter = 0; feature_iter < feature_vector.size(); feature_iter++) {
        output += feature_vector[feature_iter] * w[feature_iter];
    }
    output += w[w.size() - 1];
    return output;
} 

float cost_function(const vector<vector<float> > &features,
                    const vector<float> &y,
                    const vector<float> &w) {
    assert(features[0].size() == w.size() - 1);
    assert(features.size() == y.size());
    int n = features.size();
    float cost = 0;

    /*
     * Note : n is the number of input points
     * Note : hypothesis h(x0, x1, ... xk) = w0 * x0 + w1 * x1 ... + wk * xk + bias
     * Cost function : summation over all inputs( (h(x0, x1 ...xk) - y) * (h(x0, x1 ...xk) - y) )
     */
    for(int iter = 0; iter < n; iter++) {
        float hypothesis_result = hypothesis(features[iter], w);
        cost += (hypothesis_result - y[iter]) *
                (hypothesis_result - y[iter]); 
    } 
    
    return cost;
}

float cost_function_weight_slope(   const vector<vector<float> > &features,
                                    const vector<float> &y,
                                    const vector<float> &w,
                                    const int weight_index) {
    assert(features[0].size() == w.size() - 1);
    assert(features.size() == y.size());
    assert(weight_index < w.size());

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
        slope += (hypothesis(features[iter], w) - y[iter]) * 
                 (features[iter][weight_index] * this_weight_is_not_bias); 
    } 
    
    return (slope * ((float)1 / (float)(n)));
}

weights perform_linear_regression(  lr_input input,
                                    weights init_weights) {
    /* Make sure feature's x and y dim are not empty */
    assert(input.features.size() != 0);
    assert(input.features[0].size() != 0);
    assert(input.features.size() == input.y.size());
    /* Check all feature vectors are of same length */
    int num_features = input.features[0].size();
    for(unsigned int feature_iter = 0; feature_iter < input.features.size(); feature_iter++) {
        assert(input.features[feature_iter].size() == num_features);
    }

    /* If the w in init_weights is empty !! then the user
     * did not pass any initial weights.
     */
    if(init_weights.w.size() == 0) {
        /* Initialize Random weights */
        init_weights = weights(num_features + 1);
    }

    /* Use references from here on */
    vector<float> &w = init_weights.w;
    vector<float> &y = input.y;
    vector<vector<float> > &features = input.features;
    
    int iter = 0;
    while(iter < input.epoch) {
        vector<float> weight_slopes;
        for(unsigned int weight_iter = 0; weight_iter < w.size(); weight_iter++) {
            float slope = cost_function_weight_slope( features, y, w, weight_iter) * input.learning_rate;
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
    
    return init_weights;
}

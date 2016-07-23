
/* Defines functions in lr.hpp */

#include "lr.hpp"
#include "lr_utils.hpp"
#include "./../../utils/csv_helper.hpp"
#include <cassert>
#include <cfloat>
#include <cstdio>

#define ERR_OUT(stmt) if(stmt) { cerr<<"Error out - "<<__FILE__<<" - "<<__LINE__<<endl;  return DBL_MAX; }

/* Define utilities first */
/*
 * Split the csv data vector into features and expected outputs;
 */
void split_csv_vector(vector<vector<double> > &inputs, vector<vector<double> > &f, vector<double> &y) {
    unsigned int num_rows = inputs.size();
    assert(num_rows != 0);
    unsigned int num_cols = inputs[0].size();
    assert(num_cols != 0);
    
    /*
     * TODO : assert f and y to be empty
     */

    /* Fill expected outputs vector */
    for(unsigned int row = 0; row < num_rows; row++) {
        y.push_back(inputs[row][num_cols - 1]);
    }

    /* Fill features matrix */
    for(unsigned int row = 0; row < num_rows; row++) {
        vector<double> feature_vector;
        for(unsigned int col = 0; col < num_cols - 1; col++) {
            feature_vector.push_back(inputs[row][col]);
        }
        f.push_back(feature_vector);
    }
}

/*
 * Construct lr_input from a given csv file;
 * Arguments :
 *      ip        - Result object that is filled
 *      filename  - Name of the csv file
 * Returns 0 for success
 * Returns -1 for failure
 */
int construct_input(lr_input &ip, const string filename) {
    vector<vector<double> > inputs;
    if(csv_vec_vec_double(inputs, filename) != 0) {
        cerr<<"CSV to vector<vector<double> > issue "<<__FILE__<<" "<<__LINE__<<endl;
        return -1;
    }
    if(inputs.size() == 0) {
        cerr<<"Inputs are empty "<<__FILE__<<" "<<__LINE__<<endl;
        return -1;
    }

    unsigned int data_rows = inputs.size();
    unsigned int data_cols = inputs[0].size();

    /* Gotta have atleast one feature and one output */
    if(data_cols < 2) {
        cerr<<"No y in inputs "<<__FILE__<<" "<<__LINE__<<endl;
        return -1;
    }

    /* Check for jagged matrix */
    for(unsigned int row = 0; row < data_rows; row++) {
        if(inputs[row].size() != data_cols) {
            cerr<<"Inputs are jagged "<<__FILE__<<" "<<__LINE__<<endl;
            return -1;
        }
    }

    vector<vector<double> > features;
    vector<double> y;
    split_csv_vector(inputs, features, y);
    assert(features.size() == y.size());
    assert(features.size() != 0);

    ip = lr_input(features, y);

    return 0;
}

/*
 * Construct weights from a given csv file;
 * Arguments :
 *      w        - Result object that is filled
 *      filename - Name of the csv file
 * Returns 0 for success
 * Returns -1 for success
 */
int construct_weights(weights &w, const string filename) {
    vector<vector<double> > w_csv;
    if(csv_vec_vec_double(w_csv, filename) != 0) {
        cerr<<"CSV to vector<vector<double> > issue "<<__FILE__<<" "<<__LINE__<<endl;
        return -1;
    }
    if(w_csv.size() != 1 && w_csv.size() != 0) {
        cerr<<"Error in weights csv "<<__FILE__<<" "<<__LINE__<<endl;
        return -1;
    }
    if(w_csv.size() == 1) {
        w = weights(w_csv[0]);
    }
    return 0;
}

/*
 * Given features and weights, evaluate the hypothesis function.
 * Return DBL_MAX in case of error.
 * Any other value is considered success (hypothesis result).
 */
double hypothesis(const vector<double> &feature_vector, const vector<double> &w) {
    /* Weights vector will have one extra element (bias) */
    ERR_OUT(feature_vector.size() != w.size() - 1);

    double output = 0;
    for(unsigned int feature_iter = 0; feature_iter < feature_vector.size(); feature_iter++) {
        output += feature_vector[feature_iter] * w[feature_iter];
    }
    output += w[w.size() - 1];
    return output;
} 

/* 
 * Calculate the cost of using current weights.
 * Return DBL_MAX in case of error.
 * Any other value is considered success (Cost).
 */
double cost_function(const vector<vector<double> > &features,
                    const vector<double> &y,
                    const vector<double> &w) {
    ERR_OUT(features.size() == 0);
    ERR_OUT(y.size() == 0);
    ERR_OUT(w.size() == 0);
    ERR_OUT(features[0].size() != w.size() - 1);
    ERR_OUT(features.size() != y.size());
    int n = features.size();
    double cost = 0;

    for(int iter = 0; iter < n; iter++) {
        ERR_OUT(features[iter].size() != w.size() - 1);
    }

    /*
     * Note : n is the number of rows in dataset
     * Note : hypothesis h(x0, x1, ... xk) = w0 * x0 + w1 * x1 ... + wk * xk + bias
     * Cost function : summation over all inputs( (h(x0, x1 ...xk) - y) * (h(x0, x1 ...xk) - y) )
     */
    for(int iter = 0; iter < n; iter++) {
        double hypothesis_result = hypothesis(features[iter], w);
        assert(hypothesis_result != DBL_MAX);
        cost += (hypothesis_result - y[iter]) *
                (hypothesis_result - y[iter]); 
    } 
    
    return cost;
}

/* 
 * Calculate the slope of cost function w.r.t. a particular weight.
 * Return DBL_MAX in case of error.
 * Any other value is considered a Success (slope).
 */
double cost_function_weight_slope(   const vector<vector<double> > &features,
                                    const vector<double> &y,
                                    const vector<double> &w,
                                    const int weight_index) {
    ERR_OUT(features.size() == 0);
    ERR_OUT(y.size() == 0);
    ERR_OUT(w.size() == 0);
    ERR_OUT(features[0].size() != w.size() - 1);
    ERR_OUT(features.size() != y.size());
    ERR_OUT(weight_index >= w.size());
    ERR_OUT(weight_index < 0);

    for(unsigned int iter = 0; iter < features.size(); iter++) {
        ERR_OUT(features[iter].size() != w.size() - 1);
    }

    /*
     * Note : n is the number of input points
     * Note : hypothesis h(x0, x1, ... xk) = w0 * x0 + w1 * x1 ... + wk * xk + bias
     * Cost function derivative w.r.t. w(i) (i <= k) : [ (1/n) * summation over all inputs( (h(x0, x1, ... xk)  - y) *  xi) ]
     * Cost function derivative w.r.t. bias : [ (1/n) * summation over all inputs(h(x0, x1, ... xk)  - y) ]
     *
     */
    int n = features.size();
    int this_weight_is_not_bias = ((unsigned long)weight_index < w.size() - 1);
    double slope = 0;

    for(int iter = 0; iter < n; iter++) {
        double hypothesis_result = hypothesis(features[iter], w);
        assert(hypothesis_result != DBL_MAX);
        slope += (hypothesis_result - y[iter]) * 
                 ((features[iter][weight_index] * this_weight_is_not_bias) + (1 * !this_weight_is_not_bias)); 
    } 
    
    return (slope * ((double)1 / (double)(n)));
}

/*
 * Perform linear regression for the given data points;
 * Return DBL_MAX incase of error;
 * Return 0 incase of success;
 */
double perform_linear_regression(  const lr_input &input,
                                  weights &result_weights,
                                  const weights &init_weights) {
    /* Make sure feature's x and y dim are not empty */
    ERR_OUT(input.features.size() == 0);
    ERR_OUT(input.features[0].size() == 0);
    ERR_OUT(input.features.size() != input.y.size());
    ERR_OUT(!(init_weights.w.size() == 0) &&
            (init_weights.w.size() != input.features[0].size() + 1));
    /* Check all feature vectors are of same length */
    int num_features = input.features[0].size();
    for(unsigned int feature_iter = 0; feature_iter < input.features.size(); feature_iter++) {
        ERR_OUT(input.features[feature_iter].size() != num_features);
    }

    /* Clear weights in result_weights if any */
    result_weights.clear_weights();

    /* If the w in init_weights is empty !! then the user
     * did not pass any initial weights.
     */
    if(init_weights.is_empty()) {
        /* Initialize Random weights */
        result_weights.w = weights(num_features + 1).w;
    } else {
        result_weights.w = init_weights.w;
    }

    /* Use references from here on */
    vector<double> &w = result_weights.w;
    const vector<double> &y = input.y;
    const vector<vector<double> > &features = input.features;


    int iter = 0;
    while(iter < input.epoch) {
        vector<double> weight_slopes;
        for(unsigned int weight_iter = 0; weight_iter < w.size(); weight_iter++) {
            double slope = cost_function_weight_slope( features, y, w, weight_iter);
            assert(slope != DBL_MAX);
            slope *= input.learning_rate;
            weight_slopes.push_back(slope);
        }
        assert(weight_slopes.size() == w.size());
        /* Update weights */
        for(unsigned int weight_iter = 0; weight_iter < w.size(); weight_iter++) {
            w[weight_iter] -= weight_slopes[weight_iter];
        }
#ifdef DEBUG
        cerr<<"epoch - "<<iter<<" | "<< "cost - "<<cost_function(features, y, w)<<endl;
#endif
        iter++;
    }

    return 0;
}


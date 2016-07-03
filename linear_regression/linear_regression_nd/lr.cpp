
/* Defines functions in lr.hpp */

#include "lr.hpp"
#include "lr_utils.hpp"
#include "./../../utils/csv_helper.hpp"
#include <cassert>
#include <cfloat>
#include <cstdio>

#define ERR_OUT(stmt) if(stmt) { cerr<<"Error out - "<<__FILE__<<" - "<<__LINE__<<endl;  return FLT_MAX; }

#ifdef WEB_RUN
// Dump data every 50th epoch
#define DUMP_FREQUENCY 50
#endif

/* Define utilities first */
/*
 * Split the csv data vector into features and expected outputs;
 */
void split_csv_vector(vector<vector<float> > &inputs, vector<vector<float> > &f, vector<float> &y) {
    unsigned int num_rows = inputs.size();
    assert(num_rows != 0);
    unsigned int num_cols = inputs[0].size();
    assert(num_cols != 0);

    /* Fill expected outputs vector */
    for(unsigned int row = 0; row < num_rows; row++) {
        y.push_back(inputs[row][num_cols - 1]);
    }

    /* Fill features matrix */
    for(unsigned int row = 0; row < num_rows; row++) {
        vector<float> feature_vector;
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
    vector<vector<float> > inputs;
    if(csv_vec_vec_float(inputs, filename) != 0) {
        cerr<<"CSV to vector<vector<float> > issue "<<__FILE__<<" "<<__LINE__<<endl;
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

    vector<vector<float> > features;
    vector<float> y;
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
    vector<vector<float> > w_csv;
    if(csv_vec_vec_float(w_csv, filename) != 0) {
        cerr<<"CSV to vector<vector<float> > issue "<<__FILE__<<" "<<__LINE__<<endl;
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
    ERR_OUT(features.size() == 0);
    ERR_OUT(y.size() == 0);
    ERR_OUT(w.size() == 0);
    ERR_OUT(features[0].size() != w.size() - 1);
    ERR_OUT(features.size() != y.size());
    int n = features.size();
    float cost = 0;

    for(int iter = 0; iter < n; iter++) {
        ERR_OUT(features[iter].size() != w.size() - 1);
    }

    /*
     * Note : n is the number of rows in dataset
     * Note : hypothesis h(x0, x1, ... xk) = w0 * x0 + w1 * x1 ... + wk * xk + bias
     * Cost function : summation over all inputs( (h(x0, x1 ...xk) - y) * (h(x0, x1 ...xk) - y) )
     */
    for(int iter = 0; iter < n; iter++) {
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
    float slope = 0;

    for(int iter = 0; iter < n; iter++) {
        float hypothesis_result = hypothesis(features[iter], w);
        assert(hypothesis_result != FLT_MAX);
        slope += (hypothesis_result - y[iter]) * 
                 ((features[iter][weight_index] * this_weight_is_not_bias) + (1 * !this_weight_is_not_bias)); 
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
            w[weight_iter] -= weight_slopes[weight_iter];
        }


#ifdef DEBUG
        cerr<<"epoch - "<<iter<<" | "<< "cost - "<<cost_function(features, y, w)<<endl;
#endif

#ifdef WEB_RUN
        if(iter % DUMP_FREQUENCY == 0 || iter == input.epoch) {
            FILE *fp = NULL;
            fp = fopen("./lr_dump.txt", "w+");
            assert(fp != NULL && "Cannot open/create file");
            for(unsigned int weight_iter = 0; weight_iter < w.size(); weight_iter++) {
                fprintf(fp, "%f, ", w[weight_iter]);
            }
            fprintf(fp, " - %f \n", cost_function(features, y, w));
            fclose(fp);
        }
#endif
        iter++;
    }

    return 0;
}


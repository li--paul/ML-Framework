#ifndef __LR_2D_H__
#define __LR_2D_H__

/* This file declares the headers to use
 * linear regression 2D code;
 */

#include <iostream>
#include <vector>
using namespace std;

typedef struct weights {
    /* Hypothesis is h = mx + c */
    float m; // weight 0
    float c; // weight 1

    weights() {
        /* Random */
        m = 5;
        c = 2; 
    }
    weights(float m_, float c_) {
        m = m_;
        c = c_;
    }
}weights; 

typedef struct lr_2d_input {
    /* 2d input points */
    vector<float> input_x;
    vector<float> input_y;

    /* Learning parameters */
    float learning_rate;
    int epoch;
    
    lr_2d_input() {
        /* def values */
        learning_rate = 0.0001;
        epoch = 10000;
    }
    
    lr_2d_input(vector<float> input_x_, vector<float> input_y_, float learning_rate_, int epoch_) {
        input_x = input_x_;
        input_y = input_y_;
        learning_rate = learning_rate_;
        epoch = epoch_;
    }

    lr_2d_input(vector<float> input_x_, vector<float> input_y_) {
        input_x = input_x_;
        input_y = input_y_;
        learning_rate = 0.0001;
        epoch = 10000;
    }
}lr_2d_input;

weights perform_linear_regression_2d(lr_2d_input input, weights init_weights = weights());

#endif // __LR_2D_H__

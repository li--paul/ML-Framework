
/* Defines functions in lr_2d.hpp */

#include "lr_2d.hpp"
#include <cassert>
#define DEBUG

float cost_function_m_slope(vector<float> input_x,
                            vector<float> input_y,
                            float m,
                            float c) {
    assert(input_x.size() == input_y.size());

    /*
     * Note : n is the number of input points
     * Cost function : [ (1/2n) * summation over all inputs( (2*m*x*x) + (2*x*x*c) - (2*x*y)) ]
     *
     */
    int n = input_x.size();
    float slope = 0;

    for(int iter = 0; iter < n; iter++) {
        slope +=    (2 * m * input_x[iter] * input_x[iter]) +
                    (2 * input_x[iter] * c) -
                    (2 * input_x[iter] * input_y[iter]);        
    } 
    return (slope * ((float)1 / (float)(2 * n)));
}

float cost_function_c_slope(vector<float> input_x,
                            vector<float> input_y,
                            float m,
                            float c) {
    assert(input_x.size() == input_y.size());

    /*
     * Note : n is the number of input points
     * Cost function : [ (1/2n) * summation over all inputs((2*c) + (2*m*x) - (2*y)) ]
     *
     */
    int n = input_x.size();
    float slope = 0;

    for(int iter = 0; iter < n; iter++) {
        slope +=    (2 * c) +
                    (2 * m * input_x[iter]) - 
                    (2 * input_y[iter]);
    } 
    return (slope * ((float)1 / (float)(2 * n)));
}

weights perform_linear_regression_2d(  lr_2d_input input,
                                       weights init_weights) {
    assert(input.input_x.size() == input.input_y.size());
    assert(input.input_x.size() > 0);
    /* Randomly init weights */
    weights w = init_weights;
    
    int iter = 0;
    while(iter < input.epoch) {
        float m_slope = cost_function_m_slope(input.input_x, input.input_y, w.m, w.c) * input.learning_rate;
        float c_slope = cost_function_c_slope(input.input_x, input.input_y, w.m, w.c) * input.learning_rate;
#ifdef DEBUG
        cerr<<"m_slope "<<m_slope<<" | "<<"c_slope "<<c_slope<<endl;
#endif
        w.m -= m_slope;
        w.c -= c_slope;  
        iter++;
    }
    
    return w;
}

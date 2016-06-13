// Test 2d linear regression 

#include <iostream>
#include <vector>
using namespace std;

#include "lr_2d.hpp"

void fill_inputs(vector<float>& x, vector<float>& y) {
    x.push_back(0.5); y.push_back(13);
    x.push_back(1);   y.push_back(8);
    x.push_back(2);   y.push_back(24);
    x.push_back(3);   y.push_back(18);
    x.push_back(4);   y.push_back(30);
    x.push_back(4.5); y.push_back(19);
    x.push_back(5);   y.push_back(57);
    x.push_back(6);   y.push_back(49);
    x.push_back(6.5); y.push_back(49);
    x.push_back(7);   y.push_back(73);
    x.push_back(8);   y.push_back(90);
}

int main() {
    vector<float> x;
    vector<float> y;
    
    fill_inputs(x, y);
    lr_2d_input ip = lr_2d_input(x, y);
    weights w = perform_linear_regression_2d(ip);
    /* Print weights */
    cout<<" m - "<<w.m<<endl;
    cout<<" c - "<<w.c<<endl; 

    return 0;
}


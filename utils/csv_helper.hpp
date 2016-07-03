#ifndef __CSV_HELPER_HPP__
#define __CSV_HELPER_HPP__
/*
 * This file declares all functions
 * to process csv files;
 * i.e. csv file -> vector<vector<float> > 
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*
 * csv file -> vector<vector<float> >;
 * supports only ',' as a delimiter 
 */
vector<vector<float> > csv_vec_vec_float(const string file_name);
#endif // __CSV_HELPER_HPP__



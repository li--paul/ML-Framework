#ifndef __CSV_HELPER_HPP__
#define __CSV_HELPER_HPP__
/*
 * This file declares all functions
 * to process csv files;
 * i.e. csv file -> vector<vector<double> > 
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*
 * csv file -> vector<vector<double> >;
 * supports only ',' as a delimiter;
 * This is a wrapper function that just called get_tokens_as_double 
 * Returns 0 for Success;
 * Return -1 for failure;
 */
int csv_vec_vec_double(vector<vector<double> > &op, const string file_name);
#endif // __CSV_HELPER_HPP__



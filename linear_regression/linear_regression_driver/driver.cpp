#include <iostream>
#include <cstring> 
#include <string>
#include <vector>
#include <cassert>
#include <cstdio>
#include <cstdlib>

using namespace std;

#include "./../linear_regression_nd/lr.hpp"
#include "./../linear_regression_nd/lr_utils.hpp"

#define DEBUG

void print_help() {
    string help = " The program expects 4 arguments always;                                        \n\
                    Argument 1 : Name of the .csv file that contains data                          \n\
                    Argument 2 : Name of the .csv file that conatins initial                       \n\
                                 weights (Pass 0 to use random weights)                            \n\
                    Argument 3 : Learning rate of the algorithm (pass zero to use default values)  \n\
                    Argument 4 : Epoch (padd negative number to use default value) ";
    cout<<help<<endl;
}

template<typename T> 
void print_vector(vector<T> vec) {
    for(unsigned int iter = 0; iter < vec.size(); iter++) {
        cout<<vec[iter]<<endl;
    }
    cout<<endl;
}

template<typename T>
void print_vector_vector(vector<vector<T> > vec) {
    for(unsigned int i = 0; i < vec.size(); i++) {
        for(unsigned int j = 0; j < vec[i].size(); j++) {
            cout<<vec[i][j]<<" ";
        }
        cout<<endl;
    }
}
/*
 * Returns the size of the file
 */
size_t get_file_size(string file_name) {
    FILE *fp = NULL;
    fp = fopen(file_name.c_str(), "r");
    assert(fp != NULL && "Can't open csv");
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fclose(fp);
    return file_size;
} 

/*
 * Return all the lines in the file.
 */
vector<string> get_lines(string file_name) {
    /* Open file */
    FILE *fp = NULL;
    fp = fopen(file_name.c_str(), "r");
    assert(fp != NULL && "Can't open csv");
    size_t file_size = get_file_size(file_name); 

    /* Buffer to hold file contents */
    char *file_contents = NULL;
    file_contents = (char *)malloc(file_size + 1);
    assert(file_contents != NULL && "Cannot malloc memory");
    file_contents[file_size] = '\0';

    int num_read = fread(file_contents, 1, file_size, fp);
    assert(num_read == file_size && "File not read properly");

    vector<string> ret_val;
    /* Get lines */;
    char *token = strtok(file_contents, "\n");
    while(token != NULL) {
        ret_val.push_back(string(token));
        token = strtok(NULL, "\n");
    }

    free(file_contents);
    fclose(fp);

    return ret_val;
}

vector<vector<string> > get_tokens(vector<string> lines) {
    vector<vector<string> > tokens;
    for(unsigned int line_iter = 0; line_iter < lines.size(); line_iter++) {
        vector<string> tokens_in_line;
        string line = lines[line_iter];
        
        /* Get a mutable line */
        char *mutable_line = NULL;
        mutable_line = (char *)malloc(line.length() + 1);
        for(unsigned int char_iter = 0; char_iter < line.length(); char_iter++) {
            mutable_line[char_iter] = line[char_iter];
        }
        mutable_line[line.length()] = '\0';

        /* Get tokens */
        char *token = strtok(mutable_line, ",");
        while(token != NULL) {
            tokens_in_line.push_back(string(token));
            token = strtok(NULL, ",");
        }
        tokens.push_back(tokens_in_line);

        free(mutable_line);
    }
    return tokens;
}

vector<vector<float> > get_tokens_as_float(string file_name) {
    vector<string> lines = get_lines(file_name);
    vector<vector<string> > tokens = get_tokens(lines);
    vector<vector<float> > tok_as_float;

    for(unsigned int token_i = 0; token_i < tokens.size(); token_i++) {
        vector<float> ip;
        for(unsigned int token_j = 0; token_j < tokens[token_i].size(); token_j++) {
            double value = atof(tokens[token_i][token_j].c_str());
            ip.push_back((float)value);
        }
        tok_as_float.push_back(ip);
    }
    return tok_as_float;
}

bool validate_inputs(vector<vector<float> > &ip, vector<vector<float> > &w) {
    unsigned int num_rows = ip.size();
    if(num_rows == 0) {
        cerr<<"Inputs not available"<<endl;
        return false;
    }
    unsigned int num_cols = ip[0].size();
    /* Check for data consistency */
    for(unsigned int row = 0; row < num_rows; row++) {
        if(ip[row].size() != num_cols) {
            cerr<<"Inputs jagged! "<<endl;
            return false;
        }
    }

    if(w.size() != 0) {
        /* Not random */
        if(w[0].size() != ip[0].size()) {
            cerr<<"Not enough weights "<<endl;
            return false;
        }
    }
    return true;
}

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

int main(int argc, char *argv[]) {
    if (argc < 5) {
        print_help();
        cerr<<"Arguments not proper !"<<endl;
        return -1;
    }

    string input_file_arg, weights_file_arg, learning_rate_arg, epoch_arg;  

    input_file_arg = argv[1];
    weights_file_arg = argv[2];
    learning_rate_arg = argv[3];
    epoch_arg = argv[4];

#ifdef DEBUG
    cout<<"Input file    - "<<input_file_arg<<endl;
    cout<<"Weights file  - "<<weights_file_arg<<endl;
    cout<<"Learning rate - "<<learning_rate_arg<<endl;
    cout<<"Epoch arg     - "<<epoch_arg<<endl;

    vector<string> lines = get_lines(input_file_arg);
    vector<vector<string> > tokens = get_tokens(lines);

    /* Print lines */
    cout<<"--- Lines ----"<<endl;
    print_vector(lines);
    cout<<"-------------"<<endl;
    /* Print tokens */
    cout<<"--- Tokens ----"<<endl;
    print_vector_vector(tokens);
    cout<<"----------------"<<endl;
#endif

    vector<vector<float> > inputs = get_tokens_as_float(input_file_arg);
    float learning_rate = (float)atof(learning_rate_arg.c_str());
    int epoch = (int)atof(epoch_arg.c_str());
    vector<vector<float> > init_weights;
    if(weights_file_arg != "0") {
        init_weights = get_tokens_as_float(weights_file_arg);
        assert(init_weights.size() == 1);
    } else {
        init_weights.push_back(vector<float> ());
    } 

    if(!validate_inputs(inputs, init_weights)) {
        return -1;
    }

    vector<vector<float> > features;
    vector<float> y;
    vector<float> w_vec; 

    split_csv_vector(inputs, features, y);
    assert(features.size() == y.size());
    assert(features.size() != 0);

    if(init_weights.size() != 0) {
        w_vec = init_weights[0];
        assert(w_vec.size() == features[0].size() + 1);
    }

    /* Prepare linear regression inputs */
    lr_input regression_input;
    weights regression_weights;
    learning_rate = (learning_rate == 0) ? regression_input.learning_rate : learning_rate;
    epoch = (epoch < 0) ? regression_input.epoch : epoch;
    regression_input = lr_input(features, y, learning_rate, epoch);
    regression_weights = weights(w_vec); 

    weights result_weights;
    
    float ret_val = perform_linear_regression(regression_input, result_weights, regression_weights);
    assert(ret_val == 0);

#ifdef DEBUG
    cout<<"--- Inputs ---"<<endl;
    print_vector_vector(inputs);
    cout<<"--------------"<<endl;
    cout<<"--- Weights ---"<<endl;
    print_vector_vector(init_weights);
    cout<<endl<<"---------------"<<endl;

    cout<<"Learning rate - "<<regression_input.learning_rate<<endl;
    cout<<"epoch - "<<regression_input.epoch<<endl;

    int iter = 0;
    while(iter < 10) {
        float ret_val = perform_linear_regression(regression_input, result_weights, regression_weights);
        assert(ret_val == 0);
        regression_weights = result_weights;
        float cost = cost_function(regression_input.features, regression_input.y, result_weights.w);
        cout<<"Iteration "<<iter<<" : "<<cost<<endl;
        iter++;
    }
#endif

    return 0;
}

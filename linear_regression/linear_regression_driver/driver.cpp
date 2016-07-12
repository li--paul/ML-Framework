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

#ifdef WEB_RUN
// Dump data every 50th epoch
#define DUMP_FREQUENCY 50
#endif

void print_help() {
#ifdef WEB_RUN
    string help = " The program expects 5 arguments always;                                        \n\
                    Argument 1 : Name of the .csv file that contains data                          \n\
                    Argument 2 : Name of the .csv file that conatins initial                       \n\
                                 weights (Pass 0 to use random weights)                            \n\
                    Argument 3 : Learning rate of the algorithm (pass zero to use default values)  \n\
                    Argument 4 : Epoch (padd negative number to use default value)                 \n\
                    Argument 5 : Dump file prefix [to uniquely identify the run]";
#else
    string help = " The program expects 4 arguments always;                                        \n\
                    Argument 1 : Name of the .csv file that contains data                          \n\
                    Argument 2 : Name of the .csv file that conatins initial                       \n\
                                 weights (Pass 0 to use random weights)                            \n\
                    Argument 3 : Learning rate of the algorithm (pass zero to use default values)  \n\
                    Argument 4 : Epoch (padd negative number to use default value) ";
#endif
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

bool validate_inputs(lr_input &ip, weights &w) {

    unsigned int data_rows = ip.features.size();
    if(data_rows == 0) { 
        cerr<<"0 data rows ! "<<endl;
        return false;
    }
    unsigned int num_features = ip.features[0].size();
    if(ip.y.size() != data_rows) {
        cerr<<"Feature rows and y rows not equal !"<<endl;
        return false;
    }
    if(w.w.size() != 0 && w.w.size() != num_features + 1) {
        cerr<<"Less/More weights !! "<<endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
#ifdef WEB_RUN
    if (argc < 6) {
#else
    if (argc < 5) {
#endif
        print_help();
        cerr<<"Arguments not proper !"<<endl;
        return -1;
    }

    string input_file_arg, weights_file_arg, learning_rate_arg, epoch_arg;  

    input_file_arg = argv[1];
    weights_file_arg = argv[2];
    learning_rate_arg = argv[3];
    epoch_arg = argv[4];
#ifdef WEB_RUN
    string file_prefix = argv[5];
#endif

#ifdef DEBUG
    cout<<"Input file    - "<<input_file_arg<<endl;
    cout<<"Weights file  - "<<weights_file_arg<<endl;
    cout<<"Learning rate - "<<learning_rate_arg<<endl;
    cout<<"Epoch arg     - "<<epoch_arg<<endl;
#ifdef WEB_RUN
    cout<<"File prefix   - "<<file_prefix<<endl;
#endif // WEB_RUN
#endif // DEBUG

    lr_input regression_input;
    weights regression_weights;

    if(construct_input(regression_input, input_file_arg) != 0) {
        cerr<<"Input error - csv file "<<__FILE__<<" "<<__LINE__<<endl;
        return -1;
    }
    if(weights_file_arg == "0") {
        /* Dont care */
    } else {
        if(construct_weights(regression_weights, weights_file_arg) != 0) {
            cerr<<"Weights error - csv file "<<__FILE__<<" "<<__LINE__<<endl;
            return -1;
        }
    }
    float learning_rate = (float)atof(learning_rate_arg.c_str());
    regression_input.learning_rate = (learning_rate == 0) ? regression_input.learning_rate : learning_rate;

    int epoch = (int)atof(epoch_arg.c_str());
    regression_input.epoch = (epoch < 0) ? regression_input.epoch : epoch;

    /* Total iterations to master ! */
    int epoch_total = regression_input.epoch;

    if(!validate_inputs(regression_input, regression_weights)) {
        return -1;
    }

#ifdef DEBUG
    {
        /* Print configuation */
        cout<<"Learning rate - "<<regression_input.learning_rate<<endl;
        cout<<"Epoch - "<<regression_input.epoch<<endl;
        weights result_weights;

        int iter = 0;
        while(iter < 10) {
            float ret_val = perform_linear_regression(regression_input, result_weights, regression_weights);
            assert(ret_val == 0);
            regression_weights = result_weights;
            float cost = cost_function(regression_input.features, regression_input.y, result_weights.w);
            cout<<"Iteration "<<iter<<" : "<<cost<<endl;
            iter++;
        }
    }
#endif

#ifdef WEB_RUN

    /* Dump the data that we are working on
     * for the web_interface to read and show
     * in plotly
     */
    string input_dump_filename = "./" + file_prefix + "_input_dump.txt";
    FILE *fp_input = NULL;
    fp_input = fopen(input_dump_filename.c_str(), "w+");
    assert(fp_input != NULL && "Can't open file");
    for(unsigned int f_i = 0; f_i < regression_input.features.size(); f_i++) {
        for(unsigned int f_j = 0; f_j < regression_input.features[f_i].size(); f_j++) {
            fprintf(fp_input, "%f, ", regression_input.features[f_i][f_j]);
        }
        fprintf(fp_input, "%f \n", regression_input.y[f_i]);
    }
    fclose(fp_input);

    int epoch_done = 0;
    weights result_weights;
    while(epoch_done < epoch_total) {
        int epoch_remaining = (epoch_total - epoch_done); 
        int epoch_current = 0;
        if(epoch_remaining < DUMP_FREQUENCY) {
            epoch_current = epoch_remaining;
        } else { 
            epoch_current = DUMP_FREQUENCY;
        }
        assert(epoch_current != 0);

        /* Run regression */
        regression_input.epoch = epoch_current;
        float ret_val = perform_linear_regression(regression_input, result_weights, regression_weights);
        assert(ret_val == 0);

        /* Dump data */
        FILE *fp = NULL;
        string dump_file_name = "./" + file_prefix + "_weights_dump.txt";
        fp = fopen(dump_file_name.c_str(), "w+");
        assert(fp != NULL && "Cannot open/create file");
        for(unsigned int weight_iter = 0; weight_iter < result_weights.w.size(); weight_iter++) {
            fprintf(fp, "%f, ", result_weights.w[weight_iter]);
        }
        fprintf(fp, " - %f \n", cost_function(regression_input.features, regression_input.y, result_weights.w));
        fclose(fp);

        /* Initialize next run's weights */
        regression_weights = result_weights;

        epoch_done += epoch_current;
    }
#endif

    return 0;
}

#include <iostream>
#include <cstring> 
#include <string>
#include <vector>
#include <cassert>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define DEBUG

void print_help() {
    string help = " The program expects 4 arguments always;                                        \n\
                    Argument 1 : Name of the .csv file that contains data                          \n\
                    Argument 2 : Name of the .csv file that conatins initial                       \n\
                                 weights (Pass \"def_weights.csv\" to use random weights)          \n\
                    Argument 3 : Learning rate of the algorithm (pass zero to use default values)  \n\
                    Argument 4 : Epoch (padd negative number to use default value) ";
    cout<<help<<endl;
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
    file_contents = (char *)malloc(file_size);
    assert(file_contents != NULL && "Cannot malloc memory");

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

int main(int argc, char *argv[]) {
    if (argc < 5) {
        print_help();
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
#endif

    vector<string> lines = get_lines(input_file_arg);
    vector<vector<string> > tokens = get_tokens(lines);

#ifdef DEBUG
    /* Print lines */
    cout<<"--- Lines ----"<<endl;
    for(unsigned int iter = 0; iter < lines.size(); iter++) {
        cout<<lines[iter]<<endl;
    }
    cout<<"-------------"<<endl;
    /* Print tokens */
    cout<<"--- Tokens ----"<<endl;
    for(unsigned int token_i = 0; token_i < tokens.size(); token_i++) {
        for(unsigned int token_j = 0; token_j < tokens[token_i].size(); token_j++) {
            cout<<tokens[token_i][token_j]<<", ";
        }
        cout<<endl;
    }
    cout<<"----------------"<<endl;
#endif
    return 0;

}

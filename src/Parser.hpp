#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Component.hpp"

using namespace std;

class Parser{
    public:
        //Public functions
        Parser();
        ~Parser();
        bool parse(const string filename);
        void print_components();
        
        //Public Variables
                
    private:
        //HelperFunctions
        bool tokenize(const string filename);
        bool generateComponents();
        int index_by_name(const string name);
        int vector_index(const string &value, const vector<string> &vec);
        int generate_type(const vector<string> &line);
        int extract_int(const string str);
        bool is_number(string str);

        //Private Variables
        vector<vector<string>> tokens;
        vector<Component> components;
};

#endif
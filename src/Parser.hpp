#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>
#include <map>

#include "ParserException.hpp"
#include "Operation.hpp"
#include "IfOperation.hpp"
#include "graph.hpp"

using namespace std;

class Parser{
    public:
        //Public functions
        Parser();
        ~Parser();
        void parse(const string filename);
        void print_operations();
        Graph get_graph();
        void setVerbosity(bool v);
        
        //Public Variables
        map<int,Operation> operations;
        vector<vector<int>> states;   //keeps track of which operation ID pertains to which state
                
    private:
        //HelperFunctions
        int id_by_name(const string name);
        bool tokenize(const string filename);
        Resource_Type generate_type(string sym);
        int extract_int(const string str);
        bool is_number(string str);
        
        //new functions
        tuple<Operation,Operation> generateIO();
        void generateOperations(vector<vector<string>> tokens);
        tuple<vector<vector<string>>,vector<vector<string>>,vector<vector<string>>> getBrackets(vector<vector<string>> vector);
        bool verbose;
        
        //Private Variables
        vector<vector<string>> tokens;
        Operation INOP;
        Operation OUOP;
};

#endif

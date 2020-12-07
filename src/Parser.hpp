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

class Parser{
    public:
        //Public functions
        Parser();
        ~Parser();
        void parse(const std::string filename);
        void print_operations();
        Graph get_graph();
        void setVerbosity(bool v);
        void generateSortedStates(Graph g);
        
        //Public Variables
        map<int,Operation> operations;
        vector<vector<int>> states;   //keeps track of which operation ID pertains to which state
        vector<vector<int>> final_states;   //this is the states list following modification by the scheduling algorithm        
    private:
        //HelperFunctions
        int id_by_name(const std::string name);
        bool tokenize(const std::string filename);
        Resource_Type generate_type(std::string sym);
        int extract_int(const std::string str);
        bool is_number(std::string str);
        vector<int> getPreds(int target, vector<int> cstate);
        
        //new functions
        tuple<Operation,Operation> generateIO();
        void generateOperations(vector<vector<std::string>> tokens);
        tuple<vector<vector<std::string>>,vector<vector<std::string>>,vector<vector<std::string>>> getBrackets(vector<vector<std::string>> vector);
        bool verbose;
        
        //Private Variables
        vector<vector<std::string>> tokens;
        Operation INOP;
        Operation OUOP;
};

#endif

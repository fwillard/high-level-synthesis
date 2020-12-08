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
        void generateSortedStates(Graph g, int lambda);
        
        //Public Variables
        std::map<int,Operation> operations;
        std::vector<std::vector<int>> states;   //keeps track of which operation ID pertains to which state
        std::vector<std::vector<int>> final_states;   //this is the states list following modification by the scheduling algorithm        
    private:
        //HelperFunctions
        int id_by_name(const std::string name);
        bool tokenize(const std::string filename);
        Resource_Type generate_type(std::string sym);
        int extract_int(const std::string str);
        bool is_number(std::string str);
        std::vector<int> getPreds(int target, std::vector<int> cstate);
        
        //new functions
        std::tuple<Operation,Operation> generateIO();
        void generateOperations(std::vector<std::vector<std::string>> tokens);
        std::tuple<std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>> getBrackets(std::vector<std::vector<std::string>> vec);
        bool verbose;
        
        //Private Variables
        std::vector<std::vector<std::string>> tokens;
        Operation INOP;
        Operation OUOP;
};

#endif

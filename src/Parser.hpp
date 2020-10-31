#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Parser{
    public:
        //Struct containing information for each component in the circuit including inputs and outputs
        struct Component{
            enum component_type{NONE, INPUT, OUTPUT, REG, ADD, SUB, MUL, COMP, MUX2x1, SHR, SHL, DIV, MOD, INC, DEC};
            string name = "default";
            component_type type = NONE;
            bool sign = false;
            int datawidth = -1;

            void display();
        }; 

        //Public functions
        Parser();
        ~Parser();
        bool parse(string filename);

        //Public Variables
                
    private:
        //Private Functions
        bool tokenize(string filename);
        bool generateComponents();

        //Private Variables
        vector<vector<string>> tokens;
        vector<Component> components;
};

#endif
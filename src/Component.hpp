#ifndef COMPONENT
#define COMPONENT

#include <iostream>
#include <vector>
#include <string> 

using namespace std;
 
//Struct containing information for each component in the circuit including inputs and outputs
class Component{
    public:
        enum component_type{CONST, INPUT, OUTPUT, WIRE, REG, ADD, SUB, MUL, SHR, SHL, DIV, MOD, INC, DEC, MUX2x1, COMP};
        enum component_sub_type{LT,GT,EQ};

        Component();
        Component(string n, Component::component_type t, bool sn, int width, int id);

        string name = "default";
        component_type type = CONST;
        component_sub_type subtype = EQ;
        bool sign = false;
        int datawidth = 8;
        int id;


        vector<int> inputs;
        vector<int> outputs;
}; 


#endif
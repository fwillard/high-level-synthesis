#ifndef COMPONENT
#define COMPONENT

#include <iostream>
#include <vector>
#include <string> 

using namespace std;
 
//Struct containing information for each component in the circuit including inputs and outputs
class Component{
    public:
        enum component_type{NONE, INPUT, OUTPUT, WIRE, REG, ADD, SUB, MUL, SHR, SHL, DIV, MOD, INC, DEC, MUX2x1, COMP};

        Component();
        Component(string n, Component::component_type t, bool sn, int width, int id);

        string name = "default";
        component_type type = NONE;
        bool sign = false;
        int datawidth = -1;
        int id;


        vector<int> inputs;
        int output;
}; 


#endif
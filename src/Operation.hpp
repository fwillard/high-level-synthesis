#ifndef OPERATION
#define OPERATION

#include <iostream>
#include <vector>
#include <string> 
#include "graph.hpp"

using namespace std;
 
//Struct containing information for each component in the circuit including inputs and outputs
class Operation{
    public:
        Operation();
        Operation(string n, Resource_Type t, bool sn, int width, int id, int out=-1, int a0=-1, int a1=-1, int a2=-1, string sym="NOP");

        int id;
        int datawidth = 8;
        string name = "default";
        Resource_Type type = Resource_Type::NOP;
        bool sign = false;
        string symbol = "none";
        int output_id, arg0_id, arg1_id, arg2_id;

        bool isif = false;
}; 


#endif
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

        int id;                                     //The ID of the node, used for graph generation
        int datawidth = 8;                          //Datawith, used for verilog
        string name = "default";                    //Variable name (for in/out/reg/wire), used for verilog
        Resource_Type type = Resource_Type::NOP;    //Type, used for graph generation 
        bool sign = false;                          //sign, used for verilog
        string symbol = "none";                     //the symbol of the operation (i.e "+" or "<<"), used for verilog
        int output_id, arg0_id, arg1_id, arg2_id;   //these are the id's of arguments, used for verilog 
        vector<int> predicessors, successors;       //Immidiate predicessor and Successor IDs, used for graph generation
        bool isif = false;                          //Weather or not this is used in an if block, which will give the verilog sequential structrue
}; 


#endif
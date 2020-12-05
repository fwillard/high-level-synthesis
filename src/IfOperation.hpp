#ifndef IFOPERATION
#define IFOPERATION

#include "Operation.hpp"

using namespace std;
 
//Struct containing information for each component in the circuit including inputs and outputs
class IfOperation : Operation{
    public:
        IfOperation();

        vector<Operation> pre_operations;
        vector<Operation> post_operations;
        vector<Operation> true_operations;
        vector<Operation> false_operations;
}; 


#endif
#ifndef IFOPERATION
#define IFOPERATION

#include "Operation.hpp"

using namespace std;
 
//Struct containing information for each component in the circuit including inputs and outputs
class IfOperation : Operation{
    public:
        IfOperation();

        vector<int> pre_operations;
        vector<int> post_operations;
        vector<int> true_operations;
        vector<int> false_operations;
}; 


#endif
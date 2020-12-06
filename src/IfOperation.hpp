#ifndef IFOPERATION
#define IFOPERATION

#include "Operation.hpp"

using namespace std;
 
//Struct containing information for each component in the circuit including inputs and outputs
class IfOperation : public Operation{
    public:
        IfOperation();
        IfOperation(int condition, int t_state, int f_state);  
}; 


#endif
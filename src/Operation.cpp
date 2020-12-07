#include "Operation.hpp"

Operation::Operation(){

}

Operation::Operation(std::string n, Resource_Type t, bool sn, int width, int id, int out, int a0, int a1, int a2, std::string sym){
    this->name = n;
    this->type = t;
    this->sign = sn;
    this->datawidth = width;
    this->id = id;
    this->output_id = out;
    this->arg0_id = a0;
    this->arg1_id = a1;
    this->arg2_id = a2;
    this->symbol = sym;
}

#include "IfOperation.hpp"

IfOperation::IfOperation(){
    this->isif = true;
}

IfOperation::IfOperation(int condition, int t_state, int f_state){
    this->isif = true;

    this->condition_id = condition;
    this->true_state = t_state;
    this->false_state = f_state;

    this->type = Resource_Type::LOGICAL;
    this->sign = false;
    this->datawidth = 1;
}
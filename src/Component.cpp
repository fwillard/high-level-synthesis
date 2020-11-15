#include "Component.hpp"

Component::Component(){

}

Component::Component(string n, Component::component_type t, bool sn, int width, int id){
    this->name = n;
    this->type = t;
    this->sign = sn;
    this->datawidth = width;
    this->id = id;
}

//
//  node.cpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//

#include "node.hpp"
Node::Node(int id, bool is_reg){
    this->id = id;
    this->is_reg = is_reg;
}
int Node::get_id(){
    return id;
}
std::vector<Edge> Node::get_adjacent(){
    return adjacent;
}
void Node::add_adjacent(int end, double weight){
    adjacent.push_back(Edge(end,weight));
}

Color Node::get_color(){
    return color;
}
void Node::set_color(Color color){
    this->color = color;
}

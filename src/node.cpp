//
//  node.cpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//

#include "node.hpp"
Node::Node(int id){
    this->id = id;
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

//
//  edge.cpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//

#include "edge.hpp"
Edge::Edge(int end_vertex, double weight){
    this->end_vertex = end_vertex;
    this->weight = weight;
}
int Edge::get_end_vertex(){
    return end_vertex;
}
double Edge::get_weight(){
    return weight;
}

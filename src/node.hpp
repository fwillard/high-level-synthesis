//
//  node.hpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//

#ifndef node_hpp
#define node_hpp

#include <stdio.h>
#include <vector>
#include "edge.hpp"
enum Color{
    white,
    gray
};
class Node{
private:
    int id;
    std::vector<Edge> adjacent;
    Color color = white;
    
public:
    Color get_color();
    void set_color(Color color);
    bool is_reg;
    Node(){};
    Node(int id, bool is_reg = false);
    int get_id();
    std::vector<Edge> get_adjacent();
    void add_adjacent(int end, double weight);
    bool operator==(const Node& rhs) const {return this->id == rhs.id;}
};
#endif /* node_hpp */

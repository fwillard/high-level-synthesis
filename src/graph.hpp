//
//  graph.hpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//
//  A directed weighted graph implementation for critical path calcs

#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <unordered_map>
#include <vector>
#include "node.hpp"
class Graph{
private:
    std::unordered_map<int, Node> graph;
    Node get_node(int id);
    static bool contains(std::vector<Node>, Node n);
public:
    Graph();
    void add(int source, int dest, int weight);
    std::vector<Edge> find_adjacent(int id);
    bool is_connected(int source, int dest);
};
#endif /* graph_hpp */

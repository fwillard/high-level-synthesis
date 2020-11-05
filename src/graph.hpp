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
#include <list>
#include "node.hpp"
class Graph{
private:
    std::unordered_map<int, Node> graph;
    Node get_node(int id);
    static bool contains(std::vector<Node>, Node n);
    static void reg_to_reg_topo_sort(Graph g, std::list<std::pair<int, Node> > l, std::pair<int, Node> v);
    static void reg_to_reg_ts_visit(Graph g, std::list<std::pair<int, Node> > l, std::pair<int, Node> v);
public:
    Graph();
    void add(int source, int dest, int weight);
    std::vector<Edge> find_adjacent(int id);
    bool is_connected(int source, int dest);
    static double critical_path(Graph g);
    std::unordered_map<int, Node> get_graph();
};
#endif /* graph_hpp */

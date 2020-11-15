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
#include <map>
#include <vector>
#include <list>
enum Color{
    WHITE,
    GRAY,
    BLACK
};
struct vertex{
    typedef std::pair<int, vertex*> vert;
    std::vector<vert> adjacent;
    int id;
    vertex(int i, bool b, double w) : id(i), is_reg(b), weight(w) {}
    Color color;
    double dist;
    double weight;
    bool is_reg = false;
};

class Graph{
private:
    static void topo_vist(Graph*, std::list<vertex*>*, vertex*);
    static void topo_sort(Graph*, std::list<vertex*>*, vertex*);

public:
    typedef std::map<int, vertex *> vertex_map;
    vertex_map graph;
    void add_vertex(const int, double, bool is_reg = false);
    void add_edge(const int from, const int to, double weight = 0.0);
    static double critical_path(Graph*);
};
#endif /* graph_hpp */

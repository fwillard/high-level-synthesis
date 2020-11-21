//
//  graph.hpp
//  hlysn
//
//  Created by Finn Willard on 10/28/20.
//

#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include <list>
enum class Resource_Type{
    ADDER,
    MULTIPLIER,
    LOGICAL,
    DIVIDER,
    NOP
};

struct vertex{
    typedef std::pair<int, vertex*> vert;
    std::vector<vert> adjacent;
    int id;
    Resource_Type type;
    vertex(int i, Resource_Type t) : id(i), type(t) {}
};

class Graph{
private:
public:
    typedef std::map<int, vertex *> vertex_map;
    vertex_map graph;
    void add_vertex(const int, Resource_Type);
    void add_edge(const int from, const int to, double weight = 0.0);
};
#endif /* graph_hpp */

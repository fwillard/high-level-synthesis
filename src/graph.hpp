//
//  graph.hpp
//  hlysn
//
//  Created by Finn Willard on 10/28/20.
//

#ifndef graph_hpp
#define graph_hpp

#include <iostream>
#include <map>
#include <vector>
enum class Resource_Type{
    ADDER,
    MULTIPLIER,
    LOGICAL,
    DIVIDER,
    NOP
};

enum class Color{
    WHITE,
    GRAY,
    BLACK
};

struct vertex{
    typedef std::pair<double, vertex*> vert;
    std::vector<vertex*> adjacent;
    int id;
    Resource_Type type;
    Color color;
    vertex(int i, Resource_Type t) : id(i), type(t) {}
};

class Graph{
private:
    static bool visit(vertex*);
public:
    //public members
    typedef std::map<int, vertex *> vertex_map;
    vertex_map graph;
    
    //set up functions
    void add_vertex(const int, Resource_Type);
    void add_edge(const int, const int);
    void remove_edge(const int, const int);
    
    //algorithms
    static bool is_acyclic(Graph g);
    
};
#endif /* graph_hpp */

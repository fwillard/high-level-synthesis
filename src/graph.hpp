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
    std::vector<vertex*> outputs;
    std::vector<vertex*> inputs;
    int id;
    int cycle = -1;
    int delay;
    Resource_Type type;
    Color color;
    vertex(int i, Resource_Type t) : id(i), type(t) {
        switch (t) {
            case Resource_Type::ADDER:
            case Resource_Type::LOGICAL:
            case Resource_Type::NOP:
                delay = 1;
                break;
            case Resource_Type::MULTIPLIER:
                delay = 2;
                break;
            case Resource_Type::DIVIDER:
                delay = 3;
                break;
        }
    }
};

class Graph{
private:
    static bool visit(vertex*);
    void deep_copy(const Graph&);
public:
    //public members
    typedef std::map<int, vertex *> vertex_map;
    vertex_map graph;
    
    //set up functions
    void add_vertex(const int, Resource_Type);
    void add_edge(const int, const int);
    
    //algorithms
    static bool is_acyclic(Graph);
    
    //default constructor
    Graph(){};
    //copy constructor
    Graph(const Graph&);
    
    // Assignment operator
    Graph& operator=(const Graph&);
};
#endif /* graph_hpp */

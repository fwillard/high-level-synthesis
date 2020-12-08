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
#include <memory>

enum class Resource_Type : int{
    ADDER,
    MULTIPLIER,
    LOGICAL,
    DIVIDER,
    NOP
};

enum class Color : int{
    WHITE,
    GRAY,
    BLACK
};

struct vertex{
    std::vector<std::shared_ptr<vertex>> outputs;
    std::vector<std::shared_ptr<vertex>> inputs;
    int id;
    int cycle = -1;
    int delay;
    double total_force;
    int min_force_cycle = -1;
    std::pair<int, int> time_frame;
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
//                delay = 1;
                break;
            case Resource_Type::DIVIDER:
                delay = 3;
//                delay = 1;
                break;
        }
    }
    
    double get_op_probability(int);
};

class Graph{
private:
    static bool visit(std::shared_ptr<vertex>);
    void deep_copy(const Graph&);
public:
    //public members
    using vertex_map = std::map<int, std::shared_ptr<vertex>>;
    vertex_map graph;
    
    //set up functions
    void add_vertex(const int, Resource_Type);
    void add_edge(const int, const int);
    
    //algorithms
    static bool is_acyclic(Graph);
    double get_type_probability(int, Resource_Type);
    double calc_self_force(int, std::shared_ptr<vertex>);
    double calc_predecessor_force(int, std::shared_ptr<vertex>);
    double calc_successor_force(int, std::shared_ptr<vertex>);
    
    //default constructor
    Graph(){};
    //copy constructor
    Graph(const Graph&);
    
    // Assignment operator
    Graph& operator=(const Graph&);
    
    //util
    void output_data(){
        for(auto v : graph){
            for(auto u : v.second->outputs){
                std::cout << v.second->id << " ";
                std::cout << u->id << " ";
                std::cout << "\n";

            }
        }
    }
};
#endif /* graph_hpp */

//
//  graph.cpp
//  hlsyn
//
//  Created by Finn Willard on 10/28/20.
//

#include "graph.hpp"

void Graph::add_vertex(const int id, Resource_Type t){
    vertex_map::iterator itr = graph.find(id);
    if(itr == graph.end()){
        vertex *v;
        v = new vertex(id, t);
        graph[id] = v;
        return;
    }
    printf("\nVertex %d already exists!", id);
}

void Graph::add_edge(const int from, const int to, double weight){
    vertex *f = (graph.find(from)->second);
    vertex *t = (graph.find(to)->second);
    std::pair<double, vertex *> edge = std::make_pair(weight, t);
    f->adjacent.push_back(edge);
}

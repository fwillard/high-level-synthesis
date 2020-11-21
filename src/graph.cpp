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

void Graph::add_edge(const int from, const int to){
    vertex *f = (graph.find(from)->second);
    vertex *t = (graph.find(to)->second);
    f->adjacent.push_back(t);
}

bool Graph::is_acyclic(Graph g){
    //checks if a graph is acyclic by depth first search
    for(auto v : g.graph){
        v.second->color = Color::WHITE;
    }
  
    for(auto v : g.graph){
        if(v.second->color == Color::WHITE){
            if(!visit(v.second)){
                return false;
            }
        }
    }
    return true;
}

bool Graph::visit(vertex *v){
    if(v->color == Color::BLACK){
        return true;
    }
    else if (v->color == Color::GRAY){
        return false;
    }
    
    v->color = Color::GRAY;
    
    for(auto u : v->adjacent){
        if(!visit(u)){
            return false;
        }
    }
    
    v->color = Color::BLACK;
    
    return true;
}

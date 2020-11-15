//
//  graph.cpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//

#include "graph.hpp"

void Graph::add_vertex(const int id){
    vertex_map::iterator itr = graph.find(id);
    if(itr == graph.end()){
        vertex *v;
        v = new vertex(id);
        graph[id] = v;
        return;
    }
    printf("\nVertex %d already exists!", id);
}

void Graph::add_edge(const int from, const int to, double cost){
    vertex *f = (graph.find(from)->second);
    vertex *t = (graph.find(to)->second);
    std::pair<double, vertex *> edge = std::make_pair(cost, t);
    f->adjacent.push_back(edge);
}

void Graph::topo_sort(Graph *g, std::list<vertex*> *l){
    for(auto u : g->graph){
        u.second->color = WHITE;
    }
    for(auto u : g->graph){
        if(u.second->color == WHITE){
            topo_vist(g, l, u.second);
        }
    }
}

void Graph::topo_vist(Graph *g, std::list<vertex*> *l, vertex* u){
    u->color = GRAY;
    for(auto e: u->adjacent){
        vertex* v = e.second;
        if(v->color == WHITE){
            topo_vist(g, l, v);
        }
    }
    u->color = BLACK;
    l->push_front(u);
}

double Graph::critical_path(Graph *g){
    std::list<vertex*> l;
    topo_sort(g, &l);
    for(auto u : g->graph){
        u.second->dist = 0;
    }
    for(auto u : l){
        for(auto e : u->adjacent){
            vertex *v = e.second;
            if ((u->dist + e.first) > v->dist) {
                v->dist = u->dist + e.first;
                v->pred = u;
            }
        }
    }
    double max = 0;
    for(auto u : g->graph){
        if(u.second->dist > max){
            max = u.second->dist;
        }
    }
    
    return max;
}

//
//  graph.cpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//

#include "graph.hpp"

void Graph::add_vertex(const int id, double weight, bool is_reg){
    vertex_map::iterator itr = graph.find(id);
    if(itr == graph.end()){
        vertex *v;
        v = new vertex(id, is_reg, weight);
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

void Graph::topo_sort(Graph *g, std::list<vertex*> *l, vertex* u){
    for(auto u : g->graph){
        u.second->color = WHITE;
    }
    topo_vist(g, l, u);
}

void Graph::topo_vist(Graph *g, std::list<vertex*> *l, vertex* u){
    l->clear();
    u->color = GRAY;
    for(auto e : u->adjacent){
        vertex *v = e.second;
        if((v->color == WHITE) && (!v->is_reg)){
            topo_vist(g, l, v);
        }
        else if((v->color == WHITE) && (v->is_reg)){
            v->color = BLACK;
            l->push_front(v);
        }
    }
    u->color = BLACK;
    l->push_front(u);
}

double Graph::critical_path(Graph *g){
    double max = 0;
    std::list<vertex*> l;
    for(auto u : g->graph){
        if(u.second->is_reg){
            topo_sort(g, &l, u.second);
            for(auto v : l){
                v->dist = 0;
            }
            for(auto v : l){
                if((v->is_reg && v == u.second) || !v->is_reg){
                    for (auto e : v->adjacent) {
                        vertex *w = e.second;
                        if((v->dist + e.first + w->weight) > w->dist){
                            w->dist = v->dist + e.first + w->weight;
                        }
                    }
                }
            }
            for(auto v : l){
                if(v->is_reg && v->dist > max){
                    max = v->dist;
                }
            }
        }
        
    }
    return max;
}

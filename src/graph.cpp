//
//  graph.cpp
//  hlsyn
//
//  Created by Finn Willard on 10/28/20.
//

#include "graph.hpp"

//GRAPH METHODS
void Graph::add_vertex(const int id, Resource_Type t){
    vertex_map::iterator itr = graph.find(id);
    if(itr == graph.end()){
        std::shared_ptr<vertex> v = std::make_shared<vertex>(id, t);
        graph[id] = v;
        return;
    }
    printf("\nVertex %d already exists!", id);
}

void Graph::add_edge(const int from, const int to){
    std::shared_ptr<vertex> f = (graph.find(from)->second);
    std::shared_ptr<vertex> t = (graph.find(to)->second);
    f->outputs.push_back(t);
    t->inputs.push_back(f);
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

bool Graph::visit(std::shared_ptr<vertex> v){
    if(v->color == Color::BLACK){
        return true;
    }
    else if (v->color == Color::GRAY){
        return false;
    }
    
    v->color = Color::GRAY;
    
    for(auto u : v->outputs){
        if(!visit(u)){
            return false;
        }
    }
    
    v->color = Color::BLACK;
    
    return true;
}

void Graph::deep_copy(const Graph& source){
    graph.clear();
    int i = 0;
    //create copys of nodes
    for(auto v : source.graph){
        add_vertex(v.second->id, v.second->type);
        graph[i]->color = v.second->color;
        graph[i]->cycle = v.second->cycle;
        graph[i]->time_frame = v.second->time_frame;
        i++;
    }
    
    //create copys of edges
    for(auto v : source.graph){
        for(auto u : v.second->outputs){
            add_edge(v.second->id, u->id);
        }
    }
    
}

//copy constructor
Graph::Graph(const Graph& source){
    deep_copy(source);
}

Graph& Graph::operator=(const Graph& source){
    // check for self-assignment
    if (this != &source)
    {
        // now do the deep copy
        deep_copy(source);
    }
 
    return *this;
}

double Graph::get_type_probability(int time, Resource_Type type){
    double sum = 0.0;
    //sum all probabilities for the specified type
    for(auto v : graph){
        if(v.second->type == type){
            sum += v.second->get_op_probability(time);
        }
    }
    
    return sum;
}

double Graph::calc_self_force(int j, std::shared_ptr<vertex> v){
    double sum = 0.0;
    //sum all possible times in time frame
    for(int i = v->time_frame.first; i <= v->time_frame.second; i++){
        //if it is time we want, 1 - op_prob, else 0 - op_prob
        if(i == j){
            sum += get_type_probability(i, v->type) * (1 - v->get_op_probability(i));
        }
        else{
            sum += get_type_probability(i, v->type) * (0 - v->get_op_probability(i));
        }
    }
    return sum;
}

double Graph::calc_predecessor_force(int j, std::shared_ptr<vertex> v){
    return 0.0;
}

double Graph::calc_successor_force(int j, std::shared_ptr<vertex> v){
    return 0.0;
}

// VERTEX METHODS
double vertex::get_op_probability(int time){
    //check if in time frame
    if((time_frame.first <= time) && (time <= time_frame.second)){
        double frame_width = time_frame.second - time_frame.first + 1;
        return 1.0 / frame_width;
    }
    //return 0 if outside time frame
    return 0;
}


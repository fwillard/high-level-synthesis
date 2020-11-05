//
//  graph.cpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//

#include "graph.hpp"

Graph::Graph(){
}

void Graph::add(int source, int dest, int weight){
    //get node for source
    Node s = get_node(source);
    
    //add node to adjacency list
    s.add_adjacent(dest, weight);
}

Node Graph::get_node(int id){
    //checks if node is in graph
    if(graph.find(id) == graph.end()){
        //node not in graph so add it
        Node node(id);
        graph[id] = node;
        return node;
    }
    else{
        return graph[id];
    }
}
std::unordered_map<int, Node> Graph::get_graph(){
    return graph;
}

std::vector<Edge> Graph::find_adjacent(int id){
    Node n = get_node(id);
    return n.get_adjacent();
}

bool Graph::is_connected(int source, int dest){
    Node s = get_node(source);
    Node d = get_node(dest);
    
    std::vector<Edge> sourceList = s.get_adjacent();
    
    for(auto e : sourceList){
        if(e.get_end_vertex() == dest){
            return true;
        }
    }
    return false;
}

//helper function to check if vector contains node
bool Graph::contains(std::vector<Node> v, Node n){
    if (std::find(v.begin(), v.end(), n) != v.end()){
        return true;
    }
    else{
        return false;
    }
}

double Graph::critical_path(Graph g){
    double max = 0.0;
    std::list<std::pair<int, Node> >  l;
    for (auto u : g.get_graph()) {
        
//        if(u.second.is_reg){
//            reg_to_reg_topo_sort(g, l, u);
//        }
        reg_to_reg_topo_sort(g, l, u);
        
    }
}

void Graph::reg_to_reg_topo_sort(Graph g, std::list<std::pair<int, Node> > l, std::pair<int, Node> u){
    for (auto u : g.get_graph()) {
        u.second.set_color(white);
        reg_to_reg_ts_visit(g, l, u);
    }
}

void Graph::reg_to_reg_ts_visit(Graph g, std::list<std::pair<int, Node> > l, std::pair<int, Node> u){
    u.second.set_color(gray);
    for (auto e : u.second.get_adjacent()){
        Node v = g.get_node(e.get_end_vertex());
        if((v.get_color() == white) && (!v.is_reg)){
//            reg_to_reg_ts_visit(g, l, );
        }
    }
}

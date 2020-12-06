//
//  main.cpp
//  hlsyn
//
//  Created by Finn Willard on 11/19/20.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <climits>
#include "graph.hpp"
#include "scheduling.hpp"
#include "Parser.hpp"

int main(int argc, char** argv){
    
    if(argc < 3){
        std::cerr << "Usage: " << argv[0] << " cFile " << "latency " << "verilogFile" << std::endl;
        return 1;
    }
    std::string c_file = argv[1];
    std::string verilog_file = argv[3];
    
    errno = 0;
    char *endptr;
    long int latency;
    
    latency = std::strtol(argv[2], &endptr, 10);
    
    if(errno != 0 || *endptr != '\0' || latency > INT_MAX){
        std::cerr << "Invalid value for latency: " << argv[2] << "\nValue must be a valid int" <<  std::endl;
        return 1;
    }

    Parser p;
    p.setVerbosity(true);
    p.parse(argv[1]);

/*    
    Graph g;
    
    g.add_vertex(0, Resource_Type::NOP);
    g.add_vertex(1, Resource_Type::MULTIPLIER);
    g.add_vertex(2, Resource_Type::MULTIPLIER);
    g.add_vertex(3, Resource_Type::MULTIPLIER);
    g.add_vertex(4, Resource_Type::ADDER);
    g.add_vertex(5, Resource_Type::ADDER);
    g.add_vertex(6, Resource_Type::MULTIPLIER);
    g.add_vertex(7, Resource_Type::MULTIPLIER);
    g.add_vertex(8, Resource_Type::MULTIPLIER);
    g.add_vertex(9, Resource_Type::ADDER);
    g.add_vertex(10, Resource_Type::ADDER);
    g.add_vertex(11, Resource_Type::ADDER);
    g.add_vertex(12, Resource_Type::NOP);
    
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(0, 6);
    g.add_edge(0, 8);
    g.add_edge(0, 10);
    
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    
    g.add_edge(6, 7);
    g.add_edge(8, 9);
    g.add_edge(10, 11);
    
    
    g.add_edge(3, 4);
    g.add_edge(4, 5);
    g.add_edge(7, 5);
    
    g.add_edge(5, 12);
    g.add_edge(9, 12);
    g.add_edge(11, 12);
    
//    g.add_edge(4, 1); //creates a cycle;
    
    if(!Graph::is_acyclic(g)){
        std::cerr << "Graph is not acyclic" << std::endl;
        return 1;
    }
    
    Scheduler s;
    s.force_directed(g, latency);

    
    */
    return 0;
}

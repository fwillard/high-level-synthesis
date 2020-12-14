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
#include "VerilogGen.hpp"

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
    p.parse(c_file);

    Graph g = p.get_graph();
    
    if(!Graph::is_acyclic(g)){
        std::cerr << "Graph is not acyclic" << std::endl;
        return 1;
    }
    
    Scheduler s;
    Graph g2 = s.force_directed(g, latency);

    std::cout << "\n\nFinal Schedule: " << std::endl;

    s.print_schedule(g2); std::cout << "\n\n";
    p.generateSortedStates(g2,latency);
    p.print_operations(p.final_states);

    VerilogGen v(&p);
    v.setVerbosity(false);
    v.generate(c_file,verilog_file);
    return 0;
}

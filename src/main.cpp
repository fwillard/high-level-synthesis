//
//  main.cpp
//  hlsyn
//
//  Created by Finn Willard on 11/19/20.
//

#include <iostream>
#include <string>
#include <cstdlib>

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
    
    return 0;
}

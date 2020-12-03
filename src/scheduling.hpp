//
//  scheduling.hpp
//  hlsyn
//
//  Created by Finn Willard on 11/19/20.
//

#ifndef scheduling_hpp
#define scheduling_hpp

#include <iostream>
#include "graph.hpp"

class Scheduler{
private:
    std::shared_ptr<vertex> find_unscheduled_asap(Graph*);
    std::shared_ptr<vertex> find_unscheduled_alap(Graph*);
    Graph asap(Graph);
    Graph alap(Graph, int);
public:
    void force_directed(Graph, int);
    
    //util
    static void print_schedule(Graph);
};
#endif /* scheduling_hpp */

//
//  scheduling.hpp
//  hlsyn
//
//  Created by Finn Willard on 11/19/20.
//

#ifndef scheduling_hpp
#define scheduling_hpp

#include <stdio.h>
#include "graph.hpp"
class Scheduler{
private:
    void asap(Graph *g);
    void alap(Graph *g, int lambda);
public:
    void force_directed(Graph *g, int lambda);
};
#endif /* scheduling_hpp */

//
//  edge.hpp
//  dpgen
//
//  Created by Finn Willard on 10/28/20.
//

#ifndef edge_hpp
#define edge_hpp

#include <stdio.h>
class Edge{
private:
    int end_vertex;
    double weight;
public:
    Edge(){};
    Edge(int end_vertex, double weight);
    int get_end_vertex();
    double get_weight();
};

#endif /* edge_hpp */

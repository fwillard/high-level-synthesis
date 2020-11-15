#include "Parser.hpp"
#include "Component.hpp"
#include "graph.hpp"
#include <iostream>

using namespace std;

int main(){
    string testfile = "assignment_2_circuits/574a_circuit5.txt";

//    Parser p = Parser();
//    p.parse(testfile);
  
    Graph g;
    for (int i = 1; i < 10; i++) {
        g.add_vertex(i);
    }
    g.add_edge(1, 4, 1.5);
    g.add_edge(3, 4, 1);
    g.add_edge(4, 5, 7);
    g.add_edge(5, 6, 4);
    g.add_edge(3, 6, 1);
    g.add_edge(9, 6, 10);
    g.add_edge(2, 9, 3);
    g.add_edge(9, 8, 2);
    g.add_edge(6, 7, 6);
    
    double max = Graph::critical_path(&g);
    printf("Critical Path : %.3lf ns\n", max);
}

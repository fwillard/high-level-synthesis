#include "Parser.hpp"
#include "Component.hpp"
#include "graph.hpp"
#include <iostream>


using namespace std;
bool checkFileExists(char file[]);

int main(int argc, char *argv[])
{
	// check that 2 arguments are given
	if (argc != 3) {
		std::cout << "Warning: " << \
			"2 arguments are required!" << std::endl;
		return -1;
	}
	// check that input file exists
	if (!checkFileExists(argv[1])) {
		std::cout << "Warning: "  << \
			"File "  << argv[1]  << \
			" does not exist." << std::endl;
		return -1;
	}

    Parser p = Parser();
    p.parse(argv[1]);

    Graph g = p.get_graph();
    double max = Graph::critical_path(&g);
    printf("Critical Path : %.3lf ns\n", max);
/*    
    //THIS IS A TEST GRAPH FOR CRITICAL PATH CALCULATION
    Graph g;
    g.add_vertex(0, 0);
    g.add_vertex(1, 8);
    g.add_vertex(2, 8);
    g.add_vertex(3, 8);
    g.add_vertex(4, 11, true);
    g.add_vertex(5, 11, true);
    g.add_vertex(6, 11, true);
    g.add_vertex(7, 7);
    g.add_vertex(8, 7);
    g.add_vertex(9, 12);
    g.add_vertex(10, 7);
    g.add_vertex(11, 11, true);
    g.add_vertex(12, 0);
    
    //nop
    g.add_edge(0, 1);
    g.add_edge(0, 4);
    g.add_edge(0, 5);
    g.add_edge(0, 9);
    g.add_edge(0, 6);
    g.add_edge(0, 3);
    g.add_edge(0, 11);
    
    g.add_edge(1, 4);
    g.add_edge(5, 1);
    g.add_edge(2, 5);
    g.add_edge(6, 2);
    g.add_edge(3, 6);
    g.add_edge(10, 3);
    g.add_edge(4, 8);
    g.add_edge(5, 7);
    g.add_edge(4, 7);
    g.add_edge(5, 8);
    g.add_edge(6, 9);
    g.add_edge(6, 11);
    g.add_edge(8, 10);
    g.add_edge(9, 10);
    g.add_edge(7, 12);
    g.add_edge(11, 12);
    */
    //double max = Graph::critical_path(&g);
//    printf("Critical Path : %.3lf ns\n", max);

return 0;
}

bool checkFileExists(char file[])
{
	std::fstream fileStream;
	fileStream.open(file);
	bool returnValue = !fileStream.fail();
	fileStream.close();
	return returnValue;
}

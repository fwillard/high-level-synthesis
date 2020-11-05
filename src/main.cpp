#include "Parser.cpp"
#include <iostream>

using namespace std;

int main(){
    string testfile = "assignment_2_circuits/574a_circuit5.txt";

    Parser p = Parser();
    p.parse(testfile);
  
    Graph g;
    Graph::critical_path(g);
}

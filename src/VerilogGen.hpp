#ifndef VERILOGGEN
#define VERILOGGEN

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tgmath.h>
#include"Parser.hpp"

using namespace std;

class VerilogGen{
    public:

    VerilogGen(Parser *p);
    ~VerilogGen();
    void generate(string infile, string outfile);
    void setVerbosity(bool v);

    private:
    Parser *parser;
    int state_size;
    bool verbose;

    void generateHeader(string filename, stringstream &ss);
    void generateIO(stringstream &ss);
    void generateInitial(stringstream &ss);
    void generateLogic(stringstream &ss);
    void generateControl(stringstream &ss);
    void writeFile(string filename, stringstream &ss);
    void printOperation(string indent, stringstream &ss, int op);
};

#endif
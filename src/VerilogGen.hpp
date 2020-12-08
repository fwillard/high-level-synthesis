#ifndef VERILOGGEN
#define VERILOGGEN

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tgmath.h>
#include"Parser.hpp"

class VerilogGen{
    public:

    VerilogGen(Parser *p);
    ~VerilogGen();
    void generate(std::string infile, std::string outfile);
    void setVerbosity(bool v);

    private:
    Parser *parser;
    int state_size;
    bool verbose;

    void generateHeader(std::string filename, std::stringstream &ss);
    void generateIO(std::stringstream &ss);
    void generateInitial(std::stringstream &ss);
    void generateLogic(std::stringstream &ss);
    void generateControl(std::stringstream &ss);
    void writeFile(std::string filename, std::stringstream &ss);
    void printOperation(std::string indent, std::stringstream &ss, int op);
};

#endif
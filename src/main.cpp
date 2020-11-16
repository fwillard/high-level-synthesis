#include "Parser.hpp"
#include "Component.hpp"
#include "graph.hpp"
#include <iostream>
#include <cstring>

using namespace std;
bool checkFileExists(char file[]);
void writeVerilog(char inputFile[], char outputFile[]);
map<int, Component> tempComponents;


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

	// get reference to Parsers goodies
	tempComponents = p.components;

	writeVerilog(argv[1], argv[2]);

    Graph g = p.get_graph();
    double max = Graph::critical_path(&g);
    printf("Critical Path : %.3lf ns\n", max);

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


void writeVerilog(char inputFile[], char outputFile[])
{
	map<int, Component>::iterator it;
	string types[] = { "CONST", "INPUT", "OUTPUT", "WIRE", "REG", "ADD", "SUB", "MUL", "SHR", "SHL", "DIV", "MOD", "INC", "DEC", "MUX2x1", "COMP", "COMP", "COMP" };

	std::cout << "Verilog file:" << std::endl;

	//std::cout << "Output file shall be: " << \
		 outputFile <<  std::endl;

	// tokenize line
	std::vector <std::string> tokens;
	std::stringstream lineStream(inputFile);
	std::string tempString;
#if defined (_WIN32)
	while (getline(lineStream, tempString, '\\')) {
#else
	while (getline(lineStream, tempString, '/')) {
#endif
		tokens.push_back(tempString);
	}

	std::string temp = tokens.back();
	std::string circuitName = temp.substr(0, temp.size() - 4);

	std::stringstream ss;

	// up front stuff
	ss << "`timescale 1ns / 1ps" << std::endl;
	ss << "module circuit_" << circuitName << "(";

	
	// have all the outputs, inputs, wires shown in the module parameters
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (it->second.type == Component::INPUT || it->second.type == Component::OUTPUT)
		{
			ss << it->second.name.c_str();

			if (it != tempComponents.end()) ss << ", ";
		}
	}

	// add clock and reset for REG
	ss << "Clk, Rst";

	ss << ");" << std::endl;

	// inputs
	ss << "\t" << "input Clk, Rst;" << std::endl;
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (strcmp(types[it->second.type].c_str(), "INPUT") == 0)
		{
			ss << "\t" << "INPUT";
			if (it->second.sign)
			{
				ss << " SIGNED";
			}

			if (tempComponents.size() != 1)
			{
				ss << " [" << tempComponents.size() - 1 << ":0] ";
			}

			ss << it->second.name.c_str() << ";" << std::endl;
		}
	}

	ss << std::endl;

	// outputs
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (strcmp(types[it->second.type].c_str(), "OUTPUT") == 0)
		{
			ss << "\t" << "OUTPUT";
			ss << " [" << tempComponents.size() - 1 << ":0]";
			ss << " " << it->second.name.c_str() << ";" << std::endl;
		}
	}

	ss << std::endl;
	
	// wires
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (strcmp(types[it->second.type].c_str(), "WIRE") == 0)
		{
			ss << "\t" << "WIRE";
			if (it->second.sign)
			{
				ss << " SIGNED";
			}

			if (tempComponents.size() != 1)
			{
				ss << " [" << tempComponents.size() - 1 << ":0]";
			}

			ss << " " << it->second.name.c_str() << ";" << std::endl;
		}
	}

	ss << std::endl;

	// registers
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (strcmp(types[it->second.type].c_str(), "REG") == 0) {
			ss << "\t" << "REG";
			ss << " [" << tempComponents.size() - 1 << ":0]";
			ss << " " << it->second.name.c_str() << ";" << std::endl;
		}

	}

	ss << std::endl;
	/*
	for (const module& mod : circuitModules)
	{
		// if any input OR output is signed, the module needs to be signed.
		bool signVariableFound = false;

		for (const variable& var : mod.in)
		{
			if (var.sign) signVariableFound = true;
		}

		for (const variable& var : mod.out)
		{
			if (var.sign) signVariableFound = true;
		}

		ss << "\t";

		// prepend S to make module signed, unless if REG component
		if (signVariableFound && (mod.operation != "REG"))
		{
			ss << "S";
		}

		ss << mod.operation << " #(.DATAWIDTH(" << mod.width;
		ss << ")) " << mod.operation << "_" << mod.id << "(";

		// COMP
		if (mod.operation == "COMP")
		{
			ss << padVar(mod.in[0], mod) << ", " << padVar(mod.in[1], mod) << ", ";
			if (mod.moduleOutput == "eq")
			{
				ss << "0, 0, " << mod.out[0].name;
			}
			if (mod.moduleOutput == "gt")
			{
				ss << mod.out[0].name << ", 0, 0";
			}
			if (mod.moduleOutput == "lt")
			{
				ss << "0, " << mod.out[0].name << ", 0";
			}
		}

		// MUX
		else if (strcmp(mod.operation.c_str(), "MUX2x1") == 0)
		{
			ss << padVar(mod.in[1], mod) << ", ";
			ss << padVar(mod.in[2], mod) << ", ";
			ss << mod.in[0].name << ", ";
			ss << padVar(mod.out[0], mod);
		}

		// REG
		else if (strcmp(mod.operation.c_str(), "REG") == 0)
		{
			ss << padVar(mod.in[0], mod) << ", Clk, Rst, ";
			ss << mod.out[0].name;
		}

		// all other modules
		else
		{
			for (const variable& var : mod.in)
			{
				ss << padVar(var, mod) << ", ";
			}

			std::vector<variable> outputs = mod.out;

			for (int i = 0; i < outputs.size(); i++)
			{
				ss << padVar(outputs[i], mod);
				if (i != outputs.size() - 1) ss << ", ";
			}
		}
		ss << "); //" << mod.inputLine << std::endl;
	}

	*/
	ss << std::endl << "endmodule";
	ss << std::endl;

	std::cout << std::endl << ss.str() << std::endl;

	// write ss to file and finish
	std::ofstream outFile;
	outFile.open(outputFile);

	if (!outFile) {
		cerr << "Can't open output file " << outputFile << endl;
		exit(1);
	}

	outFile << ss.rdbuf();
	outFile.close();

	}

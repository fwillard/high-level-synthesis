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
			"Argument Format: dpgen netlistFile verilogFile" << std::endl;
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
	string types[] = { "CONST", "input", "output", "wire", "REG", "ADD", "SUB", "MUL", "SHR", "SHL", "DIV", "MOD", "INC", "DEC", "MUX2x1", "COMP", "COMP", "COMP" };

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
		if (it->second.type == Component::INPUT)
		{
			ss << "\t" << "input";
			if (it->second.sign)
			{
				ss << " signed";
			}

			if (it->second.datawidth != 1)
			{
				ss << " [" << it->second.datawidth - 1 << ":0] ";
			}

			ss << it->second.name.c_str() << ";" << std::endl;
		}
	}

	ss << std::endl;

	// outputs
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (it->second.type == Component::OUTPUT)
		{
			ss << "\t" << "output";
			ss << " [" << it->second.datawidth - 1 << ":0]";
			ss << " " << it->second.name.c_str() << ";" << std::endl;
		}
	}

	ss << std::endl;
	
	// wires
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (it->second.type == Component::WIRE)
		{
			ss << "\t" << "wire";
			if (it->second.sign)
			{
				ss << " signed";
			}

			if (it->second.datawidth != 1)
			{
				ss << " [" << it->second.datawidth - 1 << ":0]";
			}

			ss << " " << it->second.name.c_str() << ";" << std::endl;
		}
	}

	ss << std::endl;

	// registers
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (it->second.type == Component::REG && it->first >= 2) {
			ss << "\t" << "REG";
			ss << " [" << it->second.datawidth << ":0]";
			ss << " " << it->second.name.c_str() << ";" << std::endl;
		}

	}

	ss << std::endl;
	
	// modules
	// need to mod this to start at the first item past wire to get to modules
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if ((types[it->second.type] != "REG") && (types[it->second.type] != "INPUT") && (types[it->second.type] != "OUTPUT")
			&& (types[it->second.type] != "WIRE"))
		{

			/* DO THIS SIGNED STUFF LAST
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
			*/




			ss << "\t" << types[it->second.type] << " #(.DATAWIDTH(" << it->second.datawidth;
			ss << ")) " << types[it->second.type] << "_" << it->first << "(";


			/*
			// COMP
			if (types[it->second.type] == "COMP")
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
			*/

			// all other modules
			//else
			{
				/*
				for (const variable& var : mod.in)
				{
					ss <<  ", "; // padVar(var, mod) <<
				}*/

				for (int i = 0; i < it->second.outputs.size(); i++) {
					ss << tempComponents.at(it->second.outputs.at(i)).name << ", ";
				}
				/*
				std::vector<variable> outputs = mod.out;

				for (int i = 0; i < outputs.size(); i++)
				{
					ss << padVar(outputs[i], mod);
					if (i != outputs.size() - 1) ss << ", ";
				}
				*/
				for (int i = 0; i < it->second.inputs.size(); i++)
				{
					ss << tempComponents.at(it->second.inputs.at(i)).name;

					if (i != it->second.inputs.size() - 1) ss << ", ";
				}
			}


			ss << "); " << std::endl;

			

		} // end if loop
	}

	ss << std::endl << "endmodule";
	ss << std::endl;

	std::cout << std::endl << ss.str() << std::endl;

		// write ss to file and finish
		std::ofstream outFile;
		outFile.open(outputFile);

		if (!outFile) 
		{
			cerr << "Can't open output file " << outputFile << endl;
			exit(1);
		}

		outFile << ss.rdbuf();
		outFile.close();
	
	}

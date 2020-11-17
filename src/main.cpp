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

	ss << std::endl << "";

	// outputs
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (it->second.type == Component::OUTPUT)
		{
			ss << "\t" << "output reg";
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
		if ((it->second.type == Component::REG) && (it->first >= 2)) {
			ss << "\t" << "REG";
			ss << " [" << it->second.datawidth << ":0]";
			ss << " " << it->second.name.c_str() << ";" << std::endl;
		}

	}

	ss << std::endl;
	
	// modules
	for (it = tempComponents.begin(); it != tempComponents.end(); it++)
	{
		if (/*(it->second.type != Component::REG ) &&*/ (it->second.type != Component::INPUT ) && (it->second.type != Component::OUTPUT )
			&& (it->second.type != Component::WIRE ) && (it->second.type != Component::CONST ))
		{
			// make me pretty			
			ss << "\t";

			// if any input OR output is signed, the module needs to be signed.
			// prepend S to make module signed, unless if REG component
			if ((it->second.sign = 1) && (it->second.type != Component::REG ))
			{
				ss << "S";
			}
			
			if ((it->second.name != "INOP"))
			{
				ss << types[it->second.type] << " #(.DATAWIDTH(" << it->second.datawidth;
				ss << ")) " << types[it->second.type] << "_" << it->first << "(";
			}


			// if MUX
			if (it->second.type == Component::MUX2x1 )
			{

				ss << tempComponents.at(it->second.inputs.at(1)).name << ", ";
				ss << tempComponents.at(it->second.inputs.at(2)).name << ", ";
				ss << tempComponents.at(it->second.inputs.at(0)).name << ", ";
				// outputs
				for (int i = 0; i < it->second.outputs.size(); i++)
				{
					ss << tempComponents.at(it->second.outputs.at(i)).name;
				}
			}
			// if COMP
			else if (it->second.type == Component::COMP )
			{
				// inputs
				for (int i = 0; i < it->second.inputs.size(); i++)
				{
					ss << tempComponents.at(it->second.inputs.at(i)).name << ", ";
				}

				if (it->second.subtype == Component::EQ)
				{
					ss << "0, 0, ";

					// outputs
					for (int i = 0; i < it->second.outputs.size(); i++)
					{
						ss << tempComponents.at(it->second.outputs.at(i)).name;
					}
				}
				if (it->second.subtype == Component::GT) 
				{
					// outputs
					for (int i = 0; i < it->second.outputs.size(); i++)
					{
						ss << tempComponents.at(it->second.outputs.at(i)).name;
					}

					ss << ", 0, 0";
				}
				if (it->second.subtype == Component::LT) {
					ss << "0, ";

					// outputs
					for (int i = 0; i < it->second.outputs.size(); i++)
					{
						ss << tempComponents.at(it->second.outputs.at(i)).name;
					}

				    ss << ", 0";
				}

				// outputs
				for (int i = 0; i < it->second.outputs.size(); i++)
				{
					ss << tempComponents.at(it->second.outputs.at(i)).name;
				}

			}
			// if REG
			else if ((it->second.type == Component::REG ) && (it->second.name != "INOP"))
			{
				// outputs
				if(it->second.name == "OUOP"){
					ss << tempComponents.at(tempComponents.at(it->second.inputs.at(0)).inputs.at(0)).name;
				}
				for (int i = 0; i < it->second.outputs.size(); i++)
				{
					ss << tempComponents.at(it->second.outputs.at(i)).name;
				}
				ss << ", Clk, Rst, ";
				// inputs
				for (int i = 0; i < it->second.inputs.size(); i++)
				{
					ss << tempComponents.at(it->second.inputs.at(i)).name;
				}

				
			}
			else if(it->second.name != "INOP")
			{
				// inputs
				for (int i = 0; i < it->second.inputs.size(); i++)
				{
					ss << tempComponents.at(it->second.inputs.at(i)).name << ", ";
				}
				// outputs
				for (int i = 0; i < it->second.outputs.size(); i++)
				{
					ss << tempComponents.at(it->second.outputs.at(i)).name;
				}
			}

			if ((it->second.name != "INOP"))
			{
				ss << "); " << std::endl;
			}

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

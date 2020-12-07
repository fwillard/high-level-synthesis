#include "VerilogGen.hpp"

VerilogGen::VerilogGen(Parser *p){
    this->parser = p;
}

VerilogGen::~VerilogGen(){
   // delete this->parser; //prevent dangling pointer
}

void VerilogGen::generateHeader(string filename, stringstream &ss){
    // tokenize line
	std::vector <std::string> tokens;
	std::stringstream lineStream(filename);
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

    // up front stuff
	ss << "`timescale 1ns / 1ps" << endl << endl;
	ss << "module circuit_" << circuitName << "(";

    // have all the outputs, inputs, wires shown in the module parameters
    map<int, Operation>::iterator it;
    for(it = this->parser->operations.begin(); it != this->parser->operations.end(); it++){
        if(it->second.symbol == "input" || it->second.symbol == "output"){
            ss << it->second.name;
            if (it != this->parser->operations.end()) ss << ", ";
        }
    }

	// add clock and reset for REG
	ss << "Done, Start, Clk, Rst);" << endl;
}

void VerilogGen::generateIO(stringstream &ss){
    map<int, Operation>::iterator it;

    string keywords[3] = {"input","output","reg"};

    //inputs
    map<int,string> signed_items;
    map<int,string> items;
    for(string keyword : keywords){
        for(it = this->parser->operations.begin(); it != this->parser->operations.end(); it++){
            if(it->second.symbol == keyword){
                if(it->second.sign){ //signed inputs
                    if(signed_items.find(it->second.datawidth) == signed_items.end()){ //new datawidth
                        if(it->second.datawidth > 1){
                            signed_items.insert({it->second.datawidth,"\t" + keyword + " signed [" + to_string(it->second.datawidth -1) + ":0] " + it->second.name});
                        }
                        else{
                            signed_items.insert({it->second.datawidth,"\t" + keyword + " signed " + it->second.name});
                        }   
                    }
                    else{ //existting datawidth
                        signed_items.at(it->second.datawidth) = signed_items.at(it->second.datawidth) + ", " + it->second.name;
                    }
                }
                else{ //unsigned inputs
                    if(items.find(it->second.datawidth) == items.end()){ //new datawidth
                        if(it->second.datawidth > 1){
                            items.insert({it->second.datawidth,"\t" + keyword + " [" + to_string(it->second.datawidth -1) + ":0] " + it->second.name});
                        }
                        else{
                            items.insert({it->second.datawidth,"\t" + keyword + " " + it->second.name});
                        }   
                    }
                    else{ //existting datawidth
                        items.at(it->second.datawidth) = items.at(it->second.datawidth) + ", " + it->second.name;
                    }
                }
            }
        }
        //add items to the stream
        for(auto const& it : items){
            ss << it.second << ";" << endl;
        }
        //add signed items to the stream
        for(auto const& it : signed_items){
            ss << it.second << ";" << endl;
        }

        items.clear();
        signed_items.clear();
    }

    //add state register and done output
    ss << endl << "\tinput Start;" << endl;
    ss << "\toutput Done;" << endl;
    this->state_size = (int)ceil(log2(this->parser->states.size()));
    ss << "\treg [" << to_string(this->state_size-1) << ":0] state;" << endl; //state register based on the number of states
}

void VerilogGen::generateInitial(stringstream &ss){
    ss << "\tinitial begin" << endl;
    ss << "\t\tstate = " << to_string(this->state_size) << "'d0;" << endl;
    ss << "\tend" << endl;
}

void VerilogGen::generateLogic(stringstream &ss){
    ss << "\talways @(state) begin" << endl;
    ss << "\t\tcase(state)" << endl;
    
    vector<int> state;
    for(int i = 1; i< this->parser->states.size()-1; i++){ //skip first and last state which are static
        state = this->parser->states.at(i);
        if(this->parser->operations.at(state.at(0)).symbol == "if" || this->parser->operations.at(state.at(0)).symbol == "NOP") //skip ifs, they are handled in the state control
            continue;

        ss << "\t\t\t" << to_string(this->state_size) << "'d" << i << ":" << endl;//list case

        for(int op : state){
            if(this->parser->operations.at(op).symbol != "if" && this->parser->operations.at(op).symbol != "NOP") //skip over if states, they are isued in the state control
                printOperation("\t\t\t\t", ss, op);
        }
    }

    ss << "\t\t\t" << this->state_size << "'d" << this->parser->states.size()-1 << ":" << endl;
    ss << "\t\t\t\tdone = 1;" << endl;

    ss << "\t\tendcase" << endl; //end case
    ss << "\tend" << endl;   //end always
}

void VerilogGen::generateControl(stringstream &ss){
    ss << "\talways @(posedge Clk or posedge Rst) begin" << endl;
    ss << "\t\tif (Rst)" << endl;
    ss << "\t\t\tstate = " << to_string(this->state_size) << "'d0;" << endl;
    ss << "\t\telse" << endl;
    ss << "\t\t\tcase(state)" << endl;

    //generate the wait state
    ss << "\t\t\t\t" << this->state_size << "'d0:" << endl;
    ss << "\t\t\t\t\t" << "if (Start)" << endl;
    ss << "\t\t\t\t\t\t" << "state = " << this->state_size << "'d1;" <<endl;

    vector<int> state;
    for(int i = 1; i< this->parser->states.size()-1; i++){ //0 and end state are already taken care of
        state = this->parser->states.at(i);
        if(this->parser->operations.at(state.at(0)).symbol == "NOP") //safeguard
            continue;
        ss << "\t\t\t\t" << to_string(this->state_size) << "'d" << i << ":" << endl;//list case
        if(this->parser->operations.at(state.at(0)).symbol == "if"){ //generate if
            ss << "\t\t\t\t\t" << "if (" << this->parser->operations.at(this->parser->operations.at(state.at(0)).arg0_id).name << ")" << endl;
            ss << "\t\t\t\t\t\t" << "state = " << this->state_size << "'d" << this->parser->operations.at(state.at(0)).true_state << ";" << endl;
            ss << "\t\t\t\t\t" << "else" << endl;
            ss << "\t\t\t\t\t\t" << "state = " << this->state_size << "'d" << this->parser->operations.at(state.at(0)).false_state << ";" << endl;
        }
        else{
            ss << "\t\t\t\t\t" << "state = " << this->state_size << "'d" << this->parser->operations.at(state.at(0)).true_state << ";" << endl;
        }
    }

    ss << "\t\t\t\t" << this->state_size << "'d" << this->parser->states.size()-1 << ":" << endl;
    ss << "\t\t\t\t\t" << "state = " << this->state_size << "'d0;" <<endl;

    ss << "\t\t\tendcase" << endl; //end case
    ss << "\tend" << endl;   //end always
}

void VerilogGen::printOperation(string indent, stringstream &ss, int op){
    if(this->parser->operations.at(op).symbol == "if"){// if this is an if, generate an if switchc
        return;
    } 
    int out = this->parser->operations.at(op).output_id;
    int a0 = this->parser->operations.at(op).arg0_id;
    int a1 = this->parser->operations.at(op).arg1_id;
    int a2 = this->parser->operations.at(op).arg2_id;

    ss << indent << this->parser->operations.at(out).name << " = "; //output =
    ss << this->parser->operations.at(a0).name << " ";              //output = arg0 
    ss << this->parser->operations.at(op).symbol << " ";            //output = arg0 <symbol> 
    ss << this->parser->operations.at(a1).name;                     //output = arg0 <symbol> arg1
    if(a2 > 0)
        ss << " : "<< this->parser->operations.at(a2).symbol;       //output = arg0 <symbol> arg1 : arg2
    ss << ";" << endl;
}

void VerilogGen::generate(string filename){

	std::stringstream ss; //stream used for writing
    generateHeader(filename, ss); //module header
    generateIO(ss);
    ss  << endl << endl;

    generateInitial(ss);
    ss  << endl << endl;

    generateLogic(ss);
    ss  << endl << endl;

    generateControl(ss);
    ss  << endl << endl;
    //end file
    ss << "endmodule";
    ss  << endl << endl;

    cout << ss.str();
}
#include "VerilogGen.hpp"

VerilogGen::VerilogGen(Parser *p){
    this->parser = p;
    this->verbose = false;
}

VerilogGen::~VerilogGen(){
   // delete this->parser; //prevent dangling pointer
}

void VerilogGen::setVerbosity(bool v){
    this->verbose = v;
}

void VerilogGen::generateHeader(std::string filename, std::stringstream &ss){
    // up front stuff
	ss << "`timescale 1ns / 1ps" <<std::endl <<std::endl;
	ss << "module HLSM (Clk, Rst, Start, Done";

    // have all the outputs, inputs, wires shown in the module parameters
    std::map<int, Operation>::iterator it;
    for(it = this->parser->operations.begin(); it != this->parser->operations.end(); it++){
        if(it->second.symbol == "input" || it->second.symbol == "output"){
            if (it != this->parser->operations.end()) ss << ", ";
            ss << it->second.name;
        }
    }

	ss << ");" << std::endl;
}

void VerilogGen::generateIO(std::stringstream &ss){
    std::map<int, Operation>::iterator it;

    std::string keywords[3] = {"input","output","reg"};

    //inputs
    std::map<int,std::string> signed_items;
    std::map<int,std::string> items;
    for(std::string keyword : keywords){
        for(it = this->parser->operations.begin(); it != this->parser->operations.end(); it++){
            if(it->second.symbol == keyword){
                if(it->second.sign){ //signed inputs
                    if(signed_items.find(it->second.datawidth) == signed_items.end()){ //new datawidth
                        if(it->second.datawidth > 1){
                            if(keyword == "output") {
                                signed_items.insert({it->second.datawidth,"\t" + keyword + " reg signed [" + std::to_string(it->second.datawidth -1) + ":0] " + it->second.name});
                            }
                            else{
                                signed_items.insert({it->second.datawidth,"\t" + keyword + " signed [" + std::to_string(it->second.datawidth -1) + ":0] " + it->second.name});
                            }
                            
                        }
                        else if(keyword == "output") {
                            signed_items.insert({it->second.datawidth,"\t" + keyword + " reg signed" + it->second.name});
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
                            
                            if(keyword == "output") {
                                items.insert({it->second.datawidth,"\t" + keyword + " reg [" + std::to_string(it->second.datawidth -1) + ":0] " + it->second.name});
                            }
                            else {
                                items.insert({it->second.datawidth,"\t" + keyword + " [" + std::to_string(it->second.datawidth -1) + ":0] " + it->second.name});
                            }
                        }
                        else if(keyword == "output") {
                            items.insert({it->second.datawidth,"\t" + keyword + " reg " + it->second.name});
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
            ss << it.second << ";" <<std::endl;
        }
        //add signed items to the stream
        for(auto const& it : signed_items){
            ss << it.second << ";" <<std::endl;
        }

        items.clear();
        signed_items.clear();
    }

    //add state register and done output and clk/rst
    ss <<std::endl << "\tinput Start;" <<std::endl;
    ss << "\toutput reg Done;" <<std::endl;
    ss <<std::endl << "\tinput Clk;" <<std::endl;
    ss << "\toutput reg Rst;" <<std::endl;
    
    this->state_size = (int)ceil(log2(this->parser->final_states.size()));
    ss << "\treg [" << std::to_string(this->state_size-1) << ":0] state;" <<std::endl; //state register based on the number of states
}

void VerilogGen::generateInitial(std::stringstream &ss){
    ss << "\tinitial begin" <<std::endl;
    ss << "\t\tstate = " << std::to_string(this->state_size) << "'d0;" <<std::endl;
    ss << "\t\tDone = 1'd0;" << std::endl;
    ss << "\tend" <<std::endl;
}

void VerilogGen::generateLogic(std::stringstream &ss){
    ss << "\talways @(state) begin" <<std::endl;
    ss << "\t\tcase(state)" <<std::endl;

    //reset all outputs to 0{
    ss << "\t\t\t" << std::to_string(this->state_size) << "'d0:" << std::endl;
    ss << "\t\t\t\tbegin" << std::endl;
    std::map<int, Operation>::iterator it;
    for(it = this->parser->operations.begin(); it != this->parser->operations.end(); it++){
        if(it->second.symbol == "output"){
            ss << "\t\t\t\t\t" << it->second.name << " = " << std::to_string(it->second.datawidth) << "'d0;"<< std::endl;
        }
    }
    ss << "\t\t\t\t\tDone = 1'd0;" << std::endl;
    ss << "\t\t\t\tend" << std::endl;
    
    std::vector<int> state;
    for(int i = 1; i< this->parser->final_states.size()-1; i++){ //skip first and last state which are static
        state = this->parser->final_states.at(i);
        if(state.empty())
            continue;
        if(this->parser->operations.at(state.at(0)).symbol == "if" || this->parser->operations.at(state.at(0)).symbol == "NOP") //skip ifs, they are handled in the state control
            continue;
        
        ss << "\t\t\t" << std::to_string(this->state_size) << "'d" << i << ":" <<std::endl;//list case
        
        ss << "\t\t\t\tbegin" <<std::endl;//begin

        for(int op : state){
            if(this->parser->operations.at(op).symbol != "if" && this->parser->operations.at(op).symbol != "NOP") //skip over if states, they are isued in the state control
                printOperation("\t\t\t\t", ss, op);
        }
        ss << "\t\t\t\tend" <<std::endl;//end
    }

    ss << "\t\t\t" << this->state_size << "'d" << this->parser->final_states.size()-1 << ":" <<std::endl;
    ss << "\t\t\t\tbegin" <<std::endl;//begin
    ss << "\t\t\t\t\tDone = 1'd1;" <<std::endl;
    ss << "\t\t\t\tend" <<std::endl;//end

    ss << "\t\tendcase" <<std::endl; //end case
    ss << "\tend" <<std::endl;   //end always
}

void VerilogGen::generateControl(std::stringstream &ss){
    ss << "\talways @(posedge Clk or posedge Rst) begin" <<std::endl;
    ss << "\t\tif (Rst)" <<std::endl;
    //ss << "\t\t\tbegin" <<std::endl;//begin
    ss << "\t\t\t\tstate = " << std::to_string(this->state_size) << "'d0;" <<std::endl;

    ss << "\t\telse" <<std::endl;
    ss << "\t\t\tcase(state)" <<std::endl;

    //generate the wait state
    ss << "\t\t\t\t" << this->state_size << "'d0:" <<std::endl;
    ss << "\t\t\t\t\tbegin" <<std::endl;//begin
    ss << "\t\t\t\t\t" << "if (Start)" <<std::endl;
    
    ss << "\t\t\t\t\t\t" << "state = " << this->state_size << "'d1;" << std::endl;
    
    ss << "\t\t\t\t\tend" <<std::endl;//end
    
    std::vector<int> state;
    for(int i = 1; i< this->parser->final_states.size()-1; i++){ //0 and end state are already taken care of
        state = this->parser->final_states.at(i);
        if(!state.empty() && this->parser->operations.at(state.at(0)).symbol == "NOP") //safeguard
            continue;
        ss << "\t\t\t\t" << std::to_string(this->state_size) << "'d" << i << ":" <<std::endl;//list case
        ss << "\t\t\t\t\tbegin" <<std::endl;//begin

        std::vector<int> if_ops;
        if(!state.empty()){
            for(int j = 0; j<state.size(); j++){
                if(this->parser->operations.at(state.at(j)).symbol == "if")
                    if_ops.push_back(state.at(j));
            }
        }

        if(if_ops.empty()){ //if there isnt an if
            if(!state.empty()){
                ss << "\t\t\t\t\t" << "state = " << this->state_size << "'d" << std::to_string(this->parser->operations.at(state.at(0)).true_state) << ";" <<std::endl;
            }
            else{
                ss << "\t\t\t\t\t" << "state = " << this->state_size << "'d" << i+1 << ";" <<std::endl;
            } 
        }
        else{
            for(int k : if_ops){ //generate if
                ss << "\t\t\t\t\t" << "if (" << this->parser->operations.at(this->parser->operations.at(k).arg0_id).name << ")" <<std::endl;
                ss << "\t\t\t\t\t\t" << "state = " << this->state_size << "'d" << this->parser->operations.at(k).true_state << ";" <<std::endl;
                ss << "\t\t\t\t\t" << "else" <<std::endl;
                ss << "\t\t\t\t\t\t" << "state = " << this->state_size << "'d" << this->parser->operations.at(k).false_state << ";" <<std::endl;
            }
        }
        ss << "\t\t\t\t\tend" <<std::endl;//end
    }

    ss << "\t\t\t\t" << this->state_size << "'d" << this->parser->final_states.size()-1 << ":" <<std::endl;
    ss << "\t\t\t\t\tbegin" <<std::endl;//begin
    ss << "\t\t\t\t\t" << "state = " << this->state_size << "'d0;" << std::endl;
    ss << "\t\t\t\t\tend" <<std::endl;//end

    ss << "\t\t\tendcase" <<std::endl; //end case
    ss << "\tend" <<std::endl;   //end always
}

void VerilogGen::printOperation(std::string indent, std::stringstream &ss, int op){
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
        ss << " : "<< this->parser->operations.at(a2).name;         //output = arg0 <symbol> arg1 : arg2
    ss << ";" <<std::endl;
}

void VerilogGen::writeFile(std::string filename, std::stringstream &ss){
        std::ofstream outFile;
		outFile.open(filename);

		if (!outFile) 
		{
			std::cerr << "Can't open output file " << filename <<std::endl;
			exit(1);
		}

        std::string outstr = ss.str();
        if(this->verbose){
            std::cout << "\n\nVerilog File Contents\n===========================================================================\n\n";
            std::cout << outstr;
        }

		outFile << outstr;
		outFile.close();
}

void VerilogGen::generate(std::string infile, std::string outfile){

	std::stringstream ss; //stream used for writing
    generateHeader(infile, ss); //module header
    generateIO(ss);
    ss  <<std::endl <<std::endl;

    generateInitial(ss);
    ss  <<std::endl <<std::endl;

    generateLogic(ss);
    ss  <<std::endl <<std::endl;

    generateControl(ss);
    ss  <<std::endl <<std::endl;
    //end file
    ss << "endmodule";
    ss  <<std::endl <<std::endl;
    
    writeFile(outfile,ss);
}

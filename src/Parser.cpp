#include "Parser.hpp"

Parser::Parser(){
    this->verbose = false;
}

Parser::~Parser(){

}

//helper function usede in predicessor calculations
std::vector<int> mergeVectors(std::vector<int> a,std::vector<int> b){
    std::vector<int> ret = a;
    ret.insert( ret.end(), b.begin(), b.begin() + b.size() );

    std::vector<int>::iterator ip;
    sort(ret.begin(), ret.end());
    ip = std::unique(ret.begin(), ret.end()); 
    ret.resize(std::distance(ret.begin(), ip));

    return ret;
}

void Parser::parse(const std::string filename){    
    
    tokenize(filename);

    if(this->verbose){
        std::cout << "\n\nTokens\n===========================================================================\n\n";
        for(std::vector<std::string> line : this->tokens){
            for(std::string token: line){
                std::cout << token << " ";
            }
            std::cout << "\n";
        }
    }

    //loop through all tokens re-populating the list of operations
    this->operations.clear(); 
    this->states.clear();

    std::tuple<Operation, Operation> NOPs = generateIO(); //generare all IO operations, these dont impact the graph

    //Add INOP first
    std::vector<int> wait_state = {0};
    this->states.push_back(wait_state);
    Operation INOP = std::get<0>(NOPs);
    this->operations.insert({0,INOP});

    generateOperations(this->tokens);

    

    //Add OUOP last
    std::vector<int> end_state = {(int)operations.size()};
    Operation OUOP = std::get<1>(NOPs);
    OUOP.id = (int)operations.size();
    for(auto const& it : this->operations){//Add predecessors for OUOP
        if(it.second.symbol == "output"){
            std::vector<int> tmp;
            OUOP.predisessors = mergeVectors(OUOP.predisessors,getPreds(it.second.id,tmp));
        }
    }
    this->states.push_back(end_state);
    this->operations.insert({(int)operations.size(),OUOP}); 

    //Add predecessors for all if blocks
    for(std::vector<int> state : this->states){
        if(this->operations.at(state.at(0)).symbol == "if"){ //if this state has an if block, it will be alone
            for( int op : this->states.at(this->operations.at(state.at(0)).true_state) ){ //loop through true block
                this->operations.at(op).predisessors.push_back(state.at(0)); //add id of if
                this->operations.at(op).predisessors = mergeVectors(this->operations.at(op).predisessors,this->operations.at(op).predisessors);
            } 
            for(int op : this->states.at(this->operations.at(state.at(0)).true_state)){ //loop through false block, even if it is OUOP
                this->operations.at(op).predisessors.push_back(state.at(0)); //add id of if
                this->operations.at(op).predisessors = mergeVectors(this->operations.at(op).predisessors,this->operations.at(op).predisessors);
            }
            
        }
    }

    //Print everything to console (if set) for debugging and visibility 
    if(this->verbose){
        std::cout << "\n\n States and Operations\n===========================================================================\n\n";
        print_operations();
    }    
}

bool Parser::tokenize(const std::string filename){
    
    std::vector<std::vector<std::string> > current_lines = std::vector<std::vector<std::string> >();
    
    std::ifstream infile(filename);

    if(!infile){
        throw ParserException("ERROR: Input file \"" + filename +"\" could not be found");
    }

    std::string token_string; 
    while (std::getline(infile, token_string))
    {
        std::vector<std::string> current_tokens = std::vector<std::string>();
        std::istringstream iss(token_string); 

        for(std::string s; iss >> s; ){ 
            if(s.find("//") != std::string::npos){
                break;
            }
            current_tokens.push_back(s); 
        }
        if(current_tokens.size() > 0){
            current_lines.push_back(current_tokens);
        }
    }

    this->tokens = current_lines;
    return true;
}

std::tuple<Operation,Operation> Parser::generateIO(){
    //This generates all of inputs outputs and variables(reges) for the sake of verilog
    //Each of these components will be left out of the graph for the purpose of scheduling. They are only useful for verilog
    //The inputs and outputs of each of these will not be populated, as they are just tracked as INOP and OUOP
    //its set up to be able to configure INOP and OUOP based on variables, but currently it does not matter. 
    //Variables will be populated with data in generateOperators

    Operation INOP = Operation("INOP", Resource_Type::NOP, false, 1, 0);
    INOP.true_state = 1;//proceed to first actual state
    Operation OUOP = Operation("OUOP", Resource_Type::NOP, false, 1, operations.size());
    OUOP.true_state = 0;//loop to beginning
    std::string symbols[3] = {"input", "output", "variable"};

    //Continually read the first line of the tokens, removing it after parsing, break if not reading input output or variable 
    while(true){  
        std::vector<std::string> line = this->tokens.at(0);

        if(line.size() < 3){ //make sure we dont run into an indexing error
            throw ParserException("ERROR: Inputs, Outputs, and Variables must be delcared with datawidth");
        }

        //NOTE: Configure INOP and OUOP here if needed

        if(find(begin(symbols), end(symbols), line.at(0)) != end(symbols)){
            std::string width_str = line.at(1);

            Operation new_op("default", Resource_Type::NOP, false, 1, 0);
            new_op.datawidth = extract_int(width_str);
            new_op.sign = width_str.find("UInt");
            new_op.symbol = line.at(0);
            if(line.at(0) == "variable")
                new_op.symbol = "reg";
            for(std::string name : std::vector<std::string>(line.begin()+2,line.end())){ //Loop though new vector of names
                std::string namestr = name;
                int pos = namestr.find(",");
                if (pos != std::string::npos)
                    namestr.erase(pos, 1);

                new_op.name = namestr;
                new_op.id = operations.size()+1;
                this->operations.insert({operations.size()+1,new_op});
            }
        }
        else{
            break;
        }

        this->tokens.erase(this->tokens.begin());
    }

    return std::make_tuple(INOP, OUOP);
}

std::tuple<std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>> Parser::getBrackets(std::vector<std::vector<std::string>> vec){
    //This function takes in a vector of parsed lines, starting with an if statement, and then returns
    //the "cut out" if/else bloock and the remaining lines from the original vector
    std::vector<std::vector<std::string>> remainder_vector = vec; //copy of input vector to cut
    std::vector<std::vector<std::string>> if_vector;
    std::vector<std::vector<std::string>> else_vector;
    
    std::vector<std::string> line = remainder_vector.at(0);
    if (line.at(0) != "if"){
        throw ParserException("DEV ERROR: only tokens starting with an 'if' line can be passed to getBrackets");
    }
    else {
        if (line.at(line.size()-1) !="{" ){
            throw ParserException("ERROR: If statements must be enclosed in brackets");
        }
    }
    int open_count = 0;   //number of open brackets
    int closed_count = 0; //number of closed brackets 

    bool record = false;
    
    do{ //generate if block
        std::vector<std::string> line = remainder_vector.at(0);
        if(find(begin(line),end(line),"{") != end(line)){
            open_count++;
        }
        else if(find(begin(line),end(line),"}") != end(line)){
            closed_count++;

            if(open_count == closed_count){
                record = false;
            }
        }

        if(record)
        if_vector.push_back(remainder_vector.at(0));

        remainder_vector.erase(remainder_vector.begin());

        record = true;
    } while(open_count != closed_count); 
    
    record = false;
    if (remainder_vector.size() > 0 && remainder_vector.at(0).at(0) == "else"){  //generate else block if present
        line = remainder_vector.at(0);
        do{
            std::vector<std::string> line = remainder_vector.at(0);
            if(find(begin(line),end(line),"{") != end(line)){
                open_count++;
            }
            else if(find(begin(line),end(line),"}") != end(line)){
                closed_count++;

                if(open_count == closed_count)
                    record = false;
            }

            if(record)
                else_vector.push_back(remainder_vector.at(0));

            remainder_vector.erase(remainder_vector.begin());

            record = true;
        } while(open_count != closed_count);
    }

    return make_tuple(if_vector, else_vector, remainder_vector); 
}

std::vector<int> Parser::getPreds(int target, std::vector<int> cstate){ 
//returns all operable nodes that output the target id, this is used during generateOperations only
//this function does not add if statements as predicessors 

    std::vector<int> preds;
    if(this->operations.at(target).symbol == "input"){ //if target is an input, the pred is INOP
        preds.push_back(0); //point to INOP
    } 
    else{ //if target is an output or variable, loop through all existing nodes in all states 
        for(int op = cstate.size()-1; op >=0; op--){ //loop through unsaved "current_state" backwards
            if(this->operations.at(cstate.at(op)).output_id == target){
                preds.push_back(cstate.at(op));
            }
        }
        for(std::vector<int> state : this->states){ //loop through saved states forwards, shouldnt make a difference
            for(int op = state.size()-1; op >=0; op--){ 
                if(this->operations.at(state.at(op)).output_id == target){
                    preds.push_back(state.at(op));
                }
            }
        }
    }
    return preds;
}

void Parser::generateOperations(std::vector<std::vector<std::string>> tokens){
    //this recursive function generates a map of operations that later need to be parsed for states
    std::vector<std::vector<std::string>> if_tokens;
    std::vector<std::vector<std::string>> else_tokens;
    std::vector<int> current_state; //this creastes a new state on each call of generateOperations

    while(tokens.size()>0){ //loop until tokens are exhausted
        std::vector<std::string> line = tokens.at(0);
        Operation current_op; 

        if(line.at(0) == "if"){ //when an if is encountered, create a new state and set the current operation to that new if operation. 
            //this->states.push_back(current_state);
            //create a new if operation, most configuration in handled in the ifoperation constructor
            //else (false state) is handled after the initial recursive call

            //Save the current operation sequence as the state before the new if state
            if(current_state.size() >0){
                this->states.push_back(current_state);
                current_state.clear();
            }

            //Create new if operation and inject it into its own state, immidiately added
            int id = this->operations.size(); //seperated out so this can be used after recursive call
            IfOperation if_op(id_by_name(line.at(2)), this->states.size(), -1);
            if_op.id = id;
            if_op.arg0_id = id_by_name(line.at(2));
            if_op.name = "IF_" + line.at(2);
            if_op.true_state = this->states.size()+1;
            if_op.symbol = "if";
            if_op.predisessors = getPreds(id_by_name(line.at(2)),current_state); //only one argument to check
            this->operations.insert({id,if_op});

            current_state.push_back(if_op.id);
            this->states.push_back(current_state);
            current_state.clear();

            std::tuple<std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>> split_tokens;
            split_tokens = getBrackets(tokens);

            if_tokens = std::get<0>(split_tokens);
            else_tokens = std::get<1>(split_tokens);
            tokens = std::get<2>(split_tokens);

            generateOperations(if_tokens);  
            current_state.clear();

            if(else_tokens.size()>0){
                this->operations.at(id).false_state = this->states.size();
                generateOperations(else_tokens); //handle else block (if it exists)
                current_state.clear();
            }else{
                this->operations.at(id).false_state = this->states.size();
            }
        }
        else{
            //set parsed members
            std::vector<int> preds; //vector to keep track of preds
            current_op.id = this->operations.size();
            current_op.name = "OP_" + std::to_string(this->operations.size()); 
            current_op.output_id = id_by_name(line.at(0));
            current_op.arg0_id = id_by_name(line.at(2)); //Assignment only
            preds = getPreds(id_by_name(line.at(2)),current_state);
            current_op.symbol = line.at(3);
            if(line.size() > 3){ //Arithmetic
                current_op.arg1_id = id_by_name(line.at(4));
                preds = mergeVectors(preds,getPreds(id_by_name(line.at(4)),current_state));
            }
            if(line.size() > 5){ //Comparitor
                current_op.arg2_id = id_by_name(line.at(6));
                preds = mergeVectors(preds,getPreds(id_by_name(line.at(6)),current_state));
            }else{
                current_op.arg2_id = -1;
            }

            current_op.predisessors = preds;
            current_op.type = generate_type(line.at(3));
            current_op.true_state = this->states.size()+1; //use the true op for non IF operations to indicate the next state

            current_state.push_back(this->operations.size());
            this->operations.insert({this->operations.size(),current_op});
            tokens.erase(tokens.begin()); //end by removing the component just added
        }
    }
    if(current_state.size() >0)
        this->states.push_back(current_state);
}

int Parser::id_by_name(const std::string name){
    for(int i = 0; i<this->operations.size(); i++){
        if(this->operations.at(i).name == name){
            return this->operations.at(i).id;
        }
    }

    throw ParserException("ERROR: Undeclared variable: " + name);
}

Resource_Type Parser::generate_type(std::string sym){
    std::string adder_args[2] = {"+","-"};
    std::string logic_args[9] = {">>","<<","%","++","--","?","<",">","=="};
    std::string nop_args[5]    = {"const","input","output","wire","register"};

    if(sym == "*")
        return Resource_Type::MULTIPLIER;
    if(sym == "/")
        return Resource_Type::DIVIDER;
    if(find(begin(adder_args),end(adder_args),sym) != end(adder_args))
        return Resource_Type::ADDER;
    if(find(begin(logic_args),end(logic_args),sym) != end(logic_args))
        return Resource_Type::LOGICAL;
    return Resource_Type::NOP; 
}

void Parser::print_operations(){
    std::string types[] = {"ADDER", "MULTIPLIER", "LOGICAL", "DIVIDER", "NOP"};

    std::map<int, Operation>::iterator it;

    //create title block
    std::cout << std::left << std::setw(2) << "ID" << " | "; 
    std::cout << std::left << std::setw(7) << "NAME" << " | ";
    std::cout << std::left << std::setw(10) << "TYPE" << " | ";
    std::cout << std::left << std::setw(8) << "SYM" << " | ";
    std::cout << std::left << std::setw(2) << "TS" << " | ";
    std::cout << std::left << std::setw(2) << "FS" << " | ";
    std::cout << std::left << std::setw(6) << "OUTPUT" << " | ";
    std::cout << std::left << std::setw(6) << "ARG 0" << " | ";
    std::cout << std::left << std::setw(6) << "ARG 1" << " | ";
    std::cout << std::left << std::setw(6) << "ARG 2" << " | ";
    std::cout << std::left << "PREDS" << " | ";
    //std::cout << "( Outputs | Inputs )\n";
    std::cout << "\n";
    std::cout << std::setfill('_') << std::setw(120) << "";
    std::cout << std::setfill(' ');
    std::cout << "\n";

    //output all components
    int count = 0;
    for( std::vector<int> state : this->states){
        std::cout << std::right << std::setfill('-') << std::setw(120) << "STATE " << std::to_string(count); //output the current state number
        count++;
        std::cout << std::setfill(' ');
        std::cout << "\n";
        for( int i : state){
            Operation op = this->operations.at(i);
            std::cout << std::left << std::setw(2) << op.id << " | "; 
            std::cout << std::left << std::setw(7) << op.name << " | " ;
            std::cout << std::left << std::setw(10) << types[(int)op.type] << " | ";
            std::cout << std::left << std::setw(8) << op.symbol << " | ";
            std::cout << std::left << std::setw(2) << op.true_state << " | ";
            if(!op.isif){    
                std::cout << std::left << std::setw(2) << "" << " | ";
            }
            else{
                std::cout << std::left << std::setw(2) << op.false_state << " | ";
            }

            if(op.output_id >= 0 && !op.isif){
                std::cout << std::left << std::setw(6) << this->operations.at(op.output_id).name << " | ";
            }else{
                std::cout << std::left << std::setw(6) << "" << " | ";
            }
            if(op.arg0_id >= 0 && !op.isif){
                std::cout << std::left << std::setw(6) << this->operations.at(op.arg0_id).name << " | ";
            }else{
                std::cout << std::left << std::setw(6) << "" << " | ";
            }
            if(op.arg1_id >= 0 && !op.isif){
                std::cout << std::left << std::setw(6) << this->operations.at(op.arg1_id).name << " | ";
            }else{
                std::cout << std::left << std::setw(6) << "" << " | ";
            }
            if(op.arg2_id >= 0 && !op.isif){
                std::cout << std::left << std::setw(6) << this->operations.at(op.arg2_id).name << " | ";
            }else{
                std::cout << std::left << std::setw(6) << "" << " | ";
            }
            
            for(int p : op.predisessors){
                std::cout << p << ", ";
            }
            std::cout << "\n";
        }
        
    }
    std::cout << "\n\n";
}

int Parser::extract_int(const std::string str){
    std::string result = "";
    for (char c : str) {
        if (isdigit(c)) result += c;
    }

    return stoi(result); 
}

bool Parser::is_number(std::string str){
    for (char c : str) {
        if (!isdigit(c)) return false;
    }

    return true;
}

Graph Parser::get_graph(){
    std::string types[] = {"ADDER", "MULTIPLIER", "LOGICAL", "DIVIDER", "NOP"};//for debugging
    Graph g;

    std::map<int, Operation>::iterator it; //iterator to parse through map

    //add all operators to the graph (this excludes inputs, outputs, and variables)
    for(it = this->operations.begin(); it != this->operations.end(); it++){ //ignore empty
        if(it->second.type == Resource_Type::NOP && !(it->second.name == "INOP" || it->second.name == "OUOP"))
            continue;
        g.add_vertex(it->second.id, it->second.type);
        //std::cout << "Add Vertex: (" << it->second.id << " , " << types[(int)it->second.type] << ")\n";//for debugging
    }

    for(it = this->operations.begin(); it != this->operations.end(); it++){ //loop through all operators again, now connecting them using predicessors
        if(it->second.type == Resource_Type::NOP && !(it->second.name == "OUOP")) //INOP has no preds
            continue;
        for(int i : it->second.predisessors){
            //std::cout << "Add Edge: (" << it->second.id << " , " << i << ")\n";//for debugging
            g.add_edge(i, it->second.id);
        }   
    }

    return g;
}

void Parser::setVerbosity(bool v){
    this->verbose = v;
}

void Parser::generateSortedStates(Graph g, int lambda){
    std::vector<std::vector<int>> new_states;
    
    for(int i=0; i<lambda+2; i++){
        new_states.push_back(std::vector<int>());
    }

    new_states.at(0) = this->states.at(0);
    new_states.at(lambda+1) = this->states.at(this->states.size()-1);

    for(auto v : g.graph){
        if(v.second->type == Resource_Type::NOP)
            continue;

        new_states.at(v.second->cycle).push_back(v.second->id);
    }

    this->final_states = new_states; //stub
}

#include "Parser.hpp"

Parser::Parser(){
    this->verbose = false;
}

Parser::~Parser(){

}

void Parser::parse(const string filename){    
    
    tokenize(filename);

    if(this->verbose){
        cout << "\n\nTokens\n===========================================================================\n\n";
        for(vector<string> line : this->tokens){
            for(string token: line){
                cout << token << " ";
            }
            cout << "\n";
        }
    }

    //loop through all tokens re-populating the list of operations
    this->operations.clear(); 
    this->states.clear();

    

    tuple<Operation, Operation> NOPs = generateIO(); //generare all IO operations, these dont impact the graph

    //Add INOP first
    vector<int> wait_state = {0};
    this->states.push_back(wait_state);
    Operation INOP = get<0>(NOPs);
    this->operations.insert({0,INOP});

    generateOperations(this->tokens);

    //Add OUOP last
    vector<int> end_state = {(int)operations.size()};
    this->states.push_back(end_state);
    Operation OUOP = get<1>(NOPs);
    OUOP.id = (int)operations.size();
    this->operations.insert({(int)operations.size(),OUOP});

    

    //Print everything to console (if set) for debugging and visibility 
    if(this->verbose){
        cout << "\n\n States and Operations\n===========================================================================\n\n";
        print_operations();
    }    
}

bool Parser::tokenize(const string filename){
    
    vector<vector<string> > current_lines = vector<vector<string> >();
    
    ifstream infile(filename);

    if(!infile){
        throw ParserException("ERROR: Input file \"" + filename +"\" could not be found");
    }

    string token_string; 
    while (getline(infile, token_string))
    {
        vector<string> current_tokens = vector<string>();
        istringstream iss(token_string); 

        for(string s; iss >> s; ){ 
            if(s.find("//") != string::npos){
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

tuple<Operation,Operation> Parser::generateIO(){
    //This generates all of inputs outputs and variables(reges) for the sake of verilog
    //Each of these components will be left out of the graph for the purpose of scheduling. They are only useful for verilog
    //The inputs and outputs of each of these will not be populated, as they are just tracked as INOP and OUOP
    //its set up to be able to configure INOP and OUOP based on variables, but currently it does not matter. 
    //Variables will be populated with data in generateOperators

    Operation INOP = Operation("INOP", Resource_Type::NOP, false, 1, 0);
    INOP.true_state = 1;//proceed to first actual state
    Operation OUOP = Operation("OUOP", Resource_Type::NOP, false, 1, operations.size());
    OUOP.true_state = 0;//loop to beginning
    string symbols[3] = {"input", "output", "variable"};

    //Continually read the first line of the tokens, removing it after parsing, break if not reading input output or variable 
    while(true){  
        vector<string> line = this->tokens.at(0);

        if(line.size() < 3){ //make sure we dont run into an indexing error
            throw ParserException("ERROR: Inputs, Outputs, and Variables must be delcared with datawidth");
        }

        //NOTE: Configure INOP and OUOP here if needed

        if(find(begin(symbols), end(symbols), line.at(0)) != end(symbols)){
            string width_str = line.at(1);

            Operation new_op("default", Resource_Type::NOP, false, 1, 0);
            new_op.datawidth = extract_int(width_str);
            new_op.sign = width_str.find("UInt");
            new_op.symbol = line.at(0);
            for(string name : vector<string>(line.begin()+2,line.end())){ //Loop though new vector of names
                string namestr = name;
                int pos = namestr.find(",");
                if (pos != string::npos)
                    namestr.erase(pos, 1);

                new_op.name = namestr;
                this->operations.insert({operations.size()+1,new_op});
            }
        }
        else{
            break;
        }

        this->tokens.erase(this->tokens.begin());
    }

    return make_tuple(INOP, OUOP);
}

tuple<vector<vector<string>>,vector<vector<string>>,vector<vector<string>>> Parser::getBrackets(vector<vector<string>> vec){
    //This function takes in a vector of parsed lines, starting with an if statement, and then returns
    //the "cut out" if/else bloock and the remaining lines from the original vector
    vector<vector<string>> remainder_vector = vec; //copy of input vector to cut
    vector<vector<string>> if_vector;
    vector<vector<string>> else_vector;
    
    vector<string> line = remainder_vector.at(0);
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
        vector<string> line = remainder_vector.at(0);
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
            vector<string> line = remainder_vector.at(0);
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

void printTokens(vector<vector<string>> t){
    for(vector<string> line : t){
        for(string s : line){
            cout << s << " ";
        }
        cout << endl;
    }
}

void Parser::generateOperations(vector<vector<string>> tokens){
    //this recursive function generates a map of operations that later need to be parsed for states
    vector<vector<string>> if_tokens;
    vector<vector<string>> else_tokens;
    vector<int> current_state; //this creastes a new state on each call of generateOperations

    while(tokens.size()>0){ //loop until tokens are exhausted
        vector<string> line = tokens.at(0);
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
            if_op.name = "IF_" + to_string(this->states.size());
            if_op.true_state = this->states.size()+1;
            if_op.symbol = "if";
            this->operations.insert({id,if_op});

            current_state.push_back(if_op.id);
            this->states.push_back(current_state);
            current_state.clear();

            tuple<vector<vector<string>>,vector<vector<string>>,vector<vector<string>>> split_tokens;
            split_tokens = getBrackets(tokens);

            if_tokens = get<0>(split_tokens);
            else_tokens = get<1>(split_tokens);
            tokens = get<2>(split_tokens);

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
            current_op.id = this->operations.size();
            current_op.name = "OP_" + to_string(this->operations.size()); 
            current_op.output_id = id_by_name(line.at(0));
            current_op.arg0_id = id_by_name(line.at(2)); //Assignment only
            current_op.symbol = line.at(3);
            if(line.size() > 3) //Arithmetic
                current_op.arg1_id = id_by_name(line.at(4));
            if(line.size() > 5) //Comparitor
                current_op.arg2_id = id_by_name(line.at(6));

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

int Parser::id_by_name(const string name){
    for(int i = 0; i<this->operations.size(); i++){
        if(this->operations.at(i).name == name){
            return this->operations.at(i).id;
        }
    }

    throw ParserException("ERROR: Undeclared variable: " + name);
}

Resource_Type Parser::generate_type(string sym){
    string adder_args[2] = {"+","-"};
    string logic_args[9] = {">>","<<","%","++","--","?","<",">","=="};
    string nop_args[5]    = {"const","input","output","wire","register"};

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
    string types[] = {"ADDER", "MULTIPLIER", "LOGICAL", "DIVIDER", "NOP"};

    map<int, Operation>::iterator it;

    //create title block
    cout << left << setw(2) << "ID" << "  |  "; 
    cout << left << setw(7) << "NAME" << "  |  ";
    cout << left << setw(10) << "TYPE" << "  |  ";
    cout << left << setw(8) << "SYM" << "  |  ";
    cout << left << setw(2) << "TS" << "  |  ";
    cout << left << setw(2) << "FS" << "  |";
    //cout << "( Outputs | Inputs )\n";
    cout << "\n";
    cout << setfill('_') << setw(75) << "";
    cout << setfill(' ');
    cout << "\n";

    //output all components
    int count = 0;
    for( vector<int> state : this->states){
        cout << right << setfill('-') << setw(75) << "STATE " + to_string(count); //output the current state number
        count++;
        cout << setfill(' ');
        cout << "\n";
        for( int i : state){
            Operation op = this->operations.at(i);
            cout << left << setw(2) << op.id << "  |  "; 
            cout << left << setw(7) << op.name << "  |  " ;
            cout << left << setw(10) << types[(int)op.type] << "  |  ";
            cout << left << setw(8) << op.symbol << "  |  ";
            cout << left << setw(2) << op.true_state << "  |  ";
            if(!op.isif){    
                cout << left << setw(2) << "" << "  |  ";
            }
            else{
                cout << left << setw(2) << op.false_state << "  |  ";
            }
            cout << "\n";
        }
        
    }
    cout << "\n\n";
}

int Parser::extract_int(const string str){
    string result = "";
    for (char c : str) {
        if (isdigit(c)) result += c;
    }

    return stoi(result); 
}

bool Parser::is_number(string str){
    for (char c : str) {
        if (!isdigit(c)) return false;
    }

    return true;
}

Graph Parser::get_graph(){
    Graph g;

   // g.add_vertex(12, Resource_Type::NOP);
   // g.add_edge(0, 1);

    map<int, Operation>::iterator it; //iterator to parse through map

    //add all operators to the graph (this excludes inputs, outputs, and variables)
    for(it = this->operations.begin(); it != this->operations.end(); it++){ //ignore empty
        if(it->second.type == Resource_Type::NOP && !(it->second.symbol == "INOP" || it->second.symbol == "OUOP"))
            continue;
        g.add_vertex(it->second.id, it->second.type);
    }



    return g;
}

void Parser::setVerbosity(bool v){
    this->verbose = v;
}
#include "Parser.hpp"

Parser::Parser(){

}

Parser::~Parser(){

}

bool Parser::parse(const string filename){    
    
    tokenize(filename);
    generateComponents();

    cout << "\n\nTokens\n==================================================================\n\n";
    for(vector<string> line : this->tokens){
        for(string token: line){
            cout << token << " ";
        }
        cout << "\n";
    }

    cout << "\n\nComponents\n==================================================================\n\n";
    print_components();
    
    return true;
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

bool Parser::generateComponents(){
    //loop through all tokens re-populating the list of components
    this->components.clear(); 

    //generate inop and ouop
    this->components.insert({0,Component("INOP",Component::REG,false,1,0)});
    this->components.insert({1,Component("OUOP",Component::REG,false,1,1)});

    int id_count = 2;

    Component current_component;
    Component tmp; 

    for(vector<string> line : this->tokens){ // loop through all tokenized lines

        if(line.size() < 3){
            return false; 
        }
        int token_index = 1;

        int current_type = generate_type(line);

        //Declare local variables used in switch statemeelement at indexnt 
        string width_str, namestr, arg0, arg1, arg2, arg3;
        int width, output_index, index_1, index_2, index_3, pos;
        vector<Component> consts;
        switch(current_type){
            case 1:
            case 2:
            case 3:
            case 4:   //input, output, wire, reg' (Doesnt address input or output)
                current_component = Component();
                tmp = Component();

                width_str = line.at(1);
                width = -1;

                //Signed or Unsigned
                if(width_str.find("UInt") == 0){
                    current_component.sign = false;
                }
                else if(width_str.find("Int") == 0){
                    current_component.sign = true;
                }
                else{  
                    //This is an error, UInt or Int must be specified with a datawidth
                    return false;
                }
                
                //Datawidth
                //TODO: Add error checking to number parsing
                current_component.datawidth = extract_int(width_str);
                

                //Type
                current_component.type = static_cast<Component::component_type>(current_type);

                //Name and add to list of components
                for(int i = 2; i<line.size(); i++){
                    tmp = current_component;
                    namestr = line.at(i);

                    pos = namestr.find(",");
                    if (pos != string::npos)
                    {
                        namestr.erase(pos, 1);
                    }

                    tmp.name = namestr;

                    //assign ID for node creation
                    tmp.id = id_count;
                    id_count++;


                    if(current_type == Component::INPUT){
                        if(this->components.at(0).datawidth < tmp.datawidth)
                            this->components.at(0).datawidth = tmp.datawidth;
                        if(!this->components.at(0).sign && tmp.sign)
                            this->components.at(0).sign = true;
                        this->components.at(0).outputs.push_back(tmp.id);

                        tmp.inputs.push_back(0);
                    
                    }
                    else if(current_type == Component::OUTPUT){
                        if(this->components.at(1).datawidth < tmp.datawidth)
                            this->components.at(1).datawidth = tmp.datawidth;
                        if(!this->components.at(1).sign && tmp.sign)
                            this->components.at(1).sign = true; 
                        this->components.at(1).inputs.push_back(tmp.id);

                        tmp.outputs.push_back(1);                 
                    }

                        this->components.insert({tmp.id,tmp});
                    }

                break;
            case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:  
                arg0 =    line.at(0);
                arg1 =    line.at(2);
                arg2 =    line.at(4);

                current_component = Component(); //Create a new component for the operator 
                current_component.type = static_cast<Component::component_type>(generate_type(line));               
                current_component.id = id_count; id_count++;
                
                //handle the output
                if(!is_number(arg0)){
                    output_index = index_by_name(arg0);

                    if(output_index < 0){
                        //Throw error for unknown wire/output
                        throw ParserException("ERROR: Unknown Wire or component Output: " + arg0);
                    }

                    if(components.at(output_index).type <= 4 && components.at(output_index).type >= 2){ //if it is a wire/out/reg
                        components.at(output_index).inputs.push_back(current_component.id);
                    }
                    current_component.outputs.push_back(output_index);
                }
                else{
                    throw ParserException("ERROR: Output cannot be assigned to a constant");
                }

                //handle first argument
                if(!is_number(arg1)){
                    index_1 = index_by_name(arg1);

                    if(index_1 < 0){
                        //Throw error for unknown wire/input
                        throw ParserException("ERROR: Unknown Wire or component Input: " + arg1);
                    }
                    if(components.at(index_1).type >= 1 || components.at(index_1).type <= 4 && components.at(index_1).type != 2){ //if it is a wire/out/reg
                        components.at(index_1).outputs.push_back(current_component.id);
                    }
                    current_component.inputs.push_back(index_1);
                }
                else{ //handle constants
                    //add a new constant component, handles as INOP by graph
                    Component tmp = Component("Node_"+to_string(id_count), Component::CONST, current_component.sign, current_component.datawidth,id_count);
                    id_count++;
                    tmp.outputs.push_back(current_component.id);

                    current_component.inputs.push_back(tmp.id);
                    consts.push_back(tmp);
                }
                
                //handle second argument
                if(!is_number(arg2)){
                    index_2 = index_by_name(arg2);

                    if(index_2 < 0){
                        //Throw error for unknown wire/input
                        throw ParserException("ERROR: Unknown Wire or component Input: " + arg2);
                    }
                    if(components.at(index_2).type >= 1 || components.at(index_2).type <= 4 && components.at(index_2).type != 2){ //if it is a wire/out/reg
                        components.at(index_2).outputs.push_back(current_component.id);
                    }
                    current_component.inputs.push_back(index_2);
                }
                else{ //handle constants
                    Component tmp = Component("Node_"+to_string(id_count), Component::CONST, current_component.sign, current_component.datawidth,id_count);
                    id_count++;
                    tmp.outputs.push_back(current_component.id);

                    current_component.inputs.push_back(tmp.id);
                    consts.push_back(tmp);
                }

                if(current_type == 14){ //third input for mux
                    arg3 = line.at(6);

                    if(!is_number(arg3)){
                        index_3 = index_by_name(arg3);

                        if(index_3 < 0){
                            //Throw error for unknown wire/input
                            throw ParserException("ERROR: Unknown Wire or component Input: " + arg3);
                        }
                    if(components.at(index_3).type >= 1 || components.at(index_3).type <= 4 && components.at(index_3).type != 2){ //if it is a wire/out/reg
                        components.at(index_3).outputs.push_back(current_component.id);
                    }                        
                        current_component.inputs.push_back(index_3);
                    }
                    else{ //handle constants
                        Component tmp = Component("Node_"+to_string(id_count), Component::CONST, current_component.sign, current_component.datawidth,id_count);
                        id_count++;
                        tmp.outputs.push_back(current_component.id);

                        current_component.inputs.push_back(tmp.id);
                        consts.push_back(tmp);
                    }      
                }       

                //Other properties come from the input/output components
                current_component.name = "Node_" + to_string(current_component.id);
                current_component.datawidth = this->components.at(current_component.inputs.at(0)).datawidth;
                current_component.sign = this->components.at(current_component.inputs.at(0)).sign;

                for(int i =0; i<consts.size(); i++){ //add all consts that were generated as inputs
                    consts.at(i).datawidth = current_component.datawidth;
                    consts.at(i).sign = current_component.sign;
                    this->components.insert({consts.at(i) .id, consts.at(i) });
                }
                consts.clear(); //clear for next loop

                this->components.insert({current_component.id,current_component});

                break;
            default:        //handle assignment
                if(line.size() > 3){
                    //Throw error for illigal syntax
                    throw ParserException("ERROR: Illegal Assignment or Operator: " + line.at(3));
                }  

                if(line.size() < 3){
                    throw ParserException("ERROR: Invalid number of arguments");
                }        

                arg0 = line.at(0);
                arg1 = line.at(2);

                if(!is_number(arg0) && !is_number(arg1)){
                    output_index = index_by_name(arg0);
                    index_1 = index_by_name(arg1);

                    if(index_1 < 0){
                            //Throw error for unknown wire/input
                            throw ParserException("ERROR: Unknown Wire or component: " + arg1);
                    }

                    if(output_index < 0){
                            //Throw error for unknown wire/input
                            throw ParserException("ERROR: Unknown Wire or component: " + arg0);
                    }

                    this->components.at(output_index).inputs.push_back(index_1);
                    this->components.at(index_1).outputs.push_back(output_index);
                }
                else{
                    throw ParserException("ERROR: Invalid assignment");
                }

                break;
        }
    }
    return true;
}

int Parser::vector_index(const string &value, const vector<string> &vec)
{
    for(int i = 0; i<vec.size(); i++){
        if(vec.at(i) == value){
            return i;
        }
    }
    return -1;
}

int Parser::index_by_name(const string name){
    for(int i = 0; i<this->components.size(); i++){
        if(this->components.at(i).name == name){
            return i;
        }
    }
    return -1;
}

int Parser::generate_type(const vector<string> &line){
    vector<string> types = {"const","input","output","wire","register","+","-","*",">>","<<","/","%","++","--","?","<",">","=="};
    //                 NONE, INPUT, OUTPUT, WIRE, REG, ADD, SUB, MUL, MUX2x1, SHR, SHL, DIV, MOD, INC, DEC, COMP

    string start = line.at(0); //The first token in the line
    

    int tmp_type = vector_index(start, types);

    if(tmp_type >= 0){ //one of input, output, wire, register
        return tmp_type;
    }

    if(line.size() >= 4){
        string op =  line.at(3); // the fourth token is always the operator for non in, out, wire, reg
        tmp_type = vector_index(op, types);
    }     

    //Throw an error, the line is misformatted or uses an invalid operator
    if(tmp_type >15){
        tmp_type = 15;
    }
    return tmp_type;
}

void Parser::print_components(){
    string types[] = {"CONST", "INPUT", "OUTPUT", "WIRE", "REG", "ADD", "SUB", "MUL", "SHR", "SHL", "DIV", "MOD", "INC", "DEC", "MUX2x1", "COMP", "COMP", "COMP"};

    map<int, Component>::iterator it;

    //create title block
           
    cout << left << setw(2) << "ID" << "  |  "; 
    cout << left << setw(7) << "NAME" << "  |  ";
    cout << left << setw(6) << "TYPE" << "  |  ";
    cout << left << setw(4) << "SIGN" << "  |  ";
    cout << left << setw(2) << "DW\n";
    cout << setfill('_') << setw(75) << "";
    cout << setfill(' ');
    cout << "\n";

    //output all components
    for( it = this->components.begin(); it != this->components.end(); it++ ){
        
        cout << left << setw(2) << it->first << "  |  "; 
        cout << left << setw(7) << it->second.name << "  |  " ;
        cout << left << setw(6) << types[it->second.type] << "  |  ";
        cout << left << setw(4) << it->second.sign << "  |  ";
        cout << left << setw(2) << it->second.datawidth;
        cout << "  |  (";

        for(int i=0; i<it->second.outputs.size();i++){
            cout << this->components.at(it->second.outputs.at(i)).name << " ";
        }

        cout << " | ";

        for(int i=0; i<it->second.inputs.size();i++){
            cout << this->components.at(it->second.inputs.at(i)).name << " ";
        }
        cout << ")\n";
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

    map<int, Component>::iterator it;

    //add all verticies

    for ( it = this->components.begin(); it != this->components.end(); it++ ){
        if(it->first < 2){//inop and ouop
            g.add_vertex(it->first, 0, true);
        }
        else{
            if(it->second.type <4 && it->second.type > 0){ //skip on input, output, wire 0 is const which is handled as INOP
                g.add_vertex(it->first, 0 , false);
            }
            else
            {
                if(it->second.type == Component::CONST){ //treat consts as inop (only difference is type = reg)
                    g.add_vertex(it->first, this->weights.at(make_pair(Component::REG,it->second.datawidth)), it->second.type == Component::REG);
                }
                else {
                    g.add_vertex(it->first, this->weights.at(make_pair(it->second.type,it->second.datawidth)), it->second.type == Component::REG);
                }
            }
        }
    }

    //add all edges (look at outputs only), graph is directed
    for ( it = this->components.begin(); it != this->components.end(); it++ ){
        for(int i = 0; i < it->second.outputs.size(); i++){
            g.add_edge(it->second.id,it->second.outputs.at(i));
        }
    }

    return g;
}

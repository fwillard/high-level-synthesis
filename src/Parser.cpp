#include "Parser.hpp"

Parser::Parser(){
//    this->tokens = {};
//    this->components = {};
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
        return false;
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
    this->components.insert({0,Component("INOP",Component::NONE,false,1,0)});
    this->components.insert({1,Component("OUOP",Component::NONE,false,1,1)});

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
        Component *point = NULL;
        switch(current_type){
            case 1 ... 4:   //input, output, wire, reg' (Doesnt address input or output)
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
                if(current_type == 1){
                    this->components.at(0).datawidth = current_component.datawidth;
                    this->components.at(0).sign = current_component.sign;

                    current_component.inputs.push_back(0);
                }
                else if(current_type == 2){
                    this->components.at(1).datawidth = current_component.datawidth;
                    this->components.at(1).sign = current_component.sign;   

                    current_component.output = 1;                 
                }
                

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

                    this->components.insert({tmp.id,tmp});
                }

                break;
            case 5 ... 17:  //comparitor
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
                        return false;
                        //TODO throw error for unknown wire/output
                    }

                    if(components.at(output_index).type <= 4 && components.at(output_index).type > 1){ //if it is a wire/out/reg
                        components.at(output_index).inputs.push_back(current_component.id);
                    }
                    current_component.output = output_index;
                }
                else{
                    return false;
                    //TODO throw error, unknown syntax
                }

                //handle first argument
                if(!is_number(arg1)){
                    index_1 = index_by_name(arg1);

                    if(index_1 < 0){
                        return false;
                        //TODO throw error for unknown wire/input
                    }
                    if(components.at(index_1).type >= 1 || components.at(index_1).type <= 4 && components.at(index_1).type != 2){ //if it is a wire/out/reg
                        components.at(index_1).output = current_component.id;
                    }
                    current_component.inputs.push_back(index_1);
                }
                else{
                    return false;
                    //TODO throw error, unknown syntax
                }
                
                //handle second argument
                if(!is_number(arg2)){
                    index_2 = index_by_name(arg2);

                    if(index_2 < 0){
                        return false;
                        //TODO throw error for unknown wire/input
                    }
                    if(components.at(index_2).type >= 1 || components.at(index_2).type <= 4 && components.at(index_2).type != 2){ //if it is a wire/out/reg
                        components.at(index_2).output = current_component.id;
                    }
                    current_component.inputs.push_back(index_2);
                }
                else{
                    return false;
                    //TODO throw error, unknown syntax
                }

                if(current_type == 14){ //third input for mux
                    arg3 = line.at(6);

                    if(!is_number(arg3)){
                        index_3 = index_by_name(arg3);

                        if(index_3 < 0){
                            return false;
                            //TODO throw error for unknown wire/input
                        }
                    if(components.at(index_3).type >= 1 || components.at(index_3).type <= 4 && components.at(index_3).type != 2){ //if it is a wire/out/reg
                        components.at(index_3).output = current_component.id;
                    }                        
                        current_component.inputs.push_back(index_3);
                    }
                    else{
                        return false;
                        //TODO throw error, unknown syntax
                    }      
                }       

                //Other properties come from the input/output components
                current_component.name = "Node_" + to_string(current_component.id);
                current_component.datawidth = this->components.at(current_component.inputs.at(0)).datawidth;
                current_component.sign = this->components.at(current_component.inputs.at(0)).sign;

                this->components.insert({current_component.id,current_component});

               //multiplexer
                break;
            default:        //error: unknown operator
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
    vector<string> types = {"none","input","output","wire","register","+","-","*",">>","<<","/","%","++","--","?","<",">","=="};
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
    return tmp_type;
}

void Parser::print_components(){
    Component *current;
    string types[] = {"NONE", "INPUT", "OUTPUT", "WIRE", "REG", "ADD", "SUB", "MUL", "SHR", "SHL", "DIV", "MOD", "INC", "DEC", "MUX2x1", "COMP", "COMP", "COMP"};

    for(int i = 0; i< this->components.size(); i++){
        current = &this->components.at(i);
        cout << current->id << "  |  " << current->name << "  |  " << types[current->type] << "  |  " << current->sign << "  |  " << current->datawidth;
        cout << "  |  (" << this->components.at(current->output).name << " | ";
        for(int i=0; i<current->inputs.size();i++){
            cout << this->components.at(current->inputs.at(i)).name << " ";
        }
        cout << ")\n";
    }
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

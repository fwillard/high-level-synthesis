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

    Component current_component;
    Component tmp; 

    for(vector<string> line : this->tokens){ // loop through all tokenized lines

        if(line.size() < 3){
            return false; 
        }
        int token_index = 1;

        int current_type = generate_type(line);


        //Declare local variables used in switch statemeelement at indexnt 
        string width_str, namestr, arg1, arg2, arg3;
        int width, index, index_1, index_2, index_3, pos;
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

                    this->components.push_back(tmp);
                }

                break;
            case 5 ... 13:  //2-input operators
            case 15 ... 17:  //comparitor
                namestr = line.at(0);
                arg1 =    line.at(2);
                arg2 =    line.at(4);

                current_component = this->components.at(index); //copy of the component in memory, prevents pointer issues 


                index = index_by_name(line.at(0));
                if(index < 0){
                    //TODO: Missing wire/output error
                    return false;
                }

                point = this->components.data();
                //handle first argument
                if(!is_number(arg1)){
                    index_1 = index_by_name(arg1);

                    if(index_1 < 0){
                        return false;
                        //TODO throw error for unknown wire/input
                    }
                    point += index_1; //move pointer to selected index
                    current_component.inputs.push_back(point);
                }
                else{
                    
                }
                
                point = this->components.data();
                //handle second argument
                if(!is_number(arg2)){
                    index_2 = index_by_name(arg1);

                    if(index_2 < 0){
                        return false;
                        //TODO throw error for unknown wire/input
                    }
                    point += index_2; //move pointer to selected index
                    current_component.inputs.push_back(point);
                }
                else{
                
                }
                
                


                
                break;
            case 14:        //multiplexer
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
    vector<string> types = {"none","input","output","wire","register","+","-","*","?",">>","<<","/","%","++","--","<",">","=="};
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
        cout << current->name << "  |  " << types[current->type] << "  |  " << current->sign << "  |  " << current->datawidth << "\n";
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

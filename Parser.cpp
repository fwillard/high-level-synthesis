#include "Parser.hpp"

Parser::Parser(){
    this->tokens = vector<vector<string>>();
    this->components = vector<Parser::Component>();
}

Parser::~Parser(){

}

bool Parser::parse(string filename){    
    
    tokenize(filename);
    generateComponents();

    for(vector<string> line : this->tokens){
        for(string token: line){
            cout << token << " ";
        }

        cout << "\n";
    }
    
    return true;
}

bool Parser::tokenize(string filename){
    
    vector<vector<string>> current_lines = vector<vector<string>>();
    
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
            current_tokens.push_back(s); 
        }

        current_lines.push_back(current_tokens);
    }

    this->tokens = current_lines;
    return true;
}

bool Parser::generateComponents(){
    //loop through all tokens re-populating the list of components
    this->components.clear();

    Parser::Component current_component;

    for(vector<string> line : this->tokens){ // loop through all tokenized lines
        int token_index = 1;
        string start = line.get(0);
        //determine if this is a declaration or an assignment
        while(token_index < line.size()){
            if(start == "input"){

            }
            else if(start == "output"){

            }
            else if(start == "wire"){

            }
            else{

            }
        }
    }
    return true;
}

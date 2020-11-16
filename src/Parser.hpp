#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include "Component.hpp"
#include "graph.hpp"

using namespace std;

class Parser{
    public:
        //Public functions
        Parser();
        ~Parser();
        bool parse(const string filename);
        void print_components();
        Graph get_graph();
        
        //Public Variables
                
    private:
        //HelperFunctions
        bool tokenize(const string filename);
        bool generateComponents();
        int index_by_name(const string name);
        int vector_index(const string &value, const vector<string> &vec);
        int generate_type(const vector<string> &line);
        int extract_int(const string str);
        bool is_number(string str);
        

        //Private Variables
        vector<vector<string> > tokens;
        map<int,Component> components;

        //Weights lookup table
        map<pair<Component::component_type,int>, double> weights = {
            {make_pair(Component::REG,1),2.616},{make_pair(Component::REG,2),2.644},{make_pair(Component::REG,8),2.879},{make_pair(Component::REG,16),3.061},{make_pair(Component::REG,32),3.602},{make_pair(Component::REG,64),3.696},
            {make_pair(Component::ADD,1),2.704},{make_pair(Component::ADD,2),3.713},{make_pair(Component::ADD,8),4.824},{make_pair(Component::ADD,16),5.638},{make_pair(Component::ADD,32),7.270},{make_pair(Component::ADD,64),9.566},
            {make_pair(Component::SUB,1),3.024},{make_pair(Component::SUB,2),3.412},{make_pair(Component::SUB,8),4.890},{make_pair(Component::SUB,16),5.569},{make_pair(Component::SUB,32),7.253},{make_pair(Component::SUB,64),9.566},
            {make_pair(Component::MUL,1),2.438},{make_pair(Component::MUL,2),3.651},{make_pair(Component::MUL,8),7.453},{make_pair(Component::MUL,16),7.811},{make_pair(Component::MUL,32),12.395},{make_pair(Component::MUL,64),15.354},
            {make_pair(Component::COMP,1),3.031},{make_pair(Component::COMP,2),3.934},{make_pair(Component::COMP,8),5.949},{make_pair(Component::COMP,16),6.256},{make_pair(Component::COMP,32),7.264},{make_pair(Component::COMP,64),8.416},
            {make_pair(Component::MUX2x1,1),4.083},{make_pair(Component::MUX2x1,2),4.115},{make_pair(Component::MUX2x1,8),4.815},{make_pair(Component::MUX2x1,16),5.623},{make_pair(Component::MUX2x1,32),8.079},{make_pair(Component::MUX2x1,64),11.095},
            {make_pair(Component::SHR,1),3.644},{make_pair(Component::SHR,2),4.007},{make_pair(Component::SHR,8),5.178},{make_pair(Component::SHR,16),6.460},{make_pair(Component::SHR,32),8.819},{make_pair(Component::SHR,64),11.095},
            {make_pair(Component::DIV,1),0.619},{make_pair(Component::DIV,2),2.144},{make_pair(Component::DIV,8),15.439},{make_pair(Component::DIV,16),33.093},{make_pair(Component::DIV,32),86.312},{make_pair(Component::DIV,64),243.233},
            {make_pair(Component::SHL,1),3.614},{make_pair(Component::SHL,2),3.980},{make_pair(Component::SHL,8),5.152},{make_pair(Component::SHL,16),6.549},{make_pair(Component::SHL,32),8.565},{make_pair(Component::SHL,64),11.220},
            {make_pair(Component::MOD,1),0.758},{make_pair(Component::MOD,2),2.149},{make_pair(Component::MOD,8),16.078},{make_pair(Component::MOD,16),35.563},{make_pair(Component::MOD,32),88.142},{make_pair(Component::MOD,64),250.583},
            {make_pair(Component::INC,1),1.792},{make_pair(Component::INC,2),2.218},{make_pair(Component::INC,8),3.111},{make_pair(Component::INC,16),3.471},{make_pair(Component::INC,32),4.347},{make_pair(Component::INC,64),6.200},
            {make_pair(Component::DEC,1),1.792},{make_pair(Component::DEC,2),2.218},{make_pair(Component::DEC,8),3.108},{make_pair(Component::DEC,16),3.701},{make_pair(Component::DEC,32),4.685},{make_pair(Component::DEC,64),6.503}
        };
};

#endif

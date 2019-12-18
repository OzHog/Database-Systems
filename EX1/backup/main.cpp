//#include "./Syntactic_Categories/cyntactic_categories.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include "Query/Query.hpp"


//#define LINE_LENGTH 255
using namespace std;

//-----------Schems class----------

    //-----------Schems class hpp----------

    //#include <stdio.h>
    //#include <string>
    //#include <iostream>
    #include <fstream>

    #define LINE_LENGTH 255
    #define SCHEMS_LENGTH 2
    //using namespace std;

    class Column
    {

        private:
            string name;
            string type;
            Column(const char *name, const char *type) : name(name), type(type){};

        public:
            friend class Scheme;
            friend class Schems;
    };

    class Scheme
    {

        private:
            string name;
            Column **columns;
            int columns_amaunt;
            int getColumnsAmount(const char *columns);
            Scheme(const char *name, char *columns);
            ~Scheme(){
                for(int i = 0 ; i < columns_amaunt ; i++) {
                    delete columns[i];
                }
                delete columns;
            };

        public:
            friend class Schems;
            bool isExist(char *scheme);
    };

    class Schems
    {

        private:
            Scheme *scheme[SCHEMS_LENGTH];

        public:
            Schems();
            bool isSchemeExist(string scheme);
            bool isAttributeExist(string scheme, string attribute);
    };

    //-----------Schems class cpp----------

    int Scheme::getColumnsAmount(const char *columns)
    {
        int counter = 1;
        int i = 0;
        int columns_length = strlen(columns);
        for (i = 0; i < columns_length; i++)
        {
            if (columns[i] == ',')
                counter++;
        }

        return counter;
    }

    Scheme::Scheme(const char *name, char *columns)
    {
        int columns_amount = 1;
        int i = 0, char_position;
        char *buffer;
        string strbuffer, column_name, column_type;
        this->name = string(name);

        columns_amount = getColumnsAmount(columns);
        this->columns_amaunt = columns_amount;
        this->columns = (Column **)new Column *[columns_amount];

        buffer = strtok(columns, " ,\0");

        for (i = 0; i < columns_amount; i++)
        {
            strbuffer = string(buffer);
            char_position = strbuffer.find(":");
            column_name = strbuffer.substr(0, char_position);
            column_type = strbuffer.substr(char_position + 1, strbuffer.length() - 1);

            this->columns[i] = new Column(column_name.data(), column_type.data());
            buffer = strtok(NULL, " ,\0");
        }
    }

    Schems::Schems()
    {

        string input;
        char line[LINE_LENGTH] = {"\0"};
        char name[LINE_LENGTH] = {"\0"};
        char columns[LINE_LENGTH] = {"\0"};

        char *buffer;
        int i = 0;
        ifstream scheme_file;
        scheme_file.open("Schems/scheme.txt");
        if (scheme_file.is_open())
        {
            while (getline(scheme_file, input))
            {
                strcpy(line, input.data());
                buffer = strtok(line, "(");
                strcpy(name, buffer);

                buffer = strtok(NULL, ")");
                strcpy(columns, buffer);
                this->scheme[i] = new Scheme(name, columns);
                i++;
            }
        }
        else
        {
            cout << "Scheme file connection failed.\n";
        }
    }

    bool Schems::isSchemeExist(string scheme)
    {

        for (int i = 0; i < SCHEMS_LENGTH; i++)
        {
            if (this->scheme[i]->name.compare(scheme) == 0)
                return true;
        }

        return false;
    }

    bool Schems::isAttributeExist(string scheme, string attribute)
    {
        for (int i = 0; i < SCHEMS_LENGTH; i++)
        {
            if (this->scheme[i]->name.compare(scheme) == 0)
            {
                for (int j = 0; j < this->scheme[i]->columns_amaunt; j++)
                {
                    if (this->scheme[i]->columns[j]->name.compare(attribute) == 0)
                        return true;
                }

                return false;
            }
        }

        return false;
    }

//

//-----------O_D class----------

    //-----------O_D class hpp----------

    class Optional_Distinct
    {

        private:
            bool distinct;

            Optional_Distinct(string distinct)
            {
                if (distinct.compare("DISTINCT") == 0)
                    this->distinct = true;
                else
                    this->distinct = false;
            }

        public:
            static Optional_Distinct *parseO_D(string input);
    };

    //-----------O_D class cpp----------
    Optional_Distinct *Optional_Distinct::parseO_D(string input)
    {
        if (input.compare("DISTINCT") == 0 || input.compare("\0") == 0)
            return new Optional_Distinct(input);

        return nullptr;
    }
    //

//-----------Attribute class----------
    //-----------Attribute class hpp----------

    class Attribute
    {
        private:
            string scheme;
            string attribute;
            Attribute(string scheme, string attribute) : scheme(scheme), attribute(attribute){};
        public:
            static Attribute *parseAttribute(string attribute);
    };

    class Att_List
    {

        private:
            Attribute *attribute;
            Att_List *att_list;
            Att_List(Attribute *attribute, Att_List *att_list = nullptr) : attribute(attribute), att_list(att_list){};

        public:
            friend class Attribute_List;
            ~Att_List()
            {
                if (att_list)
                    delete att_list;
                
                delete attribute;
            };
            static Att_List *parseAttList(string str_att_list);
    };

    class Attribute_List
    {

        private:
            bool all;
            Att_List *att_list;
            Attribute_List(Att_List *att_list, bool all) : att_list(att_list), all(all){};

        public:
            static Attribute_List *parseAttributeList(string str_att_list);
            ~Attribute_List() { delete att_list; };
    };

    //-----------Attribute class cpp----------

    Attribute *Attribute::parseAttribute(string attribute)
    {
        Attribute *p_attribute = nullptr;
        Schems schems;

        size_t separator = attribute.find(".");
        string scheme = attribute.substr(0, separator);
        attribute.assign(attribute.substr(separator + 1));

        if (schems.isAttributeExist(scheme, attribute))
            p_attribute = new Attribute(scheme, attribute);

        return p_attribute;
    }

    Att_List *Att_List::parseAttList(string str_att_list)
    {
        Att_List *att_list = nullptr, *result = nullptr;
        Attribute *attribute;
        size_t separator;

        separator = str_att_list.find(",");
        //attribute = Att_List::cutAttribute(&str_att_list);

        if (separator == string::npos)
        {
            attribute = Attribute::parseAttribute(str_att_list);
        }
        else
        {
            attribute = Attribute::parseAttribute(str_att_list.substr(0, separator));

            att_list = parseAttList(str_att_list.substr(separator + 1));
        }

        if (attribute != nullptr)
            result = new Att_List(attribute, att_list);

        return result;
    }

    Attribute_List *Attribute_List::parseAttributeList(string str_att_list)
    {

        Att_List *att_list;
        Attribute_List *attribute_list = nullptr;
        bool all = false;

        if(str_att_list.compare("*") == 0) {
            all = true;
        }
        else{
            att_list = Att_List::parseAttList(str_att_list);
        }

        if (att_list != nullptr || all)
            attribute_list = new Attribute_List(att_list, all);

        return attribute_list;
    }

//

//-----------Table_List class----------
    //-----------Table_List class hpp----------
    class Table_List
    {

        private:
            string table;
            Table_List *table_list;
            Table_List(string table, Table_List *table_list = nullptr) : table(table), table_list(table_list){};

        public:
            ~Table_List()
            {
                if (table_list != nullptr)
                {
                    delete table_list;
                }
            };
            static Table_List *parseTableList(string str_table_list);
    };

    //-----------Table_List class cpp----------
    Table_List *Table_List::parseTableList(string str_table_list)
    {
        string table;
        Table_List *table_list = nullptr, *result = nullptr;
        Schems schems;
        size_t separator;

        separator = str_table_list.find(",");

        if (separator == string::npos)
        {
            table.assign(str_table_list);
        }
        else
        {
            table = str_table_list.substr(0, separator);
            table_list = Table_List::parseTableList(str_table_list.substr(separator + 1));
        }

        if (schems.isSchemeExist(table))
            result = new Table_List(table, table_list);

        return result;
    }

//

//-----------Condition class----------
    //-----------Condition class hpp----------
    #include <stack> 
    class Constant
    {

        public:
            enum eType
            {
                INTEGER,
                STRING,
                ATTRIBUTE
            };
            eType type;

            static Constant *parseConstant(string constant);
            
            ~Constant()
            {
                switch (type)
                {
                case INTEGER:
                    delete int_constant;
                    break;
                case STRING:
                    delete str_constant;
                    break;
                case ATTRIBUTE:
                    delete att_constant;
                    break;
                }
            }

        private:
            int *int_constant;
            string *str_constant;
            Attribute *att_constant;

            static bool isInteger(string input);
            Constant(int *int_constant) : int_constant(int_constant), str_constant(nullptr), att_constant(nullptr), type(INTEGER){};
            Constant(string *str_condition) : int_constant(nullptr), str_constant(str_constant), att_constant(nullptr), type(STRING){};
            Constant(Attribute *att_condition) : int_constant(nullptr), str_constant(nullptr), att_constant(att_constant), type(ATTRIBUTE){};

        
    };

    class Simple_Condition
    {

        public:
            enum eRel_Op
            {
                a_NotEqual_b,
                a_GreaterEqual_b,
                a_LowerEqual_b,
                a_Greater_b,
                a_Lower_b,
                a_Equal_b
            };
            static Simple_Condition *parseSimpleCondition(string simple_cond);
            ~Simple_Condition()
            {
                delete constant_a;
                delete constant_b;
            }

        private:
            Constant *constant_a;
            eRel_Op rel_op;
            Constant *constant_b;

            int *castString(string str);
            Simple_Condition(Constant *ondition_a, eRel_Op rel_op, Constant *condition_b) 
                            : constant_a(constant_a), rel_op(rel_op), constant_b(constant_b){};
            
    };

    class Condition
    {

        public:
            enum eOperator
            {
                OR = 2,
                AND = 3
                
            };
            static Condition *parseCondition(string condition);
            static size_t findRootOperand(string &condition, bool &hasBrackets);

        private:
        Simple_Condition *simple_condition;
        Condition *condition_a;
        eOperator _operator;
        Condition *condition_b;
        bool hasBrackets;

        Condition(Simple_Condition *simple_condition) : simple_condition(simple_condition){};
        Condition(Condition *condition_a, eOperator _operator, Condition *condition_b, bool hasBrackets) 
                : condition_a(condition_a), _operator(_operator), condition_b(condition_b), hasBrackets(hasBrackets){};

    };

    //-----------Condition class cpp----------


    bool Constant::isInteger(string input)
    {
        int input_len = input.length();
        for(int i = 0 ; i < input_len ; i++)
            if(input[i] < '0' || input[i] > '9')
                return false;

        return true;
    }

    Simple_Condition *Simple_Condition::parseSimpleCondition(string simple_cond)
    {
        Constant *constant_a, *constant_b;
        const string Rel_Op[6] = {"<>", ">=", "<=", ">", "<", "="};
        string str_condition_a, str_condition_b, *attribute;
        eRel_Op rel_op;
        size_t separator;
        Simple_Condition *simple_condition = nullptr;

        if ((simple_cond.find("AND") == string::npos) && (simple_cond.find("OR") == string::npos)
            && (simple_cond.find("(") == string::npos) && (simple_cond.find(")") == string::npos))
        {

            for (int i = 0; i < 6; i++)
            {
                separator = simple_cond.find(Rel_Op[i]);

                if (separator != string::npos)
                {
                    str_condition_a.assign(simple_cond.substr(0, separator));
                    rel_op = (eRel_Op)i;
                    int int_condition;
                    str_condition_b.assign(simple_cond.substr(separator + Rel_Op[i].length()));
                    constant_a = Constant::parseConstant(str_condition_a);
                    constant_b = Constant::parseConstant(str_condition_b);
                    break;
                }
            }

            if (constant_a && constant_b)
                simple_condition = new Simple_Condition(constant_a, rel_op, constant_b);
        }

        return simple_condition;
    }

    Constant *Constant::parseConstant(string constant)
    {
        int *int_constant = nullptr, num;
        string *str_constant = nullptr;
        Attribute *att_constant = nullptr;
        Constant *resulte = nullptr;

        if (constant.find(".") != string::npos)
        {
            att_constant = Attribute::parseAttribute(constant);

            if (att_constant)
                resulte = new Constant(att_constant);
        }
        else
        {
            if (isInteger(constant))
            {
                num = stoi(constant.c_str());

                int_constant = new int(num);
                resulte = new Constant(int_constant);
            }
            else
            {
                str_constant = new string(constant);
                resulte = new Constant(str_constant);
            }
        }

        return resulte;
    }

    size_t Condition::findRootOperand(string &condition, bool &hasBrackets)
    {
        stack<string> s;
        stack<size_t> s_index;
        string str_temp;
        size_t int_temp,open_braket_index = -1, close_braket_index = -1, root_operand_index = -1;

        string x; 
    
        // Traversing the Expression 
        for (int i = 0; i < condition.length(); i++) 
        { 
            switch(condition[i]) {

                case('('):
                    // Push the element in the stack 
                    s.push(string(condition.substr(i,1)));
                    s_index.push(i);
                    break;
                    
                case(')'):
                {   
                    if(s.empty())
                        return -1;

                    close_braket_index = i;
                    
                    // Store the top element
                    str_temp = s.top(); 
                    s.pop(); 

                    
                    int_temp = s_index.top();
                    s_index.pop();
                
                    while (!s.empty() && (str_temp.compare("AND") == 0 || str_temp.compare("OR") == 0))
                    {

                        root_operand_index = int_temp;
                        str_temp = s.top(); 
                        s.pop(); 
                        int_temp = s_index.top();
                        s_index.pop();
                    }
                    

                    if (str_temp.compare("(") != 0)
                    {
                        hasBrackets = false;
                        return false;
                    }
                        

                    open_braket_index = int_temp;
                    break;
                } 

            
            case('A'):
                str_temp.assign(condition.substr(i,3));
                
                if(str_temp.compare("AND") == 0) {
                    s.push(str_temp);
                    s_index.push(i);
                    i += 2;
                }
                break;

            case ('O'):
                str_temp.assign(condition.substr(i,2));
                
                if(str_temp.compare("OR") == 0) {
                    s.push(str_temp);
                    s_index.push(i);
                    i++;
                    
                }
                break;
            
            }

        } 

        if (!s.empty()) 
        {
            open_braket_index = -1;
            close_braket_index = -1;
            hasBrackets = false;
            do {
                str_temp = s.top();
                
                if(str_temp.compare("(") == 0)
                {
                    hasBrackets = false;
                    return -1;
                }
                
                s.pop(); 
                root_operand_index = s_index.top();
                s_index.pop();
            } while(!s.empty());
        }

        if(open_braket_index != -1 && close_braket_index != -1)
        {
            hasBrackets = true;
            condition.erase(close_braket_index, 1);
            condition.erase(open_braket_index, 1);
            root_operand_index--; // (OR) -> OR (char O index's changed)
        }
    
        return root_operand_index; 
    }

    Condition *Condition::parseCondition(string condition)
    {
        string str_condition_a, str_condition_b, bracket, operand;
        char ch;
        Simple_Condition *simple_condition = nullptr;
        Condition *condition_a = nullptr, *condition_b = nullptr;
        eOperator _operator;
        Condition *finalcondition = nullptr;
        size_t and_or_index = 0, open_bracket = 0, close_bracket = 0;
        bool hasBrackets = false;

        //if(condition.back() == ';'){
            //condition.erase(condition.end()); //delete ';'

        simple_condition = Simple_Condition::parseSimpleCondition(condition);
        if(!simple_condition) 
        {
            if(condition.find("AND") != string::npos || condition.find("OR") != string::npos)
            {
                and_or_index = findRootOperand(condition, hasBrackets);
                if(and_or_index != -1) {
                    str_condition_a.assign(condition.substr(0 , and_or_index  - 1));

                    ch = condition[and_or_index];
                    if(ch == 'O')
                        str_condition_b.assign(condition.substr(and_or_index + strlen("OR") + 1));
                    else
                        str_condition_b.assign(condition.substr(and_or_index + strlen("AND") + 1));

                    condition_a = Condition::parseCondition(str_condition_a);
                    condition_b = Condition::parseCondition(str_condition_b);
                }
            }
            else 
            {
                if((condition.front() == '(') == (condition.back() == ')')) 
                {
                    hasBrackets = (condition.front() == '(') && (condition.back() == ')');

                    if(hasBrackets) //delete brackets
                    {
                        condition.erase(condition.find("("),1);
                        condition.erase(condition.find(")"),1);
                        //finalcondition = Condition::parseCondition(condition);
                        simple_condition = Simple_Condition::parseSimpleCondition(condition);
                    }
                }
            }
        
        }
        
        if(simple_condition)
            finalcondition = new Condition(simple_condition);
        else if(condition_a && condition_b)
            finalcondition = new Condition(condition_a, _operator, condition_b, hasBrackets);


        return finalcondition;
    }

//

//-----------Query class----------

//-----------Query class hpp----------
#define STRQUERY_LENGTH 7

class Query
{

    private:
        string *strquery;
        Optional_Distinct *o_d;
        Attribute_List *attribute_list;
        Table_List *table_list;
        Condition *condition;

        Query(string *strquery, Optional_Distinct *o_d, Attribute_List *attribute_list, Table_List *table_list, Condition *condition)
                :strquery(strquery), o_d(o_d), attribute_list(attribute_list), table_list(table_list), condition(condition){};
        ~Query() {
            delete strquery;
            delete o_d;
            delete attribute_list;
            delete table_list;
            delete condition;
        }

        static string *brackInput(char *input);
        enum eQueryComponent
        {
            SELECT,
            O_D,
            ATTRIBUTE_LIST,
            FROM,
            TABLE_LIST,
            WHERE,
            CONDITION
        };

        static eQueryComponent eComponent;

    public:
        static Query* parseQuery(char *query);
};

//-----------Query class cpp----------
Query* Query::parseQuery(char *input)
{

    string *strquery = brackInput(input);
    Optional_Distinct *o_d;
    Attribute_List *attribute_list;
    Table_List *table_list;
    Condition *condition;
    Query *query = nullptr;
    eQueryComponent component = SELECT;

    do
    {
        switch (component)
        {
        case SELECT:
            if (strquery[component].compare("SELECT") != 0)
            {
                cout << "Parsing SELECT failed" << endl;
                return nullptr;
            }
            break;

        case O_D:
            o_d = Optional_Distinct::parseO_D(strquery[component]);
            if (o_d == nullptr)
            {
                cout << "Parsing <o_d> failed" << endl;
                return nullptr;
            }
            break;
        case ATTRIBUTE_LIST:
            attribute_list = Attribute_List::parseAttributeList(strquery[component]);
            if (attribute_list == nullptr)
            {
                cout << "Parsing <attribute_list> failed" << endl;
                return nullptr;
            }
            break;
        case FROM:
            if (strquery[component].compare("FROM") != 0)
            {
                cout << "Parsing FROM failed" << endl;
                return nullptr;
            }
            break;
        case TABLE_LIST:
            table_list = Table_List::parseTableList(strquery[component]);
            if (table_list == nullptr)
            {
                cout << "Parsing <table_list> failed" << endl;
                return nullptr;
            }
            break;
        case WHERE:
            if (strquery[component].compare("WHERE") != 0)
            {
                cout << "Parsing WHERE failed" << endl;
                return nullptr;
            }
            break;
        case CONDITION:
            condition = Condition::parseCondition(strquery[component]);
            if (condition == nullptr)
            {
                cout << "Parsing <condition> failed" << endl;
                return 0;
            }
            break;
        }
        component = eQueryComponent(component + 1);
    } while (component <= CONDITION);

    query = new Query(strquery, o_d, attribute_list, table_list, condition);

    return query;
}

string *Query::brackInput(char *input)
{
    int i = 0;
    char *buffer;
    int input_len = strlen(input);

    string *bracked_input = new string[7];
    buffer = strtok(input, " ");

    for (i; i < 6; i++)
    {
        if (i == 2 && strcmp(buffer, "FROM") == 0)
        {
            bracked_input[i].assign(bracked_input[i - 1]);
            bracked_input[i - 1].assign("\0");
            i++;
        }
        bracked_input[i].assign(buffer);

        if (i < 5)
            buffer = strtok(NULL, " ");
    }

    buffer = strtok(NULL, ";");
    bracked_input[i].assign(buffer);

    //handle input like: "SELECT DISTINCT Customers.Name              FROM Customers WHERE           Customer.Age=25;"
    if (bracked_input[i][0] == ' ')
    {
        int whitespace_counter = 1;
        while (bracked_input[i][whitespace_counter] == ' ')
            whitespace_counter++;

        bracked_input[i].erase(0, whitespace_counter);
    }

    return bracked_input;
}
//

int main()
{

    char input[LINE_LENGTH + 1] = {"\0"};

    cout << "SQL query:";

    cin.getline(input, LINE_LENGTH);


    Query *query = Query::parseQuery(input);

    if(query)
        cout << "-------------Parse Querey Succeeded-------------" << endl;

    return 0;
}

#include "syntactic_categories.hpp"


//-----------O_D class----------
    Optional_Distinct *Optional_Distinct::parseO_D(string input)
    {
        if (input.compare("DISTINCT") == 0 || input.compare("\0") == 0)
            return new Optional_Distinct(input);

        return nullptr;
    }
//

//-----------Attribute----------


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

//-----------Table_List----------
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

//-----------Condition----------
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

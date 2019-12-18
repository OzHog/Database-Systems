#ifndef syntactic_categories_hpp
#define syntactic_categories_hpp

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stack> 
#include "./Schems/Schems.hpp"


using namespace std;

//-----------Optional Distinct class----------
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


//-----------Attribute class----------

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


//-----------Table_List class----------
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

//-----------Condition class----------
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


#endif /* syntactic_categories_hpp */
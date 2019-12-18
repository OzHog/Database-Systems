#ifndef query_hpp
#define query_hpp

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../Syntactic_Categories/syntactic_categories.hpp"
#define LINE_LENGTH 255
#define STRQUERY_LENGTH 7


using namespace std;


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





#endif /* query_hpp */
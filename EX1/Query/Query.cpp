
#include "Query.hpp"


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


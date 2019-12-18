#include <stdio.h>
#include <string>
#include <iostream>
#include "Query/Query.hpp"


#define LINE_LENGTH 255
using namespace std;

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

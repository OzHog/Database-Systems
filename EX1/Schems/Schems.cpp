#include "Schems.hpp"
    
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
        scheme_file.open("scheme.txt");
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

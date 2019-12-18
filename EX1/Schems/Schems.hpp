#ifndef schems_hpp
#define schems_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>


#define LINE_LENGTH 255
#define SCHEMS_LENGTH 2
using namespace std;

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

#endif /* schems_hpp */
/*
This is a class that parses a gedcom file into a structure of 
person objects. Created by Jacob McCoy on 28 November 2023
*/

#include "GedcomParser.h"
#include <sstream>
#include <string>
#include <iostream>

GedcomParser::GedcomParser(std::string gedcom_string)
{
    this->raw_text = gedcom_string;
}

// parse the gedcom string into the map
void GedcomParser::Parse()
{
    std::stringstream text;
    text << this->raw_text;

    std::string line;

    // while there is still text to be read
    while(!text.str().empty())
    {
        // pull out a new line
        std::getline(text, line);

        // first you need to find a line that begins with 0
        // lines that begin with 0 could have people in them
        // or they could also define family relationships
        if (line[0] != '0')
            continue;
        else
        {
            // if it starts with a zero, it must begin with an @ symbol
            // otherwise, it is a header
            if (line[2] != '@')
                continue;
            else
            {
                // if the @ char is followed by an I, it is an individual
                if (line[3] == 'I')
                {
                    std::cout << "individual: " << line << '\n';
                }
                // otherwise, it is a family
                else if (line[3] == 'F')
                {
                    std::cout << "family: " << line << '\n';
                }
            }
        }

    }

}

void GedcomParser::ReplaceGedcomString(std::string gedcom_string)
{
    this->raw_text = gedcom_string;
    return;
}

std::unordered_map<fs_id, Person> GedcomParser::GetPersons() const
{
    return this->persons;
}


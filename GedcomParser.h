/*
This is a class that parses a gedcom file into a structure of 
person objects. Created by Jacob McCoy on 28 November 2023
*/

#ifndef GEDCOM_PARSER_H
#define GEDCOM_PARSER_H

#include "Person.h"
#include <string>
#include <unordered_map>

class GedcomParser
{
    public:
    GedcomParser(std::string gedcom_string);

    // parse the gedcom string into the map
    void Parse();

    void ReplaceGedcomString(std::string gedcom_string);
    std::unordered_map<fs_id, Person> GetPersons() const;
    size_t NumPersons() const;

    private:
    // the raw text from the gedcom file
    std::string raw_text;

    // a hash map that contains all the Person objects, with their
    // fs ids as keys since that's how you search
    std::unordered_map<fs_id, Person> persons;

    // some more hash maps to help you translate between id types
    std::unordered_map<gedcom_id, fs_id> gedcom_to_fs;
    std::unordered_map<id, gedcom_id> id_to_gedcom;
};

#endif //GEDCOM_PARSER_H
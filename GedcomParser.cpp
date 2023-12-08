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
// NOTE: if the gedcom file is incomplete or has something wrong with
// it, the parsing and rest of the code will not work.
void GedcomParser::Parse()
{
    std::stringstream text;
    text << this->raw_text;

    std::string line;
    id num_individuals = 0;

    std::unordered_map<gedcom_id, fs_id> gedcom_to_fs;
    std::unordered_map<id, gedcom_id> id_to_gedcom;

    // while there is still text to be read. no null characters
    // as the line being read...
    std::getline(text, line);
    std::getline(text, line);
    do
    {
        // pull out a new line

        // first you need to find a line that begins with 0 since
        // lines that begin with 0 could have people in them
        // or they could also define family relationships
        if (line[0] != '0')
        {
            std::getline(text, line);
            continue;
        }
        else
        {
            // if it starts with a zero, it must begin with an @ symbol
            // otherwise, it is a header
            if (line[2] != '@')
            {
                std::getline(text, line);
                continue;
            }
            else
            {
                // if the @ char is followed by an I, it is an individual
                if (line[3] == 'I')
                {                    
                    // pull out the gedcom ID of the person
                    gedcom_id temp_gedcom = line;
                    size_t start_index = temp_gedcom.find_first_of('@');
                    size_t end_index = temp_gedcom.find_last_of('@');
                    temp_gedcom = temp_gedcom.substr(start_index + 1, end_index - 3);

                    // pull out the name of the person
                    // start by getting a new line. it immediately succeeds the id
                    std::getline(text, line);
                    std::string name_string = "1 NAME ";
                    start_index = name_string.size();
                    end_index = line.find_last_of('\r');
                    std::string temp_name = line.substr(start_index);
                    temp_name.pop_back();

                    // pull out the FS ID of the person
                    // go through line until you get to one like this:
                    // "1 _FSFTID "
                    std::getline(text, line);
                    while(line.find("1 _FSFTID") == std::string::npos)
                    {
                        std::getline(text, line);
                    }
                    // pull out the ID. All familysearch ids are 7 characters plus a dash
                    fs_id temp_fs_id = line.substr(line.find_last_of(' ') + 1);
                    temp_fs_id.pop_back(); // get rid of the \r at the bottom

                    // make a new person, to be added to the list
                    Person new_person(num_individuals++, temp_gedcom, temp_fs_id, temp_name);

                    // gedcom_to_fs is used to pair people up in families
                    gedcom_to_fs[temp_gedcom] = temp_fs_id;
                    id_to_gedcom[num_individuals - 1] = temp_gedcom;

                    // pull out the family identifiers
                    // skip until you get to them
                    std::getline(text, line);
                    while(line.find("1 FAM") == std::string::npos)
                    {
                        std::getline(text, line);
                    }
                    // once you find them, pull them out
                    while(line.find("1 FAM") != std::string::npos)
                    {
                        gedcom_id family_id = line.substr(line.find_first_of('@') + 1);
                        // pop it twice to get rid of the new line and the bonus @addindex
                        family_id.pop_back();
                        family_id.pop_back();

                        // add it to the right family vector
                        if (line.find("1 FAMS") != std::string::npos)
                        {
                            new_person.AddFamilySpouse(family_id);
                        }
                        else
                        {
                            new_person.AddFamilyChild(family_id);
                        }
                        // pull in a new line to start things over again
                        std::getline(text, line);
                    }

                    // add the wonderful new person
                    this->persons[temp_fs_id] = new_person;
                    if (temp_fs_id == "")
                    {
                        int error_HERE = 1;
                    }
                }
                // otherwise, it is a family
                else if (line[3] == 'F')
                {
                    // pull out family id
                    gedcom_id family_gedcom_id = line.substr(line.find_first_of('@') + 1, line.find_last_of('@') - line.find_first_of('@') - 1);

                    std::getline(text, line);
                    // pull out husband id
                    gedcom_id husband_gedcom_id;
                    while(line.find("1 HUSB") != std::string::npos)
                    {
                        husband_gedcom_id = line.substr(line.find_first_of('@') + 1);
                        husband_gedcom_id.pop_back();
                        husband_gedcom_id.pop_back();
                        std::getline(text, line);
                    }

                    // pull out wife id
                    gedcom_id wife_gedcom_id;
                    while(line.find("1 WIFE") != std::string::npos)
                    {
                        wife_gedcom_id = line.substr(line.find_first_of('@') + 1);
                        wife_gedcom_id.pop_back();
                        wife_gedcom_id.pop_back();
                        std::getline(text, line);
                    }

                    // pull out children ids
                    std::vector<gedcom_id> children_gedcom_ids;
                    gedcom_id child_gedcom_id;
                    while(line.find("1 CHIL") != std::string::npos)
                    {
                        child_gedcom_id = line.substr(line.find_first_of('@') + 1);
                        child_gedcom_id.pop_back();
                        child_gedcom_id.pop_back();
                        children_gedcom_ids.push_back(child_gedcom_id);
                        std::getline(text, line);
                    }

                    // convert all these gedcom ids into program ids
                    id husband_id, wife_id, child_id;
                    std::vector<id> children_ids;
                    bool husband_exists, wife_exists;
                    husband_exists = wife_exists = false;
                    // go through all the persons
                    for (auto it = this->persons.begin(); it != this->persons.end(); ++it) {
                        // if the person's husband's gedcom id is the right one,
                        if (it->second.GetGEDCOMID() == husband_gedcom_id)
                        {
                            // get its id so you can put it in the person
                            husband_id = it->second.GetID();
                            husband_exists = true;
                        }
                        // do the same thing for the wife and kids as you did for the husband
                        else if (it->second.GetGEDCOMID() == wife_gedcom_id)
                        {
                            wife_id = it->second.GetID();
                            wife_exists = true;
                        }
                        else
                        {
                            for (int i = 0; i < children_gedcom_ids.size(); i++)
                            {
                                if (it->second.GetGEDCOMID() == children_gedcom_ids.at(i))
                                {
                                    children_ids.push_back(it->second.GetID());
                                }
                            }
                        }
                    }


                    // families always come after people, so you can add in
                    // the family parts into the people right now

                    // with the husband id, you find the husband and add the wife's id
                    if (husband_exists && wife_exists)
                    {
                        this->persons[gedcom_to_fs[husband_gedcom_id]].AddSpouse(wife_id);
                    }
                    // reverse to the wife
                    if (wife_exists && husband_exists)
                    {
                        this->persons[gedcom_to_fs[wife_gedcom_id]].AddSpouse(husband_id);
                        if (husband_id > this->persons.size())
                        {
                            int uhohproblem = 1;
                        }
                    }

                    if (family_gedcom_id == "F74")
                    {
                        int u = 0;
                    }

                    // you also add the children's ids to the parents and vice versa
                    // the hearts of the children turning to their fathers...
                    for (int i = 0; i < children_ids.size(); i++)
                    {
                        this->persons[gedcom_to_fs[husband_gedcom_id]].AddChild(children_ids.at(i));
                        this->persons[gedcom_to_fs[wife_gedcom_id]].AddChild(children_ids.at(i));
                        this->persons[gedcom_to_fs[children_gedcom_ids.at(i)]].AddFather(husband_id);
                        this->persons[gedcom_to_fs[children_gedcom_ids.at(i)]].AddMother(wife_id);
                    }
                }
            }
        }
    } while(line != "\0");

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

size_t GedcomParser::NumPersons() const
{
    return this->persons.size();
}
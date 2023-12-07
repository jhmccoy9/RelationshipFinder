/*
This is a class containing the most important information about someone 
in FamilySearch. Created by Jacob McCoy on 28 November 2023
*/

#include "Person.h"
#include <sstream>

// default constructor
Person::Person(id personal_id, gedcom_id personal_gedcom_id, fs_id personal_fs_id, std::string name)
{
    this->personal_id = personal_id;
    this->personal_gedcom_id = personal_gedcom_id;
    this->personal_fs_id = personal_fs_id;
    this->name = name;
}

// empty constructor
Person::Person(){}

fs_id Person::GetFSID() const 
{
    return this->personal_fs_id;
}

gedcom_id Person::GetGEDCOMID() const
{
    return this->personal_gedcom_id;
}

id Person::GetID() const
{
    return this->personal_id;
}

std::string Person::GetName() const
{
    return this->name;
}

void Person::AddFather(id father_id)
{
    this->father_ids.push_back(father_id);
    return;
}

void Person::AddMother(id mother_id)
{
    this->mother_ids.push_back(mother_id);
    return;
}

void Person::AddSpouse(id spouse_id)
{
    this->spouse_ids.push_back(spouse_id);
}

void Person::AddChild(id child_id)
{
    this->child_ids.push_back(child_id);
    return;
}

void Person::AddFamilySpouse(gedcom_id id)
{
    this->family_spouse_ids.push_back(id);
    return;
}

void Person::AddFamilyChild(gedcom_id id)
{
    this->family_child_ids.push_back(id);
    return;
}

std::vector<id> Person::GetFathers() const
{
    return this->father_ids;
}

std::vector<id> Person::GetMothers() const
{
    return this->mother_ids;
}

std::vector<id> Person::GetSpouses() const
{
    return this->spouse_ids;
}

std::vector<id> Person::GetChildren() const
{
    return this->child_ids;
}

std::string Person::ToString() const
{
    std::stringstream to_return;
    to_return << "Name: " << this->name << '\n' <<
                 "FS ID: " << this->personal_fs_id << '\n' <<
                 "GEDCOM ID: " << this->personal_gedcom_id << '\n';
    
    to_return << "Spouse IDs:";
    for (int i = 0; i < this->spouse_ids.size(); i++)
    {
        to_return << "\n    " << this->spouse_ids.at(i);
    }

    to_return << "\nChild IDs:";
    for (int i = 0; i < this->child_ids.size(); i++)
    {
        to_return << "\n    " << this->child_ids.at(i);
    }

    to_return << "\nFather IDs:";
    for (int i = 0; i < this->father_ids.size(); i++)
    {
        to_return << "\n    " << this->father_ids.at(i);
    }

    to_return << "\nMother IDs:";
    for (int i = 0; i < this->mother_ids.size(); i++)
    {
        to_return << "\n    " << this->mother_ids.at(i);
    }
    to_return << '\n';

    return to_return.str();
}



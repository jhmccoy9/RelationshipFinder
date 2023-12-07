/*
This is a class containing the most important information about someone 
in FamilySearch. Created by Jacob McCoy on 27 November 2023
*/

#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>

typedef unsigned int id;
typedef std::string fs_id;
typedef std::string gedcom_id;

class Person
{
    public:
        // constructors
        Person(id personal_id, gedcom_id personal_gedcom_id, fs_id personal_fs_id, std::string name);
        Person();

        // get key data about a person
        fs_id GetFSID() const;
        id GetID() const;
        std::string GetName() const;
        gedcom_id GetGEDCOMID() const;

        // add relationships. the first four are for adding people
        // the last two are for adding them to families, an intermediate step
        void AddFather(id father_id);
        void AddMother(id mother_id);
        void AddSpouse(id spouse_id);
        void AddChild(id child_id);
        void AddFamilySpouse(gedcom_id id);
        void AddFamilyChild(gedcom_id id);

        // get family relationships
        std::vector<id> GetFathers() const;
        std::vector<id> GetMothers() const;
        std::vector<id> GetSpouses() const;
        std::vector<id> GetChildren() const;

        std::string ToString() const;

    private:
        // this is the id that come with the person specific to the gedcom file
        gedcom_id personal_gedcom_id;

        // personal_id is just an integer assigned by the program to keep track of the
        // person within the program
        id personal_id;

        // string that contains the FamilySearch ID of the person
        fs_id personal_fs_id;

        // full name of the person
        std::string name;

        // vectors that contain relationships
        std::vector<gedcom_id> family_spouse_ids;
        std::vector<gedcom_id> family_child_ids;
        std::vector<id> mother_ids;
        std::vector<id> father_ids;
        std::vector<id> spouse_ids;
        std::vector<id> child_ids;
};


#endif /* PERSON_H */
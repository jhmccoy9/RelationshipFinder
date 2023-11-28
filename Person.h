/*
This is a class meant to contain the most important information about someone 
in FamilySearch
*/

#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>

typedef unsigned int id;
typedef std::string fs_id;

class Person
{
    public:
        Person(id personal_id, fs_id personal_fs_id, std::string name);
        fs_id GetFSID() const;
        id GetID() const;
        std::string GetName() const;

        void AddFather(id father_id);
        void AddMother(id mother_id);
        void AddSpouse(id spouse_id);
        void AddChild(id child_id);

        std::vector<id> GetFathers() const;
        std::vector<id> GetMothers() const;
        std::vector<id> GetSpouses() const;
        std::vector<id> GetChildren() const;

        std::string ToString() const;

    private:
        id personal_id;
        fs_id personal_fs_id;
        std::vector<id> mother_ids;
        std::vector<id> father_ids;
        std::vector<id> spouse_ids;
        std::vector<id> child_ids;

}


#endif /* PERSON_H */
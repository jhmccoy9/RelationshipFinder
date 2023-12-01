#include "Person.h"
#include "GedcomParser.h"
#include "RelationshipFinder.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// #define TEST_MODE

#ifndef TEST_MODE
int main(int argc, char** argv)
{
    // pull in the data from the gedcom file
    //std::string gedcom_file = argv[1];
    std::string gedcom_file = "easy_tree.ged";
    std::ifstream in;
    in.open(gedcom_file);
    std::stringstream ss;
    ss << in.rdbuf();
    std::string gedcom_text = ss.str();
    in.close();

    // parse it
    GedcomParser parser(gedcom_text);
    parser.Parse();

    std::cout << "Num people: " << parser.NumPersons() << std::endl;

    std::unordered_map<fs_id, Person> my_family;
    my_family = parser.GetPersons();

    // make the adjacency matrix
    RelationshipFinder relationship(my_family);
    std::cout << relationship.ToString() << std::endl;

    std::cout << "int size: " << sizeof(int) << std::endl;


    return 0;
}
#endif //TEST_MODE

#ifdef TEST_MODE
int main()
{
    Person test_person(0, "s1", "fsid1234", "Jake McCoy");
    test_person.AddFather(2);
    test_person.AddMother(1);
    test_person.AddSpouse(3);
    test_person.AddChild(5);
    test_person.AddChild(6);
    std::cout << test_person.ToString();
    return 0;
}
#endif //TEST_MODE
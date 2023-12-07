#include "Person.h"
#include "GedcomParser.h"
#include "RelationshipFinder.h"
// #include "htk.h"
#include <iostream>
#include <omp.h>
#include <string>
#include <fstream>
#include <sstream>

#define NUM_THREADS 8

// TEST_MODE is used if you want runtime stats and other things
#define TEST_MODE

int main(int argc, char** argv)
{
    // set the number of threads
    omp_set_num_threads(NUM_THREADS);

    // pull in the data from the gedcom file
    //std::string gedcom_file = argv[1];
    std::string gedcom_file = "family_tree.ged";
    std::ifstream in;
    in.open(gedcom_file);
    std::stringstream ss;
    ss << in.rdbuf();
    std::string gedcom_text = ss.str();
    in.close();

    // parse it
    GedcomParser parser(gedcom_text);
    parser.Parse();

    #ifdef TEST_MODE
    std::cout << "Num people: " << parser.NumPersons() << std::endl;
    #endif

    // pull out the people so you can make the adjacency matrix
    std::unordered_map<fs_id, Person> my_family;
    my_family = parser.GetPersons();

    // make the adjacency matrix
    RelationshipFinder relationship(my_family);
    #ifdef TEST_MODE
    //std::cout << relationship.ToString() << std::endl;
    #endif

    // run floyd's algorithm:
    relationship.FloydRelationshipFinder();

    #ifdef TEST_MODE
    //std::cout << relationship.ToString() << std::endl;
    std::cout << std::endl;
    //std::cout << relationship.ToStringPath() << std::endl;
    #endif

    // calculate and display relationships based on user input
    std::cout << "Enter two people's FamilySearch IDs to show their relationship" << std::endl;
    while(true)
    {
        fs_id first, second;
        // get the first FamilySearch ID
        std::cout << "Person 1: ";
        std::cin >> first;
        std::cout << std::endl;

        // get the second FamilySearch ID
        std::cout << "Person 2: ";
        std::cin >> second;
        std::cout << std::endl;

        // display the path and keep going
        relationship.DisplayPath(first, second);

        std::cout << "To end, press ctrl+c" << std::endl << std::endl;
    }

    return 0;
}


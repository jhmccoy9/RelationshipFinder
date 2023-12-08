#include "Person.h"
#include "GedcomParser.h"
#include "RelationshipFinder.h"
#include <iostream>
#include <omp.h>
#include <string>
#include <fstream>
#include <sstream>

// set the number of threads to be used. 
#define NUM_THREADS 8

int main(int argc, char** argv)
{
    // set the number of threads
    omp_set_num_threads(NUM_THREADS);

    // pull in the data from the gedcom file
    //std::string gedcom_file = argv[1];
    std::string gedcom_file = "best_big.ged";
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

    // pull out the people so you can make the adjacency matrix
    std::unordered_map<fs_id, Person> my_family;
    my_family = parser.GetPersons();

    // make the adjacency matrix
    RelationshipFinder relationship(my_family);

    // run floyd's algorithm:
    relationship.FloydRelationshipFinder();

    // calculate and display relationships based on user input
    std::cout << "Enter two people's FamilySearch IDs to show their relationship" << std::endl;
    std::cout << "To quit, type 'exit' instead of a FamilySearch ID" << std::endl;
    while(true)
    {
        fs_id first, second;
        // get the first FamilySearch ID
        std::cout << "Person 1: ";
        std::cin >> first;
        std::cout << std::endl;

        if (first == "exit")
        {
            break;
        }

        // get the second FamilySearch ID
        std::cout << "Person 2: ";
        std::cin >> second;
        std::cout << std::endl;

        if (second == "exit")
        {
            break;
        }

        // display the path and keep going
        relationship.DisplayPath(first, second);
        std::cout << std::endl;

    }

    return 0;
}


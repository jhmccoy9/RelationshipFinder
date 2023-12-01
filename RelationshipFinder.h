/*
This is a class that parses a gedcom file into a structure of 
person objects. Created by Jacob McCoy on 28 November 2023
*/

#ifndef RELATIONSHIP_FINDER_H
#define RELATIONSHIP_FINDER_H

#include "Person.h"
#include "GedcomParser.h"
#include <string>
#include <unordered_map>
#include <vector>


class RelationshipFinder
{
    public:
        // the constructor creates the adjacency matrix dynamically
        RelationshipFinder(std::unordered_map<fs_id, Person> person_map);
        ~RelationshipFinder();

        std::string ToString();

        // pathfinding functions
        // Dijktra's algorithm: finds path between two points
        std::vector<id> DijkstraRelationshipFinder();
        // Floyd's algorithm: finds all paths between all points
        std::vector<id> FloydRelationshipFinder(bool redo = true); // idk probably change the inputs
        

    private:
        unsigned int* adjacency_matrix; // the matrix is 2d but stored as 1d
        unsigned int matrix_width; // matrix is square
        std::unordered_map<fs_id, Person> family_map;

};

#endif //RELATIONSHIP_FINDER_H
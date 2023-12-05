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
#include <iostream>
#include <vector>


class RelationshipFinder
{
    public:
        // the constructor creates the adjacency matrix dynamically
        RelationshipFinder(std::unordered_map<fs_id, Person> person_map);
        ~RelationshipFinder();

        std::string ToString() const;
        std::string ToStringPath() const;

        // Floyd's algorithm: finds all paths between all points
        void FloydRelationshipFinder(); // just updates the original adjacency matrix

        // call after running Floyd's algorithm. Prints the connection between two people
        void DisplayPath(fs_id start, fs_id end, std::unordered_map<id, fs_id> id_to_fsid);
        

    private:
        unsigned int* adjacency_matrix; // the matrix is 2d but stored as 1d
        unsigned int* prev;
        unsigned int matrix_width; // matrix is square
        mutable std::unordered_map<fs_id, Person> family_map;
        unsigned int max_dist; // maximum distance to fill in values: uint_max minus (num people + 1)
        std::unordered_map<id, fs_id> id_to_fs_id;

};

#endif //RELATIONSHIP_FINDER_H
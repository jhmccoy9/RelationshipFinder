/*
This is a class that parses a gedcom file into a structure of 
person objects. Created by Jacob McCoy on 28 November 2023
*/


#include "RelationshipFinder.h"
#include <climits>
#include <sstream>


// the constructor creates the adjacency matrix dynamically
RelationshipFinder::RelationshipFinder(std::unordered_map<fs_id, Person> person_map)
{
    // make the dynamic array
    this->adjacency_matrix = new unsigned int[this->matrix_width * this->matrix_width];

    // fill it with max values
    // TODO: PARALLELIZE
    for (int i = 0; i < this->matrix_width * this->matrix_width; i++)
    {
        this->adjacency_matrix[i] = UINT_MAX;
    }

    // for each item in the map
    // TODO: PARALLELIZE
    for (auto& person : person_map)
    {
        // pull out the id
        id current_id = person.second.GetID();

        // on that id's row, add a 1 in the column of each relationship
        // spouse relationships
        for (int i = 0; i < person.second.GetSpouses().size(); i++)
        {
            this->adjacency_matrix[current_id * this->matrix_width + person.second.GetSpouses().at(i)] = 1;
        }
        // child relationships
        for (int i = 0; i < person.second.GetChildren().size(); i++)
        {
            this->adjacency_matrix[current_id * this->matrix_width + person.second.GetChildren().at(i)] = 1;
        }
        // father relationships
        for (int i = 0; i < person.second.GetFathers().size(); i++)
        {
            this->adjacency_matrix[current_id * this->matrix_width + person.second.GetFathers().at(i)] = 1;
        }
        // mother relationships
        for (int i = 0; i < person.second.GetMothers().size(); i++)
        {
            this->adjacency_matrix[current_id * this->matrix_width + person.second.GetMothers().at(i)] = 1;
        }
    }
}
RelationshipFinder::~RelationshipFinder()
{
    // deallocate the dynamic array
    delete[] this->adjacency_matrix;
}

std::string RelationshipFinder::ToString()
{
    std::stringstream to_return;
    for (int i = 0; i < this->matrix_width; i++)
    {
        for (int j = 0; j < this->matrix_width; j++)
        {
            to_return << this->adjacency_matrix[i*this->matrix_width + j] << " ";
        }
        to_return << std::endl;
    }
    return to_return.str();
}

// later, add the pathfinding functions...

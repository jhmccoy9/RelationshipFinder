/*
This is a class that parses a gedcom file into a structure of 
person objects. Created by Jacob McCoy on 28 November 2023
*/


#include "RelationshipFinder.h"
#include <climits>
#include <sstream>
#include <iostream>


// the constructor creates the adjacency matrix dynamically
RelationshipFinder::RelationshipFinder(std::unordered_map<fs_id, Person> person_map)
{
    // copy in the person map
    this->family_map = person_map;

    // make the dynamic arrays
    this->matrix_width = person_map.size();
    this->adjacency_matrix = new unsigned int[this->matrix_width * this->matrix_width];
    this->prev = new unsigned int[this->matrix_width * this->matrix_width];

    // get the max value
    this->max_dist = UINT_MAX - this->matrix_width - 1;

    // fill it with default values
    // TODO: PARALLELIZE
    for (int i = 0; i < this->matrix_width; i++)
    {
        for (int j = 0; j < this->matrix_width; j++)
        {
            this->adjacency_matrix[i*matrix_width + j] = this->max_dist;
            this->prev[i*matrix_width + j] = i;
        }
    }

    // put zeros on the diagonal for the adjacency matrix
    // and itself on the diagonal for the previous matrix
    // TODO: parallelize
    for (int i = 0; i < this->matrix_width; i++)
    {
        this->adjacency_matrix[i*matrix_width + i] = 0;
        this->prev[i*matrix_width + i] = i;
    }

    // for each item in the map
    // TODO: PARALLELIZE. this would be a good dynamic one
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

    // error checking. matrix should be symmetric
    for (int i = 0; i < this->matrix_width; i++)
    {
        for (int j = 0; j< this->matrix_width; j++)
        {
            if (this->adjacency_matrix[i*this->matrix_width + j] != this->adjacency_matrix[j*this->matrix_width + i])
            {
                std::cerr << "Error: (" << i << "," << j << ") != (" << j << "," << i << ")\n";
            }
        }
    }

}
RelationshipFinder::~RelationshipFinder()
{
    // deallocate the dynamic array
    delete[] this->adjacency_matrix;
    delete[] this->prev;
}

std::string RelationshipFinder::ToString() const
{
    std::stringstream to_return;
    for (int i = 0; i < this->matrix_width; i++)
    {
        for (int j = 0; j < this->matrix_width; j++)
        {   unsigned int to_insert = this->adjacency_matrix[i*this->matrix_width + j];
            if (to_insert == this->max_dist)
            {
                to_return << "x" << " ";
            }
            else
            {
                to_return << to_insert << " ";
            }
            
        }
        to_return << std::endl;
    }
    return to_return.str();
}

std::string RelationshipFinder::ToStringPath() const
{
    std::stringstream to_return;
    for (int i = 0; i < this->matrix_width; i++)
    {
        for (int j = 0; j < this->matrix_width; j++)
        {   unsigned int to_insert = this->prev[i*this->matrix_width + j];
            if (to_insert == this->max_dist)
            {
                to_return << "x" << " ";
            }
            else
            {
                to_return << to_insert << " ";
            }
            
        }
        to_return << std::endl;
    }
    return to_return.str();
}

void RelationshipFinder::FloydRelationshipFinder()
{
    unsigned int matrix_width = this->matrix_width;
    for (int k = 0; k < matrix_width; k++)
    {
        // TODO: parallelize!!
        for (int i = 0; i < matrix_width; i++)
        {
            for (int j = 0; j < matrix_width; j++)
            {
                
                if (this->adjacency_matrix[i*matrix_width + j] >
                    this->adjacency_matrix[i*matrix_width + k] +
                    this->adjacency_matrix[k*matrix_width + j])
                    {
                        this->adjacency_matrix[i*matrix_width + j] =
                        this->adjacency_matrix[i*matrix_width + k] +
                        this->adjacency_matrix[k*matrix_width + j];

                        this->prev[i*matrix_width + j] = this->prev[k*matrix_width + j];
                    }
            }
        }
    }
    return;
}

void RelationshipFinder::DisplayPath(fs_id start, fs_id end, std::unordered_map<id, fs_id> id_to_fsid)
{
    id start_index, current_index, end_index;
    current_index = start_index = this->family_map[start].GetID();
    end_index = this->family_map[end].GetID();

    // base case: no connection between the two
    if (this->prev[start_index * this->matrix_width + end_index] == this->max_dist)
    {
        std::cout << "No relationship" << std::endl;
        return;
    }
    // otherwise, there is a relationship: print it out
    std::cout << "Relationship: " << std::endl;
    std::cout << this->family_map[id_to_fs_id[current_index]].GetName() << std::endl;
    while (current_index != end_index)
    {
        current_index = prev[end_index * this->matrix_width + current_index];
        std::string name;
        fs_id fs;
        fs = id_to_fs_id[current_index];
        Person person;
        person = this->family_map[fs];
        name = person.GetName();
        std::cout << name << std::endl;
    }
    return;    
    
}

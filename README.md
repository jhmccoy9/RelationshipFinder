# RelationshipFinder
A program that will take a GEDCOM file from FamilySearch and use the Floyd-Warshall algorithm and OpenMP to find the relationship between two people.

To compile:
g++ -fopenmp -O3 *cpp -o RelationshipFinder

To run:
./RelationshipFinder your_gedcom_file.ged

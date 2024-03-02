#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM
#include <vector>
#include "Random.hpp"
#include "graph.hpp"

struct Person
{
    std::vector<int> genotype;
    int fenotype;   
    double propability;
    int age;
};

inline bool operator<(const Person& q, const Person& p)
{
  return q.genotype < p.genotype;
}

void geneticAlgorithm(std::vector<int>& perm, const std::vector<int> distance, int n,std::vector<Person>& generation,int generation_size,int parrents_gen_size, double mutationRate, double crossoverRate,int itrerations,MST mst,RandomGenerator RG);
std::vector<Person> generateFirstGeneration(bool randomGen,int generation_size,MST mst,std::vector<int> distance, int n ,RandomGenerator RG);


#endif
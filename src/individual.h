#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "gp.h"
#include "cipher.h"
#include "population.h"

class Machine;

class Individual
{
private:
    vector<instruction_type> genome;
    int fitness;
	Population* population;
public:
    vector<instruction_type> getGenome() const;
    Individual* clone();
    
    Individual(Population* population);
    Individual(const Individual &other) : population(other.population) {}
    Individual(Population* population, vector<instruction_type> initialGenome);
    
    void mutate();
    Individual* crossover(const Individual& other) const;
    
    int getFitness() const; 
	void calculateFitness(Cipher *cipher);
	void optimize();
    
    friend ostream& operator<< (ostream& stream, const Individual& individual);
    friend bool individualLessThan (const shared_ptr<Individual> lhs, const shared_ptr<Individual> rhs);
    bool operator< (const shared_ptr<Individual> other);    
};

void print(const Individual& individual);

bool individualLessThan (const shared_ptr<Individual> lhs, const shared_ptr<Individual> rhs);

#endif
#ifndef POPULATION_H
#define POPULATION_H

#include "gp.h"
#include "machine.h"
#include "cipher.h"

class Individual;
class Machine;

class Population
{
private:
    double bestFitness = 0.0;

    int genome[4];
    
    void populate(int size);
	CipherSize cipherSize;
    
    Population(const Population&);
    Population operator=(const Population&);
    
public:
    vector<shared_ptr<Individual>> * individuals;
    Population(int size);
    Population(int size, CipherSize cipherSize);
    
    ~Population();
    
	void evaluate(Cipher* cipher);
	void evaluatePortion(Cipher* cipher, int start, int end);
    void switchGenerations();
    
    double getBestFitness() const;
	
	CipherSize getCipherSize() const;
	void setCipherSize(CipherSize size);
    
    void mutate();
    Population* crossover(const Population &other) const;
    
    
    friend bool populationLessThan (const shared_ptr<Population> lhs, const shared_ptr<Population> rhs);
};

bool populationLessThan (const shared_ptr<Population> lhs, const shared_ptr<Population> rhs);

#endif
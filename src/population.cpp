#include "population.h"
#include "individual.h"
#include "cipher.h"
#include <boost/concept_check.hpp>

/// Creates a population with size individuals, assigns parameters from
/// global constants.
Population::Population(int size, CipherSize cipherSize) : individuals()
{
    genome[0] = ELITISM_PERCENTAGE;
    genome[1] = MUTATION_PROBABILITY;
    genome[2] = PROGRAM_LENGTH;
    genome[3] = PROGRAM_LENGTH;
	
	this->cipherSize = cipherSize;
        
    populate(size);
}

/// Destroys the Population object
Population::~Population()
{
  delete individuals;
}

/// Creates size individuals
void Population::populate(int size)
{
    individuals = new vector<shared_ptr<Individual>>();
    for (int i = 0; i < size; i++)
    {
        shared_ptr<Individual> ptr = shared_ptr<Individual>(new Individual(this));
        individuals->push_back(ptr);
    }
}

/// Begins the fitness evaluation of some of the individuals in the population. Individuals
/// evaluated are those between start and end indexes in the individual vector.
void Population::evaluatePortion(Cipher* cipher, int start, int end)
{
  //if (end > individuals->size()) end = individuals->size();
  //cout << "Starting for chunk from " << start << " to " << end << endl;
  Machine* machine = new Machine();
  
  for (size_t i = start; i < (unsigned)end; i++)
  {
    individuals->at(i)->calculateFitness(cipher);
  }
  delete machine;
}

/// Evaluates the fitness of the entire population. Uses multi-threaded calls to 
/// evaluatePortion(). After evaluation, sorts the individual vector in descending
/// order by fitness.
void Population::evaluate(Cipher* cipher)
{
    vector<thread> pool;
    int chunk = individuals->size() / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS - 1; i++)
    {
      pool.push_back(thread(&Population::evaluatePortion, this, cipher, i * chunk, i * chunk + chunk));
    }
    evaluatePortion(cipher, (NUM_THREADS - 1) * chunk, individuals->size());
    for (auto &th: pool)
    {
      th.join();
    }
    sort(individuals->begin(), individuals->end(), &individualLessThan);
    reverse(individuals->begin(), individuals->end());
    bestFitness = individuals->at(0)->getFitness();
}

/// Replaces the current individual vector with a vector containing individuals of
/// the next generation. This is done by combining an elitism approach and a size-2
/// tournament, where children born to parents after the tournament may also be
/// mutated.
void Population::switchGenerations()
{
	cout << "Trying to evolve " << endl;
    vector<shared_ptr<Individual>> * nextGeneration = new vector<shared_ptr<Individual>>;
    for (size_t i = 0; i < individuals->size() / (100 / ELITISM_PERCENTAGE); i++)
    {
        nextGeneration->push_back(individuals->at(i));
    }
    while (nextGeneration->size() < individuals->size())
    {
      shared_ptr<Individual> parents[2];
      for (int i = 0; i < 2; i++)
      {
	shared_ptr<Individual> ind1 = individuals->at(rand() % individuals->size());
	shared_ptr<Individual> ind2 = individuals->at(rand() % individuals->size());
	parents[i] = (ind1->getFitness() > ind2->getFitness()) ? ind1 : ind2;
      }
      shared_ptr<Individual> child = shared_ptr<Individual>(parents[0]->crossover(*parents[1]));
      if (rand() % 100 < MUTATION_PROBABILITY) child->mutate();
      nextGeneration->push_back(child);
    }
    delete individuals;
    individuals = nextGeneration;
}

/// Returns the fitness of the best individual in this population
double Population::getBestFitness() const
{
    return bestFitness;
}

/// Comparer for populations. The population whose best individual has a higher
/// fitness score is considered to be larger.
bool populationLessThan (const shared_ptr<Population> lhs, const shared_ptr<Population> rhs)
{
    return lhs->getBestFitness() < rhs->getBestFitness();
}

CipherSize Population::getCipherSize() const
{
	return cipherSize;
}

void Population::setCipherSize(CipherSize cipherSize)
{
	this->cipherSize = cipherSize;
}

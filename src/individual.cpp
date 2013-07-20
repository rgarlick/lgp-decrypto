/*  
 * 	Copyright (c) 2013, Daniels Umanovskis
 * 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 *	
 */

#include "individual.h"
#include "machine.h"
#include "cipher.h"

vector<instruction_type> Individual::getGenome() const
{
	return genome;
}

int Individual::getFitness() const
{
	return fitness;
}

/// Creates a new individual that encodes a random
/// program
Individual::Individual(Population* population) : genome(), fitness()
{
    this->population = population;
	int size = PROGRAM_LENGTH;
	genome.reserve(size);
    CipherSize cipherSize = population->getCipherSize();
	for (int i = 0; i < size; i++)
	{
		instruction_type instruction;
		instruction.fill(0);
		instruction[0] = rand() % NUM_OPERATORS;
		if (instruction[0] == REMC || instruction[1] == REMR)
		{
			instruction[1] = rand() % cipherSize.columns;
		}
		else if (instruction[0] == SWAP)
		{
			instruction[1] = rand() % cipherSize.rows;
			instruction[2] = rand() % cipherSize.rows;
		}
		genome.push_back(instruction);

	}
}

/// Creates a new individual containing instructions given in
/// the vector that is passed as the argument
Individual::Individual(Population* population, vector<instruction_type> initialGenome) : genome(initialGenome), fitness(), population(population)
{
}


void Individual::mutate()
{
	int instruction = rand() % genome.size();
	int mutationPoint = rand() % 3;
    CipherSize cipherSize = population->getCipherSize();
	if (mutationPoint == 0) //operator
	{
		genome[instruction][mutationPoint] = rand() % NUM_OPERATORS;
	}
	else
    {
        if (genome[instruction][0] == SWAP || genome[instruction][0] == REMR)
        {
            genome[instruction][mutationPoint] = rand() % cipherSize.rows;
        }
        else if (genome[instruction][0] == REMC)
        {
            genome[instruction][mutationPoint] = rand() % cipherSize.columns;
        }
    }
}

/// Performs one-point crossover with the individual given as the
/// argument. Does not change the current individual, returns a new
/// one that is the child resulting from crossover.
Individual* Individual::crossover(const Individual& other) const
{
	size_t point = rand() % genome.size();
	if (point > other.genome.size()) point = other.genome.size() - 1;
	if (point == 0) point++;

	vector<instruction_type> child(genome);
	auto it = child.begin() + point;
	child.erase(it, child.end());
	auto other_it = other.genome.begin() + point;
	child.insert(child.end(), other_it, other.genome.end());

	return new Individual(population, child);
}

/// Calculates the fitness score for this individual given a vector of test cases
/// and a virtual machine pointer that points to a valid machine.
void Individual::calculateFitness(Cipher *cipher)
{
    Machine* machine = new Machine();
    machine->setCipher(new Cipher(cipher));
    machine->run(genome);
	fitness = machine->getResult()->getHomophonicFitness();
	delete machine;
}

/// Performs simple optimization of the instructions encoded by the individual. The
/// goal is to remove instructions that have no effect on the result. Not used in
/// the actual evolution, this is only for better readability of the evolved result.
void Individual::optimize()
{
	
}

/// Stream operator to allow outputting individuals with std::cout
ostream& operator<< (ostream& stream, const Individual& individual)
{
    for (auto instr : individual.genome)
	{
		switch (instr[0])
		{
			case NOOP:
				stream << "NOOP";
				break;
			case SWAP:
				stream << "SWAP " << instr[1] << " " << instr[2];
				break;
            case REMC:
                stream << "REMC " << instr[1];
                break;
            case REMR:
                stream << "REMR " << instr[1];
                break;
            case FLIP:
                stream << "FLIP";
                break;
			default:
				stream << "UNKNOWN";
				break;
		}
		stream << endl;
	}
	return stream;
}

/// A compaper for individuals. The individual with a higher fitness score is considered to
/// be larger.
bool individualLessThan (const shared_ptr<Individual> lhs, const shared_ptr<Individual> rhs)
{
	return lhs->fitness < rhs->fitness;
}

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
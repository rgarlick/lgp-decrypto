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
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

#include "gp.h"
#include "individual.h"
#include "population.h"
#include "machine.h"
#include "cipher.h"

using namespace std;

Machine* machine;

void cipherTest();

class HomophonicSolver;

int main(int argc, char** argv)
{
	srand(time(nullptr));
	if (argc < 2)
	{
		cout << "Specify the file name!" << endl;
		return 0;
	}
	
	/*std::string str = "9%P/Z/UB%kOR=pX=BWV+eGYF69HP@K!qYeMJY^UIk7qTtNQYD5)S(/9#BPORAU%fRlqEk^LMZJdr\\pFHVWe8Y@+qGD9KI)6qX85zS(RNtIYElO8qGBTQS#BLd/P#B@XqEHMU^RRkcZKqpI)Wq!85LMr9#BPDR+j=6\\N(eEUHkFZcpOVWI5+tL)l^R6H";
	Init(str);
	Solve();
	
	return 0;*/
	
	Cipher *cipher = new Cipher();
        cipher->loadFromFile(argv[1]);
        
        string s = cipher->getCiphertext();
        cout << cipher->getCiphertext() << endl;
        cout << "Cipher length is " << to_string(s.length()) << endl;
        cipher->printRows();
        
        cout << endl << endl;
        cipherTest();
        
	
	int convergenceLimit = -1;
	int maxGenerations = 1000;
	
	if (argc == 2)
	{
		cout << "Running for " << maxGenerations << " generations" << endl;
	}
	else if (argc == 4)
	{
		if (string(argv[2]) == "-c")
		{
			convergenceLimit = atoi(argv[3]);
			maxGenerations = -1;
			cout << "Running with convergence of " << convergenceLimit << endl;
		}
		else if (string(argv[2]) == "-g")
		{
			maxGenerations = atoi(argv[3]);
			cout << "Running for " << maxGenerations << " generations" << endl;
		}
		else
		{
			cout << "Invalid parameters" << endl;
			return 0;
		}
	}
	else
	{
		cout << "Run with no parameters to evolve 1000 generations" << endl;
		cout << "Or:" << endl << "Specify -c n to set a convergence limit of n generations" << endl;
		cout << "Specify -g n to run for n generations" << endl;
		return 0;
	}
	
	if (maxGenerations < 0 && convergenceLimit < 0) maxGenerations = 1000;
				
	//Start the actual algorithm
	Population *population = new Population(100, cipher->getSize());
	cout << "Population of size " << population->individuals->size() << endl;
	population->setCipherSize(cipher->getSize());
	int fittestGeneration = 0;
	int bestFitness = 0;
	for (int generation = 0; true; generation++)
	{
		population->evaluate(cipher);
		int fitness = population->individuals->at(0)->getFitness();
		if (fitness > bestFitness) 
		{
			bestFitness = fitness;
			fittestGeneration = generation;
		}
		population->switchGenerations();
		cout << "Generation: " << generation << ", fitness: " << fitness << endl;
		if ((maxGenerations > 0 && generation >= maxGenerations)
		|| (convergenceLimit > 0 && generation - fittestGeneration >= convergenceLimit))
		{
			cout << "Best individual, fitness " << population->individuals->at(0)->getFitness() << endl;
			break;
		}
		cout << *(population->individuals->at(0)) << endl;
	}
	
	// Optimize and show best individual, run it on test cases, if any
	cout << "Using best individual, converged in generation " << fittestGeneration << endl;
	Individual* best = &(*population->individuals->at(0));
	best->optimize();
	cout << *best << endl;
	
	delete population;
	
	return 0;
}

void cipherTest()
{
    Cipher *c = new Cipher();
    c->createFromString("ABC\nDEF\nGHI");
    bool ok = (c->getCiphertext().compare("ABCDEFGHI") == 0);
    cout << (ok ? "Cipher init OK" : "Cipher init FAIL") << endl;
    c->swapRows(0, 2);
    ok = (c->getCiphertext().compare("GHIDEFABC") == 0);
    cout << (ok ? "Swap lines OK" : "Swap lines FAIL") << endl;
    c->removeColumn(1);
    ok = (c->getCiphertext().compare("GIDFAC") == 0);
    cout << (ok ? "Remove columns OK" : "Remove columns FAIL, got " + c->getCiphertext()) << endl;
    c->createFromString("ABC\nDEF\nGHI");
    c->flip();
    ok = (c->getCiphertext().compare("IHGFEDCBA") == 0);
    cout << (ok ? "Flip OK" : "Flip FAIL, got " + c->getCiphertext()) << endl;
	
	Cipher* c2 = new Cipher(c);
	ok = (c != c2);
	cout << (ok ? "Copy ptr OK" : "Copy ptr FAIL") << endl;
	ok = (c->getCiphertext().compare(c2->getCiphertext()) == 0);
	cout << (ok ? "Copy ctext OK" : "Copy ctext FAIL") << endl;
	ok = (c->getSize().columns == c2->getSize().columns && c->getSize().rows == c2->getSize().rows);
	cout << (ok ? "Copy size OK" : "Copy size FAIL") << endl;
	
	c2->writeToFile("test.txt");
}
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

#ifndef GP_H
#define GP_H

#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <thread>

using namespace std;

typedef array<int, 3> instruction_type;

class Cipher;

enum Operators
{
    SWAP,
    REMC,
    REMR,
    FLIP,
    NOOP,
    NUM_OPERATORS
};

const int ELITISM_PERCENTAGE = 2;
const int MUTATION_PROBABILITY = 50;
const int PROGRAM_LENGTH = 15;

const int NUM_THREADS = 1;

#endif
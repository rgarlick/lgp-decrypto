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

const int NUM_THREADS = 4;

#endif
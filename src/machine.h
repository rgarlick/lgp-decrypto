#ifndef MACHINE_H
#define MACHINE_H

#include "gp.h"

class Cipher;

class Machine
{
private:
	int instructionsToSkip;
    
    void printRegisters();
    
    vector<double> consts = {3.14159, 11.0, 7.0, 5.0, 3.0, 1.0};
    Cipher *cipher;
public:
    void executeInstruction(instruction_type instruction);
    void run(const vector<instruction_type>& instructions);
    
    void setInputs(const vector<double>& inputs);
    void setCipher(Cipher* cipher);
    const Cipher* getResult() const;
    
    Machine();
};

#endif
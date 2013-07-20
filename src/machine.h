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
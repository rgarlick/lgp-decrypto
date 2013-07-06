#include "machine.h"
#include "cipher.h"

/// Runs the program passed to it on the machine
void Machine::run(const vector<instruction_type>& instructions)
{
	for (auto it = instructions.begin(); it != instructions.end(); it++)
	{
		executeInstruction(*it);
	}
}

/// Executes one instruction.
void Machine::executeInstruction(const instruction_type instruction)
{
	int dest_register = instruction[3];

	int op = instruction[0];
	if (instructionsToSkip > 0)
	{
		instructionsToSkip--;
		return;
	}
	int idx1, idx2;
	switch (op)
	{
		case NOOP:
			//everybody's favourite
			break;
		case FLIP:
			cipher->flip();
			break;
		case REMC:
			idx1 = instruction[1];
			if (idx1 < cipher->getSize().columns)
			{
				cipher->removeColumn(idx1);
			}
			break;
		case REMR:
			idx1 = instruction[1];
			if (idx1 < cipher->getSize().rows)
			{
				cipher->removeRow(idx1);
			}
			break;
		case SWAP:
			idx1 = instruction[1];
			idx2 = instruction[2];
			if (idx1 < cipher->getSize().rows && idx2 < cipher->getSize().rows)
			{
				cipher->swapRows(idx1, idx2);
			}
			break;
	}
}

const Cipher* Machine::getResult() const
{
	return cipher;
}

void Machine::setCipher(Cipher* cipher)
{
	this->cipher = new Cipher(cipher);
}


Machine::Machine() : instructionsToSkip(), cipher(nullptr)
{
}

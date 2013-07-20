#ifndef _STR_ARRAY_H_
#define _STR_ARRAY_H_

#include <string.h>
#include <math.h>
#include <string>

#define MAX_STRINGS 2048
#define LOG2 .693147

#define NUM_ROWS(C,R) (C%R? (C/R)+1:(C/R))

class StringArray
{
	public:
		StringArray() {num_strings=0; Clear();}
		~StringArray() {Clear();}

		int GetString(int,char*);
		int GetNumStrings() {return num_strings;}
		void Clear();
	
	private:
		char *strings[MAX_STRINGS];
		int num_strings;
};

int ChrIndex(const char*,char);
int GetUniques(const char*, char*, int*);

#endif


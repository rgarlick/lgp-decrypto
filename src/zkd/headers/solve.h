#ifndef _SOLVE_H
#define _SOLVE_H

#include "zodiac.h"

void StopSolve();
char FirstAvailable(char *exclude);
void StopNotify();
int FormatKey(char *key);
int StripWS(char *string);
int FindSolution();
void StartSolve();
void Reset();
void SetLanguage();

#endif

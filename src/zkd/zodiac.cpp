#ifndef _ZODIAC_H
#define _ZODIAC_H

#include "headers/zodiac.h"

#include "headers/map.h"
#include "headers/z340.h"

#include <map>
#include <string>

#define MAX_EXTRA	104

//cipher/key data & files
Message message; //cipher & main key
Message undo_message, redo_message; //undo/redo messages
int undo_line_size, redo_line_size;
char szCipherName[1024], szKeyName[1024], szPlainName[1024], szGraphName[1024]; //filenames
char *szCipherBase, *szKeyBase; //file basenames
char szLanguage[32];
int bMsgLoaded=false, bMapLoaded=false, bUndo=false;
int iNumber, iCurTab;
char szString[128], szStringTitle[128], szNumberTitle[128]; //word, exclude string
typedef std::map<std::string,int> STRMAP;
STRMAP dictionary, tabu_list, word_list;

//GUI data


//solver data

int iPriority, iLang;
char szExtraLtr[MAX_EXTRA+1]="";
int iBruteSymbols, iBatchBestScore;
char lprgcBatchBestKey[4096];
int iSolveType=0, iBlockSize=0;


#endif


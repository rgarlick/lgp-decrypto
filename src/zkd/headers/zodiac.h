#ifndef _ZODIAC_H
#define _ZODIAC_H

#include "map.h"
#include "z340.h"
#include <map>
#include <string>

//language
#define LANG_DIR	"language"
#define LANG_ENG	"eng"

//language IoC
#define IOC_ENG		.0665
#define IOC_RAND	.0385

#define DIOC		.0075
#define CHI			.55
#define ENT			4.1

//text constants
#define CHAR_WIDTH  7
#define CHAR_HEIGHT 12
#define TEXT_POS	0 //HWND_TOPMOST

#define MAX_EXTRA	104

//macros

//cipher/key data & files
extern Message message; //cipher & main key
extern char szCipherName[1024];
extern char szKeyName[1024];
extern char szPlainName[1024];
extern char szGraphName[1024]; //filenames

extern char *szCipherBase;
extern char *szKeyBase; //file basenames
extern char szLanguage[32];
extern int bMsgLoaded;
extern int bMapLoaded;
extern int bUndo;
extern int iNumber;
extern int iCurTab;

extern char szString[128];
extern char szStringTitle[128];
extern char szNumberTitle[128]; //word, exclude string

typedef std::map<std::string,int> STRMAP;
extern STRMAP dictionary, tabu_list, word_list;


//solver data
extern int iPriority;
extern int iLang;
extern char szExtraLtr[MAX_EXTRA+1];
extern int iBruteSymbols;
extern int iBatchBestScore;
extern char lprgcBatchBestKey[4096];
extern int iSolveType;
extern int iBlockSize;


#endif


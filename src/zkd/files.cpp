#include "headers/files.h"

/*File Functions*/

extern SOLVEINFO siSolveInfo;

void GetBaseName(const char *filename, char *&basename)
{
	if(filename[0]!='\0') basename=(char*)strrchr(filename,'\\')+1;
	else basename=NULL;
}

//open/save file name dialog
int GetFilename(char *szName, const char *szInitDir, int bSave)
{
	return 0;
}

//shell execute file
void OpenWith(char *szFileName)
{
}

void NewMessageInfo(char *filename)
{
	//get message filename
	strcpy(szCipherName,filename);
	GetBaseName(szCipherName,szCipherBase);

	//get info
	siSolveInfo.best_score=0;
	bMsgLoaded=true;
	bMapLoaded=false;
	siSolveInfo.best_key[0]='\0';

	//setup window
	bUndo=false;

	//block size
	int max_block=message.GetLength();

	tabu_list.clear();
}

int LoadMessage(char *filename, int type)
{
	int loaderror=false;

	if(!message.Read(filename)) loaderror=true;

	if(loaderror) //error loading file
	{
		printf("Cannot open message file: %s",(const char*)filename);
		return 0;
	}

	NewMessageInfo(filename);

	return 1;
}

int LoadStringMessage(const char* ciphertext)
{
	message.ReadFromString(ciphertext);
	//get info
	siSolveInfo.best_score=0;
	bMsgLoaded=true;
	bMapLoaded=false;
	siSolveInfo.best_key[0]='\0';

	//setup window
	bUndo=false;

	//block size
	int max_block=message.GetLength();

	tabu_list.clear();
	
	return 1;
}

int LoadMap(char *filename)
{
	//Map temp_map;

	if(!message.cur_map.Read(filename))
	{
		printf("Cannot open %s",filename);
		return 0;
	}

	siSolveInfo.best_key[0]='\0';

	//update symbols from loaded map
	//message.cur_map+=temp_map;

	//get map filename
	strcpy(szKeyName,filename);
	GetBaseName(szKeyName,szKeyBase);

	//get info
	siSolveInfo.best_score=0;
	bMapLoaded=true;
	message.Decode();

	//setup window
	bUndo=false;

	//SetDlgInfo();

	return 1;
}

int LoadINI() { return 1; }
int SaveINI() {	return 1; }


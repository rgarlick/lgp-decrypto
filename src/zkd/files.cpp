//  Copyright (C) 2013 Brax Sisco, Wesley Hopper, Michael Eaton,
//	Daniels Umanovskis
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

/************************************************************
 * This program is a lightweight, command-line hacked
 * derivation of the 2010 version of ZKDecrypto.
 *
 * The use of the GNU GPL for this program does not imply
 * support for the Free Software Foundation or that
 * the authors are in any way related to the Free Software
 * Foundation
 *
 ************************************************************/

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


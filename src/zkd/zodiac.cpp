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


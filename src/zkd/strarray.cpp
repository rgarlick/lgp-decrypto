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


#pragma warning( disable : 4996)
#pragma warning( disable : 4244)	// STOP MSVS2005 WARNINGS

#include "headers/strarray.h"

int StringArray::GetString(int string, char *dest_string)
{
    if(string<0 || string>=num_strings) return 0;

    strcpy(dest_string,strings[string]);
    return (int)strlen(dest_string);
}

void StringArray::Clear()
{
    for(int cur_string=0; cur_string<num_strings; cur_string++)
        if(strings[cur_string]) delete[] strings[cur_string];

    memset(strings,0,MAX_STRINGS*sizeof(char*));
    num_strings=0;
}

int ChrIndex(const char *string, char chr)
{
    char *chr_ptr=strchr((char *)string,chr);
    if(!chr_ptr) return -1;
    return int(chr_ptr-string);
}

//count frequencies of chars in a string
int GetUniques(const char *string, char *unique_str, int *unique_freq)
{
    int length, index, ascii_index[256], unique=0;

    length=(int)strlen(string);

    memset(ascii_index,0,256*sizeof(int));

    for(index=0; index<length; index++)
        ascii_index[(unsigned char)string[index]]++;

    for(index=32; index<256; index++)
        if(ascii_index[index])
        {
            if(unique_str) unique_str[unique]=index;
            if(unique_freq) unique_freq[unique]=ascii_index[index];
            unique++;
        }

    if(unique_str) unique_str[unique]='\0';

    return unique;
}



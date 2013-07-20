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



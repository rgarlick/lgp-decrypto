#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "map.h"
#include "strarray.h"
#include "macros.h"
#include <string>
#include <map>

#define MAX_SYM		256
#define MAX_PAT_LEN	15

#define SOLVE_HOMO		0
#define SOLVE_DISUB		40
#define SOLVE_PLAYFAIR	1
#define SOLVE_DBLPLAY	2
#define SOLVE_VIG		3
#define SOLVE_DICTVIG	4
#define SOLVE_RUNKEY	5
#define SOLVE_BIFID		6
#define SOLVE_TRIFID	7
#define SOLVE_PERMUTE	8
#define SOLVE_COLTRANS	9
#define SOLVE_DOUBLE	10
#define SOLVE_TRIPPLE	11
#define SOLVE_ADFGX		12
#define SOLVE_ADFGVX	13
#define SOLVE_CEMOPRTU	14
#define SOLVE_SUBPERM	15
#define SOLVE_SUBCOL	16
#define SOLVE_COLVIG	17
#define SOLVE_LORENZ	18

typedef std::map<std::string,char> FRACTMAP;

struct NGRAM
{
	char string[MAX_PAT_LEN+1];
	int length;
	int freq;
	int *positions;
	int pos_size;

	NGRAM *left;
	NGRAM *right;
};

/*Message*/
class Message
{
public:
	Message()
	{
		patterns=NULL; cipher=plain=msg_temp=NULL;
		msg_len=num_patterns=good_pat=0;
		trans_type=0;
		min_pat_len=2;
		InitKeys();

	}

	~Message() {DeleteBuffers(); }

	void AllocateBuffers(int size) {DeleteBuffers(); cipher=new char[size<<1]; plain=new char[size<<1]; msg_temp=new char[size<<1];}
	void DeleteBuffers() {if(cipher) delete[] cipher; if(plain) delete[] plain; if(msg_temp) delete[] msg_temp; cipher=plain=msg_temp=NULL;}

	int Read(const char*);
	int ReadFromString(const char* ciphertext);
	int ReadNumeric(const char*);
	int Write(const char*);
	void SetCipher(const char*);
	void SetPlain(char * new_plain) {strcpy(plain,new_plain);}

	const char * GetCipher() {return cipher;}
	const char * GetPlain() {return plain;}

	int GetLength() {return msg_len;}

	void SetExpFreq();
	void GetExpFreq(int*);
	void GetActFreq(int*);

	int GetPattern(NGRAM*);
	int GetNumPatterns() {return num_patterns;}
	long PrintPatterns(void (*print_func)(NGRAM*));

	float Multiplicity() {return float(cur_map.GetNumSymbols())/msg_len;}

	void MergeSymbols(char,char,int);
	int Simplify(char*);

	void ToUpper()
	{
	  for (int i = 0; i < strlen(cipher); i++)
	  {
	    if (cipher[i] >= 97 && cipher[i] <= 122) cipher[i] -= 32;
	  }
	  SetInfo(true);
	}

	void SetInfo(int set_maps=false);
	void FindPatterns(int);


	void SetBlockSize(int new_block_size) {block_size=new_block_size;}
	int GetBlockSize() {return block_size;}

	void SetDecodeType(int new_type) {decode_type=new_type; Decode();}
	int GetDecodeType() {return decode_type;}

	void SetTransType(int new_type) {trans_type=new_type; Decode();}
	int GetTransType() {return trans_type;}


	//decoding
	Map cur_map;
	DiMap digraph_map;

	void operator += (Message &src_msg)
	{
		//src message is longer, must reallocate
		if(src_msg.msg_len>msg_len)
		{
			if(cipher) delete[] cipher;
			if(plain) delete[] plain;
			if(msg_temp) delete[] msg_temp;

			cipher=new char[src_msg.msg_len+1];
			plain=new char[src_msg.msg_len+1];
			msg_temp=new char[(src_msg.msg_len<<1)+1];
		}

		//text
		msg_len=src_msg.msg_len;
		strcpy(cipher,src_msg.cipher);
		strcpy(plain,src_msg.plain);

		cur_map=src_msg.cur_map;
		digraph_map=src_msg.digraph_map;
		memcpy(exp_freq,src_msg.exp_freq,26*sizeof(int));

		//decoding data
		decode_type=src_msg.decode_type;

		block_size=src_msg.block_size;
		trans_type=src_msg.trans_type;

		Decode();
	}

	void operator = (Message &src_msg)
	{
		*this+=src_msg;
		FindPatterns(true);
	}

	unsigned long IntToGray(unsigned long value, int bits)
	{
		int gray=value;
		for(int shift=1; shift<=bits; shift<<=1) gray^=gray>>shift;
		return gray;
	}

/*
	void Defractionate(int size)
	{
		std::string fraction;
		FRACTMAP mapping;
		FRACTMAP::iterator iter;

		if(size>msg_len) return;

		for(int index=0; index<msg_len; index+=size)
		{
			if(msg_len-index<size) break;

			fraction.assign(cipher+index,size);
			iter=mapping.find(fraction);
			if(iter!=mapping.end()) msg_temp[index/size]=iter->second; //already in map
			else mapping[fraction]=msg_temp[index/size]=char(mapping.size()+'A');
		}

		msg_temp[msg_len/size]='\0';
		SetCipher(msg_temp);
	}
*/


	void Decode()
	{
		if(plain) switch(decode_type)
		{
			case SOLVE_HOMO:	DecodeHomo(); break;
		}
	}

	void InitKeys()
	{
		if(!strlen(key)) {strcpy(key,"ABCDE"); }

		Decode();
	}


	void SetKey(const char *split_key)
	{
		int cur_key,key_start;
		char sub_key[512];

		for(cur_key=0,key_start=0; cur_key<20; cur_key++, key_start++)
		{
			int key_length=ChrIndex(split_key+key_start,'|');
			if(key_length==-1) key_length=strlen(split_key+key_start); //last key
			const char *key_ptr=split_key+key_start;

			memcpy(sub_key,key_ptr,key_length);

			switch(decode_type)
			{
				/*case SOLVE_HOMO:	cur_map.FromKey(key_ptr); break;
				case SOLVE_DISUB:	digraph_map.FromKey(key_ptr); break;*/
			}

			key_start+=key_length;

			if(!split_key[key_start]) break;
		}

		Decode();
	}

	void GetKey(char *string, char *extra)
	{
		switch(decode_type) //set key text & max length
		{
			case SOLVE_HOMO: cur_map.ToKey(string,extra); break;
		}
	}

private:
	//pattern functions

	void FlipPlainBuffer() {std::swap(plain,msg_temp); msg_temp[0]='\0';}

	//message data
	char *cipher, *plain, *msg_temp;
	int msg_len, min_pat_len;
	int exp_freq[26];
	NGRAM *patterns;
	int num_patterns, good_pat;

	//decoding functions
	int FindPolybiusIndex(int,char,int&,int&);
	int FindTrifidIndex(char,int&,int&,int&);

	void DecodeHomo();

	//decoding keys
	char key[4096];

	//decoding variables
	int decode_type;
	int block_size;
	int trans_type; //reading direction of columnar transposition
};

#endif

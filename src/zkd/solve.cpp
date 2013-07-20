#include "headers/solve.h"

extern SOLVEINFO siSolveInfo;

//reset window state when solve stops
void StopSolve()
{
	siSolveInfo.running=false;
	message.SetKey(siSolveInfo.best_key);
}

//timer thread proc


char FirstAvailable(char *exclude)
{
	for(int letter=0; letter<26; letter++)
		if(!strchr(exclude,letter+'A'))
			return letter+'A';

	return 'Z'+1;
}

void StopNotify()
{
	//save best key
	if(siSolveInfo.best_score>iBatchBestScore)
	{
		iBatchBestScore=siSolveInfo.best_score;
		strcpy(lprgcBatchBestKey,siSolveInfo.best_key);
	}

	/*if(iBruteSymbols>0) BruteNext(iBruteSymbols-1); //in brute
	if(iBruteSymbols<0) //brute finished, display best
	{
		MessageBox(hMainWnd,"Brute Force Done","Alert",MB_OK);
		message.cur_map.FromKey(lprgcBatchBestKey);
		SetDlgInfo();
		SetDlgItemInt(hMainWnd,IDC_SCORE,iBatchBestScore,false);
		iBruteSymbols=0;
	}*/
}

//only include letters, and convert to upper case
int FormatKey(char *key)
{
	int length=strlen(key), temp_index=0;
	char cur_char, *temp=new char[length+1];

	for(int index=0; index<length; index++)
	{
		cur_char=key[index];

		if(IS_LOWER_LTR(cur_char)) cur_char-=32;
		if(!IS_UPPER_LTR(cur_char)) continue;
		temp[temp_index++]=cur_char;
	}

	temp[temp_index]='\0'; 	strcpy(key,temp); delete temp;

	return temp_index;
}

//only include letters, and convert to upper case
int StripWS(char *string)
{
	int length=strlen(string), temp_index=0;
	char *temp=new char[length+1];

	for(int index=0; index<length; index++)
		if(string[index]!=' ' && string[index]!='\r' && string[index]!='\n') temp[temp_index++]=string[index];

	temp[temp_index]='\0'; strcpy(string,temp); delete temp;

	return temp_index;
}

//solve thread proc
int FindSolution()
{
	int num_symbols;
	char key[4096];
	SYMBOL symbol;

	if(!bMsgLoaded) return 0;



	if(iSolveType==SOLVE_HOMO)
	{
		num_symbols=message.cur_map.GetNumSymbols();

		//if best key is blank, set it to empty symbols + extra letters
		if(!strlen(siSolveInfo.best_key))
			message.cur_map.ToKey(siSolveInfo.best_key,szExtraLtr);

		//key=program map + additional chars of best key
		message.cur_map.ToKey(key,siSolveInfo.best_key+num_symbols);

		for(int cur_symbol=0; cur_symbol<num_symbols; cur_symbol++)
		{
			message.cur_map.GetSymbol(cur_symbol,&symbol);
		}

		hillclimb(message,message.GetCipher(),message.GetLength(),key,false);
	}

	StopSolve(); //reset window state


	StopNotify();
	return 0;
}

//reset window state when solve starts
void StartSolve()
{
	siSolveInfo.running=true;
	FindSolution();
}

void Reset() //init solve info
{
	siSolveInfo.cur_tol=0;
	siSolveInfo.cur_tabu=0;
	siSolveInfo.last_time=0;
	siSolveInfo.best_score=0;
	//SetDlgInfo();
	tabu_list.clear();
}


//set language and load data files
void SetLanguage()
{
	char szLang[8], szGraphBase[32];
	double unigraphs[26];

	switch(iLang)
	{
		case 0: strcpy(szLanguage,"English"); strcpy(szLang,LANG_ENG); siSolveInfo.lang_ioc=(float)IOC_ENG; break;
	}

	siSolveInfo.lang_dioc=(float)DIOC;
	siSolveInfo.lang_chi=(float)CHI;
	siSolveInfo.lang_ent=(float)ENT;

	for(int n=1; n<=5; n++)
	{
		if(n==1)  strcpy(szGraphBase,"unigraphs.txt");
		if(n==2)  strcpy(szGraphBase,"bigraphs.txt");
		if(n==3)  strcpy(szGraphBase,"trigraphs.txt");
		if(n==4)  strcpy(szGraphBase,"tetragraphs.txt");
		if(n==5)  strcpy(szGraphBase,"pentagraphs.txt");

		sprintf(szGraphName,"%s/%s/%s",LANG_DIR,szLang,szGraphBase);

		if(!ReadNGraphs(szGraphName,n))
		{
			printf("Could not open %s",szGraphName);
			//SendMessage(hMainWnd,WM_CLOSE,0,0);
		}
	}

	GetUnigraphs(unigraphs);
	message.cur_map.SetUnigraphs(unigraphs);
	message.SetExpFreq();
}

#include "headers/message.h"

int Message::Read(const char *filename)
{
    FILE *msgfile;
    int size;
    char cur;

    if(!(msgfile=fopen(filename,"r"))) return 0;

    //get file size
    fseek(msgfile,0,SEEK_END);
    size=ftell(msgfile)+1;
    fseek(msgfile,0,SEEK_SET);

    AllocateBuffers(size);

    if(!cipher || !plain) return 0;

    //read from file
    msg_len=0;

    while(fscanf(msgfile,"%c",&cur)!=EOF)
    {
        if(!IS_ASCII(cur) || cur==' ') continue;
        cipher[msg_len++]=cur;
    }

    cipher[msg_len]='\0';
    fclose(msgfile);

    SetInfo(true);

    return msg_len;
}

int Message::ReadFromString(const char* ciphertext)
{
    int size = strlen(ciphertext) + 1;
    AllocateBuffers(size);
    
    for (msg_len = 0; msg_len < strlen(ciphertext); ++msg_len)
    {
      char c = ciphertext[msg_len];
      if(!IS_ASCII(c) || c==' ') continue;
        cipher[msg_len]=c;
    }
    
    cipher[msg_len] = '\0';
    
    SetInfo(true);
    
	return msg_len;
}

void Message::SetExpFreq() //set expected frequencies
{
    for(int letter=0; letter<26; letter++)
        exp_freq[letter]=ROUNDTOINT((cur_map.GetUnigraph(letter)/100)*msg_len);
}

//set all info for new cipher data
void Message::SetInfo(int set_maps)
{
    SYMBOL symbol;
    DIGRAPH digraph;
    int chr;
    char sym_a=0, sym_b=0;

    SetExpFreq(); //expected letter frequencies
 //   FindPatterns(true); //patterns

    if(!set_maps) {
        Decode();
        return;
    }

    memset(&symbol,0,sizeof(SYMBOL)); //monograph map
    cur_map.Clear(CLR_ALL);
    symbol.plain='\0';

    for(chr=0; chr<msg_len; chr++)
    {
        symbol.cipher=cipher[chr];
        cur_map.AddSymbol(symbol,1);
    }

    memset(&digraph,0,sizeof(DIGRAPH)); //digraph map
    digraph_map.Clear(CLR_ALL);
    digraph.plain1=digraph.plain2='\0';

    for(chr=0; chr<msg_len; chr++)
    {
        //if(!IS_UPPER_LTR(cipher[chr])) continue;
        if(!sym_a) sym_a=cipher[chr];
        else
        {
            sym_b=cipher[chr];

            digraph.cipher1=sym_a;
            digraph.cipher2=sym_b;
            digraph_map.AddDigraph(digraph,1);
            sym_a=sym_b=0;
        }
    }

    cur_map.SortByFreq(); //sort maps
    digraph_map.SortByFreq();

    for(chr=0; chr<msg_len-1; chr++) cur_map.AddContact(cipher[chr],cipher[chr+1]); //contact analysis

    Decode();
}

void Message::GetExpFreq(int *freq)
{
    memcpy(freq,exp_freq,26*sizeof(int));
}

void Message::GetActFreq(int *freq)
{
    memset(freq,0,26*sizeof(int));

    for(int chr=0; chr<msg_len; chr++)
    {
        if(IS_LOWER_LTR(plain[chr])) freq[(unsigned char)plain[chr]-'a']++;
        else if(IS_UPPER_LTR(plain[chr]))  freq[(unsigned char)plain[chr]-'A']++;
    }
}


//cipher modifing functions

void Message::SetCipher(const char *new_cipher)
{
    int new_len=(int)strlen(new_cipher);

    if(new_len>msg_len) //realloacte buffers
    {
        DeleteBuffers();
        AllocateBuffers(new_len);
    }

    msg_len=new_len;

    strcpy(cipher,new_cipher);
    SetInfo(true);
}

//decode cipher into plain
void Message::DecodeHomo()
{
    int cur_symbol;
    char decoder[256];
//	SYMBOL symbol;

    cur_map.GetDecoder(decoder);

    for(cur_symbol=0; cur_symbol<msg_len; cur_symbol++)
        plain[cur_symbol]=decoder[(unsigned char)cipher[cur_symbol]];

    plain[msg_len]='\0';
}

char LETTER_INDEXS2[256]= {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};



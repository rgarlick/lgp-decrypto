#include "headers/map.h"

int Map::Read(const char *filename)
{

    return 1;
}


//clear map data, you can | together mode values, or use CLR_ALL
void Map::Clear(int mode)
{
    if(mode==CLR_ALL)
    {
        num_symbols=0;
        memset(symbols,0,MAX_SYM*sizeof(SYMBOL));
        memset(locked,0,MAX_SYM);
        merge_log[0]='\0';
        return;
    }

    for(int cur_symbol=0; cur_symbol<num_symbols; cur_symbol++)
    {
        if(locked[cur_symbol]) continue;

        if(mode & CLR_CIPHER) symbols[cur_symbol].cipher=0;
        if(mode & CLR_PLAIN) symbols[cur_symbol].plain=0;
        if(mode & CLR_FREQ) symbols[cur_symbol].freq=0;
        if(mode & CLR_EXCLUDE) symbols[cur_symbol].exclude[0]='\0';
    }
}

//initialize key to array of integers, homophones for each letter
void Map::Init(int *ltr_homo)
{
    int cur_letter=0;

    memset(locked,0,num_symbols);

    for(int cur_symbol=0; cur_symbol<num_symbols; cur_symbol++)
    {
        //skip letter if no homophones left
        while(!ltr_homo[cur_letter] && cur_letter<26) cur_letter++;

        if(cur_letter<26) //if all letters have not been used
        {
            symbols[cur_symbol].plain=cur_letter+'A'; //set to letter
            ltr_homo[cur_letter]--; //dec remaining homophones for this letter
        }

        else //all letters have been used, remaining symbols are blank
        {
            symbols[cur_symbol].plain=0;
            locked[cur_symbol]=true;
        }
    }
}

//make plain text characters same as cipher characters (for transposition)
void Map::AsCipher()
{
    memset(locked,0,num_symbols);

    for(int cur_symbol=0; cur_symbol<num_symbols; cur_symbol++)
        symbols[cur_symbol].plain=symbols[cur_symbol].cipher;
}

//add/update a symbol; if inc_freq is true,
//when an existing symbol is updated its frequency is incremented
int Map::AddSymbol(SYMBOL &symbol, int inc_freq)
{
    int index;

    if(num_symbols>=MAX_SYM) return 0;

    //search for symbol, and update if it exists
    index=FindByCipher(symbol.cipher);

    if(index>-1) //existing symbol
    {
        strcpy(symbols[index].exclude,symbol.exclude);

        if(locked[index]) return num_symbols;
        //if(IS_ASCII(symbol.plain))
        symbols[index].plain=symbol.plain;
        if(inc_freq) symbols[index].freq++;
        return num_symbols;
    }

    symbols[num_symbols].cipher=symbol.cipher;
    symbols[num_symbols].plain=symbol.plain;
    symbols[num_symbols].freq=1;
    symbols[num_symbols].num_precedes=symbols[num_symbols].num_follows=0;
    strcpy(symbols[num_symbols].exclude,symbol.exclude);
    num_symbols++;

    return 1;
}

void Map::AddContact(char cipher_a, char cipher_b)
{
    int index;
    int symbol_a, symbol_b;
//	CONTACT *contact;

    symbol_a=FindByCipher(cipher_a);
    symbol_b=FindByCipher(cipher_b);

    //precedes in symbol a
    for(index=0; index<symbols[symbol_a].num_precedes; index++)
        if(symbols[symbol_a].precedes[index].symbol==&symbols[symbol_b]) //precedes contact exists
        {
            symbols[symbol_a].precedes[index].freq++;
            break;
        }

    if(index==symbols[symbol_a].num_precedes) //add new precedes contact
    {
        symbols[symbol_a].precedes[index].symbol=&symbols[symbol_b];
        symbols[symbol_a].precedes[index].freq=1;
        symbols[symbol_a].num_precedes++;
    }

    //follows in symbol b
    for(index=0; index<symbols[symbol_b].num_follows; index++)
        if(symbols[symbol_b].follows[index].symbol==&symbols[symbol_a]) //precedes contact exists
        {
            symbols[symbol_b].follows[index].freq++;
            break;
        }

    if(index==symbols[symbol_b].num_follows) //add new precedes contact
    {
        symbols[symbol_b].follows[index].symbol=&symbols[symbol_a];
        symbols[symbol_b].follows[index].freq=1;
        symbols[symbol_b].num_follows++;
    }
}

//return index of symbol with specified cipher character
int Map::FindByCipher(char cipher)
{
    for(int cur_symbol=0; cur_symbol<num_symbols; cur_symbol++)
        if(symbols[cur_symbol].cipher==cipher)
            return cur_symbol;

    return -1;
}

//get symbol at index
int Map::GetSymbol(int index, SYMBOL *symbol)
{
    if(index<0 || index>num_symbols) return 0;
    memcpy(symbol,&symbols[index],sizeof(SYMBOL));
    return 1;
}

//sort the symbols in the same order that hillclimber expects
void Map::SortByFreq()
{
    SYMBOL temp_sym;
    int freq1, freq2;
    unsigned char cipher1, cipher2;
    int next, swap;

    do //buble sort
    {
        next=false;

        for(int cur_symbol=0; cur_symbol<num_symbols-1; cur_symbol++)
        {
            cipher1=symbols[cur_symbol].cipher;
            cipher2=symbols[cur_symbol+1].cipher;
            freq1=symbols[cur_symbol].freq;
            freq2=symbols[cur_symbol+1].freq;

            if(freq1<freq2) swap=true;
            else if(freq1==freq2 && cipher1<cipher2) swap=true;
            else swap=false;

            if(swap)
            {
                memcpy(&temp_sym,&symbols[cur_symbol],sizeof(SYMBOL));
                memcpy(&symbols[cur_symbol],&symbols[cur_symbol+1],sizeof(SYMBOL));
                memcpy(&symbols[cur_symbol+1],&temp_sym,sizeof(SYMBOL));
                std::swap(locked[cur_symbol],locked[cur_symbol+1]);
                next=true;
            }
        }
    }
    while(next);
}

//hillclimb key <-> Map class conversion
void Map::ToKey(char *key, char *extra)
{
    for(int cur_symbol=0; cur_symbol<num_symbols; cur_symbol++)
    {
        if(symbols[cur_symbol].plain) key[cur_symbol]=symbols[cur_symbol].plain;
        else key[cur_symbol]=BLANK;
    }

    key[num_symbols]='\0';

    strcat(key,extra);
}

void Map::FromKey(const char *key)
{
    for(int cur_symbol=0; cur_symbol<num_symbols && key[cur_symbol]; cur_symbol++)
        symbols[cur_symbol].plain=key[cur_symbol];
}






//clear map data, you can | together mode values, or use CLR_ALL
void DiMap::Clear(int mode)
{
    if(mode==CLR_ALL)
    {
        num_digraphs=0;
        memset(digraphs,0,MAX_DI*sizeof(DIGRAPH));
        memset(locked,0,MAX_DI);
        return;
    }

    for(int cur_digraph=0; cur_digraph<num_digraphs; cur_digraph++)
    {
        if(locked[cur_digraph]) continue;

        if(mode & CLR_CIPHER) digraphs[cur_digraph].cipher1=digraphs[cur_digraph].cipher2=0;
        if(mode & CLR_PLAIN) digraphs[cur_digraph].plain1=digraphs[cur_digraph].plain2=0;
        if(mode & CLR_FREQ) digraphs[cur_digraph].freq=0;
    }
}

void DiMap::Init(int num)
{
    int cur_letter=0;

    memset(locked,0,num_digraphs);

    for(int cur_digraph=0; cur_digraph<num_digraphs; cur_digraph++)
    {
        if(cur_digraph<num)
        {
            digraphs[cur_digraph].plain1=((cur_letter++)%26)+'A';
            digraphs[cur_digraph].plain2=((cur_letter++)%26)+'A';
        }

        else locked[cur_digraph]=true;
    }
}

void DiMap::AsCipher()
{
    memset(locked,0,num_digraphs);

    for(int cur_digraph=0; cur_digraph<num_digraphs; cur_digraph++)
    {
        digraphs[cur_digraph].plain1=digraphs[cur_digraph].cipher1;
        digraphs[cur_digraph].plain2=digraphs[cur_digraph].cipher2;
    }
}

//add/update a digraph; if inc_freq is true,
//when an existing symbol is updated its frequency is incremented
int DiMap::AddDigraph(DIGRAPH &digraph, int inc_freq)
{
    if(num_digraphs>=MAX_DI) return 0;

    //search for digraph, and update if it exists
    int index=FindByCipher(digraph.cipher1,digraph.cipher2);

    if(index>-1) //existing symbol
    {
        if(locked[index]) return num_digraphs;

        digraphs[index].plain1=digraph.plain1;
        digraphs[index].plain2=digraph.plain2;
        if(inc_freq) digraphs[index].freq++;
        return num_digraphs;
    }

    digraphs[num_digraphs].cipher1=digraph.cipher1;
    digraphs[num_digraphs].cipher2=digraph.cipher2;
    digraphs[num_digraphs].plain1=digraph.plain1;
    digraphs[num_digraphs].plain2=digraph.plain2;
    digraphs[num_digraphs].freq=1;
    num_digraphs++;

    return 1;
}

//return index of symbol with specified cipher character
int DiMap::FindByCipher(char cipher1, char cipher2)
{
    for(int cur_digraph=0; cur_digraph<num_digraphs; cur_digraph++)
        if(digraphs[cur_digraph].cipher1==cipher1 && digraphs[cur_digraph].cipher2==cipher2)
            return cur_digraph;

    return -1;
}

//get digraph at index
int DiMap::GetDigraph(int index, DIGRAPH *digraph)
{
    if(index<0 || index>num_digraphs) return 0;
    memcpy(digraph,&digraphs[index],sizeof(DIGRAPH));
    return 1;
}

//swap the plain text letters for two digraphs
void DiMap::SwapSymbols(int swap1, int swap2)
{
    if(locked[swap1] || locked[swap2]) return;

    std::swap(digraphs[swap1].plain1,digraphs[swap2].plain1);
    std::swap(digraphs[swap1].plain2,digraphs[swap2].plain2);
}

//hillclimb key <-> Map class conversion
void DiMap::ToKey(char *key, char *extra)
{
    for(int cur_digraph=0; cur_digraph<num_digraphs; cur_digraph++)
    {
        if(digraphs[cur_digraph].plain1) key[cur_digraph<<1]=digraphs[cur_digraph].plain1;
        else key[cur_digraph<<1]=BLANK;
        if(digraphs[cur_digraph].plain2) key[(cur_digraph<<1)+1]=digraphs[cur_digraph].plain2;
        else key[(cur_digraph<<1)+1]=BLANK;
    }

    key[num_digraphs<<1]='\0';

    strcat(key,extra);
}

void DiMap::FromKey(const char *key)
{
    for(int cur_digraph=0; cur_digraph<num_digraphs; cur_digraph++)
    {
        digraphs[cur_digraph].plain1=key[cur_digraph<<1];
        digraphs[cur_digraph].plain2=key[(cur_digraph<<1)+1];
    }
}

//sort the symbols in the same order that hillclimber expects
void DiMap::SortByFreq()
{
    DIGRAPH temp_di;
    int freq1, freq2;
    unsigned short cipher_val1, cipher_val2;
    int next, swap;

    do //buble sort
    {
        next=false;

        for(int cur_digraph=0; cur_digraph<num_digraphs-1; cur_digraph++)
        {
            cipher_val1=digraphs[cur_digraph].cipher1<<8 | digraphs[cur_digraph].cipher2;
            cipher_val2=digraphs[cur_digraph+1].cipher1<<8 | digraphs[cur_digraph+1].cipher2;
            freq1=digraphs[cur_digraph].freq;
            freq2=digraphs[cur_digraph+1].freq;

            if(freq1<freq2) swap=true;
            else if(freq1==freq2 && cipher_val1<cipher_val2) swap=true;
            else swap=false;

            if(swap)
            {
                memcpy(&temp_di,&digraphs[cur_digraph],sizeof(DIGRAPH));
                memcpy(&digraphs[cur_digraph],&digraphs[cur_digraph+1],sizeof(DIGRAPH));
                memcpy(&digraphs[cur_digraph+1],&temp_di,sizeof(DIGRAPH));
                std::swap(locked[cur_digraph],locked[cur_digraph+1]);
                next=true;
            }
        }
    }
    while(next);
}


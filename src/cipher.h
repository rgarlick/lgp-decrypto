#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <vector>

using namespace std;

struct CipherSize
{
    int rows;
    int columns;
};

class Cipher 
{
private:
    string ciphertext;
    bool readError;
    void setError();
    vector<string> rows;
    void splitIntoRows(string cipher);
    string joinRows() const;
    CipherSize size;
    void initSize();
public:
    void printRows();
    string getCiphertext() const;
    void setCiphertext(string ciphertext);
    void loadFromFile(string filename);
    void createFromString(string cipher);
    bool getError();
    void swapRows(int idx1, int idx2);
    void removeColumn(int idx);
    void removeRow(int idx);
    void flip();
    CipherSize getSize();
	
	int getHomophonicFitness() const;
	void writeToFile(string filename) const;
        
    
    Cipher(string ciphertext);
    Cipher();
    Cipher(Cipher* other) : size(other->size), rows(other->rows) {};
};
#endif
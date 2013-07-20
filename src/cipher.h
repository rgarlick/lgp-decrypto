/*  
 * 	Copyright (c) 2013, Daniels Umanovskis
 * 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 *	
 */

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
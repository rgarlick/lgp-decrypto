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

#include "cipher.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

void Init(std::string& ciphertext);
int Solve();

Cipher::Cipher(string ciphertext) : ciphertext(ciphertext), readError(false), rows(vector<string>())
{ }

Cipher::Cipher() : ciphertext(""), readError(false), rows(vector<string>())
{ }

string Cipher::getCiphertext() const
{
    return joinRows();
}

void Cipher::setError()
{
    readError = true;
}

bool Cipher::getError()
{
    return readError;
}

void Cipher::loadFromFile(string filename)
{
    stringstream buffer;
    ifstream in(filename);
    if (in.rdstate() & ifstream::failbit)
    {
        cout << "Can not open " << filename << endl;
        setError();
        return;
    }
    buffer << in.rdbuf();
    splitIntoRows(buffer.str());
    initSize();
}

void Cipher::createFromString(string cipher)
{
    splitIntoRows(cipher);
    initSize();
}

void Cipher::splitIntoRows(string cipher)
{
    rows.clear();
    istringstream ss(cipher);
    string str;
    while (getline(ss, str))
    {
        str.erase(str.find_last_not_of(" \n\r\t")+1);
        rows.push_back(str);
    }
}

string Cipher::joinRows() const
{
    stringstream buffer;
    for (auto it : rows)
    {
        buffer << it;
    }
    return buffer.str();
}

void Cipher::printRows()
{
    for (auto &it : rows)
    {
        cout << it << endl;
    }
}

void Cipher::swapRows(int idx1, int idx2)
{
    swap(rows[idx1], rows[idx2]);
}

void Cipher::removeColumn(int idx)
{
    for (string &s : rows)
    {
        s.erase(idx, 1);
    }
    size.columns--;
}

void Cipher::removeRow(int idx)
{
    rows.erase(begin(rows) + idx);
    size.rows--;
}

void Cipher::flip()
{
    for (string &s: rows)
    {
        reverse(begin(s), end(s));
    }
    reverse(begin(rows), end(rows));
}

CipherSize Cipher::getSize()
{
    return size;
}

void Cipher::initSize()
{
    size.rows = rows.size();
    int min = 0xFFFF;
    for (string s : rows)
    {
        if (s.length() < min) min = s.length();
    }
    size.columns = min;
}

int Cipher::getHomophonicFitness() const
{
	return (rand() * 10000 % 10);
	string s = getCiphertext();
	Init(s);
	return Solve();
}

void Cipher::writeToFile(string filename) const
{
	ofstream fs(filename);
	fs << getCiphertext() << endl;
	fs.close();
}

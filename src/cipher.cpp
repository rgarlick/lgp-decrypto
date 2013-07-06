#include "cipher.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

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
	return 0;
	writeToFile("dat.txt");
	string call = "./zkdecrypto-lite dat.txt -t 3";
	return system(call.c_str());
}

void Cipher::writeToFile(string filename) const
{
	ofstream fs(filename);
	fs << getCiphertext() << endl;
	fs.close();
}
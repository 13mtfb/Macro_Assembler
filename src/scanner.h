#pragma once
#include "sstream"
#include "fstream"
#include <string>
#include <vector>

using namespace std;

class scanner {
public:
	scanner(string filepath);				// create the filestream using the filename passed to the object
	~scanner();
	int scan();								// returns next token in filestream
	vector<string> returnCompoundTokens();	// returns vector of compound tokens
	void clearCompoundTokens();				// clears vector
private:
	fstream file;							// file stream
	string current_line;					// string to hold current line
	char current_char;						// char to hold current char
	unsigned int current_position;
	bool EOF_flag;
	string buffer;
	vector<string> compoundTokens;
	int LocationOrSymbol();
	int NumericLiteralOrSymbol();
	int Symbol();
	void advanceChar();
};
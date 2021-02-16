#pragma once
#include "sstream"
#include "fstream"
#include <string>

using namespace std;

class scanner {
public:
	scanner(string filepath);			// create the filestream using the filename passed to the object
	string scan();					// returns next token in filestream
private:
	fstream file;						// file stream
	string current_line;				// string to hold current line
	char current_char;					// char to hold current char
	int current_position;
	bool EOF_flag;
	string buffer;
	string LocationOrSymbol();
	string NumericLiteralOrSymbol();
	string Symbol();
	void advanceChar();
	string XXXX = "%,#@()<>+-*/&!\"'^\\";
};
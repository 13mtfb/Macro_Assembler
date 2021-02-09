#pragma once
#include "sstream"
#include "fstream"

using namespace std;

class inputReader {
public:
	inputReader(string filepath);		// create the filestream using the filename passed to the object
	string advance();					// returns next symbol in filestream
	bool more();						// checks to see if more symbols to consume
private:
	fstream file;						// file stream
	string current_line;				// string to hold current line
	int current_pos;					// current character position in line
	string SEPARATING_CHARACTERS = " ,\t;";//
};
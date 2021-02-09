#include "stdafx.h"
#include "iostream"
#include "inputReader.h"

using namespace std;


inputReader::inputReader(string filepath) {
	file.open(filepath, fstream::in);			//open the file passed to the class
	if(!file.is_open()) {						//if not succesful, print error message
		cout << "Error in file specification" << endl;
	}											//TODO: throw error to main
	getline(file, current_line);
	//cout << current_line << endl;
	current_pos = 0;							//initialize to first position in line
}

string inputReader::advance() {// returns next symbol in filestream
	string symbol;
	int size;			//size of next symbol in filestream
	int next_pos;		//nex position of substring

	//cout << "current position: " << current_pos << endl;
	symbol = current_line.substr(current_pos, current_line.length());	// discard characters already consumed
																		// including separating character

	size  = symbol.find_first_of(SEPARATING_CHARACTERS);
	next_pos = symbol.find_first_not_of(SEPARATING_CHARACTERS,size);
	//cout << "next position: " << next_pos << " in \"" << symbol << "\"" << endl;

	if (symbol[next_pos-1] == ';' || next_pos == string::npos) {		// if comment line found
																	// or no separating characters found		
		getline(file, current_line);			// get next line
		current_pos = 0;						// reset position counter
	}
	else {
		current_pos = current_pos + next_pos;
	}

	symbol = symbol.substr(0, size);



	return symbol;
}

bool inputReader::more() {//ensures more symbols in the filestream
	return !file.eof();
}
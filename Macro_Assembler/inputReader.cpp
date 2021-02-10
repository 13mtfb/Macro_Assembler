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
	current_pos = 0;							//initialize to first position in line
	next_pos = 0;
}

string inputReader::advance() {// returns next symbol in filestream
	string symbol;
	int size;			//size of next symbol in filestream


	symbol = current_line.substr(current_pos, current_line.length());	// discard characters already consumed
																		// including separating character

	size  = symbol.find_first_of(SEPARATING_CHARACTERS);
	if (size == string::npos) {											// symbol ends at EOL
		size = current_line.length();
	}

	next_pos += size;
	

	symbol = symbol.substr(0, size);


	return symbol;
}

bool inputReader::more() {//ensures more symbols in the filestream
	int test;
	bool end_symbols;
	 // next_pos should check for the first_not_of(SEPARATING_CHARACTERS) from the current (next) position
	next_pos = current_line.find_first_not_of(SEPARATING_CHARACTERS, next_pos);
	end_symbols = false;														//assume more symbols to read
	if (current_line[next_pos-1] == ';' || next_pos == string::npos || next_pos == current_line.length()-1) {		
																			// if comment line found
																			// or no separating characters found
		current_pos = 0;						// reset position counters
		next_pos = 0;

		if (!file.eof()) {						// consume lines only containing separating characters
			do {								// TODO: throw an error if separating characters like , appear on line
				getline(file, current_line);
				test = current_line.find_first_not_of(SEPARATING_CHARACTERS);
			} while (!file.eof() && (current_line[test - 1] == ';' || test == string::npos));

			if ((current_line[test - 1] == ';' || test == string::npos)) {
				end_symbols = true;
			}

		}
		else {								// no more lines to read
			end_symbols = true;
		}



	}
	else {
		current_pos = next_pos;
	}



	return !end_symbols;
}
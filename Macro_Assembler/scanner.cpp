#include "stdafx.h"
#include "iostream"
#include "scanner.h"

using namespace std;


scanner::scanner(string filepath) {
	file.open(filepath, fstream::in);			//open the file passed to the class
	if(!file.is_open()) {						//if not succesful, print error message
		cout << "Error in file specification" << endl;
	}											//TODO: throw error to main
	getline(file, current_line);
	current_position = -1;
	EOF_flag = false;
}

string scanner::scan() {
	current_position++;
	if (current_position > current_line.length()) {			//cEOF
		getline(file, current_line);
		current_position = 0;
		if (file.eof()) {
			EOF_flag = true;
		}
	}
	current_char = current_line[current_position];
	while (1) {
		if (current_char == ' ' || current_char == '\t') {		//cSpace, cTab
			current_position++;
			current_char = current_line[current_position];
		}
		else if (current_position == current_line.length()) {	//cNewLine, cEOF
			if (EOF_flag) {
				return "pEOF";
			}
			else {
				return "pNewLine";
			}
		}
		else if (current_char == '.') {							//cPeriod
			buffer = current_char;
			return LocationOrSymbol();
		}
		else if (isdigit(current_char)) {						//cDigit
			buffer = current_char;
			return NumericLiteralOrSymbol();
		}
		else if (isalpha(current_char) || current_char == '$') {	//cLetter, cDollarsign
			buffer = current_char;
			return Symbol();
		}
		else if (current_char == ':') {							//cColon
			current_position++;
			current_char = current_line[current_position];
			if (current_char == ':') {
				return "pDoubleColon";
			}
			else {
				current_position--;
				return "pColon";
			}
		}
		else if (current_char == '=') {							//cEqual
			current_position++;
			current_char = current_line[current_position];
			if (current_char == '=') {
				return "pDoubleEqual";
			}
			else {
				current_position--;
				return "pEqual";
			}
		}
		else if (current_char == ';') {							//cSemiColon
			current_position = current_line.length();
			return "pNewLine";
		}
		else if (current_line.substr(current_position, 1).find_first_of(XXXX)!=string::npos) {
			//cXXXX
			return "C" + current_line.substr(current_position, 1);
		}
		else {													//illegalCharacter
			return "#eIllegalChar (" + current_line.substr(current_position, 1) + ")";
		}
	}
}

string scanner::LocationOrSymbol() {
	current_position++;
	current_char = current_line[current_position];
	if (current_char == '.' || current_char == '$' || isdigit(current_char) || isalpha(current_char)) {
		return Symbol();
	}
	else {
		current_position--;
		return "pPeriod";
	}
}

string scanner::NumericLiteralOrSymbol() {
	while (1) {
		current_position++;
		current_char = current_line[current_position];
		if (isdigit(current_char)) {
			buffer += current_char;
		}
		else if (current_char == '.' || isalpha(current_char) || current_char == '$') {
			return Symbol();
		}
		else {
			current_position--;
			return "pNumericLiteral (" + buffer + ")";
		}
	}
}

string scanner::Symbol() {
	while (1) {
		current_position++;
		current_char = current_line[current_position];
		if (current_char == '.' || isalpha(current_char) || current_char == '$' || isdigit(current_char)) {
			buffer += current_char;
		}
		else {
			current_position--;
			return "pSymbol (" + buffer + ")";
		}
	}
}



//string scanner::advance() {// returns next symbol in filestream
//	string symbol;
//	int size;			//size of next symbol in filestream
//
//
//	symbol = current_line.substr(current_pos, current_line.length());	// discard characters already consumed
//																		// including separating character
//
//	size  = symbol.find_first_of(SEPARATING_CHARACTERS);
//	if (size == string::npos) {											// symbol ends at EOL
//		size = current_line.length();
//	}
//
//	next_pos += size;
//	
//
//	symbol = symbol.substr(0, size);
//
//
//	return symbol;
//}
//
//bool inputReader::more() {//ensures more symbols in the filestream
//	int test;
//	bool end_symbols;
//	 // next_pos should check for the first_not_of(SEPARATING_CHARACTERS) from the current (next) position
//	next_pos = current_line.find_first_not_of(SEPARATING_CHARACTERS, next_pos);
//	end_symbols = false;														//assume more symbols to read
//	if (current_line[next_pos-1] == ';' || next_pos == string::npos || next_pos == current_line.length()-1) {		
//																			// if comment line found
//																			// or no separating characters found
//		current_pos = 0;						// reset position counters
//		next_pos = 0;
//
//		if (!file.eof()) {						// consume lines only containing separating characters
//			do {								// TODO: throw an error if separating characters like , appear on line
//				getline(file, current_line);
//				test = current_line.find_first_not_of(SEPARATING_CHARACTERS);
//			} while (!file.eof() && (current_line[test - 1] == ';' || test == string::npos));
//
//			if ((current_line[test - 1] == ';' || test == string::npos)) {
//				end_symbols = true;
//			}
//
//		}
//		else {								// no more lines to read
//			end_symbols = true;
//		}
//
//
//
//	}
//	else {
//		current_pos = next_pos;
//	}
//
//
//
//	return !end_symbols;
//}
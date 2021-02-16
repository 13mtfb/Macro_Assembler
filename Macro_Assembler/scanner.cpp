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
			advanceChar();
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
		else if (current_char == '=') {							//cEqual
			advanceChar();
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
	advanceChar();
	if (current_char == '.' || current_char == '$' || isdigit(current_char) || isalpha(current_char)) {
		buffer += current_char;
		return Symbol();
	}
	else {
		current_position--;
		return "pPeriod";
	}
}

string scanner::NumericLiteralOrSymbol() {
	while (1) {
		advanceChar();
		if (isdigit(current_char)) {
			buffer += current_char;
		}
		else if (current_char == '.' || isalpha(current_char) || current_char == '$') {
			buffer += current_char;
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
		advanceChar();
		if (current_char == '.' || isalpha(current_char) || current_char == '$' || isdigit(current_char)) {
			buffer += current_char;
		}
		else if (current_char == ':') {				//cColon
			advanceChar();
			if (current_char == ':') {
				return "pGlobalLabel (" + buffer + ")";
			}
			else {
				current_position--;
				return "pLabel (" + buffer + ")";
			}
		}
		else {
			current_position--;
			return "pSymbol (" + buffer + ")";
		}
	}
}

void scanner::advanceChar() {
	current_position++;
	current_char = current_line[current_position];
}
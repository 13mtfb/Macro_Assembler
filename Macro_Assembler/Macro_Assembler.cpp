// Macro_Assembler.cpp : Defines the entry point for the console application.
//
// Command Line Syntax:
// Macro_Assember sourcefile [/D1]
//	D1[Y/N]		Parse Debug [Default: N]
//				Creates and outputs a file parse_sourcefile.txt
//				with the ASCII parse tokens.


// general includes
#include "stdafx.h"
#include "iostream"
#include "fstream"
#include <stdexcept>

// user-defined classes
#include "scanner.h"
#include "tokens.h"

using namespace std;


//global variables
// assembler variables
int current_location_counter = 0;	//current location counter
char *current_psect;				//current program section
int line_character_counter = 0;		//counter to ensure source line doesn't exceed 132 characters


//Macro_Assembler meta variables
bool parse_debug = false;

int main(int argc, char* argv[])
{
	string filename, parse;
	fstream parse_file;
	if (argc < 2) {		//we expect at least 2 arguments: program name and source filename
		std::cerr << "Usage: " << argv[0] << " SOURCE_FILENAME" << endl;
	}
	else {
		filename = argv[1];	// TODO: add command line specification

		if (argc >= 2) {	// assume Y for now
			parse_debug = true;
			parse = "parse_" + filename + ".txt";
			parse_file.open(parse, fstream::out);
		}
		int currentToken;
		try {
			scanner Scanner(filename);
			do {
				currentToken = Scanner.scan();
				cout << currentToken << endl;
				if (parse_debug) {
					parse_file << scannerTokensASCII[currentToken] << endl;
				}
			} while (currentToken != pEOF);
		} catch (exception &ex) {
			cout << "Error: " << ex.what() << endl;
		}
	}

    return 0;
}
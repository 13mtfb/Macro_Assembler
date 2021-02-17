// Macro_Assembler.cpp : Defines the entry point for the console application.
//


// general includes
#include "stdafx.h"
#include "iostream"
#include "fstream"

// user-defined classes
#include "scanner.h"

using namespace std;


//global variables
// assembler variables
int current_location_counter = 0;	//current location counter
char *current_psect;				//current program section

// meta variables
int line_character_counter = 0;		//counter to ensure source line doesn't exceed 132 characters


int main(int argc, char* argv[])
{
	if (argc < 2) {		//we expect at least 2 arguments: program name and source filename
		std::cerr << "Usage: " << argv[0] << " SOURCE_FILENAME" << endl;
	}
	else {
		string filename = argv[1];	// TODO: add command line specification
		scanner Scanner(filename);

		string currentToken = "";

		do {
			currentToken = Scanner.scan();
			cout << currentToken << endl;
		} while (currentToken != "pEOF");
	}

    return 0;
}
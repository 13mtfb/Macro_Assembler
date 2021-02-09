// Macro_Assembler.cpp : Defines the entry point for the console application.
//


// general includes
#include "stdafx.h"
#include "iostream"
#include "fstream"

// user-defined classes
#include "inputReader.h"

using namespace std;

void next_character(char ch);			//parse character, determine class

//global variables
// assembler variables
int current_location_counter = 0;	//current location counter
char *current_psect;				//current program section

// metas variables
int line_character_counter = 0;		//counter to ensure source line doesn't exceed 132 characters


int main()
{	
	string filename = "..\\examples\\example.txt";	// TODO: add command line specification
	inputReader Reader(filename);

	while (Reader.more()) {
		cout <<"\""<< Reader.advance() <<"\""<< endl;
	}

    return 0;
}


void next_character(char ch) {
	if (isalpha(ch)) {			//character is a letter
		cout << ch << " is a letter" << endl;
		//push to current_parse character array
	}
	else if (isalnum(ch)) {
		cout << ch << " is a number" << endl;
		//push to current_parse character array (if not first position)
	}
	else if (isblank) {
		cout << "space/tab inserted" << endl;
	}
	else {
		cout << ch << " is not recognized" << endl;
	}
}
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


int main()
{	
	string filename = "..\\examples\\example.txt";	// TODO: add command line specification
	scanner Scanner(filename);

	string currentToken = "";

	do {
		currentToken = Scanner.scan();
		cout << currentToken << endl;
	} while (currentToken != "pEOF");


    return 0;
}
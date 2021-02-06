// Macro_Assembler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "fstream"

using namespace std;

void next_character(char ch);			//parse character, determine class
int is_instruction(char* mnemonic);

//global variables
// assembler variables
int current_location_counter = 0;	//current location counter
char *current_psect;				//current program section

// metas variables
int line_character_counter = 0;		//counter to ensure source line doesn't exceed 132 characters


int main()
{
	char ch;
	fstream fin;
	fin.open("..\\examples\\example.txt", fstream::in);
	if (fin.is_open()) {
		while (fin >> noskipws >> ch) {	//parse file character by character
			if (ch == '\n')				//carriage return
			{
				cout << "new line" << endl;	//end of line
				//push to parse table
			}
			else {
				next_character(ch);
			}
		}
	}
	else {
		cout << "error opening file" << endl;
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



//	function: is_instruction
//		inputs:
//			mnemonic = character array of instruction mnemonic
//		output:
//			returns integer indicating instruction type and adds opcode to next parse table entry
//			1	- single operand
//			2	- double operand
//			3	- branch
//			4	- jump & subroutine
//			5	- trap & interrupt
//			6	- miscellaneous
int is_instruction(char* mnemonic) {
	int i = 0;		//intialize iterator
	while (single_operand[i] != '\0') {
		if (strcmp(mnemonic, single_operand) == 0);	//single operand determined
		{
			cout << single__operand
			//add instruction to parse table
		}
	}


}

//parse table
// Array of (n,3) where n is the number of assembled instructions
//	op_code = op_code of assembled instruction (determined by mnemonic & addressing modes)
//	[src_operand] = optional addressing mode-dependent source operand
//	[dst_operand] = optional addressing mode-dependent destination operand
//		op_code	[src_operand]	[dst_operand]
//


//sybol table
//name	address
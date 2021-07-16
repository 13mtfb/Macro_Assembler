#include "iostream"
#include <stdexcept>

//user-defined header files
#include "parser.h"
#include "tokens.h"
#include "PST.h"

using namespace std;

//// PUBLIC METHODS ////
parser::parser(){

}

int parser::screener(string symbol) {
	// return enum which matches type of symbol
	int screenerReturn = SYMBOL;

	//length of PST arrays
	int instructionLength = sizeof(instruction) / sizeof(opCode);
	int directiveLength = sizeof(directive) / sizeof(asmDir);


	for (int i = 0; i < instructionLength; i++) {
		if (symbol == instruction[i].mnemonic) {			// if symbol matches opcode, then return opcode!
			screenerReturn = OPCODE;
		}
	}
	for (int i = 0; i < directiveLength; i++) {
		if (symbol == directive[i].directive) {
			screenerReturn = ASSEMBLERDIRECTIVE;
		}
	}

	// do the same search for the macro symbol table

	return screenerReturn;
}

//// PRIVATE METHODS ////

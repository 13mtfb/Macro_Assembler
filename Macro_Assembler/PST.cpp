//The following module defines the Macro-11 permanent symbol table (PST)

#include "stdafx.h"
#include "iostream"
#include "fstream"

using namespace std;

////// OP CODES //////

char register_symbols[] = {	'R0',	// register 0 definition
							'R1',	// register 1 definition
							'R2',	// register 2 definition
							'R3',	// register 3 definition
							'R4',	// register 4 definition
							'R5',	// register 5 definition
							'R6',	// stack pointer definition
							'R7',	// program counter definition
							'\0'	// end of array
};


////// OP CODES //////
						//	menmonic   name				opcode
char single_operand[] = {	'CLR',	// clear			X050DD
							'COM',	// complement (1's)	X051DD
							'INC',	// increment		X052DD
							'DEC',	// decrement		X053DD
							'NEG',	// negate (2's)		X054DD
							'TST',	// test				X057DD
							'ROR',	// rotate right		X060DD
							'ROL',	// rotate left		X061DD
							'ASR',	// arith shift R	X062DD
							'ASL',	// arith shift L	X063DD
							'SWAB',	// swap bytes		0003DD
							'ADC',	// add carry		X055DD
							'COM',	// subtract carry	X056DD
							'COM',	// signe extend		0067DD
							'\0'	// end of array
};

char double_operand[] = {
							'\0'	// end of array
};

char branch[] = {
							'\0'	// end of array
};

char jump_subroutine[] = {
							'\0'	// end of array
};

char trap_interrupt[] = {
							'\0'	// end of array
};

char misellaneous[] = {
							'\0'	// end of array
};

char double_operand[] = {
							'\0'	// end of array
};

char condition_code[] = {
							'\0'	// end of array
};

////// Macro-11 Directives //////

char macro_directive[] = {
							'\0'	//end of array
}


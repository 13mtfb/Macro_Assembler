//The following module defines the Macro-11 permanent symbol table (PST)

#include "stdafx.h"
#include "iostream"
#include "fstream"
#include <string>

using namespace std;

enum operatorType {
	SINGLE_OPERAND,
	DOUBLE_OPERAND,
	BRANCH,
	JUMP_SUBROUTINE,
	TRAP_INTERRUPT,
	MISCELLANEOUS,
	CONDITION_CODE,
};


struct opCode {
	string mnemonic;
	char opcode;
	int operatortype;
	
};

////// OP CODES //////

static const opCode instruction[] = {
	{"HALT",	000000, MISCELLANEOUS},			//halt
	{"WAIT",	000001, MISCELLANEOUS},			//wait for interrupt
	{"RTI",		000002, TRAP_INTERRUPT},		//return from interrupt
	{"BPT",		000003, TRAP_INTERRUPT},		//breakpoint trap
	{"IOT",		000004, TRAP_INTERRUPT},		//input/output trap
	{"RESET",	000005, MISCELLANEOUS},			//reset external bus
	{"RTT",		000006, TRAP_INTERRUPT},		//return from interrupt
	{"JMP",		000100, JUMP_SUBROUTINE},		//jump
	{"RTS",		000200, JUMP_SUBROUTINE},		//return from subroutine
	{"SPL",		000230, MISCELLANEOUS},			//set priority level
	{"NOP",		000240, MISCELLANEOUS},			//no operation
	{"CLC",		000241, CONDITION_CODE},		//clear C
	{"CLV",		000242, CONDITION_CODE },		//clear V
	{"CLZ",		000244, CONDITION_CODE },		//clear Z
	{"CLN",		000250, CONDITION_CODE },		//clear N
	{"CCC",		000257, CONDITION_CODE },		//clear all cc bits
	{"SEC",		000261, CONDITION_CODE },		//set C
	{"SEV",		000262, CONDITION_CODE },		//set V
	{"SEZ",		000264, CONDITION_CODE },		//set Z
	{"SEN",		000270, CONDITION_CODE },		//set N
	{"SCC",		000277, CONDITION_CODE },		//set all cc bits
	{"SWAB",	000300, SINGLE_OPERAND},		//swap bytes
	{"BR",		000400, BRANCH},				//branch (unconditional)
	{"BNE",		001000, BRANCH},				//br if not equal (to 0)
	{"BEQ",		001400, BRANCH},				//br if equal (to 0)
	{"BGE",		002000, BRANCH},				//br if greater or eq (to 0)
	{"BLT",		002400, BRANCH},				//br if less than (0)
	{"BGT",		003000, BRANCH},				//br if greater than (0)
	{"BLE",		003400, BRANCH},				//br if less or equal to (0)
	{"JSR",		004000, JUMP_SUBROUTINE},		//jump to subroutine
	{"CLR",		005000, SINGLE_OPERAND},		//clear
	{"COM",		005100,	SINGLE_OPERAND},		//complement (1's)
	{"INC",		005200, SINGLE_OPERAND},		//increment
	{"DEC",		005300, SINGLE_OPERAND},		//decrement
	{"NEG",		005400,	SINGLE_OPERAND},		//negate (2's)
	{"ADC",		005500, SINGLE_OPERAND},		//add carry
	{"SBC",		005600, SINGLE_OPERAND},		//subtract carry
	{"TST",		005700, SINGLE_OPERAND},		//test
	{"ROR",		006000, SINGLE_OPERAND},		//rotate right
	{"ROL",		006100, SINGLE_OPERAND},		//rotate left
	{"ASR",		006200, SINGLE_OPERAND},		//arith shift right
	{"ASL",		006300, SINGLE_OPERAND},		//arith shift left
	{"MARK",	006400, JUMP_SUBROUTINE},		//mark
	{"MFPI",	006500, MISCELLANEOUS},			//move from previous instr space
	{"MTPI",	006600, MISCELLANEOUS},			//move to previous instr space
	{"SXT",		006700, SINGLE_OPERAND},		//sign extend
};


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


////// Macro-11 Directives //////

char macro_directive[] = {
							'\0'	//end of array
};
//The following module defines the Macro-11 permanent symbol tables (PST)

#include "stdafx.h"
#include "iostream"
#include "fstream"
#include <string>

using namespace std;

enum operatorType {
	SINGLE_OPERAND,
	DOUBLE_OPERAND_1,			// OPR src, dst
	DOUBLE_OPERAND_2,			// OPR src, r | OPR r, dst
	BRANCH,
	JUMP_SUBROUTINE,
	TRAP_INTERRUPT,
	MISCELLANEOUS,
	CONDITION_CODE,
};

enum directiveType {
	ASCII,
	ASCIZ,
	ASECT,
	BLKB,
	BLKW,
	BYTE,
	// others
	WORD
};


struct opCode {
	string mnemonic;
	char opcode;
	int operatortype;
	
};

struct asmDir {									//note: not complete
	string directive;
	int directivetype;
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
	{"MOV",		010000, DOUBLE_OPERAND_1},		//move
	{"CMP",		020000, DOUBLE_OPERAND_1},		//compare
	{"BIT",		030000, DOUBLE_OPERAND_1},		//bit test (AND)
	{"BIC",		040000, DOUBLE_OPERAND_1},		//bit clear
	{"BIS",		050000, DOUBLE_OPERAND_1},		//bit set (OR)
	{"ADD",		060000, DOUBLE_OPERAND_1},		//add
	{"MUL",		070000,	DOUBLE_OPERAND_2},		//multiply
	{"DIV",		071000, DOUBLE_OPERAND_2},		//divide
	{"ASH",		072000, DOUBLE_OPERAND_2},		//shift arithmetically
	{"ASHC",	073000, DOUBLE_OPERAND_2},		//arith shift combined
	{"XOR",		074000, DOUBLE_OPERAND_2},		//exclusive OR
	//FADD
	//FSUB
	//FMUL
	//FDIV
	{"SOB",		077000, JUMP_SUBROUTINE},		//subtract 1 & br (if !=0)
	{"BPL",		100000, BRANCH},				//branch if plus 
	{"BMI",		100400, BRANCH},				//branch if minus
	{"BHI",		101000, BRANCH},				//branch if higher
	{"BLOS",	101400, BRANCH},				//branch if lower or same
	{"BVC",		102000, BRANCH},				//branch if overflow is clear
	{"BVS",		102400, BRANCH},				//branch if overflow is set
	{"BCC",		103000, BRANCH},				//br if carry is clear
	{"BHIS",	103000, BRANCH},				//branch if higher or same
	{"BCS",		103400, BRANCH},				//br if carry is set
	{"BLO",		103400, BRANCH},				//branch if lower
	{"EMT",		104000, TRAP_INTERRUPT},	
	{"TRAP",	104400, TRAP_INTERRUPT},
	{"CLRB",	105000, SINGLE_OPERAND},		//clear (byte)
	{"COMB",	105100,	SINGLE_OPERAND},		//complement (1's) (byte)
	{"INCB",	105200, SINGLE_OPERAND},		//increment (byte)
	{"DECB",	105300, SINGLE_OPERAND},		//decrement (byte)
	{"NEGB",	105400,	SINGLE_OPERAND},		//negate (2's) (byte)
	{"ADCB",	105500, SINGLE_OPERAND},		//add carry (byte)
	{"SBCB",	105600, SINGLE_OPERAND},		//subtract carry (byte)
	{"TSTB",	105700, SINGLE_OPERAND},		//test (byte)
	{"RORB",	106000, SINGLE_OPERAND},		//rotate right (byte)
	{"ROLB",	106100, SINGLE_OPERAND},		//rotate left (byte)
	{"ASRB",	106200, SINGLE_OPERAND},		//arith shift right (byte)
	{"ASLB",	106300, SINGLE_OPERAND},		//arith shift left (byte)
	{"MFPD",	106500, MISCELLANEOUS},
	{"MTPD",	106600, MISCELLANEOUS},
	{"MOVB",	110000, DOUBLE_OPERAND_1},		//move (byte)
	{"CMPB",	120000, DOUBLE_OPERAND_1},		//compare (byte)
	{"BITB",	130000, DOUBLE_OPERAND_1},		//bit test (AND) (byte)
	{"BICB",	140000, DOUBLE_OPERAND_1},		//bit clear (byte)
	{"BISB",	150000, DOUBLE_OPERAND_1},		//bit set (OR) (byte)
	{"SUB",		160000, DOUBLE_OPERAND_1}
	//Floating point instructions
};


static const asmDir directive[]{				// note: not complete
	{".ASCII",		ASCII},
	{".ASCIZ",		ASCIZ},
	{".ASECT",		ASECT},
	{".BLKB",		BLKB},
	{".BLKW",		BLKW},
	{".BYTE",		BYTE},
	// others
	{".WORD",		WORD}
};
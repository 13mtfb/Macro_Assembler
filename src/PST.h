//The following module defines the Macro-11 permanent symbol tables (PST)
#include "iostream"
#include "fstream"
#include <string>

using namespace std;

enum operatorType
{
	SINGLE_OPERAND,
	DOUBLE_OPERAND_1, // OPR src, dst
	DOUBLE_OPERAND_2, // OPR src, r | OPR r, dst
	BRANCH,
	JUMP_SUBROUTINE_RTS,
	JUMP_SUBROUTINE_MARK,
	JUMP_SUBROUTINE_SOB,
	TRAP_INTERRUPT_1, // EMT, TRAP
	TRAP_INTERRUPT_2, // BPT, IOT, RTI, RTT
	MISCELLANEOUS,
	MISCELLANEOUS_SPL,
	CONDITION_CODE,
};

enum directiveType
{
	ASCII,
	ASCIZ,
	ASECT,
	BLKB,
	BLKW,
	BYTE,
	// others
	END,
	// others
	MCALL,
	// others
	TITLE,
	WORD
};

struct opCode
{
	string mnemonic;
	unsigned short int opcode;
	int operatortype;
};

struct asmDir
{ //note: not complete
	string directive;
	int directivetype;
};

////// OP CODES //////

static const opCode instruction[] = {
	{"HALT", 0000000, MISCELLANEOUS},		 //halt
	{"WAIT", 0000001, MISCELLANEOUS},		 //wait for interrupt
	{"RTI", 0000002, TRAP_INTERRUPT_2},		 //return from interrupt
	{"BPT", 0000003, TRAP_INTERRUPT_2},		 //breakpoint trap
	{"IOT", 0000004, TRAP_INTERRUPT_2},		 //input/output trap
	{"RESET", 0000005, MISCELLANEOUS},		 //reset external bus
	{"RTT", 0000006, TRAP_INTERRUPT_2},		 //return from interrupt
	{"JMP", 0000100, SINGLE_OPERAND},		 //jump
	{"RTS", 0000200, JUMP_SUBROUTINE_RTS},	 //return from subroutine
	{"SPL", 0000230, MISCELLANEOUS_SPL},	 //set priority level
	{"NOP", 0000240, MISCELLANEOUS},		 //no operation
	{"CLC", 0000241, CONDITION_CODE},		 //clear C
	{"CLV", 0000242, CONDITION_CODE},		 //clear V
	{"CLZ", 0000244, CONDITION_CODE},		 //clear Z
	{"CLN", 0000250, CONDITION_CODE},		 //clear N
	{"CCC", 0000257, CONDITION_CODE},		 //clear all cc bits
	{"SEC", 0000261, CONDITION_CODE},		 //set C
	{"SEV", 0000262, CONDITION_CODE},		 //set V
	{"SEZ", 0000264, CONDITION_CODE},		 //set Z
	{"SEN", 0000270, CONDITION_CODE},		 //set N
	{"SCC", 0000277, CONDITION_CODE},		 //set all cc bits
	{"SWAB", 0000300, SINGLE_OPERAND},		 //swap bytes
	{"BR", 0000400, BRANCH},				 //branch (unconditional)
	{"BNE", 0001000, BRANCH},				 //br if not equal (to 0)
	{"BEQ", 0001400, BRANCH},				 //br if equal (to 0)
	{"BGE", 0002000, BRANCH},				 //br if greater or eq (to 0)
	{"BLT", 0002400, BRANCH},				 //br if less than (0)
	{"BGT", 0003000, BRANCH},				 //br if greater than (0)
	{"BLE", 0003400, BRANCH},				 //br if less or equal to (0)
	{"JSR", 0004000, DOUBLE_OPERAND_2},		 //jump to subroutine
	{"CLR", 0005000, SINGLE_OPERAND},		 //clear
	{"COM", 0005100, SINGLE_OPERAND},		 //complement (1's)
	{"INC", 0005200, SINGLE_OPERAND},		 //increment
	{"DEC", 0005300, SINGLE_OPERAND},		 //decrement
	{"NEG", 0005400, SINGLE_OPERAND},		 //negate (2's)
	{"ADC", 0005500, SINGLE_OPERAND},		 //add carry
	{"SBC", 0005600, SINGLE_OPERAND},		 //subtract carry
	{"TST", 0005700, SINGLE_OPERAND},		 //test
	{"ROR", 0006000, SINGLE_OPERAND},		 //rotate right
	{"ROL", 0006100, SINGLE_OPERAND},		 //rotate left
	{"ASR", 0006200, SINGLE_OPERAND},		 //arith shift right
	{"ASL", 0006300, SINGLE_OPERAND},		 //arith shift left
	{"MARK", 0006400, JUMP_SUBROUTINE_MARK}, //mark
	{"MFPI", 0006500, SINGLE_OPERAND},		 //move from previous instr space
	{"MTPI", 0006600, SINGLE_OPERAND},		 //move to previous instr space
	{"SXT", 0006700, SINGLE_OPERAND},		 //sign extend
	{"MOV", 0010000, DOUBLE_OPERAND_1},		 //move
	{"CMP", 0020000, DOUBLE_OPERAND_1},		 //compare
	{"BIT", 0030000, DOUBLE_OPERAND_1},		 //bit test (AND)
	{"BIC", 0040000, DOUBLE_OPERAND_1},		 //bit clear
	{"BIS", 0050000, DOUBLE_OPERAND_1},		 //bit set (OR)
	{"ADD", 0060000, DOUBLE_OPERAND_1},		 //add
	{"MUL", 0070000, DOUBLE_OPERAND_2},		 //multiply
	{"DIV", 0071000, DOUBLE_OPERAND_2},		 //divide
	{"ASH", 0072000, DOUBLE_OPERAND_2},		 //shift arithmetically
	{"ASHC", 0073000, DOUBLE_OPERAND_2},	 //arith shift combined
	{"XOR", 0074000, DOUBLE_OPERAND_2},		 //exclusive OR
	//FADD
	//FSUB
	//FMUL
	//FDIV
	{"SOB", 0077000, JUMP_SUBROUTINE_SOB}, //subtract 1 & br (if !=0)
	{"BPL", 0100000, BRANCH},			   //branch if plus
	{"BMI", 0100400, BRANCH},			   //branch if minus
	{"BHI", 0101000, BRANCH},			   //branch if higher
	{"BLOS", 0101400, BRANCH},			   //branch if lower or same
	{"BVC", 0102000, BRANCH},			   //branch if overflow is clear
	{"BVS", 0102400, BRANCH},			   //branch if overflow is set
	{"BCC", 0103000, BRANCH},			   //br if carry is clear
	{"BHIS", 0103000, BRANCH},			   //branch if higher or same
	{"BCS", 0103400, BRANCH},			   //br if carry is set
	{"BLO", 0103400, BRANCH},			   //branch if lower
	{"EMT", 0104000, TRAP_INTERRUPT_1},	   //emulator trap
	{"TRAP", 0104400, TRAP_INTERRUPT_1},   //trap
	{"CLRB", 0105000, SINGLE_OPERAND},	   //clear (byte)
	{"COMB", 0105100, SINGLE_OPERAND},	   //complement (1's) (byte)
	{"INCB", 0105200, SINGLE_OPERAND},	   //increment (byte)
	{"DECB", 0105300, SINGLE_OPERAND},	   //decrement (byte)
	{"NEGB", 0105400, SINGLE_OPERAND},	   //negate (2's) (byte)
	{"ADCB", 0105500, SINGLE_OPERAND},	   //add carry (byte)
	{"SBCB", 0105600, SINGLE_OPERAND},	   //subtract carry (byte)
	{"TSTB", 0105700, SINGLE_OPERAND},	   //test (byte)
	{"RORB", 0106000, SINGLE_OPERAND},	   //rotate right (byte)
	{"ROLB", 0106100, SINGLE_OPERAND},	   //rotate left (byte)
	{"ASRB", 0106200, SINGLE_OPERAND},	   //arith shift right (byte)
	{"ASLB", 0106300, SINGLE_OPERAND},	   //arith shift left (byte)
	{"MFPD", 0106500, SINGLE_OPERAND},	   //move from previous data space
	{"MTPD", 0106600, SINGLE_OPERAND},	   //move to previous data space
	{"MOVB", 0110000, DOUBLE_OPERAND_1},   //move (byte)
	{"CMPB", 0120000, DOUBLE_OPERAND_1},   //compare (byte)
	{"BITB", 0130000, DOUBLE_OPERAND_1},   //bit test (AND) (byte)
	{"BICB", 0140000, DOUBLE_OPERAND_1},   //bit clear (byte)
	{"BISB", 0150000, DOUBLE_OPERAND_1},   //bit set (OR) (byte)
	{"SUB", 0160000, DOUBLE_OPERAND_1}
	//Floating point instructions
};

static const asmDir directive[]{// note: not complete
								{".ASCII", ASCII},
								{".ASCIZ", ASCIZ},
								{".ASECT", ASECT},
								{".BLKB", BLKB},
								{".BLKW", BLKW},
								{".BYTE", BYTE},
								// others
								{".END", END},
								// others
								{".MCALL", MCALL},
								// others
								{".TITLE", TITLE},
								{".WORD", WORD}};
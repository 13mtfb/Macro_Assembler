#include "stdafx.h"
#include "iostream"
#include <stdexcept>

//user-defined header files
#include "parser.h"
#include "tokens.h"
#include "PST.h"

using namespace std;

//// PUBLIC METHODS ////
parser::parser(){
	locationCounter = 00;
	UST.clear();
}

void parser::parse(vector<int>l, vector<string>c) {
	line = l;
	compound = c;
	lineIndex = -1;
	compoundIndex = -1;
}

int parser::statement() {								// for now, an error is returned as -1
	switch (returnNextToken()) {
	case pLabel:
		oPushLabel(returnNextCompound(), false);
		cout << "parsed pLabel" << endl;
		return statement();
		break;
	case pGlobalLabel:
		oPushLabel(returnNextCompound(), true);
		cout << "parsed pGlobalLabel" << endl;
		return statement();
		break;
	case pSymbol:
		switch (screener(returnNextCompound())) {
		case OPCODE:
			// screener loads variable 'opcode' with operator from PST
			cout << "parsed opcode" << endl;
			return opcode();
		case ASSEMBLERDIRECTIVE:
			// TODO
			break;
		case MACROCALL:
			// TODO
			break;
		case UNKNOWN:
			switch (returnNextToken()) {
			case pEqual:
				oPushAssignment(returnCurrentCompound(), expression(), false);
				return 0;
				break;
			case pDoubleEqual:
				oPushAssignment(returnCurrentCompound(), expression(), true);
				return 0;
				break;
			default:
				return -1;
			}
		}
		break;
	case pPeriod:
		if (returnNextToken() == pEqual) {
			locationCounter = expression();
		}
		else {
			return -1;
		}
	case pNewLine:
	case pEOF:
		return 0;
		break;
	default:
		return -1;
	}
}

int parser::screener(string symbol) {
	// return enum which matches type of symbol
	int screenerReturn = UNKNOWN;

	//length of PST arrays
	int instructionLength = sizeof(instruction) / sizeof(opCode);
	int directiveLength = sizeof(directive) / sizeof(asmDir);

	string registerDefinitions[] = {
		"R1",
		"R2",
		"R3",
		"R4",
		"R5",
		"SP",
		"PC"
	};

	for (int i = 0; i < 7; i++) {
		if (symbol == registerDefinitions[i]) {
			reg = i;
			screenerReturn = REGISTER;
		}
	}


	for (int i = 0; i < instructionLength; i++) {
		if (symbol == instruction[i].mnemonic) {			// if symbol matches opcode, then return opcode!
			op = instruction[i];							//set opcode in parser object
			screenerReturn = OPCODE;
		}
	}
	for (int i = 0; i < directiveLength; i++) {				// if symbol matches assembler directive, then return that!
		if (symbol == directive[i].directive) {
			//TODO:
			// create asm variable in class and set to directive
			screenerReturn = ASSEMBLERDIRECTIVE;
		}
	}
	for (int i = 0; i < UST.size(); i++) {
		if (symbol == UST[i].name) {
			//TODO:
			// create sym variable in class and set to type (or value?)
			screenerReturn = SYMBOL;
		}
	}

	// do the same search for the macro symbol table

	return screenerReturn;
}

void parser::printUST() {
	cout << "NAME\tVALUE\tGLOBAL\tTYPE" << endl;
	string global, type;

	for (int i = 0; i < UST.size(); i++) {
		if (UST[i].global) {
			global = "Y";
		}
		else {
			global = "N";
		}
		if (UST[i].ATTRIBUTE == userSymbol::LABEL) {
			type = "LABEL";
		}
		else {
			type = "SYMBOL";
		}
		cout << UST[i].name << "\t";
		cout << std::oct << UST[i].value << "\t";
		cout << global << "\t";
		cout << type << endl;
	}

}

//// PRIVATE METHODS ////
int parser::opcode() {
	switch (op.operatortype) {
	case SINGLE_OPERAND:
		return operand();
		break;
	case DOUBLE_OPERAND_1:
		//TODO
		break;
	case DOUBLE_OPERAND_2:
		//TODO
		break;
	case BRANCH:
		//TODO
		break;
	case JUMP_SUBROUTINE_RTS:
		//TODO
		break;
	case JUMP_SUBROUTINE_MARK:
		//TODO
		break;
	case JUMP_SUBROUTINE_SOB:
		//TODO
		break;
	case TRAP_INTERRUPT_1:
		//TODO
		break;
	case TRAP_INTERRUPT_2:
		//TODO
		break;
	case CONDITION_CODE:
		//TODO
		break;
	case MISCELLANEOUS:
		//TODO
		break;
	case MISCELLANEOUS_SPL:
		//TODO
		break;
	default:					//shouldn't get here
		return -1;
	}


}

int parser::operand() {
	switch (returnNextToken()) {
	case pSymbol:
		switch (screener(returnNextCompound())) {
		case REGISTER:
			locationCounter += 2;
			cout << "aRegisterMode" << endl;
			return 0;
		default:
			//expression
			return 0;
		}





	}
	return -1;
}

int parser::expression() {
	//TODO
	return 0;
}

//// UTILITY METHODS ////

int parser::returnCurrentToken() {
	if (lineIndex < line.size()) {
		return line[lineIndex];
	}
	else {
		return -1;
	}
}

int parser::returnNextToken() {
	lineIndex++;
	return returnCurrentToken();
}

string parser::returnCurrentCompound() {
	if (compoundIndex < compound.size()) {
		return compound[compoundIndex];
	}
	else {
		return "";
	}
}

string parser::returnNextCompound() {
	compoundIndex++;
	return returnCurrentCompound();
}

//// MECHANISMS ////

void parser::oPushLabel(string label, bool global) {
	userSymbol add = {
		label,
		locationCounter,
		global,
		userSymbol::LABEL
	};

	UST.push_back(add);

}

void parser::oPushAssignment(string equate, int value, bool global) {
	userSymbol add = {
		equate,
		value,
		global,
		userSymbol::EQUATE
	};

	UST.push_back(add);
}
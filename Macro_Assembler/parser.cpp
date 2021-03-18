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
		break;
	case pNewLine:
		cout << "parsed pNewLine" << endl;
		return 0;
		break;
	case pEOF:
		cout << "parsed pEOF" << endl;
		return 0;
		break;
	default:
		cout << "eIllegalStatement" << endl;
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

// for all methods with an integer return type, the following return value applies:
	//  0 - successful
	// -X - error

int parser::opcode() {
	switch (op.operatortype) {
	case SINGLE_OPERAND:
		locationCounter += 2;
		cout << "aSingleOperand" << endl;
		if (!operand()) {
			switch (returnNextToken()) {
			case pNewLine:
				return 0;
				break;
			default:
				//TODO:
				// Replace with eIllegalStatement error
				return -1;
			}
		}
		break;
	case DOUBLE_OPERAND_1:
		locationCounter += 2;
		cout << "aDoubleOperand" << endl;
		if (!operand()) {	// if successful
			cout << "does it get here?" << endl;
			switch (returnCurrentToken()) {
			case pOperandFieldSeperator:
				if (!operand()) {
					switch (returnNextToken()) {
					case pNewLine:
						return 0;
						break;
					default:
						//TODO:
						// Replace with eIllegalStatement error
						return -1;
					}
				}
				else {
					//TODO:
					// Replace with eIllegalStatement error
					return -1;
				}
				break;
			default:
				//TODO:
				// Replace with eMissingOperator error
				return -1;
			}
		}
		else {				// else not successful
			return -1;
		}
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
			cout << "aRegisterMode" << endl;
			return 0;
			break;
		default:
			switch (expression()) {
			case 0:
				switch (returnNextToken()) {
				case pLeftParen:
					switch (registerexpression()) {
					case pRightParen:
						locationCounter += 2;
						cout << "aIndexMode" << endl;
						return 0;
						break;
					default:
						return -1;
						//TODO
						// Replace with eIllegalOperandSpecification error
					}
					break;
				default:
					locationCounter += 2;
					cout << "aRelativeMode" << endl;
				}
				break;
			case -1:
				return -1;
				//TODO
				// Replace with eIllegalOperandSpecification error
				break;
			}
		}
		break;
	case pDeferredAddressingIndicator:
		switch (returnNextToken()) {
		case pSymbol:
			switch (screener(returnNextCompound())) {
			case REGISTER:
				cout << "aRegisterDeferredMode" << endl;
				return 0;
				break;
			default:
				switch (expression()) {
				case 0:
					switch (returnNextToken()) {
					case pLeftParen:
						switch (registerexpression()) {
						case pRightParen:
							locationCounter += 2;
							cout << "aIndexDeferredMode" << endl;
							return 0;
							break;
						default:
							return -1;
							//TODO
							// Replace with eIllegalOperandSpecification error
						}
						break;
					default:
						locationCounter += 2;
						cout << "aRelativeDeferredMode" << endl;
					}
					break;
				case -1:
					return -1;
					//TODO
					// Replace with eIllegalOperandSpecification error
					break;
				}
			}
			break;
		case pMinus:
			switch (returnNextToken()) {
			case pLeftParen:
				switch(registerexpression()){
				case 0:
					switch (returnNextToken()) {
					case pRightParen:
						locationCounter += 2;
						cout << "aAutoDecrementDeferredMode" << endl;
						return 0;
						break;
					default:
						//TODO
						// Replace with eIllegalOperandSpecification error
						return -1;
					}
				break;
				case -1:
					//TODO
					// Replace with eIllegalOperandSpecification error
					return -1;
				}
				break;
			default:
				//TODO
				// Replace with eIllegalOperandSpecification error
				return -1;
			}
			break;
		case pLeftParen:
			switch (registerexpression()) {
			case 0:
				switch (returnNextToken()) {
				case pRightParen:
					switch (returnNextToken()) {
					case pPlus:
						locationCounter += 2;
						cout << "aAutoIncrementDeferredMode" << endl;
						return 0;
						break;
					default:
						//TODO
						// Replace with eIllegalOperandSpecification error
						return -1;
					}
					break;
				default:
					//TODO
					// Replace with eIllegalOperandSpecification error
					return -1;
				}
				break;
			case -1:
				//TODO
				// Replace with eIllegalOperandSpecification error
				return -1;
			}
			break;
		case pImmediateExpressionIndicator:
			switch (expression()) {
			case 0:
				locationCounter += 2;
				cout << "aAbsoluteMode" << endl;
				return 0;
				break;
			case -1:
				//TODO
				// Replace with eIllegalOperandSpecification error
				return -1;
			}
			break;
		default:
			//TODO
			// Replace with eIllegalOperandSpecification error
			return -1;
		}
	case pLeftParen:
		switch (registerexpression()) {
		case 0:
			switch (returnNextToken()) {
			case pRightParen:
				switch (returnNextToken()) {
				case pPlus:
					locationCounter += 2;
					cout << "aAutoIncrementMode" << endl;
					return 0;
					break;
				default:
					locationCounter += 2;
					cout << "aRegisterDeferredMode" << endl;
					return -0;
				}
				break;
			default:
				//TODO
				// Replace with eIllegalOperandSpecification error
				return -1;
			}
			break;
		case -1:
			//TODO
			// Replace with eIllegalOperandSpecification error
			return -1;
		}
		break;
	case pMinus:
		switch (returnNextToken()) {
			case pLeftParen:
				switch (registerexpression()) {
				case 0:
					switch (returnNextToken()) {
					case pRightParen:
						locationCounter += 2;
						cout << "aAutoDecrementMode" << endl;
						return 0;
						break;
					default:
						//TODO
						// Replace with eIllegalOperandSpecification error
						return -1;
					}
					break;
				case -1:
					//TODO
					// Replace with eIllegalOperandSpecification error
					return -1;
				}
			default:
				//TODO
				// Replace with eIllegalOperandSpecification error
				return -1;
			}
			break;
	case pImmediateExpressionIndicator:
		switch (expression()) {
		case 0:
			locationCounter += 2;
			cout << "aImmediateMode" << endl;
			return 0;
			break;
		case -1:
			//TODO
			// Replace with eIllegalOperandSpecification error
			return -1;
		}
		break;
	default:
		//TODO
		// Replace with eIllegalOperandSpecification error
		return -1;
	}
}

int parser::expression() {
	switch (term()) {
	case 0:	//successful
		switch (returnNextToken()) {
		case pPlus:
		case pMinus:
		case pMultiply:
		case pDivide:
		case pAnd:
		case pOr:
			// oPushExpressionOperator
			return expression();
		default:
			return 0;
		}
	case -1:
		return -1;
	}
	return 0;
}

int parser::registerexpression() {
	switch (returnNextToken()) {
	case pSymbol:
		switch (screener(returnNextCompound())) {
		case REGISTER:
			return 0;
			break;
		default:
			switch (expression()) {
			case 0:
				//oChooseExpression >> Type
				return 0;
				break;
			case -1:
				return -1;
				//TODO
				// Replace with eIllegalRegisterExpression error
			}
		}
	default:
		return -1;
		//TODO
		// Replace with eIllegalRegisterExpression error
	}
}

int parser::term() {
	switch (returnNextToken()) {
	case pPlus:
	case pMinus:
	case pUnary:
		//oPushTermOperator
		return term();
		break;
	case pLeftBracket:
		return expression();
		switch (returnNextToken()) {
		case pRightBracket:
			//oEvaluateTerm
			return 0;
			break;
		default:
			return -1;
			//TODO
			// Replace with eIllegalTerm error
		}
		break;
	case pNumericLiteral:
		//oEvaluateTerm
		return 0;
		break;
	case pSymbol:
		switch (screener(returnNextCompound())) {
		case SYMBOL:
		case OPCODE:
		case UNKNOWN:
			//oEvaluateTerm
			return 0;
			break;
		default:
			return -1;
			// Replace with eIllegalTerm error
		}
		break;
	case pSingleASCII:
		switch (returnNextToken()) {
		case pSymbol:
			//check if single ASCII character
			//oEvaluateTerm
			return 0;
			break;
		default:
			return -1;
			//TODO
			// Replace with eIllegalTerm error
		}
		break;
	case pDoubleASCII:
		switch (returnNextToken()) {
		case pSymbol:
			//check if double ASCII character
			//oEvaluateTerm
			return 0;
			break;
		default:
			return -1;
			//TODO
			// Replace with eIllegalTerm error
		}
		break;
	default:
		return -1;
		//TODO
		// Replace with eIllegalTerm error
	}

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
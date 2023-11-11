#include "iostream"
#include <stdexcept>

// user-defined header files
#include "parser.h"
#include "tokens.h"
#include "PST.h"

using namespace std;

//// PUBLIC METHODS ////
parser::parser()
{
	locationCounter = 00;
	UST.clear();

	// assume when constructing the object we are doing a first pass
	second_pass = false;
}

void parser::parse(vector<int> l, vector<string> c)
{
	line = l;
	compound = c;
	lineIndex = -1;
	compoundIndex = -1;
	deferredAddressing = false;
	statement();
}

void parser::statement()
{ // for now, an error is returned as -1
	switch (returnNextToken())
	{
	case pLabel:
		// attempt to insert label into the UST
		if (!second_pass)
		{
			if (oPushLabel(returnNextCompound(), false))
			{
				cout << "parsed pLabel" << endl;
				statement();
			}
			else
			{
				throw(E_MULTIPLE_DEFINED_SYMBOL);
			}
		}
		break;
	case pGlobalLabel:
		if (!second_pass)
		{
			if (oPushLabel(returnNextCompound(), true))
			{
				cout << "parsed pGlobalLabel" << endl;
				statement();
			}
			else
			{
				throw(E_MULTIPLE_DEFINED_SYMBOL);
			}
		}
		break;
	case pSymbol:
		switch (screener(returnNextCompound()))
		{
		case OPCODE:
			// screener loads variable 'opcode' with operator from PST
			cout << "parsed opcode" << endl;
			opcode();
			break;
		case ASSEMBLERDIRECTIVE:
			// TODO
			break;
		case MACROCALL:
			// TODO
			break;
		case UNKNOWN:
			switch (returnNextToken())
			{
			case pEqual:
				// TODO:
				// oPushAssignment(returnCurrentCompound(), expression(), false);
				break;
			case pDoubleEqual:
				// TODO:
				// oPushAssignment(returnCurrentCompound(), expression(), true);
				break;
			default:
				throw(E_ILLEGAL_STATEMENT_UNKNOWN_SYMBOL);
			}
		}
		break;
	case pPeriod:
		if (returnNextToken() == pEqual)
		{
			// TODO
			// locationCounter = expression();
		}
		else
		{
			throw(E_ILLEGAL_STATEMENT_UNKNOWN_PERIOD);
		}
		break;
	case pNewLine:
	case pEOF:
		// ignore - no processing needed
		break;
	default:
		throw(E_ILLEGAL_STATEMENT);
	}
}

int parser::screener(string symbol)
{
	// return enum which matches type of symbol
	int screenerReturn = UNKNOWN;

	// length of PST arrays
	int instructionLength = sizeof(instruction) / sizeof(opCode);
	int directiveLength = sizeof(directive) / sizeof(asmDir);

	string registerDefinitions[] = {
		"R0",
		"R1",
		"R2",
		"R3",
		"R4",
		"R5",
		"SP",
		"PC"};

	for (int i = 0; i < 8; i++)
	{
		if (symbol == registerDefinitions[i])
		{
			reg = i;
			screenerReturn = REGISTER;
		}
	}

	for (int i = 0; i < instructionLength; i++)
	{
		if (symbol == instruction[i].mnemonic)
		{						 // if symbol matches opcode, then return opcode!
			op = instruction[i]; // set opcode in parser object
			screenerReturn = OPCODE;
		}
	}
	for (int i = 0; i < directiveLength; i++)
	{ // if symbol matches assembler directive, then return that!
		if (symbol == directive[i].directive)
		{
			// TODO:
			//  create asm variable in class and set to directive
			screenerReturn = ASSEMBLERDIRECTIVE;
		}
	}
	for (auto i : UST)
	{
		// i.second is the userSymbol value from the map
		if (symbol == i.first)
		{
			// TODO:
			//  create sym variable in class and set to type (or value?)
			sym_val = i.second.value;
			screenerReturn = SYMBOL;
		}
	}

	// do the same search for the macro symbol table

	return screenerReturn;
}

void parser::printUST()
{
	cout << "NAME\tVALUE\tGLOBAL\tTYPE" << endl;
	string global, type;

	for (auto i : UST)
	{
		if (i.second.global)
		{
			global = "Y";
		}
		else
		{
			global = "N";
		}
		if (i.second.ATTRIBUTE == userSymbol::LABEL)
		{
			type = "LABEL";
		}
		else
		{
			type = "SYMBOL";
		}
		cout << i.first << "\t";
		cout << std::oct << i.second.value << "\t";
		cout << global << "\t";
		cout << type << endl;
	}
}

void parser::setPassTwo()
{
	second_pass = true;
}

//// PRIVATE METHODS ////

void parser::opcode()
{
	switch (op.operatortype)
	{
	case SINGLE_OPERAND:
		locationCounter += 2;
		cout << "aSingleOperand" << endl;
		operand();
		switch (returnNextToken())
		{
		case pNewLine:
			break;
		case pEOF:
			break;
		default:
			throw(E_ILLEGAL_STATEMENT);
		}
		break;
	case DOUBLE_OPERAND_1:
		locationCounter += 2;
		cout << "aDoubleOperand" << endl;
		operand();
		op.opcode = op.opcode | (reg_mode << 9);
		op.opcode = op.opcode | (reg << 6);
		deferredAddressing = false;
		switch (returnNextToken())
		{
		case pOperandFieldSeperator:
			operand();
			op.opcode = op.opcode | (reg_mode << 3);
			op.opcode = op.opcode | (reg << 0);
			cout << std::oct << std::setfill('0') << std::setw(6) << op.opcode << endl;

			switch (returnNextToken())
			{
			case pNewLine:
				break;
			case pEOF:
				break;
			default:
				throw(E_ILLEGAL_STATEMENT_INVALID_OPERAND);
			}
			break;
		default:
			throw(E_MISSING_OPERATOR);
		}
		break;
	case DOUBLE_OPERAND_2:
		// TODO
		break;
	case BRANCH:
		// TODO
		break;
	case JUMP_SUBROUTINE_RTS:
		// TODO
		break;
	case JUMP_SUBROUTINE_MARK:
		// TODO
		break;
	case JUMP_SUBROUTINE_SOB:
		// TODO
		break;
	case TRAP_INTERRUPT_1:
		// TODO
		break;
	case TRAP_INTERRUPT_2:
		// TODO
		break;
	case CONDITION_CODE:
		// TODO
		break;
	case MISCELLANEOUS:
		// TODO
		break;
	case MISCELLANEOUS_SPL:
		// TODO
		break;
	default:
		throw(E_UNDEFINED_OPCODE);
	}
}

void parser::operand()
{
	switch (returnNextToken())
	{
	case pDeferredAddressingIndicator:
		if (!deferredAddressing)
		{
			deferredAddressing = true;
			operand();
		}
		else
		{
			// doubley defined deferred addressing indicator
			throw(E_ILLEGAL_OPERAND_SPECIFICATION);
		}
		break;
	case pSymbol:
		switch (screener(returnNextCompound()))
		{
		case REGISTER:
			if (!deferredAddressing)
			{
				reg_mode = aRegisterMode;
				cout << "aRegisterMode" << endl;
			}
			else
			{
				reg_mode = aRegisterDeferredMode;
				cout << "aRegisterDeferredMode" << endl;
			}
			break;
		default:
			// re-adjust compound index to point to first
			// token in expression
			compoundIndex--;
			indexOrRelative();
		}
		break;
	case pLeftParen:
		registerexpression();
		switch (returnNextToken())
		{
		case pRightParen:
			switch (returnNextToken())
			{
			case pPlus:
				if (!deferredAddressing)
				{
					reg_mode = aAutoIncrementMode;
					cout << "aAutoIncrementMode" << endl;
				}
				else
				{
					reg_mode = aAutoIncrementDeferredMode;
					cout << "aAutoIncrementDeferredMode" << endl;
				}
				break;
			default:
				// re-adjust line index to point to token
				// immediately after expression
				lineIndex--;
				reg_mode = aRegisterDeferredMode;
				cout << "aRegisterDeferredMode" << endl;
			}
			break;
		default:
			throw(E_ILLEGAL_OPERAND_SPECIFICATION);
		}
		break;
	case pMinus:
		switch (returnNextToken())
		{
		case pLeftParen:
			registerexpression();
			switch (returnNextToken())
			{
			case pRightParen:
				if (!deferredAddressing)
				{
					reg_mode = aAutoDecrementMode;
					cout << "aAutoDecrementMode" << endl;
				}
				else
				{
					reg_mode = aAutoDecrementDeferredMode;
					cout << "aAutoDecrementDeferredMode" << endl;
				}
				break;
			default:
				throw(E_ILLEGAL_OPERAND_SPECIFICATION);
			}
			break;
		default:
			throw(E_ILLEGAL_OPERAND_SPECIFICATION);
		}
		break;
	case pImmediateExpressionIndicator:
		expression();
		locationCounter += 2;
		if (!deferredAddressing)
		{
			reg_mode = 2; // enum doesn't line up with number
			reg = 7;
			cout << "aImmediateMode" << endl;
		}
		else
		{
			reg_mode = 3; // enum doesn't line up with number
			reg = 7;
			cout << "aAbsoluteMode" << endl;
		}
		break;
	default:
		indexOrRelative();
	}
}

void parser::indexOrRelative()
{
	// re-adjust line index to point to first
	// token in expression
	lineIndex--;
	expression();
	switch (returnNextToken())
	{
	case pLeftParen:
		registerexpression();
		switch (returnNextToken())
		{
		case pRightParen:
			locationCounter += 2;
			if (!deferredAddressing)
			{
				reg_mode = aIndexMode;
				cout << "aIndexMode" << endl;
			}
			else
			{
				reg_mode = aIndexDeferredMode;
				cout << "aIndexDeferredMode" << endl;
			}
			break;
		default:
			throw(E_ILLEGAL_OPERAND_SPECIFICATION);
		}
		break;
	default:
		// re-adjust line index to point to token
		// immediately after expression
		lineIndex--;
		locationCounter += 2;
		if (!deferredAddressing)
		{
			reg_mode = 6; // enum doesn't line up with number
			reg = 7;
			cout << "aRelativeMode" << endl;
		}
		else
		{
			reg_mode = 7; // enum doesn't line up with number
			reg = 7;
			cout << "aRelativeDeferredMode" << endl;
		}
	}
}

void parser::expression()
{
	term();
	switch (returnNextToken())
	{
	case pPlus:
	case pMinus:
	case pMultiply:
	case pDivide:
	case pAnd:
	case pOr:
		// oPushExpressionOperator
		expression();
		break;
	default:
		// re-adjust line index to point to token
		// immediately after expression
		lineIndex--;
	}
}

void parser::registerexpression()
{
	switch (returnNextToken())
	{
	case pSymbol:
		switch (screener(returnNextCompound()))
		{
		case REGISTER:
			break;
		default:
			expression();
			// oChooseExpression >> Type
			//  NOTE: IF NOT ABSOLUTE THEN THROW EXCEPTION
		}
		break;
	default:
		throw(E_ILLEGAL_REGISTER_EXPRESSION);
	}
}

void parser::term()
{
	switch (returnNextToken())
	{
	case pPlus:
	case pMinus:
	case pUnary:
		// oPushTermOperator
		term();
		break;
	case pLeftBracket:
		expression();
		switch (returnNextToken())
		{
		case pRightBracket:
			// oEvaluateTerm
			break;
		default:
			throw(E_ILLEGAL_TERM);
		}
		break;
	case pNumericLiteral:
		returnNextCompound(); // for now, just consume compound token
		// oEvaluateTerm
		break;
	case pPeriod:
		// oEvaluateTerm
		break;
	case pSymbol:
		switch (screener(returnNextCompound()))
		{
		case SYMBOL:
		case OPCODE:
		case UNKNOWN:
			// oEvaluateTerm
			break;
		default:
			throw(E_ILLEGAL_TERM);
		}
		break;
	case pSingleASCII:
		switch (returnNextToken())
		{
		case pSymbol:
			// check if single ASCII character
			// oEvaluateTerm
			break;
		default:
			throw(E_ILLEGAL_TERM);
		}
		break;
	case pDoubleASCII:
		switch (returnNextToken())
		{
		case pSymbol:
			// check if double ASCII character
			// oEvaluateTerm
			break;
		default:
			throw(E_ILLEGAL_TERM);
		}
		break;
	default:
		throw(E_ILLEGAL_TERM);
	}
}

//// UTILITY METHODS ////

int parser::returnCurrentToken()
{
	if (lineIndex < line.size())
	{
		return line[lineIndex];
	}
	else
	{
		return -1;
	}
}

int parser::returnNextToken()
{
	lineIndex++;
	return returnCurrentToken();
}

string parser::returnCurrentCompound()
{
	if (compoundIndex < compound.size())
	{
		return compound[compoundIndex];
	}
	else
	{
		return "";
	}
}

string parser::returnNextCompound()
{
	compoundIndex++;
	return returnCurrentCompound();
}

//// MECHANISMS ////
// oPushLabel inserts a label into the UST hashmap
//  returns true if successful, or false if label already exists
bool parser::oPushLabel(string label, bool global)
{
	bool duplicate;
	userSymbol add = {
		locationCounter,
		global,
		userSymbol::LABEL};
	// second is the bool which indicates if the new element was inserted
	return UST.insert({label, add}).second;
}
// oPushAssignment inserts an equate into the UST hashmap
//  returns true if successful, or false if the equate already exists
bool parser::oPushAssignment(string equate, int value, bool global)
{
	userSymbol add = {
		value,
		global,
		userSymbol::EQUATE};
	// second is the bool which indicates if the new element was inserted
	return UST.insert({equate, add}).second;
}
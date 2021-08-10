#include "iostream"
#include <stdexcept>

//user-defined header files
#include "scanner.h"
#include "tokens.h"

using namespace std;

//// PUBLIC METHODS ////
scanner::scanner(string filepath)
{
	file.open(filepath, fstream::in); //open the file passed to the class
	if (!file.is_open())
	{ //if not succesful, print error message
		throw runtime_error("Could not open file");
	}
	getline(file, current_line);
	current_position = -1;
	EOF_flag = false;
}

scanner::~scanner()
{
}

int scanner::scan()
{
	current_position++;
	if (current_position > current_line.length())
	{ //cEOF
		getline(file, current_line);
		current_position = 0;
		if (file.eof())
		{
			EOF_flag = true;
		}
	}
	current_char = current_line[current_position];
	while (1)
	{
		if (current_char == ' ' || current_char == '\t')
		{ //cSpace, cTab
			advanceChar();
		}
		else if (current_position == current_line.length())
		{ //cNewLine, cEOF
			if (EOF_flag)
			{
				return pEOF;
			}
			else
			{
				return pNewLine;
			}
		}
		else if (current_char == '.')
		{ //cPeriod
			buffer = current_char;
			return LocationOrSymbol();
		}
		else if (isdigit(current_char))
		{ //cDigit
			buffer = current_char;
			return NumericLiteralOrSymbol();
		}
		else if (isalpha(current_char) || current_char == '$')
		{ //cLetter, cDollarsign
			buffer = current_char;
			return Symbol();
		}
		else if (current_char == '=')
		{ //cEqual
			advanceChar();
			if (current_char == '=')
			{
				return pDoubleEqual;
			}
			else
			{
				current_position--;
				return pEqual;
			}
		}
		else if (current_char == ';')
		{ //cSemiColon
			current_position = current_line.length();
			return pNewLine;
		}
		else if (current_char == '%')
		{ //cPercentSign
			return pRegisterTerm;
		}
		else if (current_char == ',')
		{ //cComma
			return pOperandFieldSeperator;
		}
		else if (current_char == '#')
		{ //cBang
			return pImmediateExpressionIndicator;
		}
		else if (current_char == '@')
		{ //cAt
			return pDeferredAddressingIndicator;
		}
		else if (current_char == '(')
		{ //cLeftParen
			return pLeftParen;
		}
		else if (current_char == ')')
		{ //cRightParen
			return pRightParen;
		}
		else if (current_char == '<')
		{ //cLeftBracket
			return pLeftBracket;
		}
		else if (current_char == '>')
		{ //cRightBracket
			return pRightBracket;
		}
		else if (current_char == '+')
		{ //cPlus
			return pPlus;
		}
		else if (current_char == '-')
		{ //cMinus
			return pMinus;
		}
		else if (current_char == '*')
		{ //cAsterisk
			return pMultiply;
		}
		else if (current_char == '/')
		{ //cSlash
			return pDivide;
		}
		else if (current_char == '&')
		{ //cAmpersand
			return pAnd;
		}
		else if (current_char == '!')
		{ //cExclamation
			return pOr;
		}
		else if (current_char == '"')
		{ //cDoubleQuote
			return pDoubleASCII;
		}
		else if (current_char == '\'')
		{ //cSingleQuote
			return pSingleASCII;
		}
		else if (current_char == '^')
		{ //cCircumflex
			return pUnary;
		}
		else if (current_char == '^')
		{ //cCircumflex
			return pUnary;
		}
		else if (current_char == '\\')
		{ //cBackslash
			return pBackslash;
		}
		else
		{ //illegalCharacter
			buffer = current_line.substr(current_position, 1);
			compoundTokens.push_back(buffer);
			return eIllegalChar;
		}
	}
}

vector<string> scanner::returnCompoundTokens()
{
	return compoundTokens;
}

void scanner::clearCompoundTokens()
{
	compoundTokens.clear();
}

//// PRIVATE METHODS ////
int scanner::LocationOrSymbol()
{
	advanceChar();
	if (current_char == '.' || current_char == '$' || isdigit(current_char) || isalpha(current_char))
	{
		buffer += current_char;
		return Symbol();
	}
	else
	{
		current_position--;
		return pPeriod;
	}
}

int scanner::NumericLiteralOrSymbol()
{
	while (1)
	{
		advanceChar();
		if (isdigit(current_char))
		{
			buffer += current_char;
		}
		else if (current_char == '.' || isalpha(current_char) || current_char == '$')
		{
			buffer += current_char;
			return Symbol();
		}
		else
		{
			current_position--;
			compoundTokens.push_back(buffer);
			return pNumericLiteral;
		}
	}
}

int scanner::Symbol()
{
	while (1)
	{
		advanceChar();
		if (current_char == '.' || isalpha(current_char) || current_char == '$' || isdigit(current_char))
		{
			buffer += current_char;
		}
		else if (current_char == ':')
		{ //cColon
			advanceChar();
			if (current_char == ':')
			{
				compoundTokens.push_back(buffer);
				return pGlobalLabel;
			}
			else
			{
				current_position--;
				compoundTokens.push_back(buffer);
				return pLabel;
			}
		}
		else
		{
			current_position--;
			compoundTokens.push_back(buffer);
			return pSymbol;
		}
	}
}

void scanner::advanceChar()
{
	current_position++;
	current_char = current_line[current_position];
}

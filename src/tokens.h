#pragma once

#include <string>
#include <vector>

using namespace std;

enum scannerTokens
{
	pLabel, // (LABEL: -without the ":")
	pGlobalLabel,
	pSymbol,					   // ex. "abc"
	pNumericLiteral,			   // (ex. "41")
	pEqual,						   // (ex. "=")
	pDoubleEqual,				   // (ex. "==")
	pRegisterTerm,				   // (ex. "%")
	pImmediateExpressionIndicator, // ("#")
	pDeferredAddressingIndicator,  // ("@")
	pLeftParen,					   // ("(")
	pRightParen,				   // (")")
	pPeriod,					   // (".")
	pOperandFieldSeperator,		   // (",")
	pComment,					   // (";")
	pLeftBracket,				   // ("<")
	pRightBracket,				   // (">")
	pPlus,						   // ("+")
	pMinus,						   // ("-")
	pMultiply,					   // ("*")
	pDivide,					   // ("/")
	pAnd,						   // ("&")
	pOr,						   // ("!")
	pDoubleASCII,				   // (""")
	pSingleASCII,				   // ("'")
	pUnary,						   // ("^")
	pBackslash,					   // ("\")
	pNewLine,					   // ("\LF")
	pEOF,
	eIllegalChar
};

const vector<string> scannerTokensASCII = {
	"pLabel",
	"pGlobalLabel",
	"pSymbol",
	"pNumericLiteral",
	"pEqual",
	"pDoubleEqual",
	"pRegisterTerm",
	"pImmediateExpressionIndicator",
	"pDeferredAddressingIndicator",
	"pLeftParen",
	"pRightParen",
	"pPeriod",
	"pOperandFieldSeperator",
	"pComment",
	"pLeftBracket",
	"pRightBracket",
	"pPlus",
	"pMinus",
	"pMultiply",
	"pDivide",
	"pAnd",
	"pOr",
	"pDoubleASCII",
	"pSingleASCII",
	"pUnary",
	"pBackslash",
	"pNewLine",
	"pEOF",
	"eIllegalChar"};

enum screenerTokens
{
	OPCODE,
	ASSEMBLERDIRECTIVE,
	MACROCALL,
	SYMBOL,
	REGISTER,
	UNKNOWN
};

const vector<string> screenerTokensASCII = {
	"opcode",
	"assemblerDirective",
	"macroCall",
	"symbol",
	"register",
	"unknown"};

enum parserTokens
{
	//Addressing modes
	aRegisterMode,				// (R)
	aRegisterDeferredMode,		// (@R or (ER))
	aAutoIncrementMode,			// ((ER + ))
	aAutoIncrementDeferredMode, // (@(ER) + )
	aAutoDecrementMode,			// (-(ER))
	aAutoDecrementDeferredMode, // (@-(ER))
	aIndexMode,					// (E(ER))
	aIndexDeferredMode,			// (@E(ER)
	aImmediateMode,				// (#E)
	aAbsoluteMode,				// (@#E)
	aRelativeMode,				// (E)
	aRelativeDeferredMode,		// (@E)
	//instruction emit
	aSingleOperand,
	aDoubleOperand,
	aBranch,
	aJumpSubroutineRTS,
	aJumpSubroutineMARK,
	aJumpSubroutineSOB,
	aTrapInterrupt,
	aConditionCode,
	aMiscellaneous,
	aMiscellaneousSPL
};

const vector<string> parserTokens = {
	//Addressing modes
	"aRegisterMode",
	"aRegisterDeferredMode",
	"aAutoIncrementMode",
	"aAutoIncrementDeferredMode",
	"aAutoDecrementMode",
	"aAutoDecrementDeferredMode",
	"aIndexMode",
	"aIndexDeferredMode",
	"aImmediateMode",
	"aAbsoluteMode",
	"aRelativeMode",
	"aRelativeDeferredMode",
	//instruction emit
	"aSingleOperand",
	"aDoubleOperand",
	"aBranch",
	"aJumpSubroutineRTS",
	"aJumpSubroutineMARK",
	"aJumpSubroutineSOB",
	"aTrapInterrupt",
	"aConditionCode",
	"aMiscellaneous",
	"aMiscellaneousSPL"};

enum errorType
{
	NO_ERROR,
	E_ILLEGAL_STATEMENT,
	E_MISSING_OPERATOR,
	E_UNDEFINED_OPCODE,
	E_ILLEGAL_OPERAND_SPECIFICATION,
	E_ILLEGAL_REGISTER_EXPRESSION,
	E_ILLEGAL_TERM,
	E_MULTIPLE_DEFINED_SYMBOL
};

const vector<string> errorTokensASCII = {
	"NO_ERROR",
	"E_ILLEGAL_STATEMENT",
	"E_MISSING_OPERATOR",
	"E_UNDEFINED_OPCODE",
	"E_ILLEGAL_OPERAND_SPECIFICATION",
	"E_ILLEGAL_REGISTER_EXPRESSION",
	"E_ILLEGAL_TERM",
	"E_MULTIPLE_DEFINED_SYMBOL"};
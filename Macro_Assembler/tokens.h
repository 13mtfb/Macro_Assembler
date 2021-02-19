#pragma once

#include <string>
#include <vector>

using namespace std;

enum scannerTokens {
	pLabel,							// (LABEL: -without the ":")
	pGlobalLabel,
	pSymbol,						// ex. "abc"
	pNumericLiteral,				// (ex. "41")
	pEqual,							// (ex. "=")
	pDoubleEqual,					// (ex. "==")
	pRegisterTerm,					// (ex. "%")
	pImmediateExpressionIndicator,	// ("#")
	pDeferredAddressingIndicator,	// ("@")
	pLeftParen,						// ("(")
	pRightParen,					// (")")
	pPeriod,						// (".")
	pOperandFieldSeperator,			// (",")
	pComment,						// (";")
	pLeftBracket,					// ("<")
	pRightBracket,					// (">")
	pPlus,							// ("+")
	pMinus,							// ("-")
	pMultiply,						// ("*")
	pDivide,						// ("/")
	pAnd,							// ("&")
	pOr,							// ("!")
	pDoubleASCII,					// (""")
	pSingleASCII,					// ("'")
	pUnary,							// ("^")
	pBackslash,						// ("\")
	pNewLine,						// ("\LF")
	pEOF,
	eIllegalChar
};

/*vector<string> scannerTokensASCII = {
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
	"pEOF"
	"eIllegalChar"
};*/
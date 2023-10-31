// Macro_Assembler.cpp : Defines the entry point for the console application.
//
// Command Line Syntax:
// Macro_Assember sourcefile [/D1]
//	D1[Y/N]		Parse Debug [Default: N]
//				Creates and outputs a file parse_sourcefile.txt
//				with the ASCII parse tokens.

// general includes
#include "iostream"
#include "fstream"
#include <stdexcept>

// user-defined classes
#include "scanner.h"
#include "tokens.h"
#include "parser.h"

using namespace std;

// Utility function to output parser to file for debugging
// TODO: move to a separate file, or otherwise clean it up
void output_debug(int pass, fstream &f, vector<int> &lineTokens, vector<string> &compoundTokens, parser &Parser)
{ // if parse_debug is true, output to file
	int j = 0;
	for (unsigned int i = 0; i < lineTokens.size(); i++)
	{
		f << "Pass " << pass << ": ";

		if (lineTokens[i] == pSymbol)
		{
			f << screenerTokensASCII[Parser.screener(compoundTokens[j])];
			f << "(" << compoundTokens[j] << ")";
			j++;
		}
		else
		{
			f << scannerTokensASCII[lineTokens[i]];
			if (lineTokens[i] == pLabel || lineTokens[i] == pGlobalLabel || lineTokens[i] == pNumericLiteral || lineTokens[i] == eIllegalChar)
			{
				f << "(" << compoundTokens[j] << ")";
				j++;
			}
		}
		f << endl;
	}
}

int main(int argc, char *argv[])
{
	string filename, parse;
	fstream parse_file;
	vector<int> lineTokens;
	vector<string> compoundTokens;
	if (argc < 2)
	{ // we expect at least 2 arguments: program name and source filename
		std::cerr << "Usage: " << argv[0] << " SOURCE_FILENAME" << endl;
		return 1;
	}
	filename = argv[1]; // TODO: add command line specification

	parse_file.open("scanner_debug.txt", fstream::out);

	int currentToken;
	scanner Scanner(filename);
	parser Parser;

	// pass - 1
	try
	{
		do
		{ // scan each token to end of file
			currentToken = Scanner.scan();
			lineTokens.push_back(currentToken); // add token to line
			if (currentToken == pNewLine || currentToken == pEOF)
			{
				// if reached end of line, pass vector to parse
				// call parse object with lineTokens as input
				compoundTokens = Scanner.returnCompoundTokens();

				Parser.parse(lineTokens, compoundTokens);

				// output logging
				output_debug(1, parse_file, lineTokens, compoundTokens, Parser);

				// cleanup from previous line processing
				lineTokens.clear();
				Scanner.clearCompoundTokens();
			}
		} while (currentToken != pEOF);
		Parser.printUST();
	}
	// catch assembler errors
	catch (errorType e)
	{
		cout << "Error: " << errorTokensASCII[e] << "(";
		Scanner.printCurrentLine();
		cout << ")" << endl;
	}
	// catch program errors
	catch (exception &ex)
	{
		cout << "error: " << ex.what() << endl;
	}

	Parser.setPassTwo();

	// pass - 2
	try
	{
		do
		{ // scan each token to end of file
			currentToken = Scanner.scan();
			lineTokens.push_back(currentToken); // add token to line
			if (currentToken == pNewLine || currentToken == pEOF)
			{
				// if reached end of line, pass vector to parse
				// call parse object with lineTokens as input
				compoundTokens = Scanner.returnCompoundTokens();

				Parser.parse(lineTokens, compoundTokens);

				// output logging
				output_debug(2, parse_file, lineTokens, compoundTokens, Parser);

				// cleanup from previous line processing
				lineTokens.clear();
				Scanner.clearCompoundTokens();
			}
		} while (currentToken != pEOF);
		Parser.printUST();
	}
	// catch assembler errors
	catch (errorType e)
	{
		cout << "Error: " << errorTokensASCII[e] << "(";
		Scanner.printCurrentLine();
		cout << ")" << endl;
	}
	// catch program errors
	catch (exception &ex)
	{
		cout << "error: " << ex.what() << endl;
	}

	parse_file.close();

	return 0;
}

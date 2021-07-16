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


//global variables
// assembler variables
int current_location_counter = 0;	//current location counter
char *current_psect;				//current program section
int line_character_counter = 0;		//counter to ensure source line doesn't exceed 132 characters


//Macro_Assembler meta variables
bool parse_debug = false;

int main(int argc, char* argv[])
{
	string filename, parse;
	fstream parse_file;
	vector<int> lineTokens;
	vector<string> compoundTokens;
	if (argc < 2) {		//we expect at least 2 arguments: program name and source filename
		std::cerr << "Usage: " << argv[0] << " SOURCE_FILENAME" << endl;
	}
	else {
		filename = argv[1];	// TODO: add command line specification

		if (argc > 2) {	// assume Y for now
			parse_debug = true;
			parse = "scanner_" + filename + ".txt";
			parse_file.open(parse, fstream::out);
		}
		int currentToken;
		try {
			scanner Scanner(filename);
			do {															//scan each token to end of file
				parser Parser;
				currentToken = Scanner.scan();
				lineTokens.push_back(currentToken);							//add token to line
				if (currentToken == pNewLine || currentToken == pEOF) {		//if reached end of line, pass vector to parse
					//call parse object with lineTokens as input
					compoundTokens = Scanner.returnCompoundTokens();
					Scanner.clearCompoundTokens();
					if (parse_debug){										// if parse_debug is true, output to file
						int j = 0;
						for (unsigned int i = 0; i < lineTokens.size(); i++) {
							if (lineTokens[i] == pSymbol) {
								parse_file << screenerTokensASCII[Parser.screener(compoundTokens[j])];
								parse_file << "(" << compoundTokens[j] << ")";
								j++;
							}
							else {
								parse_file << scannerTokensASCII[lineTokens[i]];
								if (lineTokens[i] == pLabel || lineTokens[i] == pGlobalLabel || lineTokens[i] == pNumericLiteral || lineTokens[i] == eIllegalChar) {
									parse_file << "(" << compoundTokens[j] << ")";
									j++;
								}
							}
							parse_file << endl;
						}
					}
					lineTokens.clear();
				}
			} while (currentToken != pEOF);
		} catch (exception &ex) {
			cout << "Error: " << ex.what() << endl;
		}
	}

    return 0;
}
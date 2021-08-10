#pragma once
#include <string>
#include <vector>
#include "PST.h"

using namespace std;

class parser
{
public:
	parser(); //constructs a parser object
	int screener(string symbol);
	void parse(vector<int> line, vector<string> compound);
	void statement();
	void printUST();

private:
	//parse methods
	void opcode();
	void operand();
	void indexOrRelative();
	void registerexpression();
	void expression();
	void term();

	//private data
	opCode op;
	int reg;

	//// UTILITY METHODS ////
	int returnCurrentToken();
	int returnNextToken();
	string returnCurrentCompound();
	string returnNextCompound();

	//// UTILITY DATA ////
	int lineIndex;
	vector<int> line;
	int compoundIndex;
	vector<string> compound;
	bool deferredAddressing;

	//// DATA STRUCTURES ////
	struct userSymbol
	{
		string name;
		int value;
		bool global;
		enum
		{
			LABEL,
			EQUATE
		} ATTRIBUTE;
		// PSECT
	};

	//// MECHANISMS ////
	void oPushLabel(string label, bool global);
	void oPushAssignment(string equate, int value, bool global);

	//// ASSEMBLER VARIABLES ////
	int locationCounter; // note: ASSUME single PSECT for now
	vector<userSymbol> UST;
};
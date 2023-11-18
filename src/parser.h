#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include "PST.h"

using namespace std;

class parser
{
public:
	parser(); // constructs a parser object
	int screener(string symbol);
	void parse(vector<int> line, vector<string> compound);
	void printUST();
	void setPassTwo();

private:
	// parse methods
	void statement();
	void opcode();
	void operand();
	void indexOrRelative();
	void registerexpression();
	void expression();
	void term();

	// private data
	opCode op;
	int reg;
	int reg_mode;
	int expr;

	int sym_val;

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
	bool oPushLabel(string label, bool global);
	bool oPushAssignment(string equate, int value, bool global);
	void oPushTermOperator(int oper);
	void oPushExpressionOperator(int oper);
	void oEvaluateTerm(int term);
	void oEvaluateExpression();

	//// ASSEMBLER VARIABLES ////
	int locationCounter; // note: ASSUME single PSECT for now
	unordered_map<string, userSymbol> UST;
	queue<int> term_operator_queue;
	queue<int> expression_operator_queue;
	queue<int> expression_queue;
	bool second_pass;
};
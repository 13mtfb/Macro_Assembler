#pragma once
#include <string>
#include <vector>

using namespace std;

class parser {
public:
	parser();							//constructs a parser object
	int screener(string symbol);
	void parse(vector<string>line);
private:
	int statement();
	int opcode();
	int operand();
	int registerexpression();
	int expression();
	int term();
};
#pragma once

#include "lexer.h"
#include "preprocessor.h"

#include <fstream>
#include <vector>
#include <string>

enum statement {
	IF_STATEMENT,
	WHILE_STATEMENT,
	FUNCTION_STATEMENT,
	ELSE_STATEMENT,
	ELIF_STATEMENT,
	SWITCH_STATEMENT,
};

class Interpreter : public Lexer, public Preprocessor {
public:
	unsigned int conditionalStage = 0;
	bool isInConditional = false;
	bool isInNameAssignment = false;
	bool isInFunctionDefinition = false;
	bool isInFunctionCall = false;
	std::string currentComparisonOperator;
	std::string currentName;
	std::string functionName;
	std::vector<int> nameScope;
	std::vector<std::string> knownNames;
	std::vector<size_t> elseJump;
	std::vector<std::vector<size_t>> jumpInstruction;
	std::vector<int> statementType;
	std::vector<size_t> breakJump;
	std::vector<size_t> continueJump;
	void bytecode(std::string instruction, std::string param);
	void preprocess();
	bool nameExists(std::string name);
	int getToken(std::string str);
	void translate();
};
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
	AND_STATEMENT,
	UNKNOWN,
};

class Interpreter : public Lexer, public Preprocessor {
public:
	int pastConditional = UNKNOWN;
	int conditionalStage = 0;
	int functionDepth = 0;
	bool isInReturnStatement = false;
	bool isInConditional = false;
	bool isInNameAssignment = false;
	bool isInFunctionDefinition = false;
	bool isInFunctionCall = false;
	std::string currentComparisonOperator;
	std::string currentName;
	std::vector<std::string> functionOperatorType;
	std::vector<std::string> functionName;
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
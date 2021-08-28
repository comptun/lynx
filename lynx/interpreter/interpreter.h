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
};

class Interpreter : public Lexer, public Preprocessor {
public:
	std::string currentName;
	std::vector<std::string> knownNames;
	std::vector<std::vector<size_t>> jumpInstruction;
	std::vector<int> statementType;
	std::vector<size_t> breakJump;
	void preprocess();
	bool nameExists(std::string name);
	int getToken(std::string str);
	void translate();
};
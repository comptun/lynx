#pragma once

#include "lexer.h"

#include <fstream>
#include <vector>
#include <string>

class Interpreter : public Lexer {
public:
	std::vector<size_t> jumpInstruction;
	int getToken(std::string str);
	void translate();
};
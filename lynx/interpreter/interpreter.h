#pragma once

#include "lexer.h"

#include <fstream>
#include <vector>
#include <string>

class Interpreter : public Lexer {
public:
	std::vector<std::vector<size_t>> jumpInstruction;
	std::vector<size_t> breakJump;
	int getToken(std::string str);
	void translate();
};
#pragma once

#include <fstream>
#include <vector>
#include <string>

class Lexer {
public:
	struct File {
		std::vector<std::string> type;
		std::vector<std::string> token;
	};
	File file;
	bool isWhitespace(char chr);
	void readCode(std::ifstream fileName);
};

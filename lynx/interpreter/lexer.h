#pragma once

#include <fstream>
#include <vector>
#include <string>

extern std::vector<std::string> tokenNames;
extern std::vector<std::string> tokenTypes;

enum tokenEnum {
	IF,
	WHILE_LOOP,
	EQUALS,
	PLUS_EQUALS,
	MINUS_EQUALS,
	DIVIDE_EQUALS,
	MULTIPLY_EQUALS,
	NAME,
	CONSTANT_VALUE,
	LBRACKET,
	RBRACKET,
	ENDIF,
	ENDWHILE,
};

class Lexer {
public:
	struct File {
		std::vector<std::string> type;
		std::vector<std::string> token;
		std::vector<int> indents;
	};
	File codeFile;
	bool isInteger(std::string num);
	void tokenize(std::string token);
	bool isWhitespace(char chr);
	void readCode(std::ifstream fileName);
};

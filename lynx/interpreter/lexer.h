#pragma once

#include <fstream>
#include <vector>
#include <string>

extern std::vector<std::string> tokenNames;
extern std::vector<std::string> tokenTypes;

enum tokenEnum {
	IF,
	WHILE_LOOP,
	FOR_LOOP,
	AND,
	FUNCTION,
	FUNCTION_END,
	EQUALS,
	PLUS_EQUALS,
	MINUS_EQUALS,
	DIVIDE_EQUALS,
	MULTIPLY_EQUALS,
	MOD_EQUALS,
	COLON,
	NAME,
	CONSTANT_VALUE,
	LBRACKET,
	RBRACKET,
	ENDIF,
	ENDWHILE,
	BREAK,
	LEFT_CURLY_BRACE,
	RIGHT_CURLY_BRACE,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
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

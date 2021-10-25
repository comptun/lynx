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
	OR,
	XOR,
	FUNCTION,
	FUNCTION_END,
	EQUALS,
	EQUAL_TO,
	GREATER_THAN_EQUAL_TO,
	LESS_THAN_EQUAL_TO,
	GREATER_THAN,
	LESS_THAN,
	NOT_EQUAL_TO,
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
	CONTINUE,
	LEFT_CURLY_BRACE,
	RIGHT_CURLY_BRACE,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	MODULUS,
	DEFINE,
	COMMA,
	PERIOD,
	JUMP_TO,
	ELSE,
	SWITCH,
	CASE,
	STRING,
	RETURN,
	AMPERSAND,
	SEMI_COLON,
	IMPORT,
	EXPONENT,
	EXPONENT_EQUALS,
};

class Lexer {
public:
	struct File {
		std::vector<std::string> type;
		std::vector<std::string> token;
		std::vector<int> indents;
	};
	File codeFile;
	std::string getImportType(std::string token);
	void removeBlankspace();
	bool isFloat(std::string num);
	bool isInteger(std::string num);
	bool special1Character(char character);
	bool special2Character(std::string characters);
	void tokenize(std::string token);
	void retokenize(std::string token, size_t pos);
	bool isWhitespace(char chr);
	void readCode(std::ifstream fileName);
};

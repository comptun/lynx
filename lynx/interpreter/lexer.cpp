#include "lexer.h"
#include <iostream>

std::vector<std::string> tokenNames = {
	"if",
	"while",
	"for",
	"and",
	"function",
	"endfunction",
	"=",
	"==",
	"+=",
	"-=",
	"/=",
	"*=",
	"%=",
	":",
	"NAME",
	"CONSTANT_VALUE",
	"(",
	")",
	"endif",
	"endwhile",
	"break",
	"{",
	"}",
	"+",
	"-",
	"*",
	"/",
	"define",
	",",
	".",
};

std::vector<std::string> tokenTypes = {
	"IF_STATEMENT",
	"WHILE_LOOP",
	"FOR_LOOP",
	"AND",
	"FUNCTION",
	"END_FUNCTION",
	"EQUALS",
	"EQUAL_TO",
	"PLUS_EQUALS",
	"MINUS_EQUALS",
	"DIVIDE_EQUALS",
	"MULTIPLY_EQUALS",
	"MOD_EQUALS",
	"COLON",
	"NAME",
	"CONSTANT_VALUE",
	"LBRACKET",
	"RBRACKET",
	"ENDIF",
	"ENDWHILE",
	"BREAK",
	"LEFT_CURLY_BRACE",
	"RIGHT_CURLY_BRACE",
	"PLUS",
	"MINUS",
	"MULTIPLY",
	"DIVIDE",
	"DEFINE",
	"COMMA",
	"PERIOD",
};

bool Lexer::isInteger(std::string num)
{
	for (size_t i = 0; i < num.size(); ++i) {
		if (!isdigit(num.at(i)))
			return false;
	}
	return true;
}

void Lexer::tokenize(std::string token)
{
	if (token != "" && token != " ") {
		codeFile.token.push_back(token);
		for (size_t i = 0; i < tokenNames.size(); ++i) {
			if (tokenNames.at(i) == token) {
				codeFile.type.push_back(tokenTypes.at(i));
				return;
			}
		}
		if (isInteger(token)) {
			codeFile.type.push_back("CONSTANT_VALUE");
			return;
		}
		codeFile.type.push_back("NAME");
	}
}

void Lexer::retokenize(std::string token, size_t pos)
{
	for (size_t i = 0; i < tokenNames.size(); ++i) {
		if (tokenNames.at(i) == token) {
			codeFile.type.at(pos) = tokenTypes.at(i);
			return;
		}
	}
	if (isInteger(token)) {
		codeFile.type.at(pos) = "CONSTANT_VALUE";
		return;
	}
	codeFile.type.at(pos) = "NAME";
}

bool Lexer::isWhitespace(char chr)
{
	return chr == char(32) or chr == char(13);
}

bool Lexer::special1Character(char character)
{
	return character == '{' or character == '}' or character == '(' or character == ')' or character == ',';
}

bool Lexer::special2Character(char character, char character2)
{
	switch (character) {
	case '=':
		switch (character2) {
		case '=':
			return true;
		}
	case '!':
		switch (character2) {
		case '=':
			return true;
		}
	case '>':
		switch (character2) {
		case '=':
			return true;
		}
	case '<':
		switch (character2) {
		case '=':
			return true;
		}
	}
	return false;
}

void Lexer::removeBlankspace()
{
	size_t pos = 0;
	for (auto i = codeFile.token.begin(); i != codeFile.token.end(); ++i, ++pos) {
		if (codeFile.token.at(pos) == "" or codeFile.token.at(pos) == " ") {
			codeFile.token.erase(i);
			codeFile.type.erase(i);
		}
	}
	for (size_t i = 0; i < codeFile.token.size(); ++i) {
		if (codeFile.token.at(i) == "" or codeFile.token.at(i) == " ")
			removeBlankspace();
	}
}

void Lexer::readCode(std::ifstream fileName)
{
	std::string line;
	std::string lineContent;
	while (getline(fileName, line)) {
		int tabNum = 0;
		for (size_t i = 0; i < line.length(); ++i) {
			if (line.at(i) == '	') {
				++tabNum;
				continue;
			}
			/*if (special2Character(line.at(i), line.at(i + 1))) {
				tokenize(lineContent);
				lineContent.clear();
				tokenize(std::string(1, line.at(i)) += std::string(1, line.at(i + 1)));
				++i;
				continue;
			}*/
			if (special1Character(line.at(i))) {
				tokenize(lineContent);
				lineContent.clear();
				tokenize(std::string(1, line.at(i)));
				continue;
			}
			if (line.at(i) == '"') {
				for (++i; line.at(i) != '"'; ++i) {
					lineContent += line.at(i);
					codeFile.indents.push_back(tabNum);
					tokenize(lineContent);
					lineContent.clear();
				}
				break;
			}
			if (i == line.length() - 1) {
				lineContent += line.at(i);
				codeFile.indents.push_back(tabNum);
				tokenize(lineContent);
				lineContent.clear();
				continue;
			}
			if (isWhitespace(line.at(i))) {
				codeFile.indents.push_back(tabNum);
				tokenize(lineContent);
				lineContent.clear();
				continue;
			}
			lineContent += line.at(i);
		}
	}
	fileName.close();
	//removeBlankspace();
}
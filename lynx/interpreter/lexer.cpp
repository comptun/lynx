﻿#include "lexer.h"
#include <iostream>

std::vector<std::string> tokenNames = {
	"if",
	"while",
	"for",
	"and",
	"or",
	"xor",
	"function",
	"endfunction",
	"=",
	"==",
	">=",
	"<=",
	">",
	"<",
	"!=",
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
	"continue",
	"{",
	"}",
	"+",
	"-",
	"*",
	"/",
	"%",
	"define",
	"import",
	",",
	".",
	"jump",
	"else",
	"switch",
	"case",
	"STRING",
	"return",
	"&",
	";",
};

std::vector<std::string> tokenTypes = {
	"IF_STATEMENT",
	"WHILE_LOOP",
	"FOR_LOOP",
	"AND",
	"OR",
	"XOR",
	"FUNCTION",
	"END_FUNCTION",
	"EQUALS",
	"EQUAL_TO",
	"HIGHER_THAN_EQUAL_TO",
	"LESS_THAN_EQUAL_TO",
	"GREATER_THAN",
	"LESS_THAN",
	"NOT_EQUAL_TO",
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
	"CONTINUE",
	"LEFT_CURLY_BRACE",
	"RIGHT_CURLY_BRACE",
	"PLUS",
	"MINUS",
	"MULTIPLY",
	"DIVIDE",
	"MODULUS",
	"DEFINE",
	"IMPORT",
	"COMMA",
	"PERIOD",
	"JUMP",
	"ELSE",
	"SWITCH",
	"CASE",
	"STRING",
	"RETURN",
	"AMPERSAND",
	"SEMI_COLON",
};

bool Lexer::isFloat(std::string num)
{
	unsigned int decimalCount = 0;
	for (size_t i = 0; i < num.size(); ++i) {
		if (i == 0 and num.at(0) == '-')
			continue;
		if (!isdigit(num.at(i)) and num.at(i) != '.' or (num.at(i) == '.' and decimalCount > 0))
			return false;
	}
	return true;
}

bool Lexer::isInteger(std::string num)
{
	for (size_t i = 0; i < num.size(); ++i) {
		if (i == 0 and num.at(0) == '-')
			continue;
		if (!isdigit(num.at(i)))
			return false;
	}
	return true;
}

void Lexer::tokenize(std::string token)
{
	if (token != "" && token != " ") {
		if (token == "(" 
			and (codeFile.type.back() != "NAME" 
			and codeFile.type.back() != "IF_STATEMENT"
			and codeFile.type.back() != "WHILE_LOOP")) {
			codeFile.token.push_back("parenthesis");
			codeFile.type.push_back("NAME");
			codeFile.token.push_back("(");
			codeFile.type.push_back("LBRACKET");
			return;
		}
		if (token == "__EMPTY_STRING__") {
			codeFile.token.push_back(" ");
			codeFile.type.push_back("CONSTANT_VALUE");
			return;
		}
		codeFile.token.push_back(token);
		for (size_t i = 0; i < tokenNames.size(); ++i) {
			if (tokenNames.at(i) == token) {
				codeFile.type.push_back(tokenTypes.at(i));
				return;
			}
		}
		if (token.at(0) == '\'') {
			codeFile.token.back() = std::to_string(static_cast<int>(codeFile.token.back().at(1)));
			codeFile.type.push_back("CONSTANT_VALUE");
			return;
		}
		if (token.at(0) == '"') {
			size_t offset = codeFile.token.back().size() - 2;
			codeFile.token.back().erase(codeFile.token.back().begin());
			codeFile.token.back().erase(offset);
			codeFile.type.push_back("CONSTANT_VALUE");
			return;
		}
		if (isInteger(token) or isFloat(token)) {
			codeFile.type.push_back("CONSTANT_VALUE");
			return;
		}
		codeFile.type.push_back("NAME");
	}
}

void Lexer::retokenize(std::string token, size_t pos)
{
	/*for (size_t i = 0; i < tokenNames.size(); ++i) {
		if (tokenNames.at(i) == token) {
			codeFile.type.at(pos) = tokenTypes.at(i);
			return;
		}
	}
	if (isInteger(token)) {
		codeFile.type.at(pos) = "CONSTANT_VALUE";
		return;
	}
	codeFile.type.at(pos) = "NAME";*/


	if (token != "" && token != " ") {
		if (token == "("
			and (codeFile.type.at(pos - 1) != "NAME"
				and codeFile.type.at(pos - 1) != "IF_STATEMENT"
				and codeFile.type.at(pos - 1) != "WHILE_LOOP")) {
			codeFile.token.at(pos) = "parenthesis";
			codeFile.type.at(pos) = "NAME";
			codeFile.token.at(pos) = "(";
			codeFile.type.at(pos) = "LBRACKET";
			return;
		}
		if (token == "__EMPTY_STRING__") {
			codeFile.token.at(pos) = " ";
			codeFile.type.at(pos) = "CONSTANT_VALUE";
			return;
		}
		codeFile.token.at(pos) = token;
		for (size_t i = 0; i < tokenNames.size(); ++i) {
			if (tokenNames.at(i) == token) {
				codeFile.type.at(pos) = tokenTypes.at(i);
				return;
			}
		}
		if (token.at(0) == '\'') {
			codeFile.token.at(pos - 1) = std::to_string(static_cast<int>(codeFile.token.at(pos - 1).at(1)));
			codeFile.type.at(pos) = "CONSTANT_VALUE";
			return;
		}
		if (token.at(0) == '"') {
			size_t offset = codeFile.token.at(pos - 1).size() - 2;
			codeFile.token.at(pos - 1).erase(codeFile.token.at(pos - 1).begin());
			codeFile.token.at(pos - 1).erase(offset);
			codeFile.type.at(pos) = "CONSTANT_VALUE";
			return;
		}
		if (isInteger(token) or isFloat(token)) {
			codeFile.type.at(pos) = "CONSTANT_VALUE";
			return;
		}
		codeFile.type.at(pos) = "NAME";
	}
}

bool Lexer::isWhitespace(char character)
{
	return character == char(32) 
		or character == char(13);
}

bool Lexer::special1Character(char character)
{
	return character == '{' 
		or character == '}' 
		or character == '(' 
		or character == ')' 
		or character == ',' 
		or character == ':' 
		or character == '&'
		or character == ';'
		or character == '='
		or character == '+'
		or character == '-'
		or character == '*'
		or character == '/'
		or character == '%'
		or character == '>'
		or character == '<'
		or character == '['
		or character == ']';
}

bool Lexer::special2Character(std::string characters)
{
	return characters == "=="
		or characters == "!="
		or characters == "<="
		or characters == ">="
		or characters == "&&"
		or characters == "||"
		or characters == "++"
		or characters == "--";
}

void Lexer::removeBlankspace()
{
	/*size_t pos = 0;
	for (auto i = codeFile.token.begin(); i != codeFile.token.end(); ++i, ++pos) {
		if (codeFile.token.at(pos) == "" or codeFile.token.at(pos) == " ") {
			codeFile.token.erase(i);
			codeFile.type.erase(i);
		}
	}
	for (size_t i = 0; i < codeFile.token.size(); ++i) {
		if (codeFile.token.at(i) == "" or codeFile.token.at(i) == " ")
			removeBlankspace();
	}*/
}

void Lexer::readCode(std::ifstream fileName)
{
	bool isInString = false;
	std::string line;
	std::string lineContent;
	while (getline(fileName, line)) {
		int tabNum = 0;
		for (size_t i = 0; i < line.length(); ++i) {
			if (line.at(i) == '"' or line.at(i) == '\'') {
				if (line.at(i + 1) == '"') {
					tokenize("__EMPTY_STRING__");
					i += 1;
					continue;
				}
				if (isInString)
					isInString = false;
				else
					isInString = true;
			}
			if (!isInString) {
				if (line.at(i) == '/' and line.at(i + 1) == '/') {
					break;
				}
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
				if (i < line.length() - 1) {
					if (special2Character(std::string(1, line.at(i)) + line.at(i + 1))) {
						tokenize(lineContent);
						lineContent.clear();
						tokenize(std::string(1, line.at(i)) + line.at(i + 1));
						i += 1;
						continue;
					}
				}
				if (special1Character(line.at(i))) {
					tokenize(lineContent);
					lineContent.clear();
					tokenize(std::string(1, line.at(i)));
					continue;
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
			}
			lineContent += line.at(i);
		}
	}
	fileName.close();
	//removeBlankspace();
}
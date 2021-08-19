#include "lexer.h"

std::vector<std::string> tokenNames = {
	"if",
	"while",
	"=",
	"+=",
	"-=",
	"/=",
	"*=",
	"NAME",
	"CONSTANT_VALUE",
	"(",
	")",
	"endif",
	"endwhile",
};

std::vector<std::string> tokenTypes = {
	"IF_STATEMENT",
	"WHILE_LOOP",
	"EQUALS",
	"PLUS_EQUALS",
	"MINUS_EQUALS",
	"DIVIDE_EQUALS",
	"MULTIPLY_EQUALS",
	"NAME",
	"CONSTANT_VALUE",
	"LBRACKET",
	"RBRACKET",
	"ENDIF",
	"ENDWHILE",
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

bool Lexer::isWhitespace(char chr)
{
	return chr == char(32) or chr == char(13);
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
}
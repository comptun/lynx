#include "lexer.h"
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
	"import",
	"^",
	"^=",
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
	"IMPORT",
	"EXPONENT",
	"EXPONENT_EQUALS",
};

bool Lexer::beginsWith(std::string str, std::string begin)
{
	if (str >= begin) {
		for (size_t i = 0; i < begin.size(); ++i) {
			if (begin.at(i) != str.at(i))
				return false;
		}
		return true;
	}
	return false;
}

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

void Lexer::m_tokenization(std::string token, std::string type)
{
	codeFile.token.push_back(token);
	codeFile.type.push_back(type);
}

void Lexer::tokenize(std::string token) 
{
	if (token != "" && token != " ") {
		if (token == "[") {
			if (codeFile.type.back() == "RBRACKET") {
				codeFile.token.pop_back();
				codeFile.type.pop_back();
				codeFile.token.push_back(",");
				codeFile.type.push_back("COMMA");
				return;
			}
			codeFile.token.push_back("array");
			codeFile.type.push_back("NAME");
			codeFile.token.push_back("(");
			codeFile.type.push_back("LBRACKET");
			return;
		}
		if (token == "]") {
			codeFile.token.push_back(")");
			codeFile.type.push_back("RBRACKET");
			return;
		}
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

std::string Lexer::getImportType(std::string token)
{
	if (token != "" && token != " ") {
		if (token == "("
			and (codeFile.type.back() != "NAME"
				and codeFile.type.back() != "IF_STATEMENT"
				and codeFile.type.back() != "WHILE_LOOP")) {
			return "LBRACKET";
		}
		if (token == "__EMPTY_STRING__") {
			return "CONSTANT_VALUE";
		}
		codeFile.token.push_back(token);
		for (size_t i = 0; i < tokenNames.size(); ++i) {
			if (tokenNames.at(i) == token) {
				return tokenTypes.at(i);
			}
		}
		if (token.at(0) == '\'') {
			return "CONSTANT_VALUE";
		}
		if (token.at(0) == '"') {
			size_t offset = codeFile.token.back().size() - 2;
			return "CONSTANT_VALUE";
		}
		if (isInteger(token) or isFloat(token)) {
			return "CONSTANT_VALUE";
		}
		return "NAME";
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
		if (token == "[") {
			codeFile.token.at(pos) = "array";
			codeFile.type.at(pos) = "NAME";
			codeFile.token.at(pos) = "(";
			codeFile.type.at(pos) = "LBRACKET";
			return;
		}
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
		or character == ']'
		or character == '^';
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
		or characters == "--"
		or characters == "+="
		or characters == "-="
		or characters == "*="
		or characters == "/="
		or characters == "%="
		or characters == "^=";
}

bool Lexer::isInVector(std::string str, std::vector<std::string> vec)
{
	for (size_t i = 0; i < vec.size(); ++i) {
		if (vec.at(i) == str)
			return true;
	}
	return false;
}

void Lexer::readCode(std::ifstream fileName)
{
	bool isInString = false;
	std::string line;
	std::string lineContent;
	while (getline(fileName, line)) {
		int tabNum = 0;
		if (beginsWith(line, "import")) {
			std::string fileName2;
			for (size_t i = 8; line.at(i) != '"'; ++i) {
				fileName2 += line.at(i);
			}
			readCode(std::ifstream(fileName2));
			readFiles.push_back(fileName2);
			continue;
		}
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
				if (i < line.length() - 1) {
					if (special2Character(std::string(1, line.at(i)) + line.at(i + 1))) {
						tokenize(lineContent);
						lineContent.clear();
						std::string character2 = std::string(1, line.at(i)) + line.at(i + 1);
						if (character2 == "+="
							or character2 == "-="
							or character2 == "/="
							or character2 == "*="
							or character2 == "%="
							or character2 == "++"
							or character2 == "^="
							or character2 == "--") {
							tokenize("=");
							tokenize(codeFile.token.at(codeFile.token.size() - 2));
							tokenize(std::string(1, line.at(i)));
							if (character2 == "++" or character2 == "--")
								tokenize("1");
						}
						else {
							tokenize(character2);
						}
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
	/*for (size_t j = 0; j < codeFile.token.size(); ++j) {
		if (codeFile.type.at(j) == "IMPORT") {
			lineContent.clear();
			line.clear();
			std::ifstream importFilename(codeFile.token.at(j + 1));
			while (getline(importFilename, line)) {
				for (size_t i = 0; i < line.length(); ++i) {
					if (line.at(i) == '"' or line.at(i) == '\'') {
						if (line.at(i + 1) == '"') {
							codeFile.token.insert(codeFile.token.begin() + j + 2, lineContent);
							codeFile.type.insert(codeFile.type.begin() + j + 2, getImportType(lineContent));
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
						if (i < line.length() - 1) {
							if (special2Character(std::string(1, line.at(i)) + line.at(i + 1))) {
								tokenize(lineContent);
								lineContent.clear();
								codeFile.token.insert(codeFile.token.begin() + j + 2, std::string(1, line.at(i)) + line.at(i + 1));
								codeFile.type.insert(codeFile.type.begin() + j + 2, getImportType(std::string(1, line.at(i)) + line.at(i + 1)));
								i += 1;
								continue;
							}
						}
						if (special1Character(line.at(i))) {
							tokenize(lineContent);
							lineContent.clear();
							codeFile.token.insert(codeFile.token.begin() + j + 2, std::string(1, line.at(i)));
							codeFile.type.insert(codeFile.type.begin() + j + 2, getImportType(std::string(1, line.at(i))));
							continue;
						}
						if (i == line.length() - 1) {
							lineContent += line.at(i);
							codeFile.token.insert(codeFile.token.begin() + j + 2, lineContent);
							codeFile.type.insert(codeFile.type.begin() + j + 2, getImportType(lineContent));
							lineContent.clear();
							continue;
						}
						if (isWhitespace(line.at(i))) {
							codeFile.token.insert(codeFile.token.begin() + j + 2, lineContent);
							codeFile.type.insert(codeFile.type.begin() + j + 2, getImportType(lineContent));
							lineContent.clear();
							continue;
						}
					}
					lineContent += line.at(i);
				}
			}
			importFilename.close();
		}
	}*/
	//removeBlankspace();
}
#include "lexer.h"

bool Lexer::isWhitespace(char chr)
{
	return chr == char(32) or chr == char(13);
}

void Lexer::readCode(std::ifstream fileName)
{
	std::string line;
	std::string lineContent;
	while (getline(fileName, line)) {
		for (size_t i = 0; i < line.length(); ++i) {
			if (line.at(i) == '"') {
				for (++i; line.at(i) != '"'; ++i) {
					lineContent += line.at(i);
					file.token.push_back(lineContent);
					lineContent.clear();
				}
				break;
			}
			if (i == line.length() - 1) {
				lineContent += line.at(i);
				file.token.push_back(lineContent);
				lineContent.clear();
				continue;
			}
			if (isWhitespace(line.at(i))) {
				file.token.push_back(lineContent);
				lineContent.clear();
				continue;
			}
			lineContent += line.at(i);
		}
	}
}
#include "byteinterpreter.h"
#include "lynx.h"
#include "opcodes.h"

#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <string>
#include <iostream>

bool ByteInterpreter::isInteger(std::string num)
{
	for (size_t i = 0; i < num.size(); ++i) {
		if (!isdigit(num.at(i)))
			return false;
	}
	return true;
}

bool ByteInterpreter::isFloat(std::string num)
{
	unsigned int decimalCount = 0;
	for (size_t i = 0; i < num.size(); ++i) {
		if (!isdigit(num.at(i)) and num.at(i) != '.' or (num.at(i) == '.' and decimalCount > 0))
			return false;
	}
	return true;
}

bool ByteInterpreter::isWhitespace(char chr)
{
	return chr == char(32) or chr == char(13);
}

void ByteInterpreter::readFile(std::ifstream fileName)
{
	std::string line;
	std::string lineContent;
	while (getline(fileName, line)) {
		for (size_t i = 0; i < line.length(); ++i) {
			if (i == line.length() - 1) {
				lineContent += line.at(i);
				file.push_back(lineContent);
				lineContent.clear();
				continue;
			}
			if (isWhitespace(line.at(i))) {
				file.push_back(lineContent);
				lineContent.clear();
				continue;
			}
			lineContent += line.at(i);
		}
	}
}

void ByteInterpreter::interpret()
{
	for (size_t instruction = 0; instruction < file.size(); instruction += 2) {
		switch (getOpcode(file.at(instruction))) {
		case LOAD_CONST:
			if (isInteger(file.at(instruction + 1)))
				stack.push_back(std::stoi(file.at(instruction + 1)));
			else if (isFloat(file.at(instruction + 1)))
				stack.push_back(std::stod(file.at(instruction + 1)));
			else
				stack.push_back(file.at(instruction + 1));
			break;
		case LOAD_NAME:

		}
	}
}
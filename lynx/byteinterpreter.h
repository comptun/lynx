#pragma once

#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <string>
#include <iostream>

class ByteInterpreter {
public:

	unsigned int getOpcode(std::string code);

	std::vector<std::string> file;

	std::vector<std::string> names;
	std::vector<unsigned int> namesReference;

	std::vector<std::variant<int, double, std::string>> stack;

	bool isWhitespace(char chr);
	bool isInteger(std::string num);
	bool isFloat(std::string num);

	void readFile(std::ifstream fileName);
	void interpret();
};

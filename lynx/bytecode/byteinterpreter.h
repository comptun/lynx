#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <variant>

#include "pcfunctions.h"

class BCfile {
public:
	std::vector<std::string> file;
};

extern BCfile bcfile;

class ByteInterpreter : public PCFunctions {
public:

	size_t instruction;
	std::vector<size_t> secondaryInstruction;

	bool conditional;

	unsigned int getOpcode(std::string code);
	size_t getNameReference(std::string name);

	struct Names {
		std::vector<std::string> identifier;
		std::vector<unsigned int> reference;
	};

	Names names;

	std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>> returnedValue;
	
	std::vector<std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>>> stack;
	//std::vector<int> stack;
	std::vector<std::vector<std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>>>> paramStack;
	std::vector<std::vector<int>> vargParamStack;

	void executePCF(std::string funcName);

	bool doesNameExist(std::string name);
	bool isWhitespace(char chr);
	bool isInteger(std::string num);
	bool isFloat(std::string num);

	void readFile(std::ifstream fileName);
	void interpret();
};
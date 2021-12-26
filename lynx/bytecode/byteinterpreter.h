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

	typedef std::vector<std::variant<long long int, long double, std::string>> t_list;
	typedef std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>> t_returnvalue;
	typedef std::vector<std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>>> t_stack;
	typedef std::vector<std::vector<std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>>>> t_paramstack;

	t_returnvalue returnedValue;
	t_stack stack;
	t_stack listStack;
	t_paramstack paramStack;
	std::vector<std::vector<int>> vargParamStack;

	long long int getListPointer(std::string listPointer);
	bool beginsWith(std::string str, std::string begin);
	void newList(long long int listPtr);
	void executePCF(std::string funcName);

	bool doesNameExist(std::string name);
	bool isWhitespace(char chr);
	bool isInteger(std::string num);
	bool isFloat(std::string num);

	void readFile(std::ifstream fileName);
	void interpret();
};
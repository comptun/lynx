#include "byteinterpreter.h"
#include "../lynx.h"
#include "opcodes.h"

#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

BCfile bcfile;

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
	unsigned long long int decimalCount = 0;
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
			if (line.at(i) == '"') {
				for (++i; line.at(i) != '"'; ++i) {
					lineContent += line.at(i);
					bcfile.file.push_back(lineContent);
					lineContent.clear();
				}
				break;
			}
			if (i == line.length() - 1) {
				lineContent += line.at(i);
				bcfile.file.push_back(lineContent);
				lineContent.clear();
				continue;
			}
			if (isWhitespace(line.at(i))) {
				bcfile.file.push_back(lineContent);
				lineContent.clear();
				continue;
			}
			lineContent += line.at(i);
		}
	}
}

bool ByteInterpreter::doesNameExist(std::string name)
{
	for (size_t i = 0; i < names.identifier.size(); ++i) {
		if (names.identifier.at(i) == name)
			return true;
	}
	return false;
}

size_t ByteInterpreter::getNameReference(std::string name)
{
	for (size_t i = 0; i < names.identifier.size(); ++i) {
		if (names.identifier.at(i) == name)
			return i;
	}
	return 404; // 404 not found
}

void ByteInterpreter::interpret()
{
	for (instruction = 0; instruction < bcfile.file.size(); instruction += 2) {
		switch (getOpcode(bcfile.file.at(instruction))) {
		case LOAD_CONST:
			if (isInteger(bcfile.file.at(instruction + 1)))
				stack.push_back(std::stoi(bcfile.file.at(instruction + 1)));
			else if (isFloat(bcfile.file.at(instruction + 1)))
				stack.push_back(std::stod(bcfile.file.at(instruction + 1)));
			else
			 	stack.push_back(bcfile.file.at(instruction + 1));
			break;
		case LOAD_REF:
			stack.push_back(static_cast<long long int>(getNameReference(bcfile.file.at(instruction + 1))));
			break;
		case STORE_REF:
			//names.reference.at(getNameReference(bcfile.file.at(instruction + 1))) = stack.back();
			break;
		case LOAD_NAME:
			stack.push_back(stack.at(getNameReference(bcfile.file.at(instruction + 1))));
			break;
		case STORE_NAME:
			if (doesNameExist(bcfile.file.at(instruction + 1)) == false) {
				names.identifier.push_back(bcfile.file.at(instruction + 1));
				names.reference.push_back(stack.size() - 1);
			}
			else {
				stack.at(getNameReference(bcfile.file.at(instruction + 1))) = stack.back();
			}
			break;
		case COMPARE:
			conditional = false;
			if (bcfile.file.at(instruction + 1) == "==") {
				if (stack.at(stack.size() - 2) == stack.back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == "!=") {
				if (stack.at(stack.size() - 2) != stack.back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == ">=") {
				if (stack.at(stack.size() - 2) >= stack.back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == "<=") {
				if (stack.at(stack.size() - 2) <= stack.back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == ">") {
				if (stack.at(stack.size() - 2) > stack.back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == "<") {
				if (stack.at(stack.size() - 2) < stack.back())
					conditional = true;
			}
			stack.pop_back();
			stack.pop_back();
			break;
		case JUMP_IF_FALSE:
			if (conditional == false)
				instruction = std::stoi(bcfile.file.at(instruction + 1));
			break;
		case JUMP_IF_TRUE:
			if (conditional == true)
				instruction = std::stoi(bcfile.file.at(instruction + 1));
			break;
		case JUMP:
			if (isInteger(bcfile.file.at(instruction + 1)))
				instruction = std::stoi(bcfile.file.at(instruction + 1));
			else
				instruction = std::get<long long int>(stack.at(names.reference.at(getNameReference(bcfile.file.at(instruction + 1)))));
			break;
		case CALL:
			if (doesNameExist(bcfile.file.at(instruction + 1)) == false) {
				executePCF(bcfile.file.at(instruction + 1));
			}
			if (doesNameExist(bcfile.file.at(instruction + 1)) == true) {
				secondaryInstruction.push_back(instruction);
				instruction = std::get<long long int>(stack.at(getNameReference(bcfile.file.at(instruction + 1))));
				/*for (size_t e = 0; e < bcfile.file.size(); e += 2) {
					if (bcfile.file.at(e) == "START_FUNCTION" and bcfile.file.at(e + 1) == bcfile.file.at(instruction + 1)) {
						instruction = e;
					}
				}*/
			}
			break;
		case ADD:
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) + std::get<long long int>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) + static_cast<long double>(std::get<long long int>(stack.at(stack.size() - 1)));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = (std::get<long double>(stack.at(stack.size() - 1))) + static_cast<long double>(std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) + std::get<long double>(stack.at(stack.size() - 1));

			if (std::holds_alternative<std::string>(stack.at(stack.size() - 1)))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<std::string>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) + std::get<std::string>(stack.at(stack.size() - 1));
			
			break;
		case SUB:
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) - std::get<long long int>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) - static_cast<long double>(std::get<long long int>(stack.at(stack.size() - 1)));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(stack.size() - 1)) - static_cast<long double>(std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) - std::get<long double>(stack.at(stack.size() - 1));

			break;
		case MUL:
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) * std::get<long long int>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) * static_cast<long double>(std::get<long long int>(stack.at(stack.size() - 1)));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = (std::get<long double>(stack.at(stack.size() - 1))) * static_cast<long double>(std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) * std::get<long double>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<std::string>(stack.at(std::get<long long int>(stack.at(stack.size() - 2))))) {
				std::string str = std::get<std::string>(stack.at(std::get<long long int>(stack.at(stack.size() - 2))));
				std::string str2;
				for (size_t n = 0; n < std::get<long long int>(stack.at(stack.size() - 1)); ++n) {
					str2 = str2 + str;
				}
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = str2;
			}

			break;
		case DIV:
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) / std::get<long long int>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) / std::get<long long int>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = (std::get<long double>(stack.at(stack.size() - 1))) / std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2))));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) / std::get<long double>(stack.at(stack.size() - 1));

			break;
		case MOD:
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) % std::get<long long int>(stack.at(stack.size() - 1));
			break;
		case START_FUNCTION:
			names.identifier.push_back(bcfile.file.at(instruction + 1));
			names.reference.push_back(stack.size());
			stack.push_back(static_cast<long long int>(instruction));
			for (; bcfile.file.at(instruction) != "END_FUNCTION"; instruction += 2);
			break;
		case END_FUNCTION:
			instruction = secondaryInstruction.back();
			secondaryInstruction.pop_back();
			break;
		case RETURN:
			instruction = secondaryInstruction.back();
			secondaryInstruction.pop_back();
			break;
		case LOAD_ARRAY: {
			
			break;
		}
		case LOAD_ARRAY_REF:
			
			break;
		case STORE_ARRAY: {
			
			break;
		}
		case PUSH_ARRAY: {
			
			break;
		}
		case POP_BACK:
			stack.pop_back();
			break;
		case LOAD_PARAM:
			stack.push_back(paramStack.back().at(0));
			paramStack.back().erase(paramStack.back().begin());
			break;
		case STORE_PARAM:
			paramStack.back().push_back(stack.back());
			break;
		case LOAD_VAR_PARAM:
			stack.push_back(vargParamStack.back().at(0));
			vargParamStack.back().erase(vargParamStack.back().begin());
			break;
		case STORE_VAR_PARAM:

			break;
		case DELETE:
			stack.at(names.reference.at(getNameReference(bcfile.file.at(instruction + 1)))) = 0;
			names.reference.at(getNameReference(bcfile.file.at(instruction + 1))) = getNameReference(bcfile.file.at(instruction + 1));
			names.identifier.at(getNameReference(bcfile.file.at(instruction + 1))) = getNameReference(bcfile.file.at(instruction + 1));
			break;
		case POP_NAME:
			/*stack.erase(stack.begin() + names.reference.back());
			names.reference.pop_back();
			names.identifier.pop_back();*/
			break;
		case SWITCH:

			break;
		case LOAD_BACK_REF:
			stack.push_back(static_cast<long long int>(stack.size() - 1));
			break;
		case RETURN_VALUE:
			instruction = secondaryInstruction.back();
			secondaryInstruction.pop_back();
			returnedValue = stack.back();
			break;
		case LOAD_RETURN_VALUE:
			stack.push_back(returnedValue);
			break;
		case NEW_PARAM_STACK: {
			std::vector<std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>>> newParamStack;
			paramStack.push_back(newParamStack);
			break;
		}
		case POP_PARAM_STACK:
			paramStack.pop_back();
			break;
		}
	}
}
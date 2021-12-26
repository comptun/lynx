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
		if (bcfile.file.at(instruction) == "LOAD_CONST") {
			if (isInteger(bcfile.file.at(instruction + 1)))
				stack.push_back(std::stoll(bcfile.file.at(instruction + 1)));
			else if (isFloat(bcfile.file.at(instruction + 1)))
				stack.push_back(std::stold(bcfile.file.at(instruction + 1)));
			else
				stack.push_back(bcfile.file.at(instruction + 1));
		}
		else if (bcfile.file.at(instruction) == "LOAD_REF") {
			stack.push_back(static_cast<long long int>(getNameReference(bcfile.file.at(instruction + 1))));
		}
		else if (bcfile.file.at(instruction) == "STORE_REF") {
			//names.reference.at(getNameReference(bcfile.file.at(instruction + 1))) = stack.back();
		}
		else if (bcfile.file.at(instruction) == "LOAD_NAME") {
			stack.push_back(stack.at(getNameReference(bcfile.file.at(instruction + 1))));
		}
		else if (bcfile.file.at(instruction) == "STORE_NAME") {
			if (doesNameExist(bcfile.file.at(instruction + 1)) == false) {
				names.identifier.push_back(bcfile.file.at(instruction + 1));
				names.reference.push_back(stack.size() - 1);
			}
			else {
				stack.at(getNameReference(bcfile.file.at(instruction + 1))) = stack.back();
			}
		}
		else if (bcfile.file.at(instruction) == "COMPARE") {
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
		}
		else if (bcfile.file.at(instruction) == "JUMP_IF_FALSE") {
			if (conditional == false)
				instruction = std::stoi(bcfile.file.at(instruction + 1));
		}
		else if (bcfile.file.at(instruction) == "JUMP_IF_TRUE") {
			if (conditional == true)
				instruction = std::stoi(bcfile.file.at(instruction + 1));
		}
		else if (bcfile.file.at(instruction) == "JUMP") {
			if (isInteger(bcfile.file.at(instruction + 1)))
				instruction = std::stoi(bcfile.file.at(instruction + 1));
			else
				instruction = std::get<long long int>(stack.at(names.reference.at(getNameReference(bcfile.file.at(instruction + 1)))));
		}
		else if (bcfile.file.at(instruction) == "CALL") {
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
		}
		else if (bcfile.file.at(instruction) == "ADD") {
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

		}
		else if (bcfile.file.at(instruction) == "SUB") {
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) - std::get<long long int>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) - static_cast<long double>(std::get<long long int>(stack.at(stack.size() - 1)));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(stack.size() - 1)) - static_cast<long double>(std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) - std::get<long double>(stack.at(stack.size() - 1));

		}
		else if (bcfile.file.at(instruction) == "MUL") {
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

		}
		else if (bcfile.file.at(instruction) == "DIV") {
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) / std::get<long long int>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) / std::get<long long int>(stack.at(stack.size() - 1));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = (std::get<long double>(stack.at(stack.size() - 1))) / std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2))));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) / std::get<long double>(stack.at(stack.size() - 1));

		}
		else if (bcfile.file.at(instruction) == "MOD") {
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))) % std::get<long long int>(stack.at(stack.size() - 1));
		}
		else if (bcfile.file.at(instruction) == "EXP") {
			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = pow(std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))), std::get<long long int>(stack.at(stack.size() - 1)));

			if (std::holds_alternative<long long int>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = pow(std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))), std::get<long long int>(stack.at(stack.size() - 1)));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = pow(std::get<long double>(stack.at(stack.size() - 1)), std::get<long long int>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))));

			if (std::holds_alternative<long double>(stack.at(stack.size() - 1)) and std::holds_alternative<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))))
				stack.at(std::get<long long int>(stack.at(stack.size() - 2))) = pow(std::get<long double>(stack.at(std::get<long long int>(stack.at(stack.size() - 2)))), std::get<long double>(stack.at(stack.size() - 1)));

		}
		else if (bcfile.file.at(instruction) == "START_FUNCTION") {
			names.identifier.push_back(bcfile.file.at(instruction + 1));
			names.reference.push_back(stack.size());
			stack.push_back(static_cast<long long int>(instruction));
			for (; bcfile.file.at(instruction) != "END_FUNCTION"; instruction += 2);
		}
		else if (bcfile.file.at(instruction) == "END_FUNCTION") {
			instruction = secondaryInstruction.back();
			secondaryInstruction.pop_back();
		}
		else if (bcfile.file.at(instruction) == "RETURN") {
			instruction = secondaryInstruction.back();
			secondaryInstruction.pop_back();
		}
		else if (bcfile.file.at(instruction) == "POP_BACK") {
			stack.pop_back();
		}
		else if (bcfile.file.at(instruction) == "LOAD_PARAM") {
			stack.push_back(paramStack.back().at(0));
			paramStack.back().erase(paramStack.back().begin());
		}
		else if (bcfile.file.at(instruction) == "STORE_PARAM") {
			paramStack.back().push_back(stack.back());
		}
		else if (bcfile.file.at(instruction) == "DELETE") {
			stack.at(names.reference.at(getNameReference(bcfile.file.at(instruction + 1)))) = 0;
			names.reference.at(getNameReference(bcfile.file.at(instruction + 1))) = getNameReference(bcfile.file.at(instruction + 1));
			names.identifier.at(getNameReference(bcfile.file.at(instruction + 1))) = getNameReference(bcfile.file.at(instruction + 1));
		}
		else if (bcfile.file.at(instruction) == "POP_NAME") {
			/*stack.erase(stack.begin() + names.reference.back());
			names.reference.pop_back();
			names.identifier.pop_back();*/
		}
		else if (bcfile.file.at(instruction) == "LOAD_BACK_REF") {
			stack.push_back(static_cast<long long int>(stack.size() - 1));
		}
		else if (bcfile.file.at(instruction) == "RETURN_VALUE") {
			instruction = secondaryInstruction.back();
			secondaryInstruction.pop_back();
			returnedValue = stack.back();
		}
		else if (bcfile.file.at(instruction) == "LOAD_RETURN_VALUE") {
			stack.push_back(returnedValue);
		}
		else if (bcfile.file.at(instruction) == "NEW_PARAM_STACK") {
			t_stack newParamStack;
			paramStack.push_back(newParamStack);
		}
		else if (bcfile.file.at(instruction) == "POP_PARAM_STACK") {
			paramStack.pop_back();
		}
	}
}


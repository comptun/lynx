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

bool ByteInterpreter::doesNameExist(std::string name)
{
	for (size_t i = 0; i < names.back().identifier.size(); ++i) {
		if (names.back().identifier.at(i) == name)
			return true;
	}
	return false;
}

size_t ByteInterpreter::getNameReference(std::string name)
{
	for (size_t i = 0; i < names.back().identifier.size(); ++i) {
		if (names.back().identifier.at(i) == name)
			return i;
	}
	return -1; // 404 not found
}

void ByteInterpreter::interpret()
{
	t_stack initStack;
	Names initNames;
	stack.push_back(initStack);
	names.push_back(initNames);
	for (instruction = 0; instruction < bcfile.file.size(); instruction += 2) {
		if (bcfile.file.at(instruction) == "LOAD_CONST") {
			if (isInteger(bcfile.file.at(instruction + 1)))
				stack.back().push_back(std::stoll(bcfile.file.at(instruction + 1)));
			else if (isFloat(bcfile.file.at(instruction + 1)))
				stack.back().push_back(std::stold(bcfile.file.at(instruction + 1)));
			else
				stack.back().push_back(bcfile.file.at(instruction + 1));
		}
		else if (bcfile.file.at(instruction) == "LOAD_REF") {
			stack.back().push_back(static_cast<long long int>(getNameReference(bcfile.file.at(instruction + 1))));
		}
		else if (bcfile.file.at(instruction) == "STORE_REF") {
			//names.reference.at(getNameReference(bcfile.file.at(instruction + 1))) = stack.back();
		}
		else if (bcfile.file.at(instruction) == "LOAD_NAME") {
			stack.back().push_back(stack.back().at(getNameReference(bcfile.file.at(instruction + 1))));
		}
		else if (bcfile.file.at(instruction) == "STORE_NAME") {
			if (doesNameExist(bcfile.file.at(instruction + 1)) == false) {
				names.back().identifier.push_back(bcfile.file.at(instruction + 1));
				names.back().reference.push_back(stack.back().size() - 1);
			}
			else {
				stack.back().at(getNameReference(bcfile.file.at(instruction + 1))) = stack.back().back();
			}
		}
		else if (bcfile.file.at(instruction) == "COMPARE") {
			conditional = false;
			if (bcfile.file.at(instruction + 1) == "==") {
				if (stack.back().at(stack.back().size() - 2) == stack.back().back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == "!=") {
				if (stack.back().at(stack.back().size() - 2) != stack.back().back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == ">=") {
				if (stack.back().at(stack.back().size() - 2) >= stack.back().back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == "<=") {
				if (stack.back().at(stack.back().size() - 2) <= stack.back().back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == ">") {
				if (stack.back().at(stack.back().size() - 2) > stack.back().back())
					conditional = true;
			}
			if (bcfile.file.at(instruction + 1) == "<") {
				if (stack.back().at(stack.back().size() - 2) < stack.back().back())
					conditional = true;
			}
			stack.back().pop_back();
			stack.back().pop_back();
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
				instruction = std::get<long long int>(stack.back().at(names.back().reference.at(getNameReference(bcfile.file.at(instruction + 1)))));
		}
		else if (bcfile.file.at(instruction) == "CALL") {
			if (doesNameExist(bcfile.file.at(instruction + 1)) == false) {
				executePCF(bcfile.file.at(instruction + 1));
			}
			if (doesNameExist(bcfile.file.at(instruction + 1)) == true) {
				secondaryInstruction.push_back(instruction);
				instruction = std::get<long long int>(stack.front().at(getNameReference(bcfile.file.at(instruction + 1))));
				stack.push_back(stack.at(0));
				names.push_back(names.at(0));
				/*for (size_t e = 0; e < bcfile.file.size(); e += 2) {
					if (bcfile.file.at(e) == "START_FUNCTION" and bcfile.file.at(e + 1) == bcfile.file.at(instruction + 1)) {
						instruction = e;
					}
				}*/
			}
		}
		else if (bcfile.file.at(instruction) == "ADD") {
			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) + std::get<long long int>(stack.back().at(stack.back().size() - 1));

			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) + static_cast<long double>(std::get<long long int>(stack.back().at(stack.back().size() - 1)));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = (std::get<long double>(stack.back().at(stack.back().size() - 1))) + static_cast<long double>(std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) + std::get<long double>(stack.back().at(stack.back().size() - 1));

			if (std::holds_alternative<std::string>(stack.back().at(stack.back().size() - 1)))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<std::string>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) + std::get<std::string>(stack.back().at(stack.back().size() - 1));

		}
		else if (bcfile.file.at(instruction) == "SUB") {
			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) - std::get<long long int>(stack.back().at(stack.back().size() - 1));

			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) - static_cast<long double>(std::get<long long int>(stack.back().at(stack.back().size() - 1)));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(stack.back().size() - 1)) - static_cast<long double>(std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) - std::get<long double>(stack.back().at(stack.back().size() - 1));

		}
		else if (bcfile.file.at(instruction) == "MUL") {
			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) * std::get<long long int>(stack.back().at(stack.back().size() - 1));

			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) * static_cast<long double>(std::get<long long int>(stack.back().at(stack.back().size() - 1)));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = (std::get<long double>(stack.back().at(stack.back().size() - 1))) * static_cast<long double>(std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) * std::get<long double>(stack.back().at(stack.back().size() - 1));

			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<std::string>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))))) {
				std::string str = std::get<std::string>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))));
				std::string str2;
				for (size_t n = 0; n < std::get<long long int>(stack.back().at(stack.back().size() - 1)); ++n) {
					str2 = str2 + str;
				}
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = str2;
			}

		}
		else if (bcfile.file.at(instruction) == "DIV") {
			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) / std::get<long long int>(stack.back().at(stack.back().size() - 1));

			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) / std::get<long long int>(stack.back().at(stack.back().size() - 1));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = (std::get<long double>(stack.back().at(stack.back().size() - 1))) / std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) / std::get<long double>(stack.back().at(stack.back().size() - 1));

		}
		else if (bcfile.file.at(instruction) == "MOD") {
			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))) % std::get<long long int>(stack.back().at(stack.back().size() - 1));
		}
		else if (bcfile.file.at(instruction) == "EXP") {
			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = pow(std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))), std::get<long long int>(stack.back().at(stack.back().size() - 1)));

			if (std::holds_alternative<long long int>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = pow(std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))), std::get<long long int>(stack.back().at(stack.back().size() - 1)));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = pow(std::get<long double>(stack.back().at(stack.back().size() - 1)), std::get<long long int>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))));

			if (std::holds_alternative<long double>(stack.back().at(stack.back().size() - 1)) and std::holds_alternative<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))))
				stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2))) = pow(std::get<long double>(stack.back().at(std::get<long long int>(stack.back().at(stack.back().size() - 2)))), std::get<long double>(stack.back().at(stack.back().size() - 1)));

		}
		else if (bcfile.file.at(instruction) == "START_FUNCTION") {
			names.front().identifier.push_back(bcfile.file.at(instruction + 1));
			names.front().reference.push_back(stack.back().size());
			stack.front().push_back(static_cast<long long int>(instruction));
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
			stack.back().pop_back();
		}
		else if (bcfile.file.at(instruction) == "LOAD_PARAM") {
			stack.back().push_back(paramStack.back().front());
			paramStack.back().erase(paramStack.back().begin());
		}
		else if (bcfile.file.at(instruction) == "STORE_PARAM") {
			paramStack.back().push_back(stack.back().back());
		}
		else if (bcfile.file.at(instruction) == "DELETE") {
			stack.back().at(names.back().reference.at(getNameReference(bcfile.file.at(instruction + 1)))) = 0;
			names.back().reference.at(getNameReference(bcfile.file.at(instruction + 1))) = getNameReference(bcfile.file.at(instruction + 1));
			names.back().identifier.at(getNameReference(bcfile.file.at(instruction + 1))) = getNameReference(bcfile.file.at(instruction + 1));
		}
		else if (bcfile.file.at(instruction) == "POP_NAME") {
			/*stack.erase(stack.begin() + names.reference.back());
			names.reference.pop_back();
			names.identifier.pop_back();*/
		}
		else if (bcfile.file.at(instruction) == "LOAD_BACK_REF") {
			stack.back().push_back(static_cast<long long int>(stack.back().size() - 1));
		}
		else if (bcfile.file.at(instruction) == "RETURN_VALUE") {
			instruction = secondaryInstruction.back();
			secondaryInstruction.pop_back();
			returnedValue = stack.back().back();
			stack.back().pop_back();
			stack.pop_back();
			names.pop_back();
		}
		else if (bcfile.file.at(instruction) == "LOAD_RETURN_VALUE") {
			stack.back().push_back(returnedValue);
		}
		else if (bcfile.file.at(instruction) == "NEW_PARAM_STACK") {
			t_stack newParamStack;
			paramStack.push_back(newParamStack);
		}
		else if (bcfile.file.at(instruction) == "POP_PARAM_STACK") {
			paramStack.pop_back();
		}
		else if (bcfile.file.at(instruction) == "NEW_STACK") {
			t_stack newStack;
			stack.push_back(newStack);
			Names newNames;
			names.push_back(newNames);
			names.back() = names.front();
			stack.back() = stack.front();
		}
		else if (bcfile.file.at(instruction) == "POP_STACK") {
			stack.pop_back();
			names.pop_back();
		}
	}
}


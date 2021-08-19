#include "interpreter.h"
#include "../bytecode/byteinterpreter.h"

int Interpreter::getToken(std::string str)
{
	for (size_t i = 0; i < tokenTypes.size(); ++i) {
		if (tokenTypes.at(i) == str)
			return i;
	}
}

void Interpreter::translate()
{
	file.push_back("LOAD_CONST");
	file.push_back("0");
	file.push_back("STORE_NAME");
	file.push_back("LYNX_START");
	for (size_t instruction = 0; instruction < codeFile.token.size();) {
		switch (getToken(codeFile.type.at(instruction))) {
		case IF:
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				file.push_back("LOAD_NAME");
				file.push_back(codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				file.push_back("LOAD_CONST");
				file.push_back(codeFile.token.at(instruction + 1));
				break;
			}
			switch (getToken(codeFile.type.at(instruction + 3))) {
			case NAME:
				file.push_back("LOAD_NAME");
				file.push_back(codeFile.token.at(instruction + 3));
				break;
			case CONSTANT_VALUE:
				file.push_back("LOAD_CONST");
				file.push_back(codeFile.token.at(instruction + 3));
				break;
			}
			file.push_back("COMPARE");
			file.push_back(codeFile.token.at(instruction + 2));
			file.push_back("JUMP_IF_FALSE");
			file.push_back("0");
			jumpInstruction.push_back(file.size() - 1);
			instruction += 4;
			break;
		case WHILE_LOOP:
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				file.push_back("LOAD_NAME");
				file.push_back(codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				file.push_back("LOAD_CONST");
				file.push_back(codeFile.token.at(instruction + 1));
				break;
			}
			switch (getToken(codeFile.type.at(instruction + 3))) {
			case NAME:
				file.push_back("LOAD_NAME");
				file.push_back(codeFile.token.at(instruction + 3));
				break;
			case CONSTANT_VALUE:
				file.push_back("LOAD_CONST");
				file.push_back(codeFile.token.at(instruction + 3));
				break;
			}
			file.push_back("COMPARE");
			file.push_back(codeFile.token.at(instruction + 2));
			file.push_back("JUMP_IF_FALSE");
			file.push_back("0");
			jumpInstruction.push_back(file.size() - 1);
			instruction += 4;
			break;
		case NAME:
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case EQUALS:
				switch (getToken(codeFile.type.at(instruction + 2))) {
				case CONSTANT_VALUE:
					file.push_back("LOAD_CONST");
					file.push_back(codeFile.token.at(instruction + 2));
					break;
				case NAME:
					file.push_back("LOAD_NAME");
					file.push_back(codeFile.token.at(instruction + 2));
					break;
				}
				file.push_back("STORE_NAME");
				file.push_back(codeFile.token.at(instruction));
				instruction += 3;
				break;
			case PLUS_EQUALS:
				file.push_back("LOAD_REF");
				file.push_back(codeFile.token.at(instruction));
				switch (getToken(codeFile.type.at(instruction + 2))) {
				case CONSTANT_VALUE:
					file.push_back("LOAD_CONST");
					file.push_back(codeFile.token.at(instruction + 2));
					break;
				case NAME:
					file.push_back("LOAD_NAME");
					file.push_back(codeFile.token.at(instruction + 2));
					break;
				}
				file.push_back("ADD");
				file.push_back("0");
				instruction += 3;
				break;
			case NAME:
				file.push_back("LOAD_NAME");
				file.push_back(codeFile.token.at(instruction + 1));
				file.push_back("CALL");
				file.push_back(codeFile.token.at(instruction));
				instruction += 2;
				break;
			case CONSTANT_VALUE:
				file.push_back("LOAD_CONST");
				file.push_back(codeFile.token.at(instruction + 1));
				file.push_back("CALL");
				file.push_back(codeFile.token.at(instruction));
				instruction += 2;
				break;
			}
			break;
		case ENDIF:
			file.at(jumpInstruction.back()) = std::to_string(file.size() - 2);
			jumpInstruction.pop_back();
			instruction += 1;
			break;
		case ENDWHILE:
			file.at(jumpInstruction.back()) = std::to_string(file.size());
			file.push_back("JUMP");
			file.push_back(std::to_string(jumpInstruction.back() - 9));
			jumpInstruction.pop_back();
			instruction += 1;
			break;
		}
	}
}

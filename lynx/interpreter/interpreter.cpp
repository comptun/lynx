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
		case IF: {
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
			std::vector<size_t> vec;
			jumpInstruction.push_back(vec);
			jumpInstruction.back().push_back(file.size() - 1);
			statementType.push_back(IF_STATEMENT);
			instruction += 4;
			break;
		}
		case WHILE_LOOP: {
			if (codeFile.token.at(instruction + 1) == "True") {
				file.push_back("LOAD_CONST");
				file.push_back("0");
				file.push_back("LOAD_CONST");
				file.push_back("0");
				file.push_back("COMPARE");
				file.push_back("==");
				instruction += 2;
			}
			else if (codeFile.token.at(instruction + 1) == "False") {
				file.push_back("LOAD_CONST");
				file.push_back("0");
				file.push_back("LOAD_CONST");
				file.push_back("0");
				file.push_back("COMPARE");
				file.push_back("!=");
				instruction += 2;
			}
			else {
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
				instruction += 4;
			}
			file.push_back("JUMP_IF_FALSE");
			file.push_back("0");
			std::vector<size_t> vec;
			jumpInstruction.push_back(vec);
			jumpInstruction.back().push_back(file.size() - 1);
			statementType.push_back(WHILE_STATEMENT);
			break;
		}
		case AND:
			if (codeFile.token.at(instruction + 1) == "True") {
				file.push_back("LOAD_CONST");
				file.push_back("0");
				file.push_back("LOAD_CONST");
				file.push_back("0");
				file.push_back("COMPARE");
				file.push_back("==");
				instruction += 2;
			}
			else if (codeFile.token.at(instruction + 1) == "False") {
				file.push_back("LOAD_CONST");
				file.push_back("0");
				file.push_back("LOAD_CONST");
				file.push_back("0");
				file.push_back("COMPARE");
				file.push_back("!=");
				instruction += 2;
			}
			else {
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
				instruction += 4;
			}
			file.push_back("JUMP_IF_FALSE");
			file.push_back("0");
			jumpInstruction.back().push_back(file.size() - 1);
			break;
		case FUNCTION:
			file.push_back("START_FUNCTION");
			file.push_back(codeFile.token.at(instruction + 1));
			if (codeFile.token.at(instruction + 2) == ":") {
				file.push_back("STORE_NAME");
				file.push_back(codeFile.token.at(instruction + 3));
				instruction += 4;
				break;
			}
			statementType.push_back(FUNCTION_STATEMENT);
			instruction += 2;
			break;
		case FUNCTION_END:
			file.push_back("END_FUNCTION");
			file.push_back("0");
			++instruction;
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
			case MOD_EQUALS:
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
				file.push_back("MOD");
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
		case LEFT_CURLY_BRACE:
			++instruction;
			break;
		case RIGHT_CURLY_BRACE:
			switch (statementType.back()) {
			case IF_STATEMENT:
				for (size_t e = 0; e < jumpInstruction.back().size(); ++e) {
					file.at(jumpInstruction.back().at(e)) = std::to_string(file.size() - 2);
				}
				jumpInstruction.pop_back();
				instruction += 1;
				break;
			case WHILE_STATEMENT:
				for (size_t e = 0; e < jumpInstruction.back().size(); ++e) {
					file.at(jumpInstruction.back().at(e)) = std::to_string(file.size());
				}
				for (size_t a = 0; a < breakJump.size(); ++a) {
					file.at(breakJump.at(a)) = std::to_string(file.size());
				}
				breakJump.clear();
				file.push_back("JUMP");
				file.push_back(std::to_string(jumpInstruction.back().front() - 9));
				jumpInstruction.pop_back();
				instruction += 1;
				break;
			case FUNCTION_STATEMENT:
				file.push_back("END_FUNCTION");
				file.push_back("0");
				++instruction;
				break;
			}
			statementType.pop_back();
			break;
		case ENDIF:
			for (size_t e = 0; e < jumpInstruction.back().size(); ++e) {
				file.at(jumpInstruction.back().at(e)) = std::to_string(file.size() - 2);
			}
			jumpInstruction.pop_back();
			instruction += 1;
			break;
		case ENDWHILE:
			for (size_t e = 0; e < jumpInstruction.back().size(); ++e) {
				file.at(jumpInstruction.back().at(e)) = std::to_string(file.size());
			}
			for (size_t a = 0; a < breakJump.size(); ++a) {
				file.at(breakJump.at(a)) = std::to_string(file.size());
			}
			breakJump.clear();
			file.push_back("JUMP");
			file.push_back(std::to_string(jumpInstruction.back().front() - 9));
			jumpInstruction.pop_back();
			instruction += 1;
			break;
		case BREAK:
			file.push_back("JUMP");
			file.push_back("0");
			breakJump.push_back(file.size() - 1);
			++instruction;
			break;
		}
	}
}

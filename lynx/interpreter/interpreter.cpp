#include "interpreter.h"
#include "../bytecode/byteinterpreter.h"

int Interpreter::getToken(std::string str)
{
	for (size_t i = 0; i < tokenTypes.size(); ++i) {
		if (tokenTypes.at(i) == str)
			return i;
	}
}

bool Interpreter::nameExists(std::string name)
{
	for (size_t i = 0; i < knownNames.size(); ++i) {
		if (knownNames.at(i) == name)
			return true;
	}
	return false;
}

void Interpreter::preprocess()
{
	define("true", "1");
	define("false", "0");
	for (size_t i = 0; i < codeFile.token.size();) {
		for (size_t j = 0; j < definitions.placeholder.size(); ++j) {
			if (codeFile.token.at(i) == definitions.placeholder.at(j)) {
				codeFile.token.at(i) = definitions.instruction.at(j);
				retokenize(codeFile.token.at(i), i);
				break;
			}
		}
		if (codeFile.token.at(i) == "define") {
			define(codeFile.token.at(i + 1), codeFile.token.at(i + 2));
			i += 3;
		}
		else {
			++i;
		}
	}
}

void Interpreter::translate()
{
	preprocess();
	nameScope.push_back(0);
	file.push_back("LOAD_CONST");
	file.push_back("0");
	file.push_back("STORE_NAME");
	file.push_back("LYNX_START");
	for (size_t instruction = 0; instruction < codeFile.token.size();) {
		switch (getToken(codeFile.type.at(instruction))) {
		case RBRACKET:
			if (isInFunctionCall) {
				file.push_back("CALL");
				file.push_back(functionName);
			}
			isInFunctionCall = false;
			isInFunctionDefinition = false;
			++instruction;
			break;
		case DEFINE:
			instruction += 3;
			break;
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
			if (codeFile.token.at(instruction + 2) != "and" && codeFile.token.at(instruction + 2) != "or" && codeFile.token.at(instruction + 2) != "{") {
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
			else {
				file.push_back("LOAD_CONST");
				file.push_back("1");
				file.push_back("COMPARE");
				file.push_back(">=");
				instruction += 2;
			}
			file.push_back("JUMP_IF_FALSE");
			file.push_back("0");
			std::vector<size_t> vec;
			jumpInstruction.push_back(vec);
			jumpInstruction.back().push_back(file.size() - 1);
			statementType.push_back(IF_STATEMENT);
			nameScope.push_back(0);
			break;
		}
		case WHILE_LOOP: {
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
			if (codeFile.token.at(instruction + 2) != "and" && codeFile.token.at(instruction + 2) != "or" && codeFile.token.at(instruction + 2) != "{") {
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
			else {
				file.push_back("LOAD_CONST");
				file.push_back("1");
				file.push_back("COMPARE");
				file.push_back(">=");
				instruction += 2;
			}
			file.push_back("JUMP_IF_FALSE");
			file.push_back("0");
			std::vector<size_t> vec;
			jumpInstruction.push_back(vec);
			jumpInstruction.back().push_back(file.size() - 1);
			statementType.push_back(WHILE_STATEMENT);
			nameScope.push_back(0);
			break;
		}
		case AND:
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
			if (codeFile.token.at(instruction + 2) != "and" && codeFile.token.at(instruction + 2) != "or" && codeFile.token.at(instruction + 2) != "{") {
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
			else {
				file.push_back("LOAD_CONST");
				file.push_back("1");
				file.push_back("COMPARE");
				file.push_back(">=");
				instruction += 2;
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
		case PLUS:
			file.push_back("LOAD_REF");
			file.push_back("_VAR_TEMP_STORAGE");
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
			file.push_back("ADD");
			file.push_back("0");
			if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
				file.push_back("LOAD_NAME");
				file.push_back("_VAR_TEMP_STORAGE");
				file.push_back("STORE_NAME");
				file.push_back(currentName);
			}
			instruction += 2;
			break;
		case MINUS:
			file.push_back("LOAD_REF");
			file.push_back("_VAR_TEMP_STORAGE");
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
			file.push_back("SUB");
			file.push_back("0");
			if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
				file.push_back("LOAD_NAME");
				file.push_back("_VAR_TEMP_STORAGE");
				file.push_back("STORE_NAME");
				file.push_back(currentName);
			}
			instruction += 2;
			break;
		case MULTIPLY:
			file.push_back("LOAD_REF");
			file.push_back("_VAR_TEMP_STORAGE");
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
			file.push_back("MUL");
			file.push_back("0");
			if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
				file.push_back("LOAD_NAME");
				file.push_back("_VAR_TEMP_STORAGE");
				file.push_back("STORE_NAME");
				file.push_back(currentName);
			}
			instruction += 2;
			break;
		case DIVIDE:
			file.push_back("LOAD_REF");
			file.push_back("_VAR_TEMP_STORAGE");
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
			file.push_back("DIV");
			file.push_back("0");
			if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
				file.push_back("LOAD_NAME");
				file.push_back("_VAR_TEMP_STORAGE");
				file.push_back("STORE_NAME");
				file.push_back(currentName);
			}
			instruction += 2;
			break;
		case NAME:
			/*for (size_t i = instruction + 1; i < codeFile.token.size(); ++i) {
				if (codeFile.token.at(i + 1) == "(")
					break;
				if (codeFile.token.at(i) == ")" && codeFile.token.at(i + 1) == "{") {
					isInFunctionDefinition = true;
					break;
				}
				if (codeFile.token.at(i) == ")" && codeFile.token.at(i + 1) != "{") {
					isInFunctionCall = true;
					break;
				}
			}
			if (isInFunctionDefinition) {
				file.push_back("START_FUNCTION");
				file.push_back(codeFile.token.at(instruction));
				statementType.push_back(FUNCTION_STATEMENT);
				if (codeFile.token.at(instruction + 2) == ")") {
					instruction += 4;
					isInFunctionCall = false;
					isInFunctionDefinition = false;
				}
				else {
					instruction += 2;
				}
			}
			if (isInFunctionCall) {
				functionName = codeFile.token.at(instruction);
				instruction += 2;
			}*/
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case LEFT_CURLY_BRACE:
				file.push_back("START_FUNCTION");
				file.push_back(codeFile.token.at(instruction));
				if (codeFile.token.at(instruction + 2) == ":") {
					file.push_back("STORE_NAME");
					file.push_back(codeFile.token.at(instruction + 3));
					instruction += 4;
					break;
				}
				statementType.push_back(FUNCTION_STATEMENT);
				nameScope.push_back(0);
				instruction += 2;
				break;
			case EQUALS:
				nameScope.back() += 1;
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
				if (codeFile.token.at(instruction + 3) != "+" and codeFile.token.at(instruction + 3) != "-" and codeFile.token.at(instruction + 3) != "*" and codeFile.token.at(instruction + 3) != "/") {
					if (nameExists(codeFile.token.at(instruction)) == false)
						knownNames.push_back(codeFile.token.at(instruction));
					file.push_back("STORE_NAME");
					file.push_back(codeFile.token.at(instruction));
				}
				else {
					currentName = codeFile.token.at(instruction);
					file.push_back("STORE_NAME");
					file.push_back("_VAR_TEMP_STORAGE");
					if (nameExists(codeFile.token.at(instruction)) == false) {
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
					}
				}
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
			for (size_t e = 0; e < nameScope.back(); ++e) {
				file.push_back("POP_NAME");
				file.push_back("0");
			}
			nameScope.pop_back();
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

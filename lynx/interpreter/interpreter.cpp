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

void Interpreter::bytecode(std::string instruction, std::string param)
{
	file.push_back(instruction);
	file.push_back(param);
}

void Interpreter::translate()
{
	tokenize("LYNX_END");
	tokenize("=");
	tokenize("0");
	tokenize("LYNX_END");
	tokenize("+=");
	tokenize("1");

	preprocess();
	nameScope.push_back(0);
	file.push_back("LOAD_CONST");
	file.push_back("0");
	file.push_back("STORE_NAME");
	file.push_back("LYNX_START");

	for (size_t instruction = 0; instruction < codeFile.token.size();) {
		switch (getToken(codeFile.type.at(instruction))) {
		case ELSE:
			if (codeFile.token.at(instruction + 1) == "if") {
				++instruction;
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
				statementType.push_back(ELIF_STATEMENT);
				nameScope.push_back(0);
				break;
			}
			else {
				statementType.push_back(ELSE_STATEMENT);
				nameScope.push_back(0);
				++instruction;
			}
			break;
		case JUMP_TO:
			file.push_back("JUMP");
			file.push_back(codeFile.token.at(instruction + 1));
			instruction += 2;
			break;
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
			isInConditional = true;
			conditionalStage = 0;
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
			/*switch (getToken(codeFile.type.at(instruction + 1))) {
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
			nameScope.push_back(0);*/
			break;
		}
		case WHILE_LOOP: {
			continueJump.push_back(file.size() - 2);
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
			isInFunctionDefinition = true;
			if (codeFile.token.at(instruction + 2) == "(") {
				file.push_back("START_FUNCTION");
				file.push_back(codeFile.token.at(instruction + 1));
				statementType.push_back(FUNCTION_STATEMENT);
				instruction += 3;
			}
			break;
		case FUNCTION_END:
			file.push_back("END_FUNCTION");
			file.push_back("0");
			++instruction;
			break;
		case PLUS:
			if (isInNameAssignment) {
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
				bytecode("POP_BACK", "0");
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_NAME", currentName);
					isInNameAssignment = false;
				}
				instruction += 2;
			}
			break;
		case MINUS:
			if (isInNameAssignment) {
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
				bytecode("POP_BACK", "0");
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_NAME", currentName);
					isInNameAssignment = false;
				}
				instruction += 2;
			}
			break;
		case MULTIPLY:
			if (isInNameAssignment) {
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
				bytecode("POP_BACK", "0");
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_NAME", currentName);
					isInNameAssignment = false;
				}
				instruction += 2;
			}
			break;
		case DIVIDE:
			if (isInNameAssignment) {
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
				bytecode("POP_BACK", "0");
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_NAME", currentName);
					isInNameAssignment = false;
				}
				instruction += 2;
			}
			break;
		case CONSTANT_VALUE:
			if (codeFile.token.at(instruction + 1) == "," and isInFunctionCall) {
				bytecode("LOAD_CONST", codeFile.token.at(instruction));
				bytecode("STORE_PARAM", "0");
				instruction += 2;
				break;
			}
			if (codeFile.token.at(instruction + 1) == ")" and isInFunctionCall) {
				bytecode("LOAD_CONST", codeFile.token.at(instruction));
				bytecode("STORE_PARAM", "0");
				instruction += 1;
				break;
			}

			if (codeFile.token.at(instruction + 1) == "," and isInFunctionDefinition) {
				bytecode("LOAD_PARAM", "0");
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				instruction += 2;
				break;
			}
			if (codeFile.token.at(instruction + 1) == ")" and isInFunctionDefinition) {
				bytecode("LOAD_PARAM", "0");
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				instruction += 1;
				break;
			}
			break;
		case NAME:
			if (codeFile.token.at(instruction + 1) == "(") {
				functionName = codeFile.token.at(instruction);
				isInFunctionCall = true;
				instruction += 2;
				break;
			}
			if (codeFile.token.at(instruction + 1) == "," and isInFunctionCall) {
				bytecode("LOAD_NAME", codeFile.token.at(instruction));
				bytecode("STORE_PARAM", "0");
				instruction += 2;
				break;
			}
			if (codeFile.token.at(instruction + 1) == ")" and isInFunctionCall) {
				bytecode("LOAD_NAME", codeFile.token.at(instruction));
				bytecode("STORE_PARAM", "0");
				instruction += 1;
				break;
			}

			if (codeFile.token.at(instruction + 1) == "," and isInFunctionDefinition) {
				bytecode("LOAD_PARAM", "0");
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				instruction += 2;
				break;
			}
			if (codeFile.token.at(instruction + 1) == ")" and isInFunctionDefinition) {
				bytecode("LOAD_PARAM", "0");
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				instruction += 1;
				break;
			}
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case COLON:
				file.push_back("LOAD_CONST");
				file.push_back(std::to_string(file.size() - 2));
				file.push_back("STORE_NAME");
				file.push_back(codeFile.token.at(instruction));
				break;
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
				if (nameExists(codeFile.token.at(instruction)) == false)
						nameScope.back() += 1;
				if (codeFile.token.at(instruction + 3) != "+" and codeFile.token.at(instruction + 3) != "-" and codeFile.token.at(instruction + 3) != "*" and codeFile.token.at(instruction + 3) != "/") {
					if (nameExists(codeFile.token.at(instruction)) == false)
						knownNames.push_back(codeFile.token.at(instruction));
					file.push_back("STORE_NAME");
					file.push_back(codeFile.token.at(instruction));
				}
				else {
					currentName = codeFile.token.at(instruction);
					bytecode("LOAD_BACK_REF", "0");
					isInNameAssignment = true;
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
			case ELIF_STATEMENT:
				file.at(elseJump.back()) = std::to_string(file.size() - 2);
				elseJump.pop_back();
				if (codeFile.token.at(instruction + 1) == "else") {
					file.push_back("JUMP");
					file.push_back("0");
					elseJump.push_back(file.size() - 1);
				}
				for (size_t e = 0; e < jumpInstruction.back().size(); ++e) {
					file.at(jumpInstruction.back().at(e)) = std::to_string(file.size() - 2);
				}
				jumpInstruction.pop_back();
				instruction += 1;
				break;
			case ELSE_STATEMENT:
				file.at(elseJump.back()) = std::to_string(file.size() - 2);
				elseJump.pop_back();
				instruction += 1;
				break;
			case IF_STATEMENT:
				if (codeFile.token.at(instruction + 1) == "else") {
					file.push_back("JUMP");
					file.push_back("0");
					elseJump.push_back(file.size() - 1);
				}
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
				continueJump.pop_back();
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
		case CONTINUE:
			bytecode("JUMP", std::to_string(continueJump.back()));
			++instruction;
			break;
		}
	}
}

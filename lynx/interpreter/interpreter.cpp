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
	//std::cout << file.size() << " " << instruction << " " << param << std::endl;
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

	bytecode("LOAD_CONST", "0");
	bytecode("STORE_NAME", "LYNX_START");

	for (size_t instruction = 0; instruction < codeFile.token.size();) {
		switch (getToken(codeFile.type.at(instruction))) {
		case ELSE:
			if (codeFile.token.at(instruction + 1) == "if") {
				++instruction;
				switch (getToken(codeFile.type.at(instruction + 1))) {
				case NAME:
					bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
					break;
				case CONSTANT_VALUE:
					bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
					break;
				}
				if (codeFile.token.at(instruction + 2) != "and" && codeFile.token.at(instruction + 2) != "or" && codeFile.token.at(instruction + 2) != "{") {
					switch (getToken(codeFile.type.at(instruction + 3))) {
					case NAME:
						bytecode("LOAD_NAME", codeFile.token.at(instruction + 3));
						break;
					case CONSTANT_VALUE:
						bytecode("LOAD_CONST", codeFile.token.at(instruction + 3));
						break;
					}
					bytecode("COMPARE", codeFile.token.at(instruction + 2));
					instruction += 4;
				}
				else {
					bytecode("LOAD_CONST", "1");
					bytecode("COMPARE", ">=");
					instruction += 2;
				}
				bytecode("JUMP_IF_FALSE", "0");
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
			bytecode("JUMP", codeFile.token.at(instruction + 1));
			instruction += 2;
			break;
		case DEFINE:
			instruction += 3;
			break;
		case IF: {
			isInConditional = true;
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				break;
			}
			bytecode("LOAD_BACK_REF", "0");
			instruction += 2;
			pastConditional = IF_STATEMENT;
			currentComparisonOperator = "undefined";
			/*switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				bytecode("LOAD_NAME");
				bytecode(codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST");
				bytecode(codeFile.token.at(instruction + 1));
				break;
			}
			if (codeFile.token.at(instruction + 2) != "and" && codeFile.token.at(instruction + 2) != "or" && codeFile.token.at(instruction + 2) != "{") {
				switch (getToken(codeFile.type.at(instruction + 3))) {
				case NAME:
					bytecode("LOAD_NAME");
					bytecode(codeFile.token.at(instruction + 3));
					break;
				case CONSTANT_VALUE:
					bytecode("LOAD_CONST");
					bytecode(codeFile.token.at(instruction + 3));
					break;
				}
				bytecode("COMPARE");
				bytecode(codeFile.token.at(instruction + 2));
				instruction += 4;
			}
			else {
				bytecode("LOAD_CONST");
				bytecode("1");
				bytecode("COMPARE");
				bytecode(">=");
				instruction += 2;
			}
			bytecode("JUMP_IF_FALSE");
			bytecode("0");
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
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				break;
			}
			if (codeFile.token.at(instruction + 2) != "and" && codeFile.token.at(instruction + 2) != "or" && codeFile.token.at(instruction + 2) != "{") {
				switch (getToken(codeFile.type.at(instruction + 3))) {
				case NAME:
					bytecode("LOAD_NAME", codeFile.token.at(instruction + 3));
					break;
				case CONSTANT_VALUE:
					bytecode("LOAD_CONST", codeFile.token.at(instruction + 3));
					break;
				}
				bytecode("COMPARE", codeFile.token.at(instruction + 2));
				instruction += 4;
			}
			else {
				bytecode("LOAD_CONST", "1");
				bytecode("COMPARE", ">=");
				instruction += 2;
			}
			bytecode("JUMP_IF_FALSE", "0");
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
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				break;
			}
			if (codeFile.token.at(instruction + 2) != "and" && codeFile.token.at(instruction + 2) != "or" && codeFile.token.at(instruction + 2) != "{") {
				switch (getToken(codeFile.type.at(instruction + 3))) {
				case NAME:
					bytecode("LOAD_NAME", codeFile.token.at(instruction + 3));
					break;
				case CONSTANT_VALUE:
					bytecode("LOAD_CONST", codeFile.token.at(instruction + 3));
					break;
				}
				bytecode("COMPARE", codeFile.token.at(instruction + 2));
				instruction += 4;
			}
			else {
				bytecode("LOAD_CONST", "1");
				bytecode("COMPARE", ">=");
				instruction += 2;
			}
			bytecode("JUMP_IF_FALSE", "0");
			jumpInstruction.back().push_back(file.size() - 1);
			break;
		case FUNCTION:
			isInFunctionDefinition = true;
			if (codeFile.token.at(instruction + 2) == "(") {
				bytecode("START_FUNCTION", codeFile.token.at(instruction + 1));
				statementType.push_back(FUNCTION_STATEMENT);
				instruction += 3;
			}
			if (codeFile.token.at(instruction) == ")") {
				++instruction;
				isInFunctionDefinition = false;
			}
			nameScope.push_back(0);
			break;
		case FUNCTION_END:
			bytecode("LOAD_CONST", "0");
			bytecode("RETURN_VALUE", "0");
			bytecode("END_FUNCTION", "0");
			++instruction;
			break;
		case COMMA:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		case RBRACKET:
			if (functionName.size() > 0) {
				bytecode("POP_BACK", "0");
				bytecode("STORE_PARAM", "0");
				bytecode("POP_BACK", "0");
				bytecode("CALL", functionName.back());
				bytecode("LOAD_RETURN_VALUE", "0");
				bytecode("LOAD_STACK_BACK", "0");
				functionName.pop_back();
				--functionDepth;
			}
			isInFunctionDefinition = false;
			++instruction;
			break;
		case EQUAL_TO:
			bytecode("POP_BACK", "0");
			currentComparisonOperator = "==";
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				break;
			}
			bytecode("LOAD_BACK_REF", "0");
			instruction += 2;
			break;
		case PLUS:
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				break;
			}
			bytecode("ADD", "0");
			bytecode("POP_BACK", "0");
			if (isInNameAssignment) {
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_NAME", currentName);
					isInNameAssignment = false;
				}
			}
			if (isInReturnStatement) {
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("RETURN_VALUE", "0");
					isInReturnStatement = false;
				}
				break;
			}
			instruction += 2;
			break;
		case MINUS:
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				break;
			}
			bytecode("SUB", "0");
			bytecode("POP_BACK", "0");
			if (isInNameAssignment) {
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_NAME", currentName);
					isInNameAssignment = false;
				}
			}
			instruction += 2;
			break;
		case MULTIPLY:
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				break;
			}
			bytecode("MUL", "0");
			bytecode("POP_BACK", "0");
			if (isInNameAssignment) {
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_NAME", currentName);
					isInNameAssignment = false;
				}
			}
			instruction += 2;
			break;
		case DIVIDE:
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case NAME:
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				break;
			}
			bytecode("DIV", "0");
			bytecode("POP_BACK", "0");
			if (isInNameAssignment) {
				if (codeFile.token.at(instruction + 2) != "+" and codeFile.token.at(instruction + 2) != "-" and codeFile.token.at(instruction + 2) != "*" and codeFile.token.at(instruction + 2) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_NAME", currentName);
					isInNameAssignment = false;
				}
			}
			instruction += 2;
			break;
		case CONSTANT_VALUE:
			bytecode("LOAD_CONST", codeFile.token.at(instruction));
			bytecode("LOAD_BACK_REF", "0");
			instruction += 1;
			if (isInReturnStatement) {
				if (codeFile.token.at(instruction) != "+" and codeFile.token.at(instruction) != "-" and codeFile.token.at(instruction) != "*" and codeFile.token.at(instruction) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("RETURN_VALUE", "0");
					isInReturnStatement = false;
				}
				break;
			}
			/*if (codeFile.token.at(instruction + 1) == "," and isInFunctionCall) {
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
			}*/

			/*if (codeFile.token.at(instruction + 1) == "," and isInFunctionDefinition) {
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
			}*/
			break;
		case NAME:
			if (isInReturnStatement) {
				bytecode("LOAD_NAME", codeFile.token.at(instruction));
				bytecode("LOAD_BACK_REF", "0");
				instruction += 1;
				if (codeFile.token.at(instruction) != "+" and codeFile.token.at(instruction) != "-" and codeFile.token.at(instruction) != "*" and codeFile.token.at(instruction) != "/") {
					bytecode("POP_BACK", "0");
					bytecode("RETURN_VALUE", "0");
					isInReturnStatement = false;
				}
				break;
			}
			if (codeFile.token.at(instruction + 1) == "(" and codeFile.token.at(instruction + 2) == ")") {
				bytecode("CALL", codeFile.token.at(instruction));
				bytecode("LOAD_RETURN_VALUE", "0");
				bytecode("LOAD_STACK_BACK", "0");
				--functionDepth;
				instruction += 3;
				break;
			}
			if (codeFile.token.at(instruction + 1) == "(") {
				bytecode("NEW_PARAM_STACK", "0");
				functionName.push_back(codeFile.token.at(instruction));
				++functionDepth;
				instruction += 2;
				break;
			}
			/*if (codeFile.token.at(instruction + 1) == ")" and isInFunctionCall) {
				bytecode("LOAD_NAME", codeFile.token.at(instruction));
				bytecode("STORE_PARAM", "0");
				instruction += 1;
				break;
			}*/

			if (codeFile.token.at(instruction + 1) == "," and isInFunctionDefinition) {
				bytecode("LOAD_PARAM", "0");
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				instruction += 2;
				break;
			}
			if (codeFile.token.at(instruction + 1) == ")" and isInFunctionDefinition) {
				bytecode("LOAD_PARAM", "0");
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				bytecode("POP_PARAM_STACK", "0");
				instruction += 1;
				break;
			}
			if (codeFile.token.at(instruction + 1) == "," or codeFile.token.at(instruction + 1) == ")") {
				bytecode("LOAD_NAME", codeFile.token.at(instruction));
				bytecode("LOAD_BACK_REF", "0");
				instruction += 1;
				break;
			}
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case COLON:
				bytecode("LOAD_CONST", std::to_string(file.size() - 2));
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				break;
			case LEFT_CURLY_BRACE:
				bytecode("START_FUNCTION", codeFile.token.at(instruction));
				if (codeFile.token.at(instruction + 2) == ":") {
					bytecode("STORE_NAME", codeFile.token.at(instruction + 3));
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
					bytecode("LOAD_CONST", codeFile.token.at(instruction + 2));
					break;
				case NAME:
					bytecode("LOAD_NAME", codeFile.token.at(instruction + 2));
					break;
				}
				if (nameExists(codeFile.token.at(instruction)) == false)
						nameScope.back() += 1;
				if (codeFile.token.at(instruction + 3) != "+" and codeFile.token.at(instruction + 3) != "-" and codeFile.token.at(instruction + 3) != "*" and codeFile.token.at(instruction + 3) != "/") {
					if (nameExists(codeFile.token.at(instruction)) == false)
						knownNames.push_back(codeFile.token.at(instruction));
					bytecode("STORE_NAME", codeFile.token.at(instruction));
				}
				else {
					currentName = codeFile.token.at(instruction);
					bytecode("LOAD_BACK_REF", "0");
					isInNameAssignment = true;
				}
				instruction += 3;
				break;
			case PLUS_EQUALS:
				bytecode("LOAD_REF", codeFile.token.at(instruction));
				switch (getToken(codeFile.type.at(instruction + 2))) {
				case CONSTANT_VALUE:
					bytecode("LOAD_CONST", codeFile.token.at(instruction + 2));
					break;
				case NAME:
					bytecode("LOAD_NAME", codeFile.token.at(instruction + 2));
					break;
				}
				bytecode("ADD", "0");
				instruction += 3;
				break;
			case MOD_EQUALS:
				bytecode("LOAD_REF", codeFile.token.at(instruction));
				switch (getToken(codeFile.type.at(instruction + 2))) {
				case CONSTANT_VALUE:
					bytecode("LOAD_CONST", codeFile.token.at(instruction + 2));
					break;
				case NAME:
					bytecode("LOAD_NAME", codeFile.token.at(instruction + 2));
					break;
				}
				bytecode("MOD", "0");
				instruction += 3;
				break;
			case NAME:
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
				bytecode("CALL", codeFile.token.at(instruction));
				instruction += 2;
				break;
			case CONSTANT_VALUE:
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
				bytecode("CALL", codeFile.token.at(instruction));
				instruction += 2;
				break;
			}
			break;
		case LEFT_CURLY_BRACE: {
			if (pastConditional == IF_STATEMENT) {
				if (isInConditional and currentComparisonOperator == "undefined") {
					bytecode("POP_BACK", "0");
					bytecode("LOAD_CONST", "1");
					bytecode("COMPARE", ">=");
					bytecode("JUMP_IF_FALSE", "0");
					std::vector<size_t> vec;
					jumpInstruction.push_back(vec);
					jumpInstruction.back().push_back(file.size() - 1);
					statementType.push_back(IF_STATEMENT);
					nameScope.push_back(0);
					isInConditional = false;
					++instruction;
					break;
				}
				if (isInConditional) {
					bytecode("POP_BACK", "0");
					bytecode("COMPARE", currentComparisonOperator);
					bytecode("JUMP_IF_FALSE", "0");
					std::vector<size_t> vec;
					jumpInstruction.push_back(vec);
					jumpInstruction.back().push_back(file.size() - 1);
					statementType.push_back(IF_STATEMENT);
					nameScope.push_back(0);
					isInConditional = false;
					++instruction;
					break;
				}
			}
			++instruction;
			break;
		}
		case RIGHT_CURLY_BRACE:
			for (size_t e = 0; e < nameScope.back(); ++e) {
				bytecode("POP_NAME", "0");
			}
			nameScope.pop_back();
			switch (statementType.back()) {
			case ELIF_STATEMENT:
				file.at(elseJump.back()) = std::to_string(file.size() - 2);
				elseJump.pop_back();
				if (codeFile.token.at(instruction + 1) == "else") {
					bytecode("JUMP", "0");
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
					bytecode("JUMP", "0");
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
				bytecode("JUMP", std::to_string(jumpInstruction.back().front() - 9));
				jumpInstruction.pop_back();
				instruction += 1;
				break;
			case FUNCTION_STATEMENT:
				bytecode("LOAD_CONST", "0");
				bytecode("RETURN_VALUE", "0");
				bytecode("END_FUNCTION", "0");
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
			bytecode("JUMP", std::to_string(jumpInstruction.back().front() - 9));
			jumpInstruction.pop_back();
			instruction += 1;
			break;
		case BREAK:
			bytecode("JUMP", "0");
			breakJump.push_back(file.size() - 1);
			++instruction;
			break;
		case CONTINUE:
			bytecode("JUMP", std::to_string(continueJump.back()));
			++instruction;
			break;
		case RETURN:
			isInReturnStatement = true;
			instruction += 1;
			break;
		case AMPERSAND:
			bytecode("LOAD_REF", codeFile.token.at(instruction + 1));
			bytecode("LOAD_BACK_REF", "0");
			instruction += 2;
			break;
		}
	}
}

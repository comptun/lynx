#include "interpreter.h"
#include "../bytecode/pcfunctions.h"
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
	define("&&", "and");
	define("||", "or");
	define("true", "1");
	define("false", "0");
	define("NULL", "0");
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
			codeFile.token.at(i).clear();
			codeFile.token.at(i + 1).clear();
			codeFile.token.at(i + 2).clear();
			codeFile.type.at(i).clear();
			codeFile.type.at(i + 1).clear();
			codeFile.type.at(i + 2).clear();
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
	nameScope.back() += 1;

	bytecode("LOAD_CONST", "0");
	bytecode("STORE_NAME", "__BYTECODE__");
	nameScope.back() += 1;

	for (size_t instruction = 0; instruction < codeFile.token.size();) {
		switch (getToken(codeFile.type.at(instruction))) {
		case ELSE:
			if (codeFile.token.at(instruction + 1) == "if") {
				/*++instruction;
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
				}*/
				statementType.push_back(ELIF_STATEMENT);
				bytecode("NEW_PARAM_STACK", "0");
				functionName.push_back("if");
				functionOperatorType.push_back("NULL");
				++functionDepth;
				std::vector<size_t> vec;
				jumpInstruction.push_back(vec);
				instruction += 3;
				//bytecode("JUMP_IF_FALSE", "0");
				//std::vector<size_t> vec;
				//jumpInstruction.push_back(vec);
				//jumpInstruction.back().push_back(file.size() - 1);
				//statementType.push_back(ELIF_STATEMENT);
				//nameScope.push_back(0);
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
			statementType.push_back(IF_STATEMENT);
			bytecode("NEW_PARAM_STACK", "0");
			functionName.push_back("if");
			functionOperatorType.push_back("NULL");
			++functionDepth;
			std::vector<size_t> vec;
			jumpInstruction.push_back(vec);
			instruction += 2;

			/*switch (getToken(codeFile.type.at(instruction + 1))) {
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
			statementType.push_back(IF_STATEMENT);
			nameScope.push_back(0);*/
			break;
		}
		case WHILE_LOOP: {
			continueJump.push_back(file.size() - 2);
			/*switch (getToken(codeFile.type.at(instruction + 1))) {
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
			break;*/
			statementType.push_back(WHILE_STATEMENT);
			std::vector<size_t> vec;
			jumpInstruction.push_back(vec);
			jumpInstruction.back().push_back(file.size() - 2);
			bytecode("NEW_PARAM_STACK", "0");
			functionName.push_back("while");
			functionOperatorType.push_back("NULL");
			++functionDepth;
			instruction += 2;
			break;
		}
		case AND:
			/*switch (getToken(codeFile.type.at(instruction + 1))) {
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
			break;*/

			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(AND_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		case OR:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(OR_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		case XOR:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(XOR_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
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
				if (codeFile.token.at(instruction - 1) != "(") {
					bytecode("POP_BACK", "0");
					bytecode("STORE_PARAM", "0");
					bytecode("POP_BACK", "0");
				}
				bytecode("CALL", functionName.back());
				if (functionName.size() > 1 or isInNameAssignment) {
					bytecode("LOAD_RETURN_VALUE", "0");
					bytecode("LOAD_BACK_REF", "0");
					if (functionOperatorType.back() != "NULL") {
						bytecode("POP_BACK", "0");
						bytecode(functionOperatorType.back(), "0");
						bytecode("POP_BACK", "0");
					}
				}
				if (functionName.back() == "if" or functionName.back() == "while") {
					bytecode("LOAD_RETURN_VALUE", "0");
					bytecode("LOAD_CONST", "1");
					bytecode("COMPARE", "==");
					bytecode("JUMP_IF_FALSE", "0");
					jumpInstruction.back().push_back(file.size() - 1);
					nameScope.push_back(0);
				}
				functionName.pop_back();
				functionOperatorType.pop_back();
				--functionDepth;
			}
			isInFunctionDefinition = false;
			++instruction;
			break;
		case PLUS:
			if (getToken(codeFile.type.at(instruction + 1)) == NAME) {
				if (codeFile.token.at(instruction + 2) == "(") {
					bytecode("NEW_PARAM_STACK", "0");
					functionName.push_back(codeFile.token.at(instruction + 1));
					++functionDepth;
					functionOperatorType.push_back("ADD");
					instruction += 3;
					break;
				}
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
			}
			else if (getToken(codeFile.type.at(instruction + 1)) == CONSTANT_VALUE) {
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
			}
			bytecode("ADD", "0");
			bytecode("POP_BACK", "0");
			instruction += 2;
			break;
		case MINUS:
			if (getToken(codeFile.type.at(instruction + 1)) == NAME) {
				if (codeFile.token.at(instruction + 2) == "(") {
					bytecode("NEW_PARAM_STACK", "0");
					functionName.push_back(codeFile.token.at(instruction + 1));
					++functionDepth;
					functionOperatorType.push_back("SUB");
					instruction += 3;
					break;
				}
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
			}
			else if (getToken(codeFile.type.at(instruction + 1)) == CONSTANT_VALUE) {
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
			}
			bytecode("SUB", "0");
			bytecode("POP_BACK", "0");
			instruction += 2;
			break;
		case MULTIPLY:
			if (getToken(codeFile.type.at(instruction + 1)) == NAME) {
				if (codeFile.token.at(instruction + 2) == "(") {
					bytecode("NEW_PARAM_STACK", "0");
					functionName.push_back(codeFile.token.at(instruction + 1));
					++functionDepth;
					functionOperatorType.push_back("MUL");
					instruction += 3;
					break;
				}
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
			}
			else if (getToken(codeFile.type.at(instruction + 1)) == CONSTANT_VALUE) {
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
			}
			bytecode("MUL", "0");
			bytecode("POP_BACK", "0");
			instruction += 2;
			break;
		case DIVIDE:
			if (getToken(codeFile.type.at(instruction + 1)) == NAME) {
				if (codeFile.token.at(instruction + 2) == "(") {
					bytecode("NEW_PARAM_STACK", "0");
					functionName.push_back(codeFile.token.at(instruction + 1));
					++functionDepth;
					functionOperatorType.push_back("DIV");
					instruction += 3;
					break;
				}
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
			}
			else if (getToken(codeFile.type.at(instruction + 1)) == CONSTANT_VALUE) {
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
			}
			bytecode("DIV", "0");
			bytecode("POP_BACK", "0");
			instruction += 2;
			break;
		case MODULUS:
			if (getToken(codeFile.type.at(instruction + 1)) == NAME) {
				if (codeFile.token.at(instruction + 2) == "(") {
					bytecode("NEW_PARAM_STACK", "0");
					functionName.push_back(codeFile.token.at(instruction + 1));
					++functionDepth;
					functionOperatorType.push_back("MOD");
					instruction += 3;
					break;
				}
				bytecode("LOAD_NAME", codeFile.token.at(instruction + 1));
			}
			else if (getToken(codeFile.type.at(instruction + 1)) == CONSTANT_VALUE) {
				bytecode("LOAD_CONST", codeFile.token.at(instruction + 1));
			}
			bytecode("MOD", "0");
			bytecode("POP_BACK", "0");
			instruction += 2;
			break;
		case CONSTANT_VALUE:
			bytecode("LOAD_CONST", codeFile.token.at(instruction));
			bytecode("LOAD_BACK_REF", "0");
			instruction += 1;
			if (isInReturnStatement) {
				if (codeFile.token.at(instruction) != "+" 
					and codeFile.token.at(instruction) != "-" 
					and codeFile.token.at(instruction) != "*" 
					and codeFile.token.at(instruction) != "/"
					and codeFile.token.at(instruction) != "%") {
					bytecode("POP_BACK", "0");
					bytecode("RETURN_VALUE", "0");
					bytecode("POP_BACK", "0");
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
				if (codeFile.token.at(instruction) != "+" 
					and codeFile.token.at(instruction) != "-" 
					and codeFile.token.at(instruction) != "*" 
					and codeFile.token.at(instruction) != "/"
					and codeFile.token.at(instruction) != "%") {
					bytecode("POP_BACK", "0");
					bytecode("RETURN_VALUE", "0");
					isInReturnStatement = false;
				}
				break;
			}
			/*if (codeFile.token.at(instruction + 1) == "(" and codeFile.token.at(instruction + 2) == ")") {
				bytecode("CALL", codeFile.token.at(instruction));
				bytecode("LOAD_RETURN_VALUE", "0");
				bytecode("LOAD_BACK_REF", "0");
				--functionDepth;
				instruction += 3;
				break;
			}*/
			if (codeFile.token.at(instruction + 1) == "(") {
				bytecode("NEW_PARAM_STACK", "0");
				functionName.push_back(codeFile.token.at(instruction));
				functionOperatorType.push_back("NULL");
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
				knownNames.push_back(codeFile.token.at(instruction));
				bytecode("LOAD_PARAM", "0");
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				nameScope.back() += 1;
				instruction += 2;
				break;
			}
			if (codeFile.token.at(instruction + 1) == ")" and isInFunctionDefinition) {
				knownNames.push_back(codeFile.token.at(instruction));
				bytecode("LOAD_PARAM", "0");
				bytecode("STORE_NAME", codeFile.token.at(instruction));
				bytecode("POP_PARAM_STACK", "0");
				nameScope.back() += 1;
				instruction += 1;
				break;
			}
			if (codeFile.token.at(instruction + 1) == "," 
				or codeFile.token.at(instruction + 1) == ")" 
				or codeFile.token.at(instruction + 1) == "+" 
				or codeFile.token.at(instruction + 1) == "-" 
				or codeFile.token.at(instruction + 1) == "*" 
				or codeFile.token.at(instruction + 1) == "/"
				or codeFile.token.at(instruction + 1) == "%"
				or codeFile.token.at(instruction + 1) == "=="
				or codeFile.token.at(instruction + 1) == "!="
				or codeFile.token.at(instruction + 1) == ">="
				or codeFile.token.at(instruction + 1) == "<="
				or codeFile.token.at(instruction + 1) == ">"
				or codeFile.token.at(instruction + 1) == "<"
				or codeFile.token.at(instruction + 1) == "and"
				or codeFile.token.at(instruction + 1) == "or"
				or codeFile.token.at(instruction + 1) == "xor") {
				bytecode("LOAD_NAME", codeFile.token.at(instruction));
				bytecode("LOAD_BACK_REF", "0");
				instruction += 1;
				break;
			}
			switch (getToken(codeFile.type.at(instruction + 1))) {
			case EQUALS:
				if (getToken(codeFile.type.at(instruction + 2)) == CONSTANT_VALUE) {
					bytecode("LOAD_CONST", codeFile.token.at(instruction + 2));
				}
				else if (getToken(codeFile.type.at(instruction + 2)) == NAME) {
					if (codeFile.token.at(instruction + 3) == "(") {
						bytecode("NEW_PARAM_STACK", "0");
						functionName.push_back(codeFile.token.at(instruction + 2));
						++functionDepth;
						functionOperatorType.push_back("NULL");
						currentName = codeFile.token.at(instruction);
						isInNameAssignment = true;
						instruction += 4;
						break;
					}
					bytecode("LOAD_NAME", codeFile.token.at(instruction + 2));
				}
				if (nameExists(codeFile.token.at(instruction)) == false)
						nameScope.back() += 1;
				if (codeFile.token.at(instruction + 3) != "+" 
					and codeFile.token.at(instruction + 3) != "-" 
					and codeFile.token.at(instruction + 3) != "*" 
					and codeFile.token.at(instruction + 3) != "/"
					and codeFile.token.at(instruction + 3) != "%") {
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
			
			++instruction;
			break;
		}
		case RIGHT_CURLY_BRACE:
			switch (statementType.back()) {
			case ELIF_STATEMENT:
				for (size_t e = 0; e < nameScope.back(); ++e) {
					knownNames.pop_back();
					bytecode("POP_NAME", "0");
				}
				nameScope.pop_back();
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
				for (size_t e = 0; e < nameScope.back(); ++e) {
					knownNames.pop_back();
					bytecode("POP_NAME", "0");
				}
				nameScope.pop_back();
				file.at(elseJump.back()) = std::to_string(file.size() - 2);
				elseJump.pop_back();
				instruction += 1;
				break;
			case IF_STATEMENT:
				for (size_t e = 0; e < nameScope.back(); ++e) {
					knownNames.pop_back();
					bytecode("POP_NAME", "0");
				}
				nameScope.pop_back();
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

				file.at(jumpInstruction.back().at(1)) = std::to_string(file.size());

				for (size_t a = 0; a < breakJump.size(); ++a) {
					file.at(breakJump.at(a)) = std::to_string(file.size());
				}
				continueJump.pop_back();
				breakJump.clear();
				bytecode("JUMP", std::to_string(jumpInstruction.back().at(0)));
				jumpInstruction.pop_back();
				for (size_t e = 0; e < nameScope.back(); ++e) {
					knownNames.pop_back();
					bytecode("POP_NAME", "0");
				}
				nameScope.pop_back();
				instruction += 1;
				break;
			case FUNCTION_STATEMENT:
				for (size_t e = 0; e < nameScope.back(); ++e) {
					knownNames.pop_back();
					bytecode("POP_NAME", "0");
				}
				nameScope.pop_back();
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
		case SEMI_COLON:
			if (isInNameAssignment) {
				bytecode("POP_BACK", "0");
				bytecode("STORE_NAME", currentName);
				isInNameAssignment = false;
				break;
			}
			if (isInReturnStatement) {
				bytecode("POP_BACK", "0");
				bytecode("RETURN_VALUE", "0");
				isInReturnStatement = false;
				break;
			}
			isInNameAssignment = false;
			isInReturnStatement = false;
			instruction += 1;
			break;
		case EQUAL_TO:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(EQUAL_TO_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		case NOT_EQUAL_TO:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(NOT_EQUAL_TO_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		case LESS_THAN_EQUAL_TO:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(LESS_THAN_EQUAL_TO_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		case GREATER_THAN_EQUAL_TO:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(GREATER_THAN_EQUAL_TO_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		case LESS_THAN:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(LESS_THAN_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		case GREATER_THAN:
			bytecode("POP_BACK", "0");
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			bytecode("LOAD_CONST", std::to_string(GREATER_THAN_EXPR));
			bytecode("STORE_PARAM", "0");
			bytecode("POP_BACK", "0");
			instruction += 1;
			break;
		}
	}
}
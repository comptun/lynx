#include "byteinterpreter.h"
#include "opcodes.h"

std::vector<std::string> opcodes = {
	"LOAD_CONST",
	"LOAD_REF",
	"STORE_REF",
	"LOAD_NAME",
	"STORE_NAME",
	"COMPARE",
	"JUMP_IF_FALSE",
	"JUMP_IF_TRUE",
	"JUMP",
	"CALL",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"START_FUNCTION",
	"END_FUNCTION",
	"LOAD_ARRAY",
	"LOAD_ARRAY_REF",
	"STORE_ARRAY",
	"PUSH_ARRAY",
	"POP_BACK",
	"LOAD_PARAM",
	"STORE_PARAM",
	"DELETE",
	"POP_NAME",
	"SWITCH_STATEMENT",
	"LOAD_VAR_PARAM",
	"STORE_VAR_PARAM",
	"LOAD_BACK_REF",
	"RETURN_VALUE",
	"LOAD_RETURN_VALUE",
	"NEW_PARAM_STACK",
	"POP_PARAM_STACK",
};

unsigned int ByteInterpreter::getOpcode(std::string code)
{
	for (int i = 0; i < opcodes.size(); ++i) {
		if (opcodes.at(i) == code) {
			return i;
		}
	}
	return 404; // 404, opcode not found!
}
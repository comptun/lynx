#include "byteinterpreter.h"
#include "opcodes.h"

std::vector<std::string> opcodes = {
	"LOAD_CONST",
	"LOAD_NAME",
	"STORE_NAME",
	"COMPARE",
	"JUMP_IF_FALSE",
	"JUMP_IF_TRUE",
	"JUMP",
	"RETURN",
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
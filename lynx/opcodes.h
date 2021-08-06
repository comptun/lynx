#pragma once

enum opcodeIndex {
	LOAD_CONST,
	LOAD_REF,
	STORE_REF,
	LOAD_NAME,
	STORE_NAME,
	COMPARE,
	JUMP_IF_FALSE,
	JUMP_IF_TRUE,
	JUMP,
	CALL,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	START_FUNCTION,
	END_FUNCTION,
	RETURN,
	LOAD_ARRAY,
	LOAD_ARRAY_REF,
	STORE_ARRAY,

	ERROR = 404,
};
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "bytecode/pcfunctions.h"
#include "bytecode/byteinterpreter.h"
#include "interpreter/interpreter.h"

class Lynx : public ByteInterpreter, public Interpreter {
public:
	Lynx();
};

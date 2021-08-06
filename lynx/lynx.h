#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "pcfunctions.h"
#include "byteinterpreter.h"
#include "translator.h"

class Lynx : public ByteInterpreter, public Translator {
public:
	Lynx();
};

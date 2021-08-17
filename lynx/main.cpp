#include "lynx.h"
#include "bytecode/byteinterpreter.h"
#include "main.h"
#include <time.h>

Lynx lynx;

int main()
{
	srand(time(NULL));
	lynx.readCode(std::ifstream("code.txt"));
	lynx.readFile(std::ifstream("bytecode.txt"));
	lynx.interpret();
	std::cin.get();
}
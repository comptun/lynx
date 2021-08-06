#include "lynx.h"
#include "byteinterpreter.h"
#include "main.h"
#include <time.h>

Lynx lynx;

int main()
{
	srand(time(NULL));
	lynx.readFile(std::ifstream("bytecode.txt"));
	lynx.interpret();
	std::cin.get();
}
#include "lynx.h"
#include "bytecode/byteinterpreter.h"
#include "main.h"
#include <time.h>

Lynx lynx;

int main()
{
	srand(time(NULL));
	lynx.readCode(std::ifstream("code.txt"));
	/*for (int i = 0; i < lynx.codeFile.indents.size(); ++i) {
		std::cout << lynx.codeFile.indents.at(i) << " ";
	}*/
	//lynx.readFile(std::ifstream("bytecode.txt"));
	/*lynx.translate();
	for (size_t i = 0; i < file.size(); i += 2) {
		std::cout << file.at(i) << " " << file.at(i + 1) << std::endl;
	}*/
	lynx.translate();
	lynx.interpret();
	std::cin.get();
}
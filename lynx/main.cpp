#include "lynx.h"
#include "bytecode/byteinterpreter.h"
#include "main.h"
#include <random>
#include <ctime>

Lynx lynx;

int main()
{
	std::srand(std::random_device{}());
	lynx.readCode(std::ifstream("code.txt"));
	/*lynx.readFile(std::ifstream("bytecode.txt"));
	lynx.interpret();*/

	/*lynx.translate();
	for (size_t i = 0; i < file.size(); i += 2) {
		std::cout << i << " " << file.at(i) << " " << file.at(i + 1) << std::endl;
	}*/

	/*for (size_t i = 0; i < lynx.codeFile.token.size(); ++i) {
		std::cout << "\"" << lynx.codeFile.token.at(i) << "\"" << " : " << lynx.codeFile.type.at(i) << std::endl;
	}*/

	//lynx.translate();

	lynx.translate();
	lynx.interpret();

	/*std::cout << "Stack: ";
	for (size_t i = 0; i < lynx.stack.size(); ++i) {
		std::cout << lynx.stack.at(i) << " ";
	}*/

	std::cin.get();
}
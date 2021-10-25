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
	
	/*lynx.translate();
	for (size_t i = 0; i < lynx.codeFile.token.size(); ++i) {
		std::cout << "\"" << lynx.codeFile.token.at(i) << "\"" << " : " << lynx.codeFile.type.at(i) << std::endl;
	}*/

	//lynx.translate();
	lynx.translate();
	lynx.interpret();
	if (std::get<long long int>(lynx.stack.at(lynx.names.reference.at(lynx.getNameReference("__BYTECODE__")))) == true) {
		std::cout << "\nCOMPILED BYTECODE:\n";
		for (size_t i = 0; i < bcfile.file.size(); i += 2) {
			std::cout << i << " " << bcfile.file.at(i) << " " << bcfile.file.at(i + 1) << std::endl;
		}
	}

	/*std::cout << "Stack: ";
	for (size_t i = 0; i < lynx.stack.size(); ++i) {
		if (std::holds_alternative<long long int>(lynx.stack.at(i)))
			std::cout << std::get<long long int>(lynx.stack.at(i)) << " ";
		if (std::holds_alternative<long double>(lynx.stack.at(i)))
			std::cout << std::get<long double>(lynx.stack.at(i)) << " ";
		if (std::holds_alternative<std::string>(lynx.stack.at(i)))
			std::cout << "\"" << std::get<std::string>(lynx.stack.at(i)) << "\" ";
	}*/

	//std::cin.get();
}
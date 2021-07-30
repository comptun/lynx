#include "lynx.h"
#include "byteinterpreter.h"

int main()
{
	Lynx lynx;
	lynx.readFile(std::ifstream("bytecode.txt"));
	for (size_t i = 0; i < lynx.file.size(); ++i) {
		std::cout << lynx.file.at(i) << " ";
	}
	std::cin.get();
}
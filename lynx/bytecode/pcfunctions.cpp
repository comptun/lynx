#include "pcfunctions.h"
#include "byteinterpreter.h"

#include <vector>
#include <math.h>

std::vector<std::string> PCFnames = {
    "print",
    "endl",
    "rand",
    "swap",
    "sqrt",
    "pow",
    "putc",
    "input",
};

void ByteInterpreter::executePCF(std::string funcName)
{
    size_t i = 0;
    for (; i < PCFnames.size(); ++i) {
        if (PCFnames.at(i) == funcName)
            break;
    }  
    switch (i) {
    case PRINT:
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            print(paramStack.back().at(i));
        }
        break;
    case ENDL:
        std::cout << std::endl;
        break;
    case RAND:
        stack.push_back(rand() % stack.back());
        break;
    case SWAP: {
        int item1 = stack.at(stack.at(stack.size() - 2));
        int item2 = stack.at(stack.at(stack.size() - 1));
        stack.at(stack.at(stack.size() - 2)) = item2;
        stack.at(stack.at(stack.size() - 1)) = item1;
        break;
    }
    case SQRT:
        stack.push_back(sqrt(stack.back()));
        break;
    case POW:
        stack.push_back(pow(stack.at(stack.size() - 2), stack.at(stack.size() - 1)));
        break;
    case PUTC:
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            std::cout << static_cast<char>(paramStack.back().at(i));
        }
        break;
    case INPUT:
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            std::cin >> stack.at(paramStack.back().at(i));
        }
        break;
    }
    paramStack.pop_back();
}
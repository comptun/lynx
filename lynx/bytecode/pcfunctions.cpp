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
    "max",
    "min",
    "sin",
    "cos",
    "tan",
    "ln",
    "isprime",
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
            std::cout << paramStack.back().at(i);
        }
        break;
    case ENDL:
        std::cout << std::endl;
        break;
    case RAND: {
        int difference = paramStack.back().at(1) - paramStack.back().at(0);
        returnedValue = rand() % difference + paramStack.back().at(0);
        break;
    }
    case SWAP: {
        int item1 = stack.at(paramStack.back().at(0));
        int item2 = stack.at(paramStack.back().at(1));
        stack.at(paramStack.back().at(1)) = item2;
        stack.at(paramStack.back().at(0)) = item1;
        break;
    }
    case SQRT:
        returnedValue = sqrt(paramStack.back().at(0));
        break;
    case POW:
        returnedValue = pow(paramStack.back().at(0), paramStack.back().at(1));
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
    case MAX:
        returnedValue = (paramStack.back().at(0) > paramStack.back().at(1) ? paramStack.back().at(0) : paramStack.back().at(1));
        break;
    case MIN:
        returnedValue = (paramStack.back().at(1) > paramStack.back().at(0) ? paramStack.back().at(0) : paramStack.back().at(1));
        break;
    case SIN:
        returnedValue = sin(paramStack.back().at(0));
        break;
    case COS:
        returnedValue = cos(paramStack.back().at(0));
        break;
    case TAN:
        returnedValue = tan(paramStack.back().at(0));
        break;
    case LN:
        returnedValue = log(paramStack.back().at(0));
        break;
    case ISPRIME: {
        bool isPrime = true;
        for (int a = 2; a < sqrt(paramStack.back().at(0)) + 1; ++a) {
            if (paramStack.back().at(0) % a == 0) {
                isPrime = false;
                returnedValue = false;
                break;
            }
        }
        if (isPrime) {
            returnedValue = true;
        }
        break;
    }
    }
    paramStack.pop_back();
}
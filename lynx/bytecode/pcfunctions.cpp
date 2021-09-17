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
            if (std::holds_alternative<int>(paramStack.back().at(i)))
                std::cout << std::get<int>(paramStack.back().at(i));
            else if (std::holds_alternative<double>(paramStack.back().at(i)))
                std::cout << std::get<double>(paramStack.back().at(i));
            else if (std::holds_alternative<std::string>(paramStack.back().at(i)))
                std::cout << std::get<std::string>(paramStack.back().at(i));
        }
        break;
    case ENDL:
        std::cout << "\n";
        break;
    case RAND: {
        int difference = std::get<int>(paramStack.back().at(1)) - std::get<int>(paramStack.back().at(0));
        returnedValue = rand() % difference + std::get<int>(paramStack.back().at(0));
        break;
    }
    case SWAP: {
        int item1 = std::get<int>(stack.at(std::get<int>(paramStack.back().at(0))));
        int item2 = std::get<int>(stack.at(std::get<int>((paramStack.back().at(1)))));
        stack.at(std::get<int>(paramStack.back().at(1))) = item2;
        stack.at(std::get<int>(paramStack.back().at(0))) = item1;
        break;
    }
    case SQRT:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = sqrt(std::get<int>(paramStack.back().at(0)));
        if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = sqrt(std::get<double>(paramStack.back().at(0)));
        break;
    case POW:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = pow(std::get<int>(paramStack.back().at(0)), std::get<int>(paramStack.back().at(1)));
        if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = pow(std::get<int>(paramStack.back().at(0)), std::get<int>(paramStack.back().at(1)));
        break;
    case PUTC:
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            std::cout << static_cast<char>(std::get<int>(paramStack.back().at(i)));
        }
        break;
    case INPUT:
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            std::cin >> std::get<int>(stack.at(std::get<int>(paramStack.back().at(i))));
        }
        break;
    case MAX:
        returnedValue = (paramStack.back().at(0) > paramStack.back().at(1) ? paramStack.back().at(0) : paramStack.back().at(1));
        break;
    case MIN:
        returnedValue = (paramStack.back().at(1) > paramStack.back().at(0) ? paramStack.back().at(0) : paramStack.back().at(1));
        break;
    case SIN:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = sin(std::get<int>(paramStack.back().at(0)));
        if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = sin(std::get<double>(paramStack.back().at(0)));
        break;
    case COS:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = cos(std::get<int>(paramStack.back().at(0)));
        if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = cos(std::get<double>(paramStack.back().at(0)));
        break;
    case TAN:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = tan(std::get<int>(paramStack.back().at(0)));
        if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = tan(std::get<double>(paramStack.back().at(0)));
        break;
    case LN:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = log(std::get<int>(paramStack.back().at(0)));
        if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = log(std::get<double>(paramStack.back().at(0)));
        break;
    case ISPRIME: {
        bool isPrime = true;
        double sqrtNum = 0;
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            sqrtNum = sqrt(std::get<int>(paramStack.back().at(0)));
        if (std::holds_alternative<double>(paramStack.back().at(0)))
            sqrtNum = sqrt(std::get<double>(paramStack.back().at(0)));
        for (int a = 2; a < sqrtNum; ++a) {
            if (std::get<int>(paramStack.back().at(0)) % a == 0) {
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
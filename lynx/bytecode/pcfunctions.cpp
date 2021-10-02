#include "pcfunctions.h"
#include "byteinterpreter.h"

#include <vector>
#include <math.h>
#include <bits/stdc++.h>
#include <conio.h>

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
    "int",
    "float",
    "string",
    "char",
    "len",
    "reverse",
    "sort",
    "asin",
    "acos",
    "atan",
    "keyPressed",
    "getKeyPressed",
    "parenthesis",
    "ceil",
    "floor",
    "abs",
    "boolexpr",
    "if",
    "while",
};

void ByteInterpreter::executePCF(std::string funcName)
{
    size_t i = 0;
    for (; i < PCFnames.size(); ++i) {
        if (PCFnames.at(i) == funcName)
            break;
    }  
    switch (i) {
    case PRINT: {
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            if (std::holds_alternative<int>(paramStack.back().at(i)))
                std::cout << std::get<int>(paramStack.back().at(i));
            else if (std::holds_alternative<double>(paramStack.back().at(i)))
                std::cout << std::get<double>(paramStack.back().at(i));
            else if (std::holds_alternative<std::string>(paramStack.back().at(i))) {
                bool isInVariableCall = false;
                std::string varName;
                for (size_t j = 0; j < std::get<std::string>(paramStack.back().at(i)).size();) {
                    if (isInVariableCall) {
                        if (std::get<std::string>(paramStack.back().at(i)).at(j) == '}') {
                            if (std::holds_alternative<int>(stack.at(getNameReference(varName))))
                                std::cout << std::get<int>(stack.at(getNameReference(varName)));
                            else if (std::holds_alternative<double>(stack.at(getNameReference(varName))))
                                std::cout << std::get<double>(stack.at(getNameReference(varName)));
                            else if (std::holds_alternative<std::string>(stack.at(getNameReference(varName))))
                                std::cout << std::get<std::string>(stack.at(getNameReference(varName)));
                            varName.clear();
                            ++j;
                            isInVariableCall = false;
                            continue;
                        }
                        varName += std::get<std::string>(paramStack.back().at(i)).at(j);
                        ++j;
                        continue;
                    }
                    if (std::get<std::string>(paramStack.back().at(i)).at(j) == '{') {
                        isInVariableCall = true;
                        ++j;
                        continue;
                    }
                    if (std::get<std::string>(paramStack.back().at(i)).at(j) == '\\'
                        and std::get<std::string>(paramStack.back().at(i)).at(j + 1) == 'n') {
                        std::cout << "\n";
                        j += 2;
                        continue;
                    }
                    std::cout << std::get<std::string>(paramStack.back().at(i)).at(j);
                    ++j;
                }
            }
        }
        break;
    }
    case ENDL:
        returnedValue = "\n";
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
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = sqrt(std::get<double>(paramStack.back().at(0)));
        break;
    case POW:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = pow(std::get<int>(paramStack.back().at(0)), std::get<int>(paramStack.back().at(1)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = pow(std::get<int>(paramStack.back().at(0)), std::get<int>(paramStack.back().at(1)));
        break;
    case PUTC:
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            std::cout << static_cast<char>(std::get<int>(paramStack.back().at(i)));
        }
        break;
    case INPUT: {
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            if (std::holds_alternative<int>(paramStack.back().at(i)))
                std::cout << std::get<int>(paramStack.back().at(i));
            else if (std::holds_alternative<double>(paramStack.back().at(i)))
                std::cout << std::get<double>(paramStack.back().at(i));
            else if (std::holds_alternative<std::string>(paramStack.back().at(i))) {
                bool isInVariableCall = false;
                std::string varName;
                for (size_t j = 0; j < std::get<std::string>(paramStack.back().at(i)).size();) {
                    if (isInVariableCall) {
                        if (std::get<std::string>(paramStack.back().at(i)).at(j) == '}') {
                            if (std::holds_alternative<int>(stack.at(getNameReference(varName))))
                                std::cout << std::get<int>(stack.at(getNameReference(varName)));
                            else if (std::holds_alternative<double>(stack.at(getNameReference(varName))))
                                std::cout << std::get<double>(stack.at(getNameReference(varName)));
                            else if (std::holds_alternative<std::string>(stack.at(getNameReference(varName))))
                                std::cout << std::get<std::string>(stack.at(getNameReference(varName)));
                            varName.clear();
                            ++j;
                            isInVariableCall = false;
                            continue;
                        }
                        varName += std::get<std::string>(paramStack.back().at(i)).at(j);
                        ++j;
                        continue;
                    }
                    if (std::get<std::string>(paramStack.back().at(i)).at(j) == '{') {
                        isInVariableCall = true;
                        ++j;
                        continue;
                    }
                    if (std::get<std::string>(paramStack.back().at(i)).at(j) == '\\'
                        and std::get<std::string>(paramStack.back().at(i)).at(j + 1) == 'n') {
                        std::cout << "\n";
                        j += 2;
                        continue;
                    }
                    std::cout << std::get<std::string>(paramStack.back().at(i)).at(j);
                    ++j;
                }
            }
        }
        std::string input;
        std::cin >> input;
        returnedValue = input;
        break;
    }
    case MAX:
        returnedValue = (paramStack.back().at(0) > paramStack.back().at(1) ? paramStack.back().at(0) : paramStack.back().at(1));
        break;
    case MIN:
        returnedValue = (paramStack.back().at(1) > paramStack.back().at(0) ? paramStack.back().at(0) : paramStack.back().at(1));
        break;
    case SIN:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = sin(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = sin(std::get<double>(paramStack.back().at(0)));
        break;
    case COS:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = cos(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = cos(std::get<double>(paramStack.back().at(0)));
        break;
    case TAN:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = tan(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = tan(std::get<double>(paramStack.back().at(0)));
        break;
    case LN:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = log(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = log(std::get<double>(paramStack.back().at(0)));
        break;
    case ISPRIME: {
        bool isPrime = true;
        double sqrtNum = 0;
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            sqrtNum = sqrt(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
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
    case INT_CAST:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = paramStack.back().at(0);
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = static_cast<int>(std::get<double>(paramStack.back().at(0)));
        else if (std::holds_alternative<std::string>(paramStack.back().at(0)))
            returnedValue = std::stoi(std::get<std::string>(paramStack.back().at(0)));
        break;
    case FLOAT_CAST:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = static_cast<double>(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = paramStack.back().at(0);
        else if (std::holds_alternative<std::string>(paramStack.back().at(0)))
            returnedValue = std::stod(std::get<std::string>(paramStack.back().at(0)));
        break;
    case STRING_CAST:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = std::to_string(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = std::to_string(std::get<double>(paramStack.back().at(0)));
        else if (std::holds_alternative<std::string>(paramStack.back().at(0)))
            returnedValue = paramStack.back().at(0);
        break;
    case CHAR_CAST:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = std::string(1, static_cast<char>(std::get<int>(paramStack.back().at(0))));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = std::string(1, static_cast<char>(std::get<double>(paramStack.back().at(0))));
        break;
    case LEN:
        returnedValue = static_cast<int>(std::get<std::string>(paramStack.back().at(0)).size());
        break;
    case REVERSE: {
        std::string revString = std::get<std::string>(paramStack.back().at(0));
        std::reverse(revString.begin(), revString.end());
        returnedValue = revString;
        break;
    }
    case SORT: {
        std::string sortString = std::get<std::string>(paramStack.back().at(0));
        std::sort(sortString.begin(), sortString.end());
        returnedValue = sortString;
        break;
    }
    case ASIN:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = asin(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = asin(std::get<double>(paramStack.back().at(0)));
        break;
    case ACOS:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = acos(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = acos(std::get<double>(paramStack.back().at(0)));
        break;
    case ATAN:
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            returnedValue = atan(std::get<int>(paramStack.back().at(0)));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            returnedValue = atan(std::get<double>(paramStack.back().at(0)));
        break;
    case KEYPRESSED: {
        char key = _getch();
        char keypressed = ' ';
        if (std::holds_alternative<int>(paramStack.back().at(0)))
            keypressed = std::get<int>(paramStack.back().at(0));
        else if (std::holds_alternative<double>(paramStack.back().at(0)))
            keypressed = std::get<int>(paramStack.back().at(0));
        else if (std::holds_alternative<std::string>(paramStack.back().at(0)))
            keypressed = std::get<std::string>(paramStack.back().at(0)).at(0);
        returnedValue = key == keypressed;
        break;
    }
    case GETKEYPRESSED:
        returnedValue = _getch();
        break;
    case PARENTHESIS: {
        if (paramStack.back().size() > 1) {
            std::vector<int> firstRound;
            std::vector<int> secondRound;
            for (size_t j = 0; j < paramStack.back().size();) {
                switch (std::get<int>(paramStack.back().at(j + 1))) {
                case EQUAL_TO_EXPR:
                    if (paramStack.back().at(j) == paramStack.back().at(j + 2)) {
                        firstRound.push_back(true);
                        break;
                    }
                    firstRound.push_back(false);
                    break;
                case NOT_EQUAL_TO_EXPR:
                    if (paramStack.back().at(j) != paramStack.back().at(j + 2)) {
                        firstRound.push_back(true);
                        break;
                    }
                    firstRound.push_back(false);
                    break;
                case GREATER_THAN_EQUAL_TO_EXPR:
                    if (paramStack.back().at(j) >= paramStack.back().at(j + 2)) {
                        firstRound.push_back(true);
                        break;
                    }
                    firstRound.push_back(false);
                    break;
                case LESS_THAN_EQUAL_TO_EXPR:
                    if (paramStack.back().at(j) <= paramStack.back().at(j + 2)) {
                        firstRound.push_back(true);
                        break;
                    }
                    firstRound.push_back(false);
                    break;
                }
                if (j + 3 < paramStack.back().size() - 1) {
                    firstRound.push_back(std::get<int>(paramStack.back().at(j + 3)));
                    j += 4;
                    continue;
                }
                j += 3;
            }
            if (firstRound.size() > 1) {
                for (size_t j = 0; j < firstRound.size() - 1; j += 2) {
                    switch (firstRound.at(j + 1)) {
                    case AND_EXPR:
                        if (firstRound.at(j) and firstRound.at(j + 2)) {
                            secondRound.push_back(true);
                            break;
                        }
                        secondRound.push_back(false);
                        break;
                    case OR_EXPR:
                        if (firstRound.at(j) or firstRound.at(j + 2)) {
                            secondRound.push_back(true);
                            break;
                        }
                        secondRound.push_back(false);
                        break;
                    case XOR_EXPR:
                        if (firstRound.at(j) xor firstRound.at(j + 2)) {
                            secondRound.push_back(true);
                            break;
                        }
                        secondRound.push_back(false);
                        break;
                    }
                }
            }
            else {
                secondRound.push_back(firstRound.at(0));
            }
            for (size_t j = 0; j < secondRound.size(); j += 1) {
                if (secondRound.at(j) == false) {
                    returnedValue = false;
                    paramStack.pop_back();
                    return;
                }
            }
            returnedValue = true;
            break;
        }
        returnedValue = paramStack.back().at(0);
        break;
    }
    /*case CEIL:
        returnedValue = ceil(std::get<float>(paramStack.back().at(0)));
        break;
    case FLOOR:
        returnedValue = floor(std::get<float>(paramStack.back().at(0)));
        break;
    case ABS:
        if (std::holds_alternative<int>(paramStack.back().at(0))) {
            returnedValue = abs(std::get<float>(paramStack.back().at(0)));
        }
        else if (std::holds_alternative<float>(paramStack.back().at(0))) {
            returnedValue = fabs(std::get<float>(paramStack.back().at(0)));
        }
        break;*/
    case BOOLEXPR: {
        std::vector<int> firstRound;
        std::vector<int> secondRound;
        for (size_t j = 0; j < paramStack.back().size();) {
            switch (std::get<int>(paramStack.back().at(j + 1))) {
            case EQUAL_TO_EXPR:
                if (paramStack.back().at(j) == paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case NOT_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) != paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case GREATER_THAN_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) >= paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case LESS_THAN_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) <= paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case GREATER_THAN_EXPR:
                if (paramStack.back().at(j) > paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case LESS_THAN_EXPR:
                if (paramStack.back().at(j) < paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            }
            if (j + 3 < paramStack.back().size() - 1) {
                firstRound.push_back(std::get<int>(paramStack.back().at(j + 3)));
                j += 4;
                continue;
            }
            j += 3;
        }
        if (firstRound.size() > 1) {
            for (size_t j = 0; j < firstRound.size() - 1; j += 2) {
                switch (firstRound.at(j + 1)) {
                case AND_EXPR:
                    if (firstRound.at(j) and firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                case OR_EXPR:
                    if (firstRound.at(j) or firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                case XOR_EXPR:
                    if (firstRound.at(j) xor firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                }
            }
        }
        else {
            secondRound.push_back(firstRound.at(0));
        }
        for (size_t j = 0; j < secondRound.size(); j += 1) {
            if (secondRound.at(j) == false) {
                returnedValue = false;
                paramStack.pop_back();
                return;
            }
        }
        returnedValue = true;
        break;
    }
    case IF_EXPR: {
        std::vector<int> firstRound;
        std::vector<int> secondRound;
        for (size_t j = 0; j < paramStack.back().size();) {
            switch (std::get<int>(paramStack.back().at(j + 1))) {
            case EQUAL_TO_EXPR:
                if (paramStack.back().at(j) == paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case NOT_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) != paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case GREATER_THAN_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) >= paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case LESS_THAN_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) <= paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case GREATER_THAN_EXPR:
                if (paramStack.back().at(j) > paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case LESS_THAN_EXPR:
                if (paramStack.back().at(j) < paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            }
            if (j + 3 < paramStack.back().size() - 1) {
                firstRound.push_back(std::get<int>(paramStack.back().at(j + 3)));
                j += 4;
                continue;
            }
            j += 3;
        }
        if (firstRound.size() > 1) {
            for (size_t j = 0; j < firstRound.size() - 1; j += 2) {
                switch (firstRound.at(j + 1)) {
                case AND_EXPR:
                    if (firstRound.at(j) and firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                case OR_EXPR:
                    if (firstRound.at(j) or firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                case XOR_EXPR:
                    if (firstRound.at(j) xor firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                }
            }                   
        }
        else {
            secondRound.push_back(firstRound.at(0));
        }
        for (size_t j = 0; j < secondRound.size(); j += 1) {
            if (secondRound.at(j) == false) {
                returnedValue = false;
                paramStack.pop_back();
                return;
            }
        }
        returnedValue = true;
        break;
    }
    case WHILE_EXPR: {
        std::vector<int> firstRound;
        std::vector<int> secondRound;
        for (size_t j = 0; j < paramStack.back().size();) {
            switch (std::get<int>(paramStack.back().at(j + 1))) {
            case EQUAL_TO_EXPR:
                if (paramStack.back().at(j) == paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case NOT_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) != paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case GREATER_THAN_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) >= paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case LESS_THAN_EQUAL_TO_EXPR:
                if (paramStack.back().at(j) <= paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case GREATER_THAN_EXPR:
                if (paramStack.back().at(j) > paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            case LESS_THAN_EXPR:
                if (paramStack.back().at(j) < paramStack.back().at(j + 2)) {
                    firstRound.push_back(true);
                    break;
                }
                firstRound.push_back(false);
                break;
            }
            if (j + 3 < paramStack.back().size() - 1) {
                firstRound.push_back(std::get<int>(paramStack.back().at(j + 3)));
                j += 4;
                continue;
            }
            j += 3;
        }
        if (firstRound.size() > 1) {
            for (size_t j = 0; j < firstRound.size() - 1; j += 2) {
                switch (firstRound.at(j + 1)) {
                case AND_EXPR:
                    if (firstRound.at(j) and firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                case OR_EXPR:
                    if (firstRound.at(j) or firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                case XOR_EXPR:
                    if (firstRound.at(j) xor firstRound.at(j + 2)) {
                        secondRound.push_back(true);
                        break;
                    }
                    secondRound.push_back(false);
                    break;
                }
            }
        }
        else {
            secondRound.push_back(firstRound.at(0));
        }
        for (size_t j = 0; j < secondRound.size(); j += 1) {
            if (secondRound.at(j) == false) {
                returnedValue = false;
                paramStack.pop_back();
                return;
            }
        }
        returnedValue = true;
        break;
    }
    }
    /*stack.push_back(returnedValue);
    stack.push_back(static_cast<int>(stack.size() - 1));*/
    paramStack.pop_back();
}
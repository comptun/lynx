#include "pcfunctions.h"
#include "byteinterpreter.h"
#include "sha256.h"

#include <vector>
#include <math.h>

#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <chrono>
#include <ctime>

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
    "find",
    "setprecision",
    "exp",
    "sha256",
    "clock",
    "time",
    "sleep",
    "fact",
    "readf",
    "writef",
    "append",
    "pop",
    "access",
    "array",
};

long long int ByteInterpreter::getListPointer(std::string listPointer)
{
    std::string strptr;
    for (size_t i = 16; i < listPointer.size(); ++i)
        strptr.push_back(listPointer.at(i));
    return std::stoi(strptr);
}

bool ByteInterpreter::beginsWith(std::string str, std::string begin)
{
    if (str >= begin) {
        for (size_t i = 0; i < begin.size(); ++i) {
            if (begin.at(i) != str.at(i))
                return false;
        }
        return true;
    }
    return false;
}

void ByteInterpreter::newList(long long int listPtr)
{
    std::cout << "[ ";
    for (size_t i = 0; i < std::get<t_list>(listStack.at(listPtr)).size(); ++i) {
        if (std::holds_alternative<long long int>(std::get<t_list>(listStack.at(listPtr)).at(i))) {
            std::cout << std::get<long long int>(std::get<t_list>(listStack.at(listPtr)).at(i));
        }
        if (std::holds_alternative<long double>(std::get<t_list>(listStack.at(listPtr)).at(i))) {
            std::cout << std::get<long double>(std::get<t_list>(listStack.at(listPtr)).at(i));
        }
        if (std::holds_alternative<std::string>(std::get<t_list>(listStack.at(listPtr)).at(i))) {
            if (beginsWith(std::get<std::string>(std::get<t_list>(listStack.at(listPtr)).at(i)), "__LIST_POINTER__")) {
                newList(getListPointer(std::get<std::string>(std::get<t_list>(listStack.at(listPtr)).at(i))));
                continue;
            }
            std::cout << "\"" << std::get<std::string>(std::get<t_list>(listStack.at(listPtr)).at(i)) << "\"";
        }
        if (i < std::get<t_list>(listStack.at(listPtr)).size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ], ";
}

void ByteInterpreter::executePCF(std::string funcName)
{
    if (funcName == "print") {
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            if (std::holds_alternative<long long int>(paramStack.back().at(i)))
                std::cout << std::get<long long int>(paramStack.back().at(i));
            else if (std::holds_alternative<long double>(paramStack.back().at(i)))
                std::cout << std::get<long double>(paramStack.back().at(i));
            else if (std::holds_alternative<t_list>(paramStack.back().at(i)) && std::get<t_list>(paramStack.back().at(i)).size() > 0) {
                std::cout << "[ ";
                for (size_t k = 0; k < std::get<t_list>(paramStack.back().at(i)).size(); ++k) {
                    if (std::holds_alternative<long long int>(std::get<t_list>(paramStack.back().at(i)).at(k))) {
                        std::cout << std::get<long long int>(std::get<t_list>(paramStack.back().at(i)).at(k));
                    }
                    if (std::holds_alternative<long double>(std::get<t_list>(paramStack.back().at(i)).at(k))) {
                        std::cout << std::get<long double>(std::get<t_list>(paramStack.back().at(i)).at(k));
                    }
                    if (std::holds_alternative<std::string>(std::get<t_list>(paramStack.back().at(i)).at(k))) {
                        if (beginsWith(std::get<std::string>(std::get<t_list>(paramStack.back().at(i)).at(k)), "__LIST_POINTER__")) {
                            newList(getListPointer(std::get<std::string>(std::get<t_list>(paramStack.back().at(i)).at(k))));
                            continue;
                        }
                        std::cout << "\"" << std::get<std::string>(std::get<t_list>(paramStack.back().at(i)).at(k)) << "\"";
                    }
                    if (k < std::get<t_list>(paramStack.back().at(i)).size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << " ]";
            }
            else if (std::holds_alternative<std::string>(paramStack.back().at(i))) {
                if (beginsWith(std::get<std::string>(paramStack.back().at(i)), "__LIST_POINTER__")) {
                    newList(getListPointer(std::get<std::string>(paramStack.back().at(i))));
                    continue;
                }
                bool isInVariableCall = false;
                std::string varName;
                for (size_t j = 0; j < std::get<std::string>(paramStack.back().at(i)).size();) {
                    if (isInVariableCall) {
                        if (std::get<std::string>(paramStack.back().at(i)).at(j) == '}') {
                            if (std::holds_alternative<long long int>(stack.back().at(getNameReference(varName))))
                                std::cout << std::get<long long int>(stack.back().at(getNameReference(varName)));
                            else if (std::holds_alternative<long double>(stack.back().at(getNameReference(varName))))
                                std::cout << std::get<long double>(stack.back().at(getNameReference(varName)));
                            else if (std::holds_alternative<std::string>(stack.back().at(getNameReference(varName))))
                                std::cout << std::get<std::string>(stack.back().at(getNameReference(varName)));
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
        returnedValue = true;
    }
    else if (funcName == "endl") {
        returnedValue = "\n";
    }
    else if (funcName == "rand") {
        long long int difference = std::get<long long int>(paramStack.back().at(1)) - std::get<long long int>(paramStack.back().at(0));
        returnedValue = rand() % difference + std::get<long long int>(paramStack.back().at(0));
    }
    else if (funcName == "swap") {
        t_returnvalue item1 = stack.back().at(std::get<long long int>(paramStack.back().at(0)));
        t_returnvalue item2 = stack.back().at(std::get<long long int>(paramStack.back().at(1)));
        stack.back().at(std::get<long long int>(paramStack.back().at(1))) = item1;
        stack.back().at(std::get<long long int>(paramStack.back().at(0))) = item2;
    }
    else if (funcName == "sqrt") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = sqrt(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = sqrt(std::get<long double>(paramStack.back().at(0)));
    }
    else if (funcName == "pow") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = pow(std::get<long long int>(paramStack.back().at(0)), std::get<long long int>(paramStack.back().at(1)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = pow(std::get<long double>(paramStack.back().at(0)), std::get<long double>(paramStack.back().at(1)));
    }
    else if (funcName == "putc") {
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            std::cout << static_cast<char>(std::get<long long int>(paramStack.back().at(i)));
        }
    }
    else if (funcName == "input") {
        for (size_t i = 0; i < paramStack.back().size(); ++i) {
            if (std::holds_alternative<long long int>(paramStack.back().at(i)))
                std::cout << std::get<long long int>(paramStack.back().at(i));
            else if (std::holds_alternative<long double>(paramStack.back().at(i)))
                std::cout << std::get<long double>(paramStack.back().at(i));
            else if (std::holds_alternative<std::string>(paramStack.back().at(i))) {
                bool isInVariableCall = false;
                std::string varName;
                for (size_t j = 0; j < std::get<std::string>(paramStack.back().at(i)).size();) {
                    if (isInVariableCall) {
                        if (std::get<std::string>(paramStack.back().at(i)).at(j) == '}') {
                            if (std::holds_alternative<long long int>(stack.back().at(getNameReference(varName))))
                                std::cout << std::get<long long int>(stack.back().at(getNameReference(varName)));
                            else if (std::holds_alternative<long double>(stack.back().at(getNameReference(varName))))
                                std::cout << std::get<long double>(stack.back().at(getNameReference(varName)));
                            else if (std::holds_alternative<std::string>(stack.back().at(getNameReference(varName))))
                                std::cout << std::get<std::string>(stack.back().at(getNameReference(varName)));
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
    }
    else if (funcName == "pause") {
        std::cin.get();
        returnedValue = true;
    }
    else if (funcName == "max") {
        returnedValue = (paramStack.back().at(0) > paramStack.back().at(1) ? paramStack.back().at(0) : paramStack.back().at(1));
    }
    else if (funcName == "min") {
        returnedValue = (paramStack.back().at(1) > paramStack.back().at(0) ? paramStack.back().at(0) : paramStack.back().at(1));
    }
    else if (funcName == "sin") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = sin(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = sin(std::get<long double>(paramStack.back().at(0)));
    }
    else if (funcName == "cos") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = cos(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = cos(std::get<long double>(paramStack.back().at(0)));
    }
    else if (funcName == "tan") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = tan(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = tan(std::get<long double>(paramStack.back().at(0)));
    }
    else if (funcName == "ln") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = log(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = log(std::get<long double>(paramStack.back().at(0)));
    }
    else if (funcName == "isprime") {
        bool isPrime = true;
        long double sqrtNum = 0;
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            sqrtNum = sqrt(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            sqrtNum = sqrt(std::get<long double>(paramStack.back().at(0)));
        if (std::get<long long int>(paramStack.back().at(0)) == 2) {
            returnedValue = true;
            goto func_end;
        }
        if (std::get<long long int>(paramStack.back().at(0)) == 0
            or std::get<long long int>(paramStack.back().at(0)) == 1) {
            returnedValue = false;
            goto func_end;
        }
        for (long long int a = 2; a < sqrtNum + 1; ++a) {
            if (std::get<long long int>(paramStack.back().at(0)) % a == 0) {
                isPrime = false;
                returnedValue = false;
                break;
            }
        }
        if (isPrime) {
            returnedValue = true;
        }
    }
    else if (funcName == "int") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = paramStack.back().at(0);
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = static_cast<long long int>(std::get<long double>(paramStack.back().at(0)));
        else if (std::holds_alternative<std::string>(paramStack.back().at(0)))
            returnedValue = std::stoi(std::get<std::string>(paramStack.back().at(0)));
    }
    else if (funcName == "float") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = static_cast<long double>(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = paramStack.back().at(0);
        else if (std::holds_alternative<std::string>(paramStack.back().at(0)))
            returnedValue = std::stod(std::get<std::string>(paramStack.back().at(0)));
    }
    else if (funcName == "string") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = std::to_string(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = std::to_string(std::get<long double>(paramStack.back().at(0)));
        else if (std::holds_alternative<std::string>(paramStack.back().at(0)))
            returnedValue = paramStack.back().at(0);
    }
    else if (funcName == "char") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = std::string(1, static_cast<char>(std::get<long long int>(paramStack.back().at(0))));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = std::string(1, static_cast<char>(std::get<long double>(paramStack.back().at(0))));
    }
    else if (funcName == "size") {
        if (std::holds_alternative<std::string>(paramStack.back().at(0))) {
            if (beginsWith(std::get<std::string>(paramStack.back().at(0)), "__LIST_POINTER__")) {
                returnedValue = static_cast<long long int>(std::get<t_list>(listStack.at(getListPointer(std::get<std::string>(paramStack.back().at(0))))).size());
            }
            else {
                returnedValue = static_cast<long long int>(std::get<std::string>(paramStack.back().at(0)).size());
            }
        }
        else if (std::holds_alternative<t_list>(paramStack.back().at(0))) {
            returnedValue = static_cast<long long int>(std::get<t_list>(paramStack.back().at(0)).size());
        }
    }
    else if (funcName == "reverse") {
        if (std::holds_alternative<std::string>(paramStack.back().at(0))) {
            std::string revString;
            revString = std::get<std::string>(paramStack.back().at(0));
            std::reverse(revString.begin(), revString.end());
            returnedValue = revString;
        }
        else if (std::holds_alternative<t_list>(paramStack.back().at(0))) {
            t_list revVec;
            revVec = std::get<t_list>(paramStack.back().at(0));
            std::reverse(revVec.begin(), revVec.end());
            returnedValue = revVec;
        }
    }
    else if (funcName == "sort") {
        if (std::holds_alternative<std::string>(paramStack.back().at(0))) {
            std::string sortString;
            sortString = std::get<std::string>(paramStack.back().at(0));
            std::sort(sortString.begin(), sortString.end());
            returnedValue = sortString;
        }
        else if (std::holds_alternative<t_list>(paramStack.back().at(0))) {
            t_list sortVec;
            sortVec = std::get<t_list>(paramStack.back().at(0));
            std::sort(sortVec.begin(), sortVec.end());
            returnedValue = sortVec;
        }
    }
    else if (funcName == "asin") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = asin(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = asin(std::get<long double>(paramStack.back().at(0)));
    }
    else if (funcName == "acos") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = acos(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = acos(std::get<long double>(paramStack.back().at(0)));
    }
    else if (funcName == "atan") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = atan(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = atan(std::get<long double>(paramStack.back().at(0)));
    }
    else if (funcName == "keyPressed") {
        char key = _getch();
        char keypressed = ' ';
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            keypressed = std::get<long long int>(paramStack.back().at(0));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            keypressed = std::get<long long int>(paramStack.back().at(0));
        else if (std::holds_alternative<std::string>(paramStack.back().at(0)))
            keypressed = std::get<std::string>(paramStack.back().at(0)).at(0);
        returnedValue = key == keypressed;
    }
    else if (funcName == "getKeyPressed") {
        returnedValue = _getch();
    }
    else if (funcName == "parenthesis") {
        if (paramStack.back().size() > 1) {
            std::vector<long long int> firstRound;
            std::vector<long long int> secondRound;
            for (size_t j = 0; j < paramStack.back().size();) {
                switch (std::get<long long int>(paramStack.back().at(j + 1))) {
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
                    firstRound.push_back(std::get<long long int>(paramStack.back().at(j + 3)));
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
            goto func_end;
        }
        returnedValue = paramStack.back().at(0);
    }
    else if (funcName == "boolexpr" or funcName == "if" or funcName == "while") {
        std::vector<long long int> firstRound;
        std::vector<long long int> secondRound;
        for (size_t j = 0; j < paramStack.back().size();) {
            switch (std::get<long long int>(paramStack.back().at(j + 1))) {
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
                firstRound.push_back(std::get<long long int>(paramStack.back().at(j + 3)));
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
    }
    else if (funcName == "find") {
        bool found = false;
        if (std::holds_alternative<std::string>(paramStack.back().at(0))) {
            for (size_t j = 0; j < std::get<std::string>(paramStack.back().at(1)).size(); ++j) {
                if (std::get<std::string>(paramStack.back().at(1)).at(j) == std::get<std::string>(paramStack.back().at(0)).at(0)) {
                    returnedValue = static_cast<long long int>(j);
                    found = true;
                    break;
                }
            }
        }
        else if (std::holds_alternative<long long int>(paramStack.back().at(0))) {
        for (size_t j = 0; j < std::get<std::string>(paramStack.back().at(1)).size(); ++j) {
            if (std::get<std::string>(paramStack.back().at(1)).at(j) == static_cast<char>(std::get<long long int>(paramStack.back().at(0)))) {
                returnedValue = static_cast<long long int>(j);
                found = true;
                break;
            }
        }
        }
        if (found == false)
            returnedValue = -1;
    }
    else if (funcName == "setprecision") {
        std::cout << std::setprecision(std::get<long long int>(paramStack.back().at(0)));
        returnedValue = 0;
    }
    else if (funcName == "exp") {
        if (std::holds_alternative<long long int>(paramStack.back().at(0)))
            returnedValue = exp(std::get<long long int>(paramStack.back().at(0)));
        else if (std::holds_alternative<long double>(paramStack.back().at(0)))
            returnedValue = exp(std::get<long double>(paramStack.back().at(0)));
        }
    else if (funcName == "sha256") {
        if (std::holds_alternative<std::string>(paramStack.back().at(0))) {
            returnedValue = sha256(std::get<std::string>(paramStack.back().at(0)));
            goto func_end;
        }
        else if (std::holds_alternative<long long int>(paramStack.back().at(0))) {
            returnedValue = sha256(std::to_string(std::get<long long int>(paramStack.back().at(0))));
            goto func_end;
        }
        else if (std::holds_alternative<long double>(paramStack.back().at(0))) {
            returnedValue = sha256(std::to_string(std::get<long double>(paramStack.back().at(0))));
            goto func_end;
        }
        returnedValue = -1;
    }
    else if (funcName == "clock") {
        returnedValue = clock();
    }
    else if (funcName == "time") {
        if (std::get<long long int>(paramStack.back().at(0)) == 0) {
            returnedValue = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();;
        }
        if (std::get<long long int>(paramStack.back().at(0)) == 1) {
            returnedValue = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();;
        }
    }
    else if (funcName == "sleep") {
        Sleep(std::get<long long int>(paramStack.back().at(0)));
    }
    else if (funcName == "fact") {
        if (std::holds_alternative<long double>(paramStack.back().at(0))) {
            returnedValue = tgamma(std::get<long double>(paramStack.back().at(0)) + 1);
        }
        if (std::holds_alternative<long long int>(paramStack.back().at(0))) {
            returnedValue = tgamma(std::get<long long int>(paramStack.back().at(0)) + 1);
        }
    }
    else if (funcName == "readf") {
        std::string line;
        std::string fileContents;
        std::ifstream readFile(std::get<std::string>(paramStack.back().at(0)));
        while (getline(readFile, line)) {
            fileContents += line;
            fileContents += "\n";
        }
        readFile.close();
        returnedValue = fileContents;
    }
    else if (funcName == "writef") {
        std::ofstream writeFile(std::get<std::string>(paramStack.back().at(0)), std::fstream::app);
        if (writeFile.is_open()) {
            writeFile << std::get<std::string>(paramStack.back().at(1));
        }
        writeFile.close();
    }
    else if (funcName == "append") {
        t_list appendVector = std::get<t_list>(paramStack.back().at(0));
        if (std::holds_alternative<long long int>(paramStack.back().at(1))) {
            appendVector.push_back(std::get<long long int>(paramStack.back().at(1)));
        }
        if (std::holds_alternative<long double>(paramStack.back().at(1))) {
            appendVector.push_back(std::get<long double>(paramStack.back().at(1)));
        }
        if (std::holds_alternative<std::string>(paramStack.back().at(1))) {
            appendVector.push_back(std::get<std::string>(paramStack.back().at(1)));
        }
        returnedValue = appendVector;
    }
    else if (funcName == "pop") {
        t_list popVector = std::get<t_list>(paramStack.back().at(0));
        popVector.pop_back();
        returnedValue = popVector;
    }
    else if (funcName == "access") {
        long long int listPtr = 0;
        if (std::get<t_list>(paramStack.back().at(1)).size() > 1) {
            for (size_t k = 0; k < std::get<t_list>(paramStack.back().at(1)).size(); ++k) {
                listPtr = getListPointer(std::get<std::string>(std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k)))));
                if (k == std::get<t_list>(paramStack.back().at(1)).size() - 2) {
                    if (std::holds_alternative<long long int>(std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1)))))
                        returnedValue = std::get<long long int>(std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1))));
                    if (std::holds_alternative<long double>(std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1)))))
                        returnedValue = std::get<long double>(std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1))));
                    if (std::holds_alternative<std::string>(std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1)))))
                        returnedValue = std::get<std::string>(std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1))));
                }
            }
        }
        else {
            if (std::holds_alternative<long long int>(std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0)))))
                returnedValue = std::get<long long int>(std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0))));
            if (std::holds_alternative<long double>(std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0)))))
                returnedValue = std::get<long double>(std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0))));
            if (std::holds_alternative<std::string>(std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0)))))
                returnedValue = std::get<std::string>(std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0))));
        }
    }
    else if (funcName == "set") {
        long long int listPtr = 0;
        if (std::get<t_list>(paramStack.back().at(1)).size() > 1) {
            for (size_t k = 0; k < std::get<t_list>(paramStack.back().at(1)).size(); ++k) {
                listPtr = getListPointer(std::get<std::string>(std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k)))));
                if (k == std::get<t_list>(paramStack.back().at(1)).size() - 2) {
                    if (std::holds_alternative<long long int>(paramStack.back().at(2)))
                        std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1))) = std::get<long long int>(paramStack.back().at(2));
                    if (std::holds_alternative<long double>(paramStack.back().at(2)))
                        std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1))) = std::get<long double>(paramStack.back().at(2));
                    if (std::holds_alternative<std::string>(paramStack.back().at(2)))
                        std::get<t_list>(listStack.at(listPtr)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(k + 1))) = std::get<std::string>(paramStack.back().at(2));
                }
            }
        }
        else {
            if (std::holds_alternative<long long int>(paramStack.back().at(2)))
                std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0))) = std::get<long long int>(paramStack.back().at(2));
            if (std::holds_alternative<long double>(paramStack.back().at(2)))
                std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0))) = std::get<long double>(paramStack.back().at(2));
            if (std::holds_alternative<std::string>(paramStack.back().at(2)))
                std::get<t_list>(paramStack.back().at(0)).at(std::get<long long int>(std::get<t_list>(paramStack.back().at(1)).at(0))) = std::get<std::string>(paramStack.back().at(2));
        }
        returnedValue = std::get<t_list>(paramStack.back().at(0));
    }
    else if (funcName == "array") {
        t_list newArray;
        if (paramStack.back().size() > 0) {
            for (size_t k = 0; k < paramStack.back().size(); ++k) {
                if (std::holds_alternative<long long int>(paramStack.back().at(k))) {
                    newArray.push_back(std::get<long long int>(paramStack.back().at(k)));
                }
                if (std::holds_alternative<long double>(paramStack.back().at(k))) {
                    newArray.push_back(std::get<long double>(paramStack.back().at(k)));
                }
                if (std::holds_alternative<std::string>(paramStack.back().at(k))) {
                    newArray.push_back(std::get<std::string>(paramStack.back().at(k)));
                }
                if (std::holds_alternative<t_list>(paramStack.back().at(k))) {
                    listStack.push_back(std::get<t_list>(paramStack.back().at(k)));
                    newArray.push_back("__LIST_POINTER__" + std::to_string(listStack.size() - 1));
                }
            }
        }
        returnedValue = newArray;
    }
    /*stack.push_back(returnedValue);
    stack.push_back(static_cast<long long int>(stack.size() - 1));*/
func_end:
    paramStack.pop_back();
}


#include "pcfunctions.h"
#include "byteinterpreter.h"
#include "sha256.h"

#include <vector>
#include <math.h>
#include <bits/stdc++.h>
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

void ByteInterpreter::executePCF(std::string funcName)
{
    if (funcName == "print") {
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
                            if (std::holds_alternative<long long int>(stack.at(getNameReference(varName))))
                                std::cout << std::get<long long int>(stack.at(getNameReference(varName)));
                            else if (std::holds_alternative<long double>(stack.at(getNameReference(varName))))
                                std::cout << std::get<long double>(stack.at(getNameReference(varName)));
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
    }
    else if (funcName == "endl") {
        returnedValue = "\n";
    }
    else if (funcName == "rand") {
        long long int difference = std::get<long long int>(paramStack.back().at(1)) - std::get<long long int>(paramStack.back().at(0));
        returnedValue = rand() % difference + std::get<long long int>(paramStack.back().at(0));
    }
    else if (funcName == "swap") {
        std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>> item1 = stack.at(std::get<long long int>(paramStack.back().at(0)));
        std::variant<long long int, long double, std::string, std::vector<std::variant<long long int, long double, std::string>>> item2 = stack.at(std::get<long long int>(paramStack.back().at(1)));
        stack.at(std::get<long long int>(paramStack.back().at(1))) = item1;
        stack.at(std::get<long long int>(paramStack.back().at(0))) = item2;
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
                            if (std::holds_alternative<long long int>(stack.at(getNameReference(varName))))
                                std::cout << std::get<long long int>(stack.at(getNameReference(varName)));
                            else if (std::holds_alternative<long double>(stack.at(getNameReference(varName))))
                                std::cout << std::get<long double>(stack.at(getNameReference(varName)));
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
    else if (funcName == "len") {
        if (std::holds_alternative<std::string>(paramStack.back().at(0))) {
            returnedValue = static_cast<long long int>(std::get<std::string>(paramStack.back().at(0)).size());
        }
        if (std::holds_alternative<std::vector<std::variant<long long int, long double, std::string>>>(paramStack.back().at(0))) {
            returnedValue = static_cast<long long int>(std::get<std::vector<std::variant<long long int, long double, std::string>>>(paramStack.back().at(0)).size());
        }
    }
    else if (funcName == "reverse") {
        std::string revString = std::get<std::string>(paramStack.back().at(0));
        std::reverse(revString.begin(), revString.end());
        returnedValue = revString;
    }
    else if (funcName == "sort") {
        std::string sortString = std::get<std::string>(paramStack.back().at(0));
        std::sort(sortString.begin(), sortString.end());
        returnedValue = sortString;
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
        long long int fact = 1;
        for (long long int i = 1; i <= std::get<long long int>(paramStack.back().at(0)); ++i) {
            fact *= i;
        }
        returnedValue = fact;
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
        std::get<std::string>(stack.at(std::get<long long int>(paramStack.back().at(0)))).push_back(static_cast<char>(std::get<long long int>(paramStack.back().at(1))));
        goto func_end;

        if (std::holds_alternative<long long int>(paramStack.back().at(1))) {
            std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).push_back(std::get<long long int>(paramStack.back().at(1)));
        }
        if (std::holds_alternative<long double>(paramStack.back().at(1))) {
            std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).push_back(std::get<long double>(paramStack.back().at(1)));
        }
        if (std::holds_alternative<std::string>(paramStack.back().at(1))) {
            std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).push_back(std::get<std::string>(paramStack.back().at(1)));
        }
    }
    else if (funcName == "pop") {
        if (std::holds_alternative<std::string>(stack.at(std::get<long long int>(paramStack.back().at(0))))) {
            std::get<std::string>(stack.at(std::get<long long int>(paramStack.back().at(0)))).pop_back();
            goto func_end;
        }
        std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).pop_back();
    }
    else if (funcName == "access") {
        /*if (std::holds_alternative<long long int>(std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).at(std::get<long long int>(paramStack.back().at(1))))) {
            returnedValue = std::get<long long int>(std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).at(std::get<long long int>(paramStack.back().at(1))));
            break;
        }
        if (std::holds_alternative<long double>(std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).at(std::get<long long int>(paramStack.back().at(1))))) {
            returnedValue = std::get<long double>(std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).at(std::get<long long int>(paramStack.back().at(1))));
            break;
        }
        if (std::holds_alternative<std::string>(std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).at(std::get<long long int>(paramStack.back().at(1))))) {
            returnedValue = std::get<std::string>(std::get<std::vector<std::variant<long long int, long double, std::string>>>(stack.at(std::get<long long int>(paramStack.back().at(0)))).at(std::get<long long int>(paramStack.back().at(1))));
            break;
        }*/
        returnedValue = std::string(1, std::get<std::string>(stack.at(std::get<long long int>(paramStack.back().at(0)))).at(std::get<long long int>(paramStack.back().at(1))));
    }
    else if (funcName == "array") {
        std::vector<std::variant<long long int, long double, std::string>> newArray;
        if (std::holds_alternative<std::string>(stack.at(std::get<long long int>(paramStack.back().at(0))))) {
            for (long long int n = 0; n < std::get<std::string>(stack.at(std::get<long long int>(paramStack.back().at(0)))).size(); ++n) {
                newArray.push_back(std::string(1, std::get<std::string>(stack.at(std::get<long long int>(paramStack.back().at(0)))).at(n)));
            }
        }
        returnedValue = newArray;
    }
    /*stack.push_back(returnedValue);
    stack.push_back(static_cast<long long int>(stack.size() - 1));*/
func_end:
    paramStack.pop_back();
}


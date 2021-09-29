#pragma once

#include <string>

enum PCFid {
    PRINT,
    ENDL,
    RAND,
    SWAP,
    SQRT,
    POW,
    PUTC,
    INPUT,
    MAX,
    MIN,
    SIN,
    COS,
    TAN,
    LN,
    ISPRIME,
    INT_CAST,
    FLOAT_CAST,
    STRING_CAST,
    CHAR_CAST,
    LEN,
    REVERSE,
    SORT,
    ASIN,
    ACOS,
    ATAN,
    KEYPRESSED,
    GETKEYPRESSED,
    PARENTHESIS,
    CEIL,
    FLOOR,
    ABS,
};

class PCFunctions {
public:
    void executePCF(std::string funcName);
    void print(int output);
    void print(std::string output);
};
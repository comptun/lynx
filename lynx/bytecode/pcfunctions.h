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
};

class PCFunctions {
public:
    void executePCF(std::string funcName);
    void print(int output);
    void print(std::string output);
};
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
};

class PCFunctions {
public:
    void executePCF(std::string funcName);
    void print(int output);
    void print(std::string output);
};
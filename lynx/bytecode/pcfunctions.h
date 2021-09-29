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
    BOOLEXPR,
    IF_EXPR,
    WHILE_EXPR,
};

enum booleanExpressions {
    AND_EXPR = 0,
    OR_EXPR,
    XOR_EXPR,
    EQUAL_TO_EXPR,
    NOT_EQUAL_TO_EXPR,
    LESS_THAN_EXPR,
    GREATER_THAN_EXPR,
    LESS_THAN_EQUAL_TO_EXPR,
    GREATER_THAN_EQUAL_TO_EXPR,
};

class PCFunctions {
public:
    void executePCF(std::string funcName);
    void print(int output);
    void print(std::string output);
};
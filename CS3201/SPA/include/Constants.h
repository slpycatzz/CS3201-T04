#pragma once

#include <string>

const char SYMBOL_INVALID[]   = "Invalid";
const char SYMBOL_COMMENT[]   = "\\";
const char SYMBOL_PROGRAM[]   = "program";
const char SYMBOL_PROCEDURE[] = "procedure";
const char SYMBOL_STMTLIST[]  = "stmtlist";
const char SYMBOL_ASSIGN[]    = "assign";
const char SYMBOL_WHILE[]     = "while";
const char SYMBOL_IF[]        = "if";
const char SYMBOL_IF_THEN[]   = "then";
const char SYMBOL_IF_ELSE[]   = "else";
const char SYMBOL_CALL[]      = "call";
const char SYMBOL_VARIABLE[]  = "variable";
const char SYMBOL_CONSTANT[]  = "constant";

const char CHAR_SYMBOL_EQUAL             = '=';
const char CHAR_SYMBOL_MINUS             = '-';
const char CHAR_SYMBOL_PLUS              = '+';
const char CHAR_SYMBOL_MULTIPLY          = '*';
const char CHAR_SYMBOL_SEMICOLON         = ';';
const char CHAR_SYMBOL_OPENCURLYBRACKET  = '{';
const char CHAR_SYMBOL_CLOSECURLYBRACKET = '}';
const char CHAR_SYMBOL_OPENBRACKET       = '(';
const char CHAR_SYMBOL_CLOSEBRACKET      = ')';

enum Symbol {
    INVALID,
    PROGRAM,
    PROCEDURE,
    STMTLIST,
    ASSIGN, WHILE, IF, CALL,
    VARIABLE, CONSTANT,
    PLUS, MINUS, MULTIPLY
};

class Constants {
 public:
     static std::string GetPreprocessFormat(const char symbol);
     static std::string GetPreprocessFormat(std::string symbol);
     static std::string SymbolToString(Symbol symbol);
     static Symbol StringToSymbol(std::string str);
};

#include <string>

#include "Constants.h"

const char PREPROCESS_FORMAT[] = " _ ";             /* Underscore denotes the position to replace. */

std::string Constants::GetPreprocessFormat(const char symbol) {
    std::string format = PREPROCESS_FORMAT;

    return format.replace(1, 1, std::string(1, symbol));
}

std::string Constants::GetPreprocessFormat(std::string symbol) {
    std::string format = PREPROCESS_FORMAT;

    return format.replace(1, 1, symbol);
}

std::string Constants::SymbolToString(Symbol symbol) {
    switch (symbol) {
        default:
        case INVALID:   return SYMBOL_INVALID;
        case PROGRAM:   return SYMBOL_PROGRAM;
        case PROCEDURE: return SYMBOL_PROCEDURE;
        case STMTLIST:  return SYMBOL_STMTLIST;
        case ASSIGN:    return SYMBOL_ASSIGN;
        case WHILE:     return SYMBOL_WHILE;
        case IF:        return SYMBOL_IF;
        case CALL:      return SYMBOL_CALL;
        case VARIABLE:  return SYMBOL_VARIABLE;
        case CONSTANT:  return SYMBOL_CONSTANT;
        case PLUS:      return std::string(1, CHAR_SYMBOL_PLUS);
        case MINUS:     return std::string(1, CHAR_SYMBOL_MINUS);
        case MULTIPLY:  return std::string(1, CHAR_SYMBOL_MULTIPLY);
    }
}

Symbol Constants::StringToSymbol(std::string str) {
    if (str == SYMBOL_PROGRAM)   return PROGRAM;
    if (str == SYMBOL_PROCEDURE) return PROCEDURE;
    if (str == SYMBOL_STMTLIST)  return STMTLIST;
    if (str == SYMBOL_ASSIGN)    return ASSIGN;
    if (str == SYMBOL_WHILE)     return WHILE;
    if (str == SYMBOL_IF)        return IF;
    if (str == SYMBOL_CALL)      return CALL;
    if (str == SYMBOL_VARIABLE)  return VARIABLE;
    if (str == SYMBOL_CONSTANT)  return CONSTANT;

    if (str == std::string(1, CHAR_SYMBOL_PLUS))     return PLUS;
    if (str == std::string(1, CHAR_SYMBOL_MINUS))    return MINUS;
    if (str == std::string(1, CHAR_SYMBOL_MULTIPLY)) return MULTIPLY;

    return INVALID;
}

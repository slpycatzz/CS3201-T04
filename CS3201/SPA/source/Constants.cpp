#include <string>

#include "Constants.h"

const char PREPROCESS_FORMAT[] = " _ ";             /* Underscore denotes the position to replace. */

std::string Constants::getPreprocessFormat(const char symbol) {
    std::string format = PREPROCESS_FORMAT;

    return format.replace(1, 1, std::string(1, symbol));
}

std::string Constants::getPreprocessFormat(std::string symbol) {
    std::string format = PREPROCESS_FORMAT;

    return format.replace(1, 1, symbol);
}

std::string Constants::symbolToString(Symbol symbol) {
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

Symbol Constants::stringToSymbol(std::string str) {
    if (str.compare(SYMBOL_PROGRAM) == 0) {
        return PROGRAM;

    } else if (str.compare(SYMBOL_PROCEDURE) == 0) {
        return PROCEDURE;

    } else if (str.compare(SYMBOL_STMTLIST) == 0) {
        return STMTLIST;

    } else if (str.compare(SYMBOL_ASSIGN) == 0) {
        return ASSIGN;

    } else if (str.compare(SYMBOL_WHILE) == 0) {
        return WHILE;

    } else if (str.compare(SYMBOL_IF) == 0) {
        return IF;

    } else if (str.compare(SYMBOL_CALL) == 0) {
        return CALL;

    } else if (str.compare(SYMBOL_VARIABLE) == 0) {
        return VARIABLE;

    } else if (str.compare(SYMBOL_CONSTANT) == 0) {
        return CONSTANT;

    } else if (str.compare(std::string(1, CHAR_SYMBOL_PLUS)) == 0) {
        return PLUS;

    } else if (str.compare(std::string(1, CHAR_SYMBOL_MINUS)) == 0) {
        return MINUS;

    } else if (str.compare(std::string(1, CHAR_SYMBOL_MULTIPLY)) == 0) {
        return MULTIPLY;
    }

    return INVALID;
}

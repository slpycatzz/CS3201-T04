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
        case INVALID:      return SYMBOL_INVALID;
        case PROGRAM:      return SYMBOL_PROGRAM;
        case PROCEDURE:    return SYMBOL_PROCEDURE;
        case STMTLIST:     return SYMBOL_STMTLIST;
        case ASSIGN:       return SYMBOL_ASSIGN;
        case WHILE:        return SYMBOL_WHILE;
        case IF:           return SYMBOL_IF;
        case CALL:         return SYMBOL_CALL;

        case SUCH_THAT:    return SYMBOL_SUCH_THAT;
        case WITH:         return SYMBOL_WITH;
        case PATTERN:      return SYMBOL_PATTERN;
        
        case USES:         return SYMBOL_USES;
        case MODIFIES:     return SYMBOL_MODIFIES;
        case PARENT:       return SYMBOL_PARENT;
        case PARENT_STAR:  return SYMBOL_PARENT_STAR;
        case FOLLOWS:      return SYMBOL_FOLLOWS;
        case FOLLOWS_STAR: return SYMBOL_FOLLOWS_STAR;
        case CALLS:        return SYMBOL_CALLS;
        case CALLS_STAR:   return SYMBOL_CALLS_STAR;
        case NEXT:         return SYMBOL_NEXT;
        case NEXT_STAR:    return SYMBOL_NEXT_STAR;
        case AFFECTS:      return SYMBOL_AFFECTS;
        case AFFECTS_STAR: return SYMBOL_AFFECTS_STAR;

        case VARIABLE:     return SYMBOL_VARIABLE;
        case CONSTANT:     return SYMBOL_CONSTANT;
        case BOOLEAN:      return SYMBOL_BOOLEAN;
        case PROGRAM_LINE: return SYMBOL_PROGRAM_LINE;
        case STMT:         return SYMBOL_STMT;

        case PROC_NAME:    return SYMBOL_PROC_NAME;
        case VAR_NAME:     return SYMBOL_VAR_NAME;
        case VALUE:        return SYMBOL_VALUE;
        case STMTNUMBER:   return SYMBOL_STMTNUMBER;

        case PLUS:         return std::string(1, CHAR_SYMBOL_PLUS);
        case MINUS:        return std::string(1, CHAR_SYMBOL_MINUS);
        case MULTIPLY:     return std::string(1, CHAR_SYMBOL_MULTIPLY);
        case UNDERSCORE:   return std::string(1, CHAR_SYMBOL_UNDERSCORE);

        /* Unused for symbol to string conversion. */
        case QUERY_RESULT:
        case RELATION:
        case ATTRIBUTE:    return "";
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

    if (str == SYMBOL_SUCH_THAT) return SUCH_THAT;
    if (str == SYMBOL_WITH)      return WITH;
    if (str == SYMBOL_PATTERN)   return PATTERN;

    if (str == SYMBOL_USES)         return USES;
    if (str == SYMBOL_MODIFIES)     return MODIFIES;
    if (str == SYMBOL_PARENT)       return PARENT;
    if (str == SYMBOL_PARENT_STAR)  return PARENT_STAR;
    if (str == SYMBOL_FOLLOWS)      return FOLLOWS;
    if (str == SYMBOL_FOLLOWS_STAR) return FOLLOWS_STAR;
    if (str == SYMBOL_CALLS)        return CALLS;
    if (str == SYMBOL_CALLS_STAR)   return CALLS_STAR;
    if (str == SYMBOL_NEXT)         return NEXT;
    if (str == SYMBOL_NEXT_STAR)    return NEXT_STAR;
    if (str == SYMBOL_AFFECTS)      return AFFECTS;
    if (str == SYMBOL_AFFECTS_STAR) return AFFECTS_STAR;

    if (str == SYMBOL_VARIABLE)     return VARIABLE;
    if (str == SYMBOL_CONSTANT)     return CONSTANT;
    if (str == SYMBOL_BOOLEAN)      return BOOLEAN;
    if (str == SYMBOL_PROGRAM_LINE) return PROGRAM_LINE;
    if (str == SYMBOL_STMT)         return STMT;

    if (str == SYMBOL_PROC_NAME)  return PROC_NAME;
    if (str == SYMBOL_VAR_NAME)   return VAR_NAME;
    if (str == SYMBOL_VALUE)      return VALUE;
    if (str == SYMBOL_STMTNUMBER) return STMTNUMBER;

    if (str == std::string(1, CHAR_SYMBOL_PLUS))       return PLUS;
    if (str == std::string(1, CHAR_SYMBOL_MINUS))      return MINUS;
    if (str == std::string(1, CHAR_SYMBOL_MULTIPLY))   return MULTIPLY;
    if (str == std::string(1, CHAR_SYMBOL_UNDERSCORE)) return UNDERSCORE;

    /* Support symbols that have no string conversion. */
    if (str.empty()) return UNDEFINED;

    return INVALID;
}

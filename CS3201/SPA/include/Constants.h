#pragma once

#include <string>

const char SYMBOL_INVALID[]   = "Invalid";
const char SYMBOL_COMMENT[]   = "\\\\";
const char SYMBOL_PROGRAM[]   = "program";
const char SYMBOL_PROCEDURE[] = "procedure";
const char SYMBOL_STMTLIST[]  = "stmtlist";
const char SYMBOL_ASSIGN[]    = "assign";
const char SYMBOL_WHILE[]     = "while";
const char SYMBOL_IF[]        = "if";
const char SYMBOL_IF_THEN[]   = "then";
const char SYMBOL_IF_ELSE[]   = "else";
const char SYMBOL_CALL[]      = "call";

const char SYMBOL_SELECT[]    = "Select";
const char SYMBOL_SUCH[]      = "such";
const char SYMBOL_THAT[]      = "that";
const char SYMBOL_SUCH_THAT[] = "such that";
const char SYMBOL_WITH[]      = "with";
const char SYMBOL_PATTERN[]   = "pattern";

const char SYMBOL_USES[]         = "Uses";
const char SYMBOL_MODIFIES[]     = "Modifies";
const char SYMBOL_PARENT[]       = "Parent";
const char SYMBOL_PARENT_STAR[]  = "Parent*";
const char SYMBOL_FOLLOWS[]      = "Follows";
const char SYMBOL_FOLLOWS_STAR[] = "Follows*";
const char SYMBOL_CALLS[]        = "Calls";
const char SYMBOL_CALLS_STAR[]   = "Calls*";
const char SYMBOL_NEXT[]         = "Next";
const char SYMBOL_NEXT_STAR[]    = "Next*";
const char SYMBOL_AFFECTS[]      = "Affects";
const char SYMBOL_AFFECTS_STAR[] = "Affects*";

const char SYMBOL_VARIABLE[]     = "variable";
const char SYMBOL_CONSTANT[]     = "constant";
const char SYMBOL_BOOLEAN[]      = "BOOLEAN";
const char SYMBOL_PROGRAM_LINE[] = "prog_line";
const char SYMBOL_STMT[]         = "stmt";

const char SYMBOL_PROC_NAME[]   = "procName";
const char SYMBOL_VAR_NAME[]    = "varName";
const char SYMBOL_VALUE[]       = "value";
const char SYMBOL_STMTNUMBER[]  = "stmt#";

const char SYMBOL_TRUE[]  = "true";
const char SYMBOL_FALSE[] = "false";

const char CHAR_SYMBOL_EQUAL             = '=';
const char CHAR_SYMBOL_MINUS             = '-';
const char CHAR_SYMBOL_PLUS              = '+';
const char CHAR_SYMBOL_MULTIPLY          = '*';
const char CHAR_SYMBOL_SEMICOLON         = ';';
const char CHAR_SYMBOL_OPENCURLYBRACKET  = '{';
const char CHAR_SYMBOL_CLOSECURLYBRACKET = '}';
const char CHAR_SYMBOL_OPENBRACKET       = '(';
const char CHAR_SYMBOL_CLOSEBRACKET      = ')';
const char CHAR_SYMBOL_UNDERSCORE        = '_';
const char CHAR_SYMBOL_COMMA             = ',';
const char CHAR_SYMBOL_DOUBLEQUOTES      = '\"';

enum Symbol {
    INVALID, UNDEFINED,
    PROGRAM,
    PROCEDURE,
    STMTLIST,
    ASSIGN, WHILE, IF, CALL,

    QUERY_RESULT, SUCH_THAT, WITH, PATTERN,
    RELATION, ATTRIBUTE,

    USES, MODIFIES, PARENT, PARENT_STAR, FOLLOWS, FOLLOWS_STAR,
    CALLS, CALLS_STAR, NEXT, NEXT_STAR, AFFECTS, AFFECTS_STAR,

    VARIABLE, CONSTANT, BOOLEAN, PROGRAM_LINE, STMT,

    PROC_NAME, VAR_NAME, VALUE, STMTNUMBER,

    PLUS, MINUS, MULTIPLY, UNDERSCORE
};

class Constants {
 public:
     static std::string GetPreprocessFormat(const char symbol);
     static std::string GetPreprocessFormat(std::string symbol);
     static std::string SymbolToString(Symbol symbol);
     static Symbol StringToSymbol(std::string str);
};

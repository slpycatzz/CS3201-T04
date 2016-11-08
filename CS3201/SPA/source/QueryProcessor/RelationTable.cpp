#include <string>
#include <unordered_map>
#include <vector>

#include "QueryProcessor/RelationTable.h"

RelationTable::RelationTable() {
    initTable();
}

RelationTable::~RelationTable() {}

bool RelationTable::isRelationValid(Symbol relation, Symbol clause) {
    // clause = PATTERN
    // relation = ASSIGN/IF/WHILE [for-pattern]
    // relation = USES/MODIFIES/... [for-suchthat]
    if (clause == SUCH_THAT) {
        switch (relation) {
        case USES:
        case MODIFIES:
        case PARENT:
        case PARENT_TRANSITIVE:
        case FOLLOWS:
        case FOLLOWS_TRANSITIVE:
        case CALLS:
        case CALLS_TRANSITIVE:
        case NEXT:
        case NEXT_TRANSITIVE:
        case AFFECTS:
        case AFFECTS_TRANSITIVE:
        // Bonus feature
        /* 
        case CONTAINS:
        case CONTAINS_TRANSITIVE:
        case SIBLINGS:
        */
            return true;
        default:
            return false;
        }
    } else if (clause == PATTERN) {
        switch (relation) {
        case ASSIGN:
        case WHILE:
        case IF:
            return true;
        default:
            return false;
        }
    }

    return false;
}

// relation(Symbol) arg(string): might have include method to seperate varStr and expressionStr
bool RelationTable::isArgValid(Symbol relation, std::string arg, int i) {
    // relation = ASSIGN/IF/WHILE [for-pattern]
    // relation = USES/MODIFIES/... [for suchthat]
    // arg = ASSIGN/UNDERSCORE/VARIABLE/
    // PATTERN = (_"c+1"_)...

    std::vector<Symbol> argList;
    Symbol argSymbol = Constants::StringToSymbol(arg);

    switch (i) {
    case 0:
        argList = clauseMap1[relation]; return isArgFound(argList, argSymbol);
    case 1:
        argList = clauseMap2[relation]; return isArgFound(argList, argSymbol);
    case 2:
        return (argSymbol == UNDERSCORE)|| (argSymbol == STMTLIST);
    default:
        return false;
    }

    return true;
}

bool RelationTable::isArgFound(std::vector<Symbol> argList, Symbol arg) {
    for (Symbol validArg : argList) {
        if (validArg == arg) {
            return true;
        }
    }
    return false;
}

void RelationTable::initTable() {
    std::vector<Symbol> arg1;
    std::vector<Symbol> arg2;
    // relRef : ModifiesS | UsesS | Parent | ParentT | Follows | FollowsT
    // design-entity : stmt | assign | while | variable | constant | prog_line
    // stmt: assign | while | if | prog_line | call

    // stmt || assign while if call prog_line constant
    // procedure includes variable("procedureName")
    arg1 = { PROCEDURE, CALL, IF, STMT, ASSIGN, WHILE, VARIABLE, CONSTANT, PROGRAM_LINE};
    arg2 = { VARIABLE, UNDERSCORE };
    clauseMap1[USES] = arg1;
    clauseMap2[USES] = arg2;

    clauseMap1[MODIFIES] = arg1;
    clauseMap2[MODIFIES] = arg2;
    arg1.clear();
    arg2.clear();

    // stmt refers to all stmt types: stmt || assign while if call prog_line constant
    arg1 = { STMT,  WHILE, IF, CONSTANT, PROGRAM_LINE, UNDERSCORE };
    arg2 = { CALL, STMT, ASSIGN, WHILE, IF, CONSTANT, PROGRAM_LINE, UNDERSCORE };
    clauseMap1[PARENT] = arg1;
    clauseMap2[PARENT] = arg2;

    clauseMap1[PARENT_TRANSITIVE] = arg1;
    clauseMap2[PARENT_TRANSITIVE] = arg2;
    arg1.clear();
    arg2.clear();

    arg1 = { CALL, STMT, ASSIGN, WHILE, IF, CONSTANT, PROGRAM_LINE, UNDERSCORE };
    arg2 = { CALL, STMT, ASSIGN, WHILE, IF, CONSTANT, PROGRAM_LINE, UNDERSCORE };

    clauseMap1[FOLLOWS] = arg1;
    clauseMap2[FOLLOWS] = arg2;

    clauseMap1[FOLLOWS_TRANSITIVE] = arg1;
    clauseMap2[FOLLOWS_TRANSITIVE] = arg2;

    // procedure includes variable("procedureName")
    clauseMap1[NEXT] = arg1;
    clauseMap2[NEXT] = arg2;

    clauseMap1[NEXT_TRANSITIVE] = arg1;
    clauseMap2[NEXT_TRANSITIVE] = arg2;
    arg1.clear();
    arg2.clear();

    // variable refers to "procedureName" only
    arg1 = { PROCEDURE, VARIABLE, UNDERSCORE };
    arg2 = { PROCEDURE, VARIABLE, UNDERSCORE };
    clauseMap1[CALLS] = arg1;
    clauseMap2[CALLS] = arg2;

    clauseMap1[CALLS_TRANSITIVE] = arg1;
    clauseMap2[CALLS_TRANSITIVE] = arg2;
    arg1.clear();
    arg2.clear();


    arg1 = { STMT, ASSIGN, CONSTANT, PROGRAM_LINE, UNDERSCORE };
    arg2 = { STMT, ASSIGN, CONSTANT, PROGRAM_LINE, UNDERSCORE };
    clauseMap1[AFFECTS] = arg1;
    clauseMap2[AFFECTS] = arg2;

    clauseMap1[AFFECTS_TRANSITIVE] = arg1;
    clauseMap2[AFFECTS_TRANSITIVE] = arg2;
    arg1.clear();
    arg2.clear();

    // arg1: variable refers to "variableName" only
    // arg2: variable refers to "variableName" and _"expression"_
    // pattern refers to _"expression"_
    arg1 = { VARIABLE, UNDERSCORE };
    arg2 = { VARIABLE, UNDERSCORE, PATTERN };
    clauseMap1[ASSIGN] = arg1;
    clauseMap2[ASSIGN] = arg2;
    arg1.clear();
    arg2.clear();

    // variable refers to "variableName" only, exclude _"expression"_
    arg1 = { VARIABLE, UNDERSCORE };
    arg2 = { UNDERSCORE, STMTLIST };
    clauseMap1[WHILE] = arg1;
    clauseMap2[WHILE] = arg2;
    arg1.clear();
    arg2.clear();

    // variable refers to "variableName" only, exclude _"expression"_
    arg1 = { VARIABLE, UNDERSCORE };
    arg2 = { UNDERSCORE, STMTLIST };
    clauseMap1[IF] = arg1;
    clauseMap2[IF] = arg2;
    arg1.clear();
    arg2.clear();

    /*
    // Bonus feature
    arg1 = { PROCEDURE, STMTLIST, STMT, PROG_LINE, ASSIGN, CALL, WHILE, IF, PLUS, MINUS, TIMES, VARIABLE, CONSTANT };
    arg2 = { PROCEDURE, STMTLIST, STMT, PROG_LINE, ASSIGN, CALL, WHILE, IF, PLUS, MINUS, TIMES, VARIABLE, CONSTANT };
    clauseMap1[CONTAINS] = arg1;
    clauseMap2[CONTAINS] = arg2;
    clauseMap1[SIBLING] = arg1;
    clauseMap2[SIBLING] = arg2;
    clauseMap1[CONTAINS_TRANSITIVE] = arg1;
    clauseMap2[CONTAINS_TRANSITIVE] = arg2;
    arg1.clear();
    arg2.clear();
    */
}

#include <string>
#include <vector>
#include <unordered_map>
#include "QueryProcessor/RelationTable.h"

RelationTable::RelationTable() {
    initTable();
}

RelationTable::~RelationTable() {}

bool RelationTable::isArgValid(std::string relation, std::string arg, int i) {
    std::vector<std::string> argList;

    switch (i) {
    case 0:
        argList = clauseMap1[relation]; return isArgFound(argList, arg);
    case 1:
        argList = clauseMap2[relation]; return isArgFound(argList, arg);
    case 2:
        return arg.compare("underscore") == 0;
    default:
        return false;
    }

    return true;
}

bool RelationTable::isArgFound(std::vector<std::string> argList, std::string arg) {
    for (std::string validArg : argList) {
        if (validArg.compare(arg) == 0) {
            return true;
        }
    }
    return false;
}

void RelationTable::initTable() {
    std::vector<std::string> arg1;
    std::vector<std::string> arg2;
    // relRef : ModifiesS | UsesS | Parent | ParentT | Follows | FollowsT
    // design-entity : stmt | assign | while | variable | constant | prog_line
    // stmt: assign | while | if | prog_line
    arg1 = { "stmt", "assign", "while", "variable", "constant", "prog_line"};
    arg2 = { "varName", "variable", "underscore" };
    clauseMap1["Modifies"] = arg1;
    clauseMap2["Modifies"] = arg2;
    arg1.clear();
    arg2.clear();

    arg1 = { "stmt", "assign", "while", "variable", "constant", "prog_line"};
    arg2 = { "varName", "variable", "underscore" };
    clauseMap1["Uses"] = arg1;
    clauseMap2["Uses"] = arg2;
    arg1.clear();
    arg2.clear();

    arg1 = { "stmt", "assign", "while", "constant", "prog_line", "underscore"};
    arg2 = { "stmt", "assign", "while", "constant", "prog_line", "underscore"};
    clauseMap1["Parent"] = arg1;
    clauseMap2["Parent"] = arg2;

    clauseMap1["Parent*"] = arg1;
    clauseMap2["Parent*"] = arg2;
    arg1.clear();
    arg2.clear();

    arg1 = { "stmt", "assign", "while", "constant", "prog_line", "underscore" };
    arg2 = { "stmt", "assign", "while", "constant", "prog_line", "underscore" };
    clauseMap1["Follows"] = arg1;
    clauseMap2["Follows"] = arg2;

    clauseMap1["Follows*"] = arg1;
    clauseMap2["Follows*"] = arg2;
    arg1.clear();
    arg2.clear();

    arg1 = { "constantVarName", "varName", "variable", "underscore" };
    arg2 = { "constantVarName", "variable", "underscore" };
    clauseMap1["patternAssign"] = arg1;
    clauseMap2["patternAssign"] = arg2;
    arg1.clear();
    arg2.clear();

    arg1 = { "constantVarName", "varName", "variable", "underscore" };
    arg2 = { "underscore" };
    clauseMap1["pattern"] = arg1;
    clauseMap2["pattern"] = arg2;
    arg1.clear();
    arg2.clear();
}

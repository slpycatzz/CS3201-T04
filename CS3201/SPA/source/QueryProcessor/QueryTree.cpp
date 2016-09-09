#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "QueryProcessor/QueryTree.h"

QueryTree::QueryTree() {}

QueryTree::~QueryTree() {}


// varType(Symbol), varList
bool QueryTree::insertSelect(std::string varType, std::vector<std::string> varList) {
    Symbol symbol = Constants::StringToSymbol(varType);

    for (unsigned int i = 0; i < varList.size(); i++) {
        varMap[varList[i]] = symbol;
    }

    return true;
}

// wm todo: can use insertSelect instead
bool QueryTree::insertSelect(std::string varName, std::string varType) {
    Symbol symbol = Constants::StringToSymbol(varType);
    varMap[varName] = symbol;
    varList.push_back(varName);
    return true;
}

// ClauseType(Uses,Parent...), ClauseArgList, ClauseArgCount
bool QueryTree::insertSuchThat(std::string clauseName, std::vector<std::string> argList) {
    Clause clause;
    clause.setClauseType(clauseName);
    clause.setArg(argList);

    suchThatList.push_back(clause);

    return true;
}
// Varname(a,w1...), ClauseArgList, ClauseArgCount
bool QueryTree::insertPattern(std::string varName, std::vector<std::string> argList) {
    return false;
}

std::unordered_map<std::string, Symbol> QueryTree::getSelect() {
    return varMap;
}

std::vector<std::string> QueryTree::getResults() {
    return varList;
}

std::vector<Clause> QueryTree::getSuchThat() {
    return suchThatList;
}
std::vector<Clause> QueryTree::getPattern() {
    std::vector<Clause> result;
    return result;
}

std::vector<Clause> QueryTree::getClauses(std::string clauseType) {
    std::vector<Clause> result;
    if (clauseType.compare("pattern")) {
        result = getPattern();
    } else {
        result = getSuchThat();
    }
    return result;
}


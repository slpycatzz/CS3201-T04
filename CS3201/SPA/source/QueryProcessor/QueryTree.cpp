#include "QueryProcessor/QueryTree.h"
#include <string>
#include <vector>
#include <unordered_map>


QueryTree::QueryTree() {}

QueryTree::~QueryTree() {}


// varType(Symbol), varList
bool QueryTree::insertSelect(std::string varType, std::vector<std::string> varList) {
    return false;
}

// ClauseType(Uses,Parent...), ClauseArgList, ClauseArgCount
bool QueryTree::insertSuchThat(Symbol sym, std::vector<std::string> argList, int argCount) {
    return false;
}
// Varname(a,w1...), ClauseArgList, ClauseArgCount
bool QueryTree::insertPattern(std::string varName, std::vector<std::string> argList, int argCount) {
    return false;
}

std::unordered_map<Symbol, std::vector<std::string>> QueryTree::getSelect() {
    std::unordered_map<Symbol, std::vector<std::string>> result;
    return result;
}

std::vector<std::string> QueryTree::getResults() {
    std::vector<std::string> result;
    std::unordered_map<Symbol, std::vector<std::string>> resultList;
    resultList = getSelect();
    Symbol sym = VARIABLE;
    result = resultList[sym];
    return result;
}

std::vector<Clause> QueryTree::getSuchThat() {
    std::vector<Clause> result;
    return result;
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


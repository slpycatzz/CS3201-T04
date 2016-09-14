#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "QueryProcessor/QueryTree.h"
#include "Utils.h"

QueryTree::QueryTree() {}

QueryTree::~QueryTree() {}


// varType(Symbol), varList
bool QueryTree::insertSelect(std::string varType, std::vector<std::string> varList) {
    Symbol symbol = Constants::StringToSymbol(varType);

    for (unsigned int i = 0; i < varList.size(); i++) {
        varSelectMap[varList[i]] = symbol;
    }

    return true;
}

bool QueryTree::insertDeclaration(std::unordered_map<std::string, Symbol> varSymbolMap) {
    varMap = varSymbolMap;
    return true;
}

// wm todo: can use insertSelect instead
bool QueryTree::insertSelect(std::string varName, std::string varType) {
    Symbol symbol = Constants::StringToSymbol(varType);
    varSelectMap[varName] = symbol;
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
    Clause clause;
    clause.setClauseType(varName);
    clause.setArg(argList);

    patternList.push_back(clause);

    return false;
}


// wm TODO: SELECT MAY BECOME <a1,a2> (A PAIR) SO VARMAP MAY BE UNSUITABLE AFTER ITERATION 1
// considering change to varList probably?
std::unordered_map<std::string, Symbol> QueryTree::getSelect() {
    return varSelectMap;
}

std::vector<std::string> QueryTree::getResults() {
    return varList;
}

std::vector<Clause> QueryTree::getSuchThat() {
    return suchThatList;
}
std::vector<Clause> QueryTree::getPattern() {
    return patternList;
}

std::vector<Clause> QueryTree::getClauses(std::string clauseType) {
    std::vector<Clause> result;
    std::vector<std::string> typeList = Utils::SplitAndIgnoreEmpty(clauseType, ' ');
    for (std::string type : typeList) {
        if (type == "pattern") {
            std::vector<Clause> patternClauses = getPattern();
            result.insert(result.end(), patternClauses.begin(), patternClauses.end());
        } else {
            std::vector<Clause> suchThatClauses = getSuchThat();
            result.insert(result.end(), suchThatClauses.begin(), suchThatClauses.end());
        }
    }
    return result;
}

/* returns delcared Variable mapping */
std::unordered_map<std::string, Symbol> QueryTree::getVarMap() {
    return varMap;
}


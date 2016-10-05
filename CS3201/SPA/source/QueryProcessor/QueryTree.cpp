#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "Constants.h"
#include "QueryProcessor/QueryTree.h"
#include "Utils.h"

QueryTree::QueryTree() {}

QueryTree::~QueryTree() {}

bool QueryTree::insertDeclaration(std::unordered_map<std::string, Symbol> varSymbolMap) {
    for (std::pair<std::string, Symbol> pair : varSymbolMap) {
        varMap.insert(pair);
    }
    return true;
}

/* should be insert to varList only, varSelectMap to be phased out */
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

std::vector<Clause> QueryTree::getBooleanClauses() {
    return booleanClauses;
}

std::vector<std::vector<Clause>> QueryTree::getUnselectedGroups() {
    return unselectedGroups;
}

std::vector<std::vector<Clause>> QueryTree::getSelectedGroups() {
    return selectedGroups;
}

void QueryTree::setBooleanClauses(std::vector<Clause> bc) {
    booleanClauses = bc;
}

void QueryTree::setUnselectedGroups(std::vector<std::vector<Clause>> ug) {
    unselectedGroups = ug;
}

void QueryTree::setSelectedGroups(std::vector<std::vector<Clause>> sg) {
    selectedGroups = sg;
}

void QueryTree::printGroups() {
    std::ofstream output;
    output.open("tests/output.txt");

    output << "selected variables \n" << "-------------" << "\n";
    for (std::string s : getResults()) {
        output << s << " ";
    }
    output << "\n\n";

    output << "boolean clauses \n" << "-------------" << "\n";
    for (Clause clause : getBooleanClauses()) {
        output << clause.toString();
    }
    output << "\n";

    int i = 1;
    for (std::vector<Clause> group : getUnselectedGroups()) {
        output << "unselected group " << i << "\n" << "-------------" << "\n";
        i++;
        for (Clause clause : group) {
            output << clause.toString();
        }
        output << "\n";
    }

    int j = 1;
    for (std::vector<Clause> group : getSelectedGroups()) {
        output << "selected group " << j << "\n" << "-------------" << "\n";
        j++;
        for (Clause clause : group) {
            output << clause.toString();
        }
        output << "\n";
    }
    output.close();
}
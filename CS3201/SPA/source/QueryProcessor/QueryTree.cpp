#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "Constants.h"
#include "QueryProcessor/QueryTree.h"
#include "Utils.h"

QueryTree::QueryTree() {}

QueryTree::~QueryTree() {}

bool QueryTree::insertDeclaration(std::unordered_map<std::string, Symbol> variableMap) {
    for (std::pair<std::string, Symbol> pair : variableMap) {
        varMap.insert(pair);
    }
    return true;
}

// argType = varType(ASSIGN,CALL,etc...)    [for Select]
// argType = relation(uses,modifies,etc...) [for-suchthat]
// argType = varName(a1,w,ifstmt, etc...)   [for-pattern]
// argType = varName(p,c1,s,callstmt, stmt) [for-with]
bool QueryTree::insert(Symbol type, std::string argType, std::vector<std::string> argList) {
    Clause clause;
    switch (type) {
    case QUERY_RESULT:
        for (std::string var : argList) {
            varList.push_back(var);
        }
        return true;
    case SUCH_THAT:
        clause.setClauseType(argType);
        clause.setArg(argList);

        suchThatList.push_back(clause);
        return true;
    case PATTERN:
        clause.setClauseType(argType);
        clause.setArg(argList);

        patternList.push_back(clause);
        return true;
    case WITH:
        clause.setClauseType(argType);
        clause.setArg(argList);

        withList.push_back(clause);
        return true;
    default:
        return false;
    }
}

std::vector<std::string> QueryTree::getResults() {
    return varList;
}
std::unordered_map<std::string,bool> QueryTree::getResultsInfo() {
    return varAttrMap;
}
std::vector<Clause> QueryTree::getClauses() {
    std::vector<Clause> result;
    std::vector<Symbol> clauseList = { SUCH_THAT, PATTERN, WITH };
    
    result = getClauses(clauseList);
    return result;
}

std::vector<Clause> QueryTree::getClauses(std::vector<Symbol> clauseType) {
    std::vector<Clause> result;
    std::vector<Clause> clauseList;

    for (Symbol clauseName : clauseType) {
        switch(clauseName) {
        case PATTERN:
            result.insert(result.end(), patternList.begin(), patternList.end());
            break;
        case SUCH_THAT:
            result.insert(result.end(), suchThatList.begin(), suchThatList.end());
            break;
        case WITH:
            result.insert(result.end(), withList.begin(), withList.end());
            break;
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
        output << clause.toString() << "\n";
    }
    output << "\n";

    int i = 1;
    for (std::vector<Clause> group : getUnselectedGroups()) {
        output << "unselected group " << i << "\n" << "-------------" << "\n";
        i++;
        for (Clause clause : group) {
            output << clause.toString() << "\n";
        }
        output << "\n";
    }

    int j = 1;
    for (std::vector<Clause> group : getSelectedGroups()) {
        output << "selected group " << j << "\n" << "-------------" << "\n";
        j++;
        for (Clause clause : group) {
            output << clause.toString() << "\n";
        }
        output << "\n";
    }
    output.close();
}
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Constants.h"
#include "QueryProcessor/QueryTree.h"
#include "Utils.h"

using std::string;
using std::unordered_map;
using std::vector;

QueryTree::QueryTree() {}

QueryTree::~QueryTree() {}

bool QueryTree::insertDeclaration(unordered_map<string, Symbol> &variableMap) {
    /*for (std::pair<string, Symbol> pair : variableMap) {
        varMap.insert(pair);
    }*/
	/* More optimized code */
	varMap = variableMap;
    return true;
}

// inserts varAttrMap to check whether varAttribute is required by query projector
// true: varAttribute is required, false: otherwise
bool QueryTree::insert(Symbol type, string argType, unordered_map<string, bool> argList) {
    if (type != QUERY_RESULT) {
        return false;
    }

    for (std::pair<string, bool> pair : argList) {
        varAttrMap.insert(pair);
    }
    return true;
}

// argType = varType(ASSIGN,CALL,etc...)    [for Select]
// argType = relation(uses,modifies,etc...) [for-suchthat]
// argType = varName(a1,w,ifstmt, etc...)   [for-pattern]
// argType = varName(p,c1,s,callstmt, stmt) [for-with]
bool QueryTree::insert(Symbol type, string argType, vector<string> argList) {
    Clause clause;
    switch (type) {
    case QUERY_RESULT:
        for (string var : argList) {
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
        clause.setClauseType("with");
        argList.push_back(argType);
        clause.setArg(argList);

        withList.push_back(clause);
        return true;
    default:
        return false;
    }
}

vector<string> QueryTree::getResults() {
    return varList;
}

unordered_map<string, bool> QueryTree::getResultsInfo() {
    return varAttrMap;
}

vector<Clause> QueryTree::getClauses() {
    vector<Clause> result;
    vector<Symbol> clauseList = { SUCH_THAT, PATTERN, WITH };

    result = getClauses(clauseList);
    return result;
}

vector<Clause> QueryTree::getClauses(vector<Symbol> clauseType) {
    vector<Clause> result;
    vector<Clause> clauseList;

    for (Symbol clauseName : clauseType) {
        switch (clauseName) {
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
unordered_map<string, Symbol> QueryTree::getVarMap() {
    return varMap;
}

vector<Clause> QueryTree::getBooleanClauses() {
    return booleanClauses;
}

vector<std::pair<vector<string>, vector<Clause>>> QueryTree::getUnselectedGroups() {
    return unselectedGroups;
}

vector<std::pair<vector<string>, vector<Clause>>> QueryTree::getSelectedGroups() {
    return selectedGroups;
}

void QueryTree::setBooleanClauses(vector<Clause> bc) {
    booleanClauses = bc;
}

void QueryTree::setUnselectedGroups(vector<std::pair<vector<string>, vector<Clause>>> ug) {
    unselectedGroups = ug;
}

void QueryTree::setSelectedGroups(vector<std::pair<vector<string>, vector<Clause>>> sg) {
    selectedGroups = sg;
}

void QueryTree::printGroups() {
    std::ofstream output;
    output.open("tests/output.txt");

    output << "declared variables \n" << "-------------" << "\n";
    for (auto s : getVarMap()) {
        output << s.first << " " << Constants::SymbolToString(s.second) << "\n";
    }
    output << "\n\n";

    output << "selected variables \n" << "-------------" << "\n";
    for (string s : getResults()) {
        output << s << " ";
    }
    output << "\n\n";

    if (getBooleanClauses().size() > 0) {
        output << "boolean clauses \n" << "-------------" << "\n";
        for (Clause clause : getBooleanClauses()) {
            output << clause.toString() << "\n";
        }
        output << "\n";
    }

    int i = 1;
    for (std::pair<vector<string>, vector<Clause>> pair : getUnselectedGroups()) {
        output << "unselected group " << i << "\n" << "-------------" << "\n";

        for (string synonym : pair.first) {
            output << synonym << " ";
        }
        output << "\n";
        i++;
        for (Clause clause : pair.second) {
            output << clause.toString() << "\n";
        }
        output << "\n";
    }

    int j = 1;
    for (std::pair<vector<string>, vector<Clause>> pair : getSelectedGroups()) {
        output << "selected group " << j << "\n" << "-------------" << "\n";

        for (string synonym : pair.first) {
            output << synonym << " ";
        }
        output << "\n";
        j++;
        for (Clause clause : pair.second) {
            output << clause.toString() << "\n";
        }
        output << "\n";
    }
    output.close();
}

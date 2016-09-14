#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Clause.h"
#include "Constants.h"

class QueryTree {
 public:
    QueryTree();
    ~QueryTree();

    /* returns: varMap[varName] = varType e.g. varMap[a1] = ASSIGN */
    std::unordered_map<std::string, Symbol> getVarMap();
    std::vector<std::string> getResults();
    std::vector<Clause> getClauses(std::string ClauseType);

    bool insertDeclaration(std::unordered_map<std::string, Symbol>);
    bool insertSelect(std::string, std::string);
    bool insertSuchThat(std::string, std::vector<std::string>);
    bool insertPattern(std::string, std::vector<std::string>);
    

    // wm todo: unused methods, to be removed after testing
    std::unordered_map<std::string, Symbol> getSelect();
    std::vector<Clause> getPattern();
    std::vector<Clause> getSuchThat();

 private:
     // wm todo: varSelectMap redundant, only for support of 
     //             SelectList which should be the correct container
     std::unordered_map<std::string, Symbol> varSelectMap;
     /* container for list of declared variableMap to Symbol*/
     std::unordered_map<std::string, Symbol> varMap;
     std::vector<std::string> varList;
     std::vector<Clause> suchThatList;
     std::vector<Clause> patternList;
};

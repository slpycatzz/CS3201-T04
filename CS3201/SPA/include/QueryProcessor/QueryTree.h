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

    std::vector<Clause> getClauses(std::string ClauseType);
    std::vector<std::string> getResults();

    bool insertSuchThat(std::string, std::vector<std::string>);
    bool insertSelect(std::string, std::vector<std::string>);
    bool insertPattern(std::string, std::vector<std::string>);

    // wm todo: unused methods, to be removed after testing
    bool insertSelect(std::string, std::string);

    std::unordered_map<std::string, Symbol> getSelect();
    std::vector<Clause> getPattern();
    std::vector<Clause> getSuchThat();

 private:
     std::unordered_map<std::string, Symbol> varMap;
     std::vector<std::string> varList;
     std::vector<Clause> suchThatList;
     std::vector<Clause> patternList;
};

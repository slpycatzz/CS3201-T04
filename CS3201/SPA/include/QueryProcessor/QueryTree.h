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

    std::unordered_map<Symbol, std::vector<std::string>> QueryTree::getSelect();
    std::vector<Clause> QueryTree::getPattern();
    std::vector<Clause> QueryTree::getSuchThat();

    std::vector<Clause> getClauses(std::string ClauseType);
    std::vector<std::string> getResults();

 private:
     bool insertSuchThat(Symbol, std::vector<std::string>, int);
     bool insertSelect(std::string, std::vector<std::string>);
     bool insertPattern(std::string, std::vector<std::string>, int);
};

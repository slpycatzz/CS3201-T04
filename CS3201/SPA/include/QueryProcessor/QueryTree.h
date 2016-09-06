#pragma once
#include "Tree.h"
#include "Clause.h"
#include <string>

class QueryTree : public Tree {
 public:
    QueryTree();
    ~QueryTree();

	std::vector<Clause> getClauses(std::string ClauseType);
	std::vector<std::string> getResults();
};

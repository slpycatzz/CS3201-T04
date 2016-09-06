#pragma once
#include "Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
typedef std::vector<std::string> ResultList;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

 private:
	 bool evaluateClause(PKB pkb, Clause clause);
	 bool evaluateQuery(PKB pkb, QueryTree query);
	 ResultList selectClauseResults(PKB pkb, Clause clause);
	 ResultList selectQueryResults(PKB pkb, QueryTree query);
	 
};

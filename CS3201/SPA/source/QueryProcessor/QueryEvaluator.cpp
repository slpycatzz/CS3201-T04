#include "QueryProcessor/Clause.h"
#include "QueryProcessor/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}

bool QueryEvaluator::evaluateClause(PKB pkb, Clause clause) {
	return false;
}

ResultList QueryEvaluator::selectClauseResults(PKB pkb, Clause clause) {
	ResultList resultList;
	return resultList;
}

bool QueryEvaluator::evaluateQuery(PKB pkb, QueryTree query) {
	return false;
}

ResultList QueryEvaluator::selectQueryResults(PKB pkb, QueryTree query) {
	ResultList resultList;
	return resultList;
}

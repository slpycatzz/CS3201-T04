#include "QueryProcessor/Clause.h"
#include "QueryProcessor/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}

bool QueryEvaluator::evaluateClause(const PKB &pkb, const Clause &clause)
{
	return false;
}

TupleCandidateList
QueryEvaluator::selectClauseResults(const PKB &pkb, const Clause &clause,
									TupleCandidateList candidates)
{
	TupleCandidateList filteredList;
	return filteredList;
}

bool QueryEvaluator::evaluateQuery(const PKB &pkb, const QueryTree &query)
{
	return false;
}

ResultList QueryEvaluator::selectQueryResults(const PKB &pkb, const QueryTree &query)
{
	ResultList resultList;
	return resultList;
}

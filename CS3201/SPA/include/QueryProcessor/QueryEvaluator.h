#pragma once
#include "Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
typedef std::vector<std::string> ResultList;
typedef std::string Candidate;
typedef std::vector<Candidate> SingleCandidateList;
typedef std::vector<SingleCandidateList> TupleCandidateList;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

 private:
	 // evaluate a single clause to see if it is true
	 bool evaluateClause(const PKB &pkb, const Clause &clause);

	 // evaluate a single query to see if it is true
	 bool evaluateQuery(const PKB &pkb, const QueryTree &query);

	 // use the clause to filter the tuple candidate list to its sublist
	 TupleCandidateList selectClauseResults(const PKB &pkb, const Clause &clause, TupleCandidateList candidates);
	 
	 // return the result list as a list of strings after evaluating the query
	 ResultList selectQueryResults(const PKB &pkb, const QueryTree &query);
	 
};

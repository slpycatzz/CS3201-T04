#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

#include "QueryProcessor/Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
#include "TotalCombinationList.h"

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

    // return the result list as a list of strings after evaluating the query
    ResultList selectQueryResults(QueryTree &query);

	 // retrieve all possible candidates for a variable
	 PartialCombinationList getCandidates(std::pair<Synonym, Symbol> var);

	 TotalCombinationList getTotalCandidateList(QueryTree &query);

	 // evaluate a single query to see if it is true
	 bool evaluateQuery(QueryTree &query);

	 // use the clause to filter the tuple candidate list to its sublist
	 void filterByClause(Clause &clause, TotalCombinationList &combinations);

	 void filterNoVarPattern(Synonym assignStmt, Candidate lhs, Candidate expr,
		 TotalCombinationList &combinations);

	 void filterOneVarPattern(Synonym assignStmt, Synonym lhs, Candidate expr,
		 TotalCombinationList &combinations);

	 void filterTwoVarsClause(std::string clauseType,
		 Synonym &var0, Synonym &var1, TotalCombinationList &combinations);

	 void filterFirstVarClause(std::string clauseType, Synonym var,
		 Candidate constant, TotalCombinationList &combinations);

	 void filterSecondVarClause(std::string clauseType, Candidate constant,
		 Synonym var, TotalCombinationList &combinations);

	 void filterNoVarClause(std::string clauseType, Candidate const1,
		 Candidate const2, TotalCombinationList &combinations);

	 void insertMap(std::vector<std::string> list, Synonym var, PartialCombinationList &result);

	 ResultList getResultsFromCombinationList(TotalCombinationList &combinations,
		 std::vector<Synonym> &selectList);

	 PartialCombinationList getSelectedCombinations(TotalCombinationList &cands,
		 std::vector<Synonym>& selectList);

	 PartialCombinationList mergeCombinationList(PartialCombinationList &combList1,
		 PartialCombinationList &combList2);

	 bool isBoolSelect(std::unordered_map<std::string, Symbol>& selectList);

	 // evaluate a single clause to see if it is true
	 bool evaluateClause(Clause &clause, CandidateCombination &candMap);

	 bool evaluatePatternClause(Candidate assignStmt, Candidate lhsVar, std::string expr);

	 bool evaluateSuchThatClause(std::string clauseType, Candidate var0, Candidate var1);

	 bool evaluateModifies(Candidate var0, Candidate var1);
	 bool evaluateFollows(Candidate var0, Candidate var1);

	 std::string log;

private:

	bool evaluateUses(Candidate var0, Candidate var1);
	
	bool evaluateParent(Candidate var0, Candidate var1);
	bool evaluateParentStar(Candidate var0, Candidate var1);

	bool evaluateFollowsStar(Candidate var0, Candidate var1);
	bool evaluateNext(Candidate var0, Candidate var1);
	bool evaluateNextStar(Candidate var0, Candidate var1);
	bool evaluateAffects(Candidate var0, Candidate var1);
	bool evaluateAffectsStar(Candidate var0, Candidate var1);
	bool evaluateCalls(Candidate var0, Candidate var1);
	bool evaluateCallsStar(Candidate var0, Candidate var1);
	 
};

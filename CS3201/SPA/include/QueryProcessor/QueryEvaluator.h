#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

#include "QueryProcessor/Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"

typedef std::vector<std::string> ResultList;
typedef std::string Candidate;
typedef std::string VarName;
typedef int Cand;
typedef std::unordered_map<VarName, Candidate> CandidateCombination;
typedef std::vector<CandidateCombination> PartialCombinationList;
typedef std::unordered_map<VarName, PartialCombinationList> TotalCombinationList;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

    // return the result list as a list of strings after evaluating the query
    ResultList selectQueryResults(QueryTree &query);

	 // retrieve all possible candidates for a variable
	 PartialCombinationList getCandidates(std::pair<VarName, Symbol> var);

	 TotalCombinationList getTotalCandidateList(QueryTree &query);

	 // evaluate a single query to see if it is true
	 bool evaluateQuery(QueryTree &query);

	 // use the clause to filter the tuple candidate list to its sublist
	 bool selectClauseResults(Clause &clause, TotalCombinationList &totalCandMap);

	 bool FilterNoVarPattern(VarName assignStmt, Candidate lhs, Candidate expr,
		 TotalCombinationList &combinations);

	 bool FilterOneVarPattern(VarName assignStmt, VarName lhs, Candidate expr,
		 TotalCombinationList &combinations);

	 bool FilterTwoVarsClause(std::string clauseType,
		 VarName &var0, VarName &var1, TotalCombinationList &combinations);

	 bool FilterFirstVarClause(std::string clauseType, VarName var,
		 Candidate constant, TotalCombinationList &combinations);

	 bool FilterSecondVarClause(std::string clauseType, Candidate constant,
		 VarName var, TotalCombinationList &combinations);

	 bool FilterNoVarClause(std::string clauseType, Candidate const1,
		 Candidate const2, TotalCombinationList &combinations);

	 void insertMap(std::vector<std::string> list, VarName var, PartialCombinationList &result);

	 ResultList getResultsFromCombinationList(TotalCombinationList &combinations,
		 std::vector<VarName> &selectList);

	 PartialCombinationList getSelectedCombinations(TotalCombinationList &cands,
		 std::vector<VarName>& selectList);

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

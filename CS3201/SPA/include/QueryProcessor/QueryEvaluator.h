#pragma once
#include "Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
typedef std::vector<std::string> ResultList;
typedef std::string Candidate;
typedef std::string Var;
typedef int Cand;
typedef std::unordered_map<Var, Candidate> CandidateCombination;
typedef std::vector<CandidateCombination> PartialCombinationList;
typedef std::unordered_map<Var, PartialCombinationList> TotalCombinationList;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

 private:
	 // evaluate a single clause to see if it is true
	 bool evaluateClause(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 
	 bool evaluateUses(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateModifies(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateParent(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateParentStar(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateFollows(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateFollowsStar(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateNext(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateNextStar(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateAffects(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateAffectsStar(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateCalls(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 bool evaluateCallsStar(PKB &pkb, Clause &clause, CandidateCombination &candMap);

	 // retrieve all possible candidates for a variable
	 PartialCombinationList getCandidates(PKB &pkb, std::pair<Var, Symbol> var);

	 TotalCombinationList getTotalCandidateList(PKB & pkb, QueryTree & query);

	 // evaluate a single query to see if it is true
	 bool evaluateQuery(PKB &pkb, QueryTree &query);

	 // use the clause to filter the tuple candidate list to its sublist
	 bool selectClauseResults(PKB &pkb, Clause &clause, TotalCombinationList &totalCandMap);

	 void insertMap(std::vector<std::string> list, Var var, PartialCombinationList &result);

	 bool Filter(PartialCombinationList &candidateList,
		 PKB & pkb, Clause & clause,
		 TotalCombinationList & candidates);

	 bool MergeAndFilter(PartialCombinationList &candidateList0,
		 PartialCombinationList &candidateList1,
		 PKB & pkb, Clause & clause,
		 TotalCombinationList & candidates);
	 
	 // return the result list as a list of strings after evaluating the query
	 ResultList selectQueryResults(PKB&, QueryTree&);

	 ResultList getResultsFromTotalCandidateList(TotalCombinationList &cands, std::unordered_map<std::string, Symbol> &selectList);

	 bool isBoolSelect(std::unordered_map<std::string, Symbol>& selectList);
	 
};

#pragma once
#include "Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
typedef std::vector<std::string> ResultList;
typedef std::string Candidate;
typedef std::string Var;
typedef int Cand;
typedef std::unordered_map<Var, Candidate> CandidateMap;
typedef std::vector<CandidateMap> CandidateMapList;
//typedef std::vector<CandidateTuple> FlatCandidateList;
typedef std::unordered_map<Var, CandidateMapList> TotalCandidateMap;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

 private:
	 // evaluate a single clause to see if it is true
	 bool evaluateClause(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 
	 bool evaluateUses(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateModifies(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateParent(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateParentStar(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateFollows(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateFollowsStar(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateNext(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateNextStar(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateAffects(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateAffectsStar(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateCalls(PKB &pkb, Clause &clause, CandidateMap &candMap);
	 bool evaluateCallsStar(PKB &pkb, Clause &clause, CandidateMap &candMap);

	 // retrieve all possible candidates for a variable
	 CandidateMapList getCandidates(PKB &pkb, std::pair<Var, Symbol> var);

	 TotalCandidateMap getTotalCandidateList(PKB & pkb, QueryTree & query);

	 // evaluate a single query to see if it is true
	 bool evaluateQuery(PKB &pkb, QueryTree &query);

	 // use the clause to filter the tuple candidate list to its sublist
	 bool selectClauseResults(PKB &pkb, Clause &clause, TotalCandidateMap &totalCandMap);

	 void insertMap(std::vector<std::string> list, Var var, CandidateMapList &result);

	 bool Filter(CandidateMapList &candidateList,
		 PKB & pkb, Clause & clause,
		 TotalCandidateMap & candidates);

	 bool MergeAndFilter(CandidateMapList &candidateList0,
		 CandidateMapList &candidateList1,
		 PKB & pkb, Clause & clause,
		 TotalCandidateMap & candidates);
	 
	 // return the result list as a list of strings after evaluating the query
	 ResultList selectQueryResults(PKB&, QueryTree&);

	 ResultList getResultsFromTotalCandidateList(TotalCandidateMap &cands, std::unordered_map<std::string, Symbol> &selectList);

	 bool isBoolSelect(std::unordered_map<std::string, Symbol>& selectList);
	 
};

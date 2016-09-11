#pragma once
#include "Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
typedef std::vector<std::string> ResultList;
typedef std::string Candidate;
typedef std::string Var;
typedef std::string Cand;
typedef std::unordered_map<Var, Cand> CandidateMap;
typedef std::vector<CandidateMap> CandidateMapList;
//typedef std::vector<CandidateTuple> FlatCandidateList;
typedef std::unordered_map<Var, CandidateMapList> TotalCandidateList;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

 private:
	 // evaluate a single clause to see if it is true
	 bool evaluateClause(PKB&, Clause&, CandidateMap);
	 
	 bool evaluateUses(PKB&, Clause&, CandidateMap);
	 bool evaluateModifies(PKB&, Clause&, CandidateMap);
	 bool evaluateParent(PKB&, Clause&, CandidateMap);
	 bool evaluateParentStar(PKB&, Clause&, CandidateMap);
	 bool evaluateFollows(PKB&, Clause&, CandidateMap);
	 bool evaluateFollowsStar(PKB&, Clause&, CandidateMap);
	 bool evaluateNext(PKB&, Clause&, CandidateMap);
	 bool evaluateNextStar(PKB&, Clause&, CandidateMap);
	 bool evaluateAffects(PKB&, Clause&, CandidateMap);
	 bool evaluateAffectsStar(PKB&, Clause&, CandidateMap);
	 bool evaluateCalls(PKB&, Clause&, CandidateMap);
	 bool evaluateCallsStar(PKB&, Clause&, CandidateMap);

	 // retrieve all possible candidates for a variable
	 CandidateMapList getCandidates(PKB &pkb, std::pair<Var, Symbol> var);

	 TotalCandidateList getTotalCandidateList(PKB & pkb, QueryTree & query);

	 // evaluate a single query to see if it is true
	 bool evaluateQuery(PKB&, QueryTree&);

	 // use the clause to filter the tuple candidate list to its sublist
	 bool selectClauseResults(PKB&, Clause&, TotalCandidateList&);

	 void insertMap(std::vector<std::string> list, Var var, CandidateMapList &result);

	 bool Filter(CandidateMapList &candidateList,
		 PKB & pkb, Clause & clause,
		 TotalCandidateList & candidates);

	 bool MergeAndFilter(CandidateMapList &candidateList0,
		 CandidateMapList &candidateList1,
		 PKB & pkb, Clause & clause,
		 TotalCandidateList & candidates);
	 
	 // return the result list as a list of strings after evaluating the query
	 ResultList selectQueryResults(PKB&, QueryTree&);
	 
};

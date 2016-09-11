#pragma once
#include "Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
typedef std::vector<std::string> ResultList;
typedef std::string Candidate;
typedef std::string Var;
typedef int Cand;
typedef std::unordered_map<Var, Cand> CandidateTuple;
typedef std::vector<CandidateTuple> CandidateTupleList;
//typedef std::vector<CandidateTuple> FlatCandidateList;
typedef std::unordered_map<Var, CandidateTupleList> TotalCandidateList;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

 private:
	 // evaluate a single clause to see if it is true
	 bool evaluateClause(PKB&, Clause&, CandidateTuple);
	 
	 bool evaluateUses(PKB&, Clause&, CandidateTuple);
	 bool evaluateModifies(PKB&, Clause&, CandidateTuple);
	 bool evaluateParent(PKB&, Clause&, CandidateTuple);
	 bool evaluateParentStar(PKB&, Clause&, CandidateTuple);
	 bool evaluateFollows(PKB&, Clause&, CandidateTuple);
	 bool evaluateFollowsStar(PKB&, Clause&, CandidateTuple);
	 bool evaluateNext(PKB&, Clause&, CandidateTuple);
	 bool evaluateNextStar(PKB&, Clause&, CandidateTuple);
	 bool evaluateAffects(PKB&, Clause&, CandidateTuple);
	 bool evaluateAffectsStar(PKB&, Clause&, CandidateTuple);
	 bool evaluateCalls(PKB&, Clause&, CandidateTuple);
	 bool evaluateCallsStar(PKB&, Clause&, CandidateTuple);

	 // evaluate a single query to see if it is true
	 bool evaluateQuery(PKB&, QueryTree&);

	 // use the clause to filter the tuple candidate list to its sublist
	 TotalCandidateList selectClauseResults(PKB&, Clause&, TotalCandidateList&);

	 void Filter(CandidateTupleList &candidateList,
		 PKB & pkb, Clause & clause,
		 TotalCandidateList & candidates);

	 void MergeAndFilter(CandidateTupleList &candidateList0,
		 CandidateTupleList &candidateList1,
		 PKB & pkb, Clause & clause,
		 TotalCandidateList & candidates);
	 
	 // return the result list as a list of strings after evaluating the query
	 ResultList selectQueryResults(PKB&, QueryTree&);
	 
};

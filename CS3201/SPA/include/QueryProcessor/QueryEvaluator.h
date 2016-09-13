#pragma once
#include "Clause.h"
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
    ResultList selectQueryResults(PKB &pkb, QueryTree &query);

 private:
	 // evaluate a single clause to see if it is true
	 //bool evaluateClause(PKB &pkb, Clause &clause, CandidateCombination &candMap);
	 
	 bool evaluateSuchThatClause(PKB &pkb, std::string clauseType, Candidate var0, Candidate var1);
	 bool evaluateUses(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateModifies(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateParent(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateParentStar(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateFollows(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateFollowsStar(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateNext(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateNextStar(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateAffects(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateAffectsStar(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateCalls(PKB &pkb, Candidate var0, Candidate var1);
	 bool evaluateCallsStar(PKB &pkb, Candidate var0, Candidate var1);

	 // retrieve all possible candidates for a variable
	 PartialCombinationList getCandidates(PKB &pkb, std::pair<VarName, Symbol> var);

	 TotalCombinationList getTotalCandidateList(PKB & pkb, QueryTree & query);

	 // evaluate a single query to see if it is true
	 bool evaluateQuery(PKB &pkb, QueryTree &query);

	 // use the clause to filter the tuple candidate list to its sublist
	 bool selectClauseResults(PKB &pkb, Clause &clause, TotalCombinationList &totalCandMap);

	 bool FilterTwoVarsClause(PKB &pkb, std::string clauseType,
		 VarName &var0, VarName &var1, TotalCombinationList &combinations);

	 bool FilterFirstVarClause(PKB &pkb, std::string clauseType, VarName var,
		 Candidate constant, TotalCombinationList &combinations);

	 bool FilterSecondVarClause(PKB &pkb, std::string clauseType, Candidate constant,
		 VarName var, TotalCombinationList &combinations);

	 bool FilterNoVarClause(PKB &pkb, std::string clauseType, Candidate const1,
		 Candidate const2, TotalCombinationList &combinations);

	 void insertMap(std::vector<std::string> list, VarName var, PartialCombinationList &result);

	 ResultList getResultsFromCombinationList(TotalCombinationList &cands, std::unordered_map<std::string, Symbol> &selectList);

	 bool isBoolSelect(std::unordered_map<std::string, Symbol>& selectList);
	 
};

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

#include "QueryProcessor/Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
#include "TotalCombinationList.h"

typedef std::pair<std::vector<std::string>, std::vector<std::vector<Candidate>>> ResultList;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

     // return the result list as a ResultList after evaluating the query
     ResultList selectQueryResults(QueryTree &query);

	 // return the result list as a ResultList after evaluating the query
	 TotalCombinationList getQueryResults(QueryTree &query);

	 // get results for clauses of selected synonyms
	 TotalCombinationList getSelectedGroupResult(std::vector<Synonym> &synList,
		 std::unordered_map<Synonym, Symbol> &varMap,
		 std::vector<Clause> &clauseGroup,
		 std::vector<Synonym> &selectList);

	 // get results for clauses of unselected synonyms
	 bool getUnselectedGroupResult(std::vector<Synonym> &synList,
		 std::unordered_map<Synonym, Symbol> &varMap,
		 std::vector<Clause> &clauseGroup);

	 // get results for boolean clauses
	 bool getBooleanGroupResult(std::vector<Clause> &clauseGroup);

	 // retrieve all possible candidates for a variable
	 std::vector<Candidate> getCandidates(Symbol &synType);

	 TotalCombinationList getTotalCandidateList(std::unordered_map<std::string, Symbol> &varMap, std::vector<Synonym> &synList);

	 // use the clause to filter the tuple candidate list to its sublist
	 void filterByClause(Clause &clause, TotalCombinationList &combinations, std::unordered_map<Synonym, Symbol> &varMap);

	 /* filter pattern clauses */

	 void filterNoVarPattern(Synonym assignStmt, Candidate lhs, Candidate expr,
		 TotalCombinationList &combinations);

	 void filterOneVarPattern(Synonym assignStmt, Synonym lhs, Candidate expr,
		 TotalCombinationList &combinations);

	 /* filter such that clauses */

	 void filterTwoVarsClause(std::string clauseType,
		 Synonym &var0, Synonym &var1, TotalCombinationList &combinations);

	 void filterFirstVarClause(std::string clauseType, Synonym var,
		 Candidate constant, TotalCombinationList &combinations);

	 void filterSecondVarClause(std::string clauseType, Candidate constant,
		 Synonym var, TotalCombinationList &combinations);

	 void filterNoVarClause(std::string clauseType, Candidate const1,
		 Candidate const2, TotalCombinationList &combinations);

	 /* filter with clauses */

	 void filterTwoVarsWith(Synonym &var1, Synonym &var2, TotalCombinationList &combinations);

	 void filterFirstVarWith(Synonym &var, Candidate constant, TotalCombinationList &combinations);

	 void filterSecondVarWith(Candidate constant, Synonym &var, TotalCombinationList &combinations);

	 void filterNoVarWith(Candidate const1, Candidate const2, TotalCombinationList &combinations);

	 /* filter with clauses with call */

	 void filterTwoVarsCallWith(Synonym &call0, Synonym &call1, TotalCombinationList &combinations);

	 void filterOneVarCallWith(Synonym &call, Synonym &var, TotalCombinationList &combinations);

	 void filterNoVarCallWith(Synonym &call, Candidate constant, TotalCombinationList &combinations);

	 /* get results methods */

	 ResultList getResultsFromCombinationList(TotalCombinationList &combinations,
		 std::vector<Synonym> &selectList);

	 PartialCombinationList mergeCombinationList(PartialCombinationList &combList1,
		 PartialCombinationList &combList2);

	 bool isBoolSelect(std::vector<std::string>& selectList);

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

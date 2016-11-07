#pragma once

#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "QueryProcessor/Clause.h"
#include "QueryTree.h"
#include "PKB/PKB.h"
#include "TotalCombinationList.h"

typedef std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>> ResultList;

class QueryEvaluator {
 public:
    QueryEvaluator();
    ~QueryEvaluator();

    // return the result list as a ResultList after evaluating the query
    ResultList selectQueryResults(QueryTree &query);

    // return the result list as a ResultList after evaluating the query
    TotalCombinationList getQueryResults();

    // get results for clauses of selected synonyms
    TotalCombinationList getSelectedGroupResult(std::vector<Synonym> &synList,std::vector<Clause> &clauseGroup);

    // get results for clauses of unselected synonyms
    bool getUnselectedGroupResult(std::vector<Synonym> &synList, std::vector<Clause> &clauseGroup);

    // get results for boolean clauses
    bool getBooleanGroupResult();

    // retrieve all possible candidates for a variable
    std::vector<Candidate> getCandidates(Symbol &synType);

	// get the total candidate list for a list of synonyms
    TotalCombinationList getTotalCandidateList(std::vector<Synonym> &synList);

    // use the clause to filter the tuple candidate list to its sublist
    void filterByClause(Clause &clause, TotalCombinationList &combinations);

    /* filter pattern clauses */

    void filterNoVarPattern(Synonym assignStmt, Candidate lhs, std::string expr,
        TotalCombinationList &combinations);

    void filterOneVarPattern(Synonym assignStmt, Synonym lhs, std::string expr,
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

	void filterNoVarWith(std::string str1, std::string str2, TotalCombinationList &combinations);

    /* filter with clauses with call */

    void filterTwoVarsCallWith(Synonym &call0, Synonym &call1, TotalCombinationList &combinations);

    void filterOneVarCallWith(Synonym &call, Synonym &var, TotalCombinationList &combinations);

    void filterNoVarCallWith(Synonym &call, Candidate constant, TotalCombinationList &combinations);

    /* get results methods */

    ResultList getResultsFromCombinationList(TotalCombinationList &combinations);

    bool isBoolSelect(std::vector<std::string>& synList);

    std::string log;

 private:
    
	//bool evaluateClause(Clause &clause, CandidateCombination &candMap);
    bool evaluateSuchThatClause(std::string clauseType, Candidate var0, Candidate var1);
	bool evaluatePatternClause(Candidate assignStmt, Candidate lhsVar, std::string expr);
    
	bool evaluateCalls(Candidate var0, Candidate var1);
    bool evaluateCallsStar(Candidate var0, Candidate var1);
    bool evaluateModifies(Candidate var0, Candidate var1);
	bool evaluateModifiesProcedure(Candidate var0, Candidate var1);
    bool evaluateUses(Candidate var0, Candidate var1);
	bool evaluateUsesProcedure(Candidate var0, Candidate var1);
    bool evaluateParent(Candidate var0, Candidate var1);
    bool evaluateParentStar(Candidate var0, Candidate var1);
    bool evaluateFollows(Candidate var0, Candidate var1);
    bool evaluateFollowsStar(Candidate var0, Candidate var1);
    bool evaluateNext(Candidate var0, Candidate var1);
    bool evaluateNextStar(Candidate var0, Candidate var1);
    bool evaluateAffects(Candidate var0, Candidate var1);
    bool evaluateAffectsStar(Candidate var0, Candidate var1);

	std::vector<Synonym> selectList;
	std::unordered_map<Synonym, Symbol> varMap;
	std::vector<std::pair<std::vector<Synonym>, std::vector<Clause>>> unselectedGroups;
	std::vector<std::pair<std::vector<Synonym>, std::vector<Clause>>> selectedGroups;
	std::vector<Clause> booleanClauses;
};

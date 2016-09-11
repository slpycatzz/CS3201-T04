#include "QueryProcessor/Clause.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "Utils.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}

bool QueryEvaluator::evaluateClause(PKB &pkb, Clause &clause, CandidateTuple tup)
{
	std::string type = clause.getClauseType();
	if (type.compare("uses")) { evaluateUses(pkb, clause, tup); }
	else if (type.compare("modifies")) { evaluateModifies(pkb, clause, tup); }
	else if (type.compare("parent")) { evaluateParent(pkb, clause, tup); }
	else if (type.compare("parent_star")) { evaluateParentStar(pkb, clause, tup); }
	else if (type.compare("follows")) { evaluateFollows(pkb, clause, tup); }
	else if (type.compare("follows_star")) { evaluateFollowsStar(pkb, clause, tup); }
	else if (type.compare("next")) { evaluateNext(pkb, clause, tup); }
	else if (type.compare("next_star")) { evaluateNextStar(pkb, clause, tup); }
	else if (type.compare("calls")) { evaluateCalls(pkb, clause, tup); }
	else if (type.compare("calls_star")) { evaluateCallsStar(pkb, clause, tup); }
	else if (type.compare("affects")) { evaluateAffects(pkb, clause, tup); }
	else if (type.compare("affects_star")) { evaluateAffectsStar(pkb, clause, tup); }

	return false;
}

bool QueryEvaluator::evaluateUses(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateModifies(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateParent(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateParentStar(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateFollows(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateFollowsStar(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateNext(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateNextStar(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateCalls(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateCallsStar(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateAffects(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

bool QueryEvaluator::evaluateAffectsStar(PKB &pkb, Clause &clause, CandidateTuple tup) {
	return false;
}

TotalCandidateList
QueryEvaluator::selectClauseResults(PKB &pkb, Clause &clause,
									TotalCandidateList &candidates)
{
	std::vector<Var> args(clause.getArg());
	if (args.size == 1) {
		Var var = args[0];
		CandidateTupleList candidateList = candidates[var];
		Filter(candidateList, pkb, clause, candidates);
	}
	else if (args.size == 2) {
		Var var0 = args[0], var1 = args[1];
		CandidateTupleList candidateList0 = candidates[var0];
		CandidateTupleList candidateList1 = candidates[var1];
		MergeAndFilter(candidateList0, candidateList1, pkb, clause, candidates);
	}
	return candidates;
}

void QueryEvaluator::Filter(CandidateTupleList &candidateList,
	PKB & pkb, Clause & clause, TotalCandidateList & candidates)
{
	CandidateTupleList filteredList;
	for (CandidateTuple tup : candidateList) {
		if (evaluateClause(pkb, clause, tup)) {
			filteredList.push_back(tup);
		}
	}
	for (auto kv : candidates) {
		if (kv.second == candidateList) {
			candidates.insert_or_assign(kv.first, filteredList);
		}
	}
}

void QueryEvaluator::MergeAndFilter(CandidateTupleList &candidateList0,
	CandidateTupleList &candidateList1, PKB & pkb, Clause & clause,
	TotalCandidateList & candidates)
{
	CandidateTupleList filteredList;
	if (&candidateList0 == &candidateList1) {
		for (CandidateTuple tup : candidateList0) {
			if (evaluateClause(pkb, clause, tup)) {
				filteredList.push_back(tup);
			}
		}
	}
	else {
		for (CandidateTuple tup0 : candidateList0) {
			for (CandidateTuple tup1 : candidateList1) {
				CandidateTuple tup = Utils::MergeMap(tup0, tup1);
				if (evaluateClause(pkb, clause, tup)) {
					filteredList.push_back(tup);
				}
			}
		}
	}
	for (auto kv : candidates) {
		if (kv.second == candidateList0 || kv.second == candidateList1) {
			candidates.insert_or_assign(kv.first, filteredList);
		}
	}
}

bool QueryEvaluator::evaluateQuery(PKB &pkb, QueryTree &query)
{
	return false;
}

ResultList QueryEvaluator::selectQueryResults(PKB &pkb, QueryTree &query)
{
	ResultList resultList;
	return resultList;
}

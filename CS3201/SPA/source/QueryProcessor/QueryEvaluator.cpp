#include "QueryProcessor/Clause.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "Utils.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}

CandidateMapList QueryEvaluator::getCandidates(PKB &pkb, std::pair<Var, Symbol> var) {
	CandidateMapList result;
	switch (var.second) {
		case VARIABLE:
			insertMap(pkb.GetAllVariableNames(), var.first, result);
			break;
		case PROCEDURE:
			insertMap(pkb.GetAllProcedures(), var.first, result);
			break;
		case PROGRAM_LINE:
		case STMT:
		case ASSIGN:
		case IF:
		case WHILE:
			insertMap(Utils::IntToString(pkb.GetSymbolStmtNumbers(var.second)), var.first, result);
			break;
		default:
	}
	return result;
}

TotalCandidateList QueryEvaluator::getTotalCandidateList(PKB &pkb, QueryTree &query) {
	TotalCandidateList totalCandidateList;
	std::unordered_map<std::string, Symbol> varMap = query.getVarMap();
	for (auto kv : varMap) {
		CandidateMapList candidateMapList(getCandidates(pkb, kv));
		totalCandidateList.insert(kv.first, candidateMapList);
	}
	return totalCandidateList;
}

void QueryEvaluator::insertMap(std::vector<std::string> list, Var var, CandidateMapList &result)
{
	for (std::string varName : list) {
		CandidateMap candidate({ { var, varName } });
		result.push_back(candidate);
	}
}

bool QueryEvaluator::selectClauseResults(PKB &pkb, Clause &clause,
	TotalCandidateList &candidates)
{
	bool hasCandidates;
	std::vector<Var> args(clause.getArg());
	if (args.size == 1) {
		Var var = args[0];
		CandidateMapList candidateList = candidates[var];
		hasCandidates = Filter(candidateList, pkb, clause, candidates);
	}
	else if (args.size == 2) {
		Var var0 = args[0], var1 = args[1];
		CandidateMapList candidateList0 = candidates[var0];
		CandidateMapList candidateList1 = candidates[var1];
		hasCandidates = MergeAndFilter(candidateList0, candidateList1, pkb, clause, candidates);
	}
	return hasCandidates;
}

bool QueryEvaluator::Filter(CandidateMapList &candidateList,
	PKB & pkb, Clause & clause, TotalCandidateList & candidates)
{
	CandidateMapList filteredList;
	for (CandidateMap tup : candidateList) {
		if (evaluateClause(pkb, clause, tup)) {
			filteredList.push_back(tup);
		}
	}
	if (filteredList.empty()) return false;
	for (auto kv : candidates) {
		if (kv.second == candidateList) {
			candidates.insert_or_assign(kv.first, filteredList);
		}
	}
	return true;
}

bool QueryEvaluator::MergeAndFilter(CandidateMapList &candidateList0,
	CandidateMapList &candidateList1, PKB & pkb, Clause & clause,
	TotalCandidateList & candidates)
{
	CandidateMapList filteredList;
	if (&candidateList0 == &candidateList1) {
		for (CandidateMap tup : candidateList0) {
			if (evaluateClause(pkb, clause, tup)) {
				filteredList.push_back(tup);
			}
		}
	}
	else {
		for (CandidateMap tup0 : candidateList0) {
			for (CandidateMap tup1 : candidateList1) {
				CandidateMap tup = Utils::MergeMap(tup0, tup1);
				if (evaluateClause(pkb, clause, tup)) {
					filteredList.push_back(tup);
				}
			}
		}
	}
	if (filteredList.empty()) return false;
	for (auto kv : candidates) {
		if (kv.second == candidateList0 || kv.second == candidateList1) {
			candidates.insert_or_assign(kv.first, filteredList);
		}
	}
	return true;
}

bool QueryEvaluator::evaluateQuery(PKB &pkb, QueryTree &query)
{
	return false;
}

ResultList QueryEvaluator::selectQueryResults(PKB &pkb, QueryTree &query)
{
	ResultList resultList;
	std::vector<Clause> clauseList = query.getClauses("suchThat pattern");
	TotalCandidateList allCandidates(getTotalCandidateList(pkb, query));
	std::unordered_map<std::string, Symbol> selectList = query.getSelect();
	bool hasMoreCandidates;
	for (Clause clause : clauseList) {
		hasMoreCandidates = selectClauseResults(pkb, clause, allCandidates);
		if (!hasMoreCandidates) break;
	}
	/* TODO - fill in code for result selection */
	return resultList;
}

bool QueryEvaluator::evaluateClause(PKB &pkb, Clause &clause, CandidateMap tup)
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

bool QueryEvaluator::evaluateUses(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateModifies(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateParent(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateParentStar(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateFollows(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateFollowsStar(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateNext(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateNextStar(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateCalls(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateCallsStar(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateAffects(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}

bool QueryEvaluator::evaluateAffectsStar(PKB &pkb, Clause &clause, CandidateMap tup) {
	return false;
}


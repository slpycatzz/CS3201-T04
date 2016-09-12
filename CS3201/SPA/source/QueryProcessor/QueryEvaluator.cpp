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

TotalCandidateMap QueryEvaluator::getTotalCandidateList(PKB &pkb, QueryTree &query) {
	TotalCandidateMap totalCandLst;
	std::unordered_map<std::string, Symbol> varMap = query.getVarMap();
	for (auto kv : varMap) {
		CandidateMapList candMapLst(getCandidates(pkb, kv));
		totalCandLst.insert_or_assign(kv.first, candMapLst);
	}
	return totalCandLst;
}

void QueryEvaluator::insertMap(std::vector<std::string> list, Var var, CandidateMapList &result)
{
	for (std::string varName : list) {
		CandidateMap candidate({ { var, varName } });
		result.push_back(candidate);
	}
}

bool QueryEvaluator::selectClauseResults(PKB &pkb, Clause &clause,
	TotalCandidateMap &cands)
{
	bool hasCandidates;
	std::vector<Var> args(clause.getArg());
	if (args.size == 1) {
		Var var = args[0];
		CandidateMapList candidateList = cands[var];
		hasCandidates = Filter(candidateList, pkb, clause, cands);
	}
	else if (args.size == 2) {
		Var var0 = args[0], var1 = args[1];
		CandidateMapList candLst0 = cands[var0];
		CandidateMapList candLst1 = cands[var1];
		hasCandidates = MergeAndFilter(candLst0, candLst1, pkb, clause, cands);
	}
	return hasCandidates;
}

bool QueryEvaluator::Filter(CandidateMapList &candidateList,
	PKB & pkb, Clause &clause, TotalCandidateMap &cands)
{
	CandidateMapList filteredList;
	for (CandidateMap tup : candidateList) {
		if (evaluateClause(pkb, clause, tup)) {
			filteredList.push_back(tup);
		}
	}
	if (filteredList.empty()) return false;
	for (auto kv : cands) {
		if (kv.second == candidateList) {
			cands.insert_or_assign(kv.first, filteredList);
		}
	}
	return true;
}

bool QueryEvaluator::MergeAndFilter(CandidateMapList &candLst0,
	CandidateMapList &candLst1, PKB & pkb, Clause & clause,
	TotalCandidateMap &cands)
{
	CandidateMapList filteredList;
	if (&candLst0 == &candLst1) {
		for (CandidateMap tup : candLst0) {
			if (evaluateClause(pkb, clause, tup)) {
				filteredList.push_back(tup);
			}
		}
	}
	else {
		for (CandidateMap tup0 : candLst0) {
			for (CandidateMap tup1 : candLst1) {
				CandidateMap tup = Utils::MergeMap(tup0, tup1);
				if (evaluateClause(pkb, clause, tup)) {
					filteredList.push_back(tup);
				}
			}
		}
	}
	if (filteredList.empty()) return false;
	for (auto kv : cands) {
		if (kv.second == candLst0 || kv.second == candLst1) {
			cands.insert_or_assign(kv.first, filteredList);
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
	std::vector<Clause> clauseList = query.getClauses("suchThat pattern");
	TotalCandidateMap allCandidates(getTotalCandidateList(pkb, query));
	std::unordered_map<std::string, Symbol> selectList = query.getSelect();
	bool hasMoreCandidates;
	for (Clause clause : clauseList) {
		hasMoreCandidates = selectClauseResults(pkb, clause, allCandidates);
		if (!hasMoreCandidates) break;
	}
	if (isBoolSelect(selectList)) {
		ResultList resultList;
		if (hasMoreCandidates) {
			resultList.push_back(SYMBOL_TRUE);
		}
		else {
			resultList.push_back(SYMBOL_FALSE);
		}
		return resultList;
	}
	else {
		if (!hasMoreCandidates) {
			return ResultList();
		}
		else {
			ResultList resultList(getResultsFromTotalCandidateList(allCandidates, selectList));
			return resultList;
		}
	}
}

ResultList QueryEvaluator::getResultsFromTotalCandidateList(TotalCandidateMap &cands,
	std::unordered_map<std::string, Symbol> &selectList)
{
	std::vector<std::string> varList;
	std::unordered_map<std::string, std::vector<Candidate>> selectMap;
	for (auto kv : selectList) {
		varList.push_back(kv.first);
		std::vector<Candidate> candList;
		CandidateMapList* section = &cands[kv.first];
		for (CandidateMap cm : *section) {
			candList.push_back(cm[kv.first]);
		}
		selectMap[kv.first] = candList;
	}
	std::vector<std::vector<Candidate>>* resultVectors = &Utils::Flatten(selectMap, varList, 0, varList.size() - 1);
	ResultList resultList;
	for (std::vector<Candidate> candTup : *resultVectors) {
		resultList.push_back(Utils::VectorToString(candTup));
	}
	return resultList;
}

bool QueryEvaluator::isBoolSelect(std::unordered_map<std::string, Symbol> &selectList) {
	if (selectList.size == 1 && selectList.begin()->second == BOOLEAN) {
		return true;
	}
	else {
		return false;
	}
}

bool QueryEvaluator::evaluateClause(PKB &pkb, Clause &clause, CandidateMap &tup)
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

bool QueryEvaluator::evaluateUses(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	Var var(tup[args[1]]);
	Var arg0(tup[args[0]]);
	if (Utils::IsNonNegativeNumeric(arg0)) {
		int stmtNo(Utils::StringToInt(arg0));
		return pkb.IsUses(stmtNo, var);
	}
	else {
		return pkb.IsUsesProcedure(arg0, var);
	}
}

bool QueryEvaluator::evaluateModifies(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	Var var(tup[args[1]]);
	Var arg0(tup[args[0]]);
	if (Utils::IsNonNegativeNumeric(arg0)) {
		int stmtNo(Utils::StringToInt(arg0));
		return pkb.IsModifies(stmtNo, var);
	}
	else {
		return pkb.IsModifiesProcedure(arg0, var);
	}
}

bool QueryEvaluator::evaluateParent(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	int stmtNo1(Utils::StringToInt(tup[args[1]]));
	int stmtNo2(Utils::StringToInt(tup[args[0]]));
	return pkb.IsParent(stmtNo1, stmtNo2);
}

bool QueryEvaluator::evaluateParentStar(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	int stmtNo1(Utils::StringToInt(tup[args[1]]));
	int stmtNo2(Utils::StringToInt(tup[args[0]]));
	return pkb.IsParentTransitive(stmtNo1, stmtNo2);
}

bool QueryEvaluator::evaluateFollows(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	int stmtNo1(Utils::StringToInt(tup[args[1]]));
	int stmtNo2(Utils::StringToInt(tup[args[0]]));
	return pkb.IsFollows(stmtNo1, stmtNo2);
}

bool QueryEvaluator::evaluateFollowsStar(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	int stmtNo1(Utils::StringToInt(tup[args[1]]));
	int stmtNo2(Utils::StringToInt(tup[args[0]]));
	return pkb.IsFollowsTransitive(stmtNo1, stmtNo2);
}

bool QueryEvaluator::evaluateNext(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	int stmtNo1(Utils::StringToInt(tup[args[1]]));
	int stmtNo2(Utils::StringToInt(tup[args[0]]));
	//return pkb.IsNext(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateNextStar(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	int stmtNo1(Utils::StringToInt(tup[args[1]]));
	int stmtNo2(Utils::StringToInt(tup[args[0]]));
	//return pkb.IsNextTransitive(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateCalls(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	Var proc1(tup[args[1]]);
	Var proc2(tup[args[0]]);
	//return pkb.isCalls(proc1, proc2);
	return false;
}

bool QueryEvaluator::evaluateCallsStar(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	Var proc1(tup[args[1]]);
	Var proc2(tup[args[0]]);
	//return pkb.isCallsTransitive(proc1, proc2);
	return false;
}

bool QueryEvaluator::evaluateAffects(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	int stmtNo1(Utils::StringToInt(tup[args[1]]));
	int stmtNo2(Utils::StringToInt(tup[args[0]]));
	//return pkb.IsAffects(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateAffectsStar(PKB &pkb, Clause &clause, CandidateMap &tup) {
	std::vector<Var> args(clause.getArg());
	int stmtNo1(Utils::StringToInt(tup[args[1]]));
	int stmtNo2(Utils::StringToInt(tup[args[0]]));
	//return pkb.IsAffectTransitive(stmtNo1, stmtNo2);
	return false;
}


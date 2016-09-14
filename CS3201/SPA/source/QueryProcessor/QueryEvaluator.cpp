#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "QueryProcessor/Clause.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "Utils.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}

PartialCombinationList QueryEvaluator::getCandidates(PKB &pkb, std::pair<VarName, Symbol> var) {
	PartialCombinationList result;
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
			break;
	}
	return result;
}

TotalCombinationList QueryEvaluator::getTotalCandidateList(PKB &pkb, QueryTree &query) {
	TotalCombinationList totalCandLst;
	std::unordered_map<std::string, Symbol> varMap = query.getVarMap();
	for (auto kv : varMap) {
		PartialCombinationList candMapLst(getCandidates(pkb, kv));
		totalCandLst.insert_or_assign(kv.first, candMapLst);
	}
	return totalCandLst;
}

void QueryEvaluator::insertMap(std::vector<std::string> list, VarName var, PartialCombinationList &result)
{
	for (std::string varName : list) {
		CandidateCombination candidate({ { var, varName } });
		result.push_back(candidate);
	}
}

bool QueryEvaluator::selectClauseResults(PKB &pkb, Clause &clause,
	TotalCombinationList &combinations)
{
	bool hasCandidates;
	std::string type(clause.getClauseType());
	if (type.compare(SYMBOL_PATTERN)) {

	}
	else {
		std::vector<VarName> args(clause.getArg());
		VarName var0(args[0]), var1(args[1]);
		if (Utils::IsLiteral(var0)) {
			if (Utils::IsLiteral(var1)) {
				FilterNoVarClause(pkb, type, var0, var1, combinations);
			}
			else {
				FilterSecondVarClause(pkb, type, var0, var1, combinations);
			}
		}
		else {
			if (Utils::IsLiteral(var1)) {
				FilterFirstVarClause(pkb, type, var0, var1, combinations);
			}
			else {
				FilterTwoVarsClause(pkb, type, var0, var1, combinations);
			}
		}
	}
	return hasCandidates;
}

bool QueryEvaluator::FilterTwoVarsClause(PKB &pkb, std::string clauseType,
	VarName &var0, VarName &var1, TotalCombinationList &combinations)
{
	PartialCombinationList candLst0(combinations[var0]);
	PartialCombinationList candLst1(combinations[var1]);
	PartialCombinationList filteredList;
	if (&candLst0 == &candLst1) {
		for (CandidateCombination tup : candLst0) {
			Candidate arg0(tup[var0]), arg1(tup[var1]);
			if (evaluateSuchThatClause(pkb, clauseType, arg0, arg1)) {
				filteredList.push_back(tup);
			}
		}
	}
	else {
		for (CandidateCombination tup0 : candLst0) {
			Candidate arg0(tup0[var0]);
			for (CandidateCombination tup1 : candLst1) {
				Candidate arg1(tup1[var1]);
				if (evaluateSuchThatClause(pkb, clauseType, arg0, arg1)) {
					filteredList.push_back(Utils::MergeMap(tup0, tup1));
				}
			}
		}
	}
	if (filteredList.empty()) return false;
	for (auto kv : combinations) {
		if (kv.second == candLst0 || kv.second == candLst1) {
			combinations.insert_or_assign(kv.first, filteredList);
		}
	}
	return true;
}

bool QueryEvaluator::FilterFirstVarClause(PKB &pkb, std::string clauseType,
 VarName var, Candidate constant, TotalCombinationList &combinations)
{
	PartialCombinationList candidateList(combinations[var]);
	PartialCombinationList filteredList;
	for (CandidateCombination tup : candidateList) {
		if (evaluateSuchThatClause(pkb, clauseType, tup[var], constant)) {
			filteredList.push_back(tup);
		}
	}
	if (filteredList.empty()) return false;
	for (auto kv : combinations) {
		if (kv.second == candidateList) {
			combinations.insert_or_assign(kv.first, filteredList);
		}
	}
	return true;
}

bool QueryEvaluator::FilterSecondVarClause(PKB &pkb, std::string clauseType,
	Candidate constant, VarName var, TotalCombinationList &combinations)
{
	PartialCombinationList candidateList(combinations[var]);
	PartialCombinationList filteredList;
	for (CandidateCombination tup : candidateList) {
		if (evaluateSuchThatClause(pkb, clauseType, constant, tup[var])) {
			filteredList.push_back(tup);
		}
	}
	if (filteredList.empty()) return false;
	for (auto kv : combinations) {
		if (kv.second == candidateList) {
			combinations.insert_or_assign(kv.first, filteredList);
		}
	}
	return true;
}

bool QueryEvaluator::FilterNoVarClause(PKB &pkb, std::string clauseType,
	Candidate const1, Candidate const2, TotalCombinationList &combinations)
{
	return evaluateSuchThatClause(pkb, clauseType, const1, const2);
}

bool QueryEvaluator::evaluateQuery(PKB &pkb, QueryTree &query)
{
	return false;
}

ResultList QueryEvaluator::selectQueryResults(PKB &pkb, QueryTree &query)
{
	std::vector<Clause> clauseList = query.getClauses("suchThat pattern");
	TotalCombinationList allCandidates(getTotalCandidateList(pkb, query));
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
			ResultList resultList(getResultsFromCombinationList(allCandidates, selectList));
			return resultList;
		}
	}
}

ResultList QueryEvaluator::getResultsFromCombinationList(TotalCombinationList &cands,
	std::unordered_map<std::string, Symbol> &selectList)
{
	std::vector<std::string> varList;
	std::unordered_map<std::string, std::vector<Candidate>> selectMap;
	for (auto kv : selectList) {
		varList.push_back(kv.first);
		std::vector<Candidate> candList;
		PartialCombinationList* section = &cands[kv.first];
		for (CandidateCombination cm : *section) {
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
	if (selectList.size() == 1 && selectList.begin()->second == BOOLEAN) {
		return true;
	}
	else {
		return false;
	}
}

bool QueryEvaluator::evaluateSuchThatClause(PKB &pkb,
	std::string clauseType, Candidate var0, Candidate var1)
{
	if (clauseType.compare("uses")) {
		evaluateUses(pkb, var0, var1);
	}
	else if (clauseType.compare("modifies")) {
		evaluateModifies(pkb, var0, var1);
	}
	else if (clauseType.compare("parent")) {
		evaluateParent(pkb, var0, var1);
	}
	else if (clauseType.compare("parent_star")) {
		evaluateParentStar(pkb, var0, var1);
	}
	else if (clauseType.compare("follows")) {
		evaluateFollows(pkb, var0, var1);
	}
	else if (clauseType.compare("follows_star")) {
		evaluateFollowsStar(pkb, var0, var1); }
	else if (clauseType.compare("next")) {
		evaluateNext(pkb, var0, var1);
	}
	else if (clauseType.compare("next_star")) {
		evaluateNextStar(pkb, var0, var1);
	}
	else if (clauseType.compare("calls")) {
		evaluateCalls(pkb, var0, var1);
	}
	else if (clauseType.compare("calls_star")) {
		evaluateCallsStar(pkb, var0, var1);
	}
	else if (clauseType.compare("affects")) {
		evaluateAffects(pkb, var0, var1);
	}
	else if (clauseType.compare("affects_star")) {
		evaluateAffectsStar(pkb, var0, var1);
	}

	return false;
}

bool QueryEvaluator::evaluateUses(PKB &pkb, Candidate procOrStmtNo, Candidate varName)
{
	if (Utils::IsNonNegativeNumeric(procOrStmtNo)) {
		int stmtNo(Utils::StringToInt(procOrStmtNo));
		return pkb.IsUses(stmtNo, varName);
	}
	else {
		return pkb.IsUsesProcedure(procOrStmtNo, varName);
	}
}

bool QueryEvaluator::evaluateModifies(PKB &pkb, Candidate procOrStmtNo, Candidate varName)
{
	if (Utils::IsNonNegativeNumeric(procOrStmtNo)) {
		int stmtNo(Utils::StringToInt(procOrStmtNo));
		return pkb.IsModifies(stmtNo, varName);
	}
	else {
		return pkb.IsModifiesProcedure(procOrStmtNo, varName);
	}
}

bool QueryEvaluator::evaluateParent(PKB &pkb, Candidate stmt1, Candidate stmt2)
{
	int stmtNo1(Utils::StringToInt(stmt1));
	int stmtNo2(Utils::StringToInt(stmt2));
	return pkb.IsParent(stmtNo1, stmtNo2);
}

bool QueryEvaluator::evaluateParentStar(PKB &pkb, Candidate stmt1, Candidate stmt2)
{
	int stmtNo1(Utils::StringToInt(stmt1));
	int stmtNo2(Utils::StringToInt(stmt2));
	return pkb.IsParentTransitive(stmtNo1, stmtNo2);
}

bool QueryEvaluator::evaluateFollows(PKB &pkb, Candidate stmt1, Candidate stmt2)
{
	int stmtNo1(Utils::StringToInt(stmt1));
	int stmtNo2(Utils::StringToInt(stmt2));
	return pkb.IsFollows(stmtNo1, stmtNo2);
}

bool QueryEvaluator::evaluateFollowsStar(PKB &pkb, Candidate stmt1, Candidate stmt2)
{
	int stmtNo1(Utils::StringToInt(stmt1));
	int stmtNo2(Utils::StringToInt(stmt2));
	return pkb.IsFollowsTransitive(stmtNo1, stmtNo2);
}

bool QueryEvaluator::evaluateNext(PKB &pkb, Candidate stmt1, Candidate stmt2)
{
	int stmtNo1(Utils::StringToInt(stmt1));
	int stmtNo2(Utils::StringToInt(stmt2));
	//return pkb.IsNext(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateNextStar(PKB &pkb, Candidate stmt1, Candidate stmt2)
{
	int stmtNo1(Utils::StringToInt(stmt1));
	int stmtNo2(Utils::StringToInt(stmt2));
	//return pkb.IsNextTransitive(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateCalls(PKB &pkb, Candidate proc1, Candidate proc2)
{
	//return pkb.IsCalls(proc1, proc2);
	return false;
}

bool QueryEvaluator::evaluateCallsStar(PKB &pkb, Candidate proc1, Candidate proc2)
{
	//return pkb.IsCallsTransitive(proc1, proc2);
	return false;
}

bool QueryEvaluator::evaluateAffects(PKB &pkb, Candidate assign1, Candidate assign2)
{
	int stmtNo1(Utils::StringToInt(assign1));
	int stmtNo2(Utils::StringToInt(assign2));
	//return pkb.IsAffects(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateAffectsStar(PKB &pkb, Candidate assign1, Candidate assign2)
{
	int stmtNo1(Utils::StringToInt(assign1));
	int stmtNo2(Utils::StringToInt(assign2));
	//return pkb.IsAffectsTransitive(stmtNo1, stmtNo2);
	return false;
}


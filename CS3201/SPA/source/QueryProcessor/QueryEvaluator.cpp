#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

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
	if (type == SYMBOL_PATTERN) {
		std::vector<VarName> args(clause.getArg());
		VarName lhs(args[0]), rhs(args[1]), assignStmt(args[2]);
		if (Utils::IsStringLiteral(lhs)) {
			hasCandidates = FilterNoVarPattern(pkb, assignStmt, Utils::LiteralToCandidate(lhs), rhs, combinations);
		}
		else {
			hasCandidates = FilterOneVarPattern(pkb, assignStmt, lhs, rhs, combinations);
		}
	}
	else {
		std::vector<VarName> args(clause.getArg());
		VarName var0(args[0]), var1(args[1]);
		if (Utils::IsLiteral(var0)) {
			if (Utils::IsLiteral(var1)) {
				hasCandidates = FilterNoVarClause(pkb, type, Utils::LiteralToCandidate(var0),
					Utils::LiteralToCandidate(var1), combinations);
			}
			else {
				hasCandidates = FilterSecondVarClause(pkb, type,
					Utils::LiteralToCandidate(var0), var1, combinations);
			}
		}
		else {
			if (Utils::IsLiteral(var1)) {
				hasCandidates = FilterFirstVarClause(pkb, type, var0,
					Utils::LiteralToCandidate(var1), combinations);
			}
			else {
				hasCandidates = FilterTwoVarsClause(pkb, type, var0, var1, combinations);
			}
		}
	}
	return hasCandidates;
}

bool QueryEvaluator::FilterNoVarPattern(PKB &pkb, VarName assignStmt, Candidate lhs,
	Candidate expr, TotalCombinationList &combinations)
{
	PartialCombinationList candidateList(combinations[assignStmt]);
	PartialCombinationList filteredList;
	for (CandidateCombination tup : candidateList) {
		if (evaluatePatternClause(pkb, tup[assignStmt], lhs, expr)) {
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

bool QueryEvaluator::FilterOneVarPattern(PKB &pkb, VarName assignStmt, VarName lhs,
	Candidate expr, TotalCombinationList &combinations)
{
	PartialCombinationList candLst0(combinations[assignStmt]);
	PartialCombinationList candLst1(combinations[lhs]);
	PartialCombinationList filteredList;
	if (&candLst0 == &candLst1) {
		for (CandidateCombination tup : candLst0) {
			Candidate stmtCand(tup[assignStmt]), lhsCand(tup[lhs]);
			if (evaluateSuchThatClause(pkb, stmtCand, lhsCand, expr)) {
				filteredList.push_back(tup);
			}
		}
	}
	else {
		for (CandidateCombination tup0 : candLst0) {
			Candidate stmtCand(tup0[assignStmt]);
			for (CandidateCombination tup1 : candLst1) {
				Candidate lhsCand(tup1[lhs]);
				if (evaluateSuchThatClause(pkb, stmtCand, lhsCand, expr)) {
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
	std::unordered_map<VarName, Symbol> selectMap = query.getSelect();
	std::vector<VarName> selectList;
	for (auto kv : selectMap) selectList.push_back(kv.first);

	bool hasMoreCandidates;
	for (Clause clause : clauseList) {
		hasMoreCandidates = selectClauseResults(pkb, clause, allCandidates);
		if (!hasMoreCandidates) break;
	}
	if (isBoolSelect(selectMap)) {
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

ResultList QueryEvaluator::getResultsFromCombinationList
(TotalCombinationList &combinations, std::vector<VarName> &selectList)
{
	PartialCombinationList
		selectedCombinations(getSelectedCombinations(combinations, selectList));
	ResultList result;
	
	for (CandidateCombination comb : selectedCombinations) {
		std::string item(Utils::MapToValueString(comb));
		result.push_back(item);
	}
	return result;
}

PartialCombinationList
QueryEvaluator::getSelectedCombinations
(TotalCombinationList &combinations, std::vector<VarName> &selectList)
{
	/* Base case for recursion */
	if (selectList.empty()) {
		PartialCombinationList baseResult;
		baseResult.push_back(CandidateCombination());
		return baseResult;
	}
	
	/* Extract the first section out */
	VarName firstVar(*selectList.begin());
	PartialCombinationList firstPartialList(combinations[firstVar]);
	std::vector<VarName> firstVarSet;
	std::vector<VarName>::iterator iter(selectList.begin());
	while (iter != selectList.end()) {
		if (combinations[*iter] == firstPartialList) {
			firstVarSet.push_back(*iter);
			iter = selectList.erase(iter);
		}
		else {
			++iter;
		}
	}
	
	/* TODO: Account for duplicates of sub-combinations */
	std::vector<CandidateCombination> reducedList;
	for (CandidateCombination comb : firstPartialList) {
		reducedList.push_back(Utils::ReduceMap(comb, firstVarSet));
	}

	/* Recursive step */
	PartialCombinationList recursedList(getSelectedCombinations(combinations, selectList));
	PartialCombinationList result = mergeCombinationList(reducedList, recursedList);
	return result;
}

PartialCombinationList QueryEvaluator::mergeCombinationList(PartialCombinationList &combList1,
	PartialCombinationList &combList2)
{
	PartialCombinationList result;
	for (CandidateCombination comb1 : combList1) {
		for (CandidateCombination comb2 : combList2) {
			result.push_back(Utils::MergeMap(comb1, comb2));
		}
	}
	return result;
}

bool QueryEvaluator::isBoolSelect(std::unordered_map<std::string, Symbol> &selectList) {
	if (selectList.size() == 1 && selectList.begin()->second == BOOLEAN) {
		return true;
	}
	else {
		return false;
	}
}

bool QueryEvaluator::evaluateClause(PKB &pkb, Clause &clause, CandidateCombination &comb) {
	std::string type(clause.getClauseType());
	std::vector<std::string> args(clause.getArg());
	if (type == SYMBOL_PATTERN) {
		Candidate assignStmt(comb[args[2]]);
		Candidate lhsVar(comb[args[0]]);
		Candidate expr(comb[args[1]]);
		return evaluatePatternClause(pkb, assignStmt, lhsVar, expr);
	}
	else {
		Candidate var0(comb[args[0]]);
		Candidate var1(comb[args[1]]);
		return evaluateSuchThatClause(pkb, type, var0, var1);
	}
}

bool QueryEvaluator::evaluatePatternClause(PKB &pkb, Candidate assignStmt,
	Candidate lhsVar, std::string expr)
{
	if (Utils::TrimSpaces(expr) == "_") {
		std::vector<TreeNode*> vt(pkb.GetAllAssignTreeNodes());
		return !vt.empty();
	}
	TreeNode* node(Utils::buildExprTree(expr));
	if (expr.find_first_of('_') == std::string::npos) {
		return pkb.IsExactPattern(Utils::StringToInt(assignStmt), lhsVar, node);
	}
	else {
		return pkb.IsSubPattern(Utils::StringToInt(assignStmt), lhsVar, node);
	}
}

bool QueryEvaluator::evaluateSuchThatClause(PKB &pkb,
	std::string clauseType, Candidate var0, Candidate var1)
{
	if (clauseType == "Uses") {
		return evaluateUses(pkb, var0, var1);
	}
	else if (clauseType == "Modifies") {
		return evaluateModifies(pkb, var0, var1);
	}
	else if (clauseType == "Parent") {
		return evaluateParent(pkb, var0, var1);
	}
	else if (clauseType == "Parent*") {
		return evaluateParentStar(pkb, var0, var1);
	}
	else if (clauseType == "Follows") {
		return evaluateFollows(pkb, var0, var1);
	}
	else if (clauseType == "Follows*") {
		return evaluateFollowsStar(pkb, var0, var1);
	}
	else if (clauseType == ("Next")) {
		return evaluateNext(pkb, var0, var1);
	}
	else if (clauseType == ("Next*")) {
		return evaluateNextStar(pkb, var0, var1);
	}
	else if (clauseType == ("Calls")) {
		return evaluateCalls(pkb, var0, var1);
	}
	else if (clauseType == ("Calls*")) {
		return evaluateCallsStar(pkb, var0, var1);
	}
	else if (clauseType == ("Affects")) {
		return evaluateAffects(pkb, var0, var1);
	}
	else if (clauseType == ("Affects*")) {
		return evaluateAffectsStar(pkb, var0, var1);
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


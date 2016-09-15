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

PartialCombinationList QueryEvaluator::getCandidates(std::pair<VarName, Symbol> var) {
	PartialCombinationList result;
	switch (var.second) {
		case VARIABLE:
			insertMap(PKB::GetAllVariableNames(), var.first, result);
			break;
		case PROCEDURE:
			insertMap(PKB::GetAllProcedures(), var.first, result);
			break;
		case PROGRAM_LINE:
		case STMT:
		case ASSIGN:
		case IF:
		case WHILE:
			insertMap(Utils::IntToString(PKB::GetSymbolStmtNumbers(var.second)), var.first, result);
			break;
		default:
			break;
	}
	return result;
}

TotalCombinationList QueryEvaluator::getTotalCandidateList(QueryTree &query) {
	TotalCombinationList totalCandLst;
	std::unordered_map<std::string, Symbol> varMap = query.getVarMap();
	for (auto kv : varMap) {
		PartialCombinationList candMapLst(getCandidates(kv));
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

bool QueryEvaluator::selectClauseResults(Clause &clause,
	TotalCombinationList &combinations)
{
	bool hasCandidates;
	std::string type(clause.getClauseType());
	if (type == SYMBOL_PATTERN) {
		std::vector<VarName> args(clause.getArg());
		VarName lhs(args[0]), rhs(args[1]), assignStmt(args[2]);
		if (Utils::IsStringLiteral(lhs)) {
			hasCandidates = FilterNoVarPattern(assignStmt, Utils::LiteralToCandidate(lhs), rhs, combinations);
		}
		else {
			hasCandidates = FilterOneVarPattern(assignStmt, lhs, rhs, combinations);
		}
	}
	else {
		std::vector<VarName> args(clause.getArg());
		VarName var0(args[0]), var1(args[1]);
		if (Utils::IsLiteral(var0)) {
			if (Utils::IsLiteral(var1)) {
				hasCandidates = FilterNoVarClause(type, Utils::LiteralToCandidate(var0),
					Utils::LiteralToCandidate(var1), combinations);
			}
			else {
				hasCandidates = FilterSecondVarClause(type,
					Utils::LiteralToCandidate(var0), var1, combinations);
			}
		}
		else {
			if (Utils::IsLiteral(var1)) {
				hasCandidates = FilterFirstVarClause(type, var0,
					Utils::LiteralToCandidate(var1), combinations);
			}
			else {
				hasCandidates = FilterTwoVarsClause(type, var0, var1, combinations);
			}
		}
	}
	return hasCandidates;
}

bool QueryEvaluator::FilterNoVarPattern(VarName assignStmt, Candidate lhs,
	Candidate expr, TotalCombinationList &combinations)
{
	PartialCombinationList candidateList(combinations[assignStmt]);
	PartialCombinationList filteredList;
	for (CandidateCombination tup : candidateList) {
		if (evaluatePatternClause(tup[assignStmt], lhs, expr)) {
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

bool QueryEvaluator::FilterOneVarPattern(VarName assignStmt, VarName lhs,
	Candidate expr, TotalCombinationList &combinations)
{
	PartialCombinationList candLst0(combinations[assignStmt]);
	PartialCombinationList candLst1(combinations[lhs]);
	PartialCombinationList filteredList;
	if (&candLst0 == &candLst1) {
		for (CandidateCombination tup : candLst0) {
			Candidate stmtCand(tup[assignStmt]), lhsCand(tup[lhs]);
			if (evaluatePatternClause(stmtCand, lhsCand, expr)) {
				filteredList.push_back(tup);
			}
		}
	}
	else {
		for (CandidateCombination tup0 : candLst0) {
			Candidate stmtCand(tup0[assignStmt]);
			for (CandidateCombination tup1 : candLst1) {
				Candidate lhsCand(tup1[lhs]);
				if (evaluatePatternClause(stmtCand, lhsCand, expr)) {
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

bool QueryEvaluator::FilterTwoVarsClause(std::string clauseType,
	VarName &var0, VarName &var1, TotalCombinationList &combinations)
{
	PartialCombinationList candLst0(combinations[var0]);
	PartialCombinationList candLst1(combinations[var1]);
	PartialCombinationList filteredList;
	if (&candLst0 == &candLst1) {
		for (CandidateCombination tup : candLst0) {
			Candidate arg0(tup[var0]), arg1(tup[var1]);
			if (evaluateSuchThatClause(clauseType, arg0, arg1)) {
				filteredList.push_back(tup);
			}
		}
	}
	else {
		for (CandidateCombination tup0 : candLst0) {
			Candidate arg0(tup0[var0]);
			for (CandidateCombination tup1 : candLst1) {
				Candidate arg1(tup1[var1]);
				if (evaluateSuchThatClause(clauseType, arg0, arg1)) {
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

bool QueryEvaluator::FilterFirstVarClause(std::string clauseType,
 VarName var, Candidate constant, TotalCombinationList &combinations)
{
	PartialCombinationList candidateList(combinations[var]);
	PartialCombinationList filteredList;
	for (CandidateCombination tup : candidateList) {
		if (evaluateSuchThatClause(clauseType, tup[var], constant)) {
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

bool QueryEvaluator::FilterSecondVarClause(std::string clauseType,
	Candidate constant, VarName var, TotalCombinationList &combinations)
{
	PartialCombinationList candidateList(combinations[var]);
	PartialCombinationList filteredList;
	for (CandidateCombination tup : candidateList) {
		if (evaluateSuchThatClause(clauseType, constant, tup[var])) {
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

bool QueryEvaluator::FilterNoVarClause(std::string clauseType,
	Candidate const1, Candidate const2, TotalCombinationList &combinations)
{
	log.append(const1 + " " + const2);
	return evaluateSuchThatClause(clauseType, const1, const2);
}

bool QueryEvaluator::evaluateQuery(QueryTree &query)
{
	return false;
}

ResultList QueryEvaluator::selectQueryResults(QueryTree &query)
{
	std::vector<Clause> clauseList = query.getClauses("suchThat pattern");
	TotalCombinationList allCandidates(getTotalCandidateList(query));
	std::unordered_map<VarName, Symbol> selectMap = query.getSelect();
	std::vector<VarName> selectList;
	for (auto kv : selectMap) selectList.push_back(kv.first);

	bool hasMoreCandidates;
	for (Clause clause : clauseList) {
		hasMoreCandidates = selectClauseResults(clause, allCandidates);
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

bool QueryEvaluator::evaluateClause(Clause &clause, CandidateCombination &comb) {
	std::string type(clause.getClauseType());
	std::vector<std::string> args(clause.getArg());
	if (type == SYMBOL_PATTERN) {
		Candidate assignStmt(comb[args[2]]);
		Candidate lhsVar(comb[args[0]]);
		Candidate expr(comb[args[1]]);
		return evaluatePatternClause(assignStmt, lhsVar, expr);
	}
	else {
		Candidate var0(comb[args[0]]);
		Candidate var1(comb[args[1]]);
		return evaluateSuchThatClause(type, var0, var1);
	}
}

bool QueryEvaluator::evaluatePatternClause(Candidate assignStmt,
	Candidate lhsVar, std::string expr)
{
	if (Utils::TrimSpaces(expr) == "_") {
		if (lhsVar == "_") {
			unsigned assignNo(Utils::StringToInt(assignStmt));
			return (PKB::GetAssignTreeNode(assignNo) != NULL);
		}
		else {
			return PKB::IsModifies(Utils::StringToInt(assignStmt), lhsVar);
		}
	}
	if (lhsVar == "_") {
		TreeNode* node(Utils::buildExprTree(expr));
		if (expr.find_first_of('_') == std::string::npos) {
			return PKB::IsExactRHS(Utils::StringToInt(assignStmt), node);
		}
		else {
			return PKB::IsSubRHS(Utils::StringToInt(assignStmt), node);
		}
	}
	TreeNode* node(Utils::buildExprTree(expr));
	if (expr.find_first_of('_') == std::string::npos) {
		return PKB::IsExactPattern(Utils::StringToInt(assignStmt), lhsVar, node);
	}
	else {
		return PKB::IsSubPattern(Utils::StringToInt(assignStmt), lhsVar, node);
	}
}

bool QueryEvaluator::evaluateSuchThatClause(
	std::string clauseType, Candidate var0, Candidate var1)
{
	if (clauseType == "Uses") {
		return evaluateUses(var0, var1);
	}
	else if (clauseType == "Modifies") {
		return evaluateModifies(var0, var1);
	}
	else if (clauseType == "Parent") {
		return evaluateParent(var0, var1);
	}
	else if (clauseType == "Parent*") {
		return evaluateParentStar(var0, var1);
	}
	else if (clauseType == "Follows") {
		return evaluateFollows(var0, var1);
	}
	else if (clauseType == "Follows*") {
		return evaluateFollowsStar(var0, var1);
	}
	else if (clauseType == ("Next")) {
		return evaluateNext(var0, var1);
	}
	else if (clauseType == ("Next*")) {
		return evaluateNextStar(var0, var1);
	}
	else if (clauseType == ("Calls")) {
		return evaluateCalls(var0, var1);
	}
	else if (clauseType == ("Calls*")) {
		return evaluateCallsStar(var0, var1);
	}
	else if (clauseType == ("Affects")) {
		return evaluateAffects(var0, var1);
	}
	else if (clauseType == ("Affects*")) {
		return evaluateAffectsStar(var0, var1);
	}

	return false;
}

bool QueryEvaluator::evaluateUses(Candidate procOrStmtNo, Candidate varName)
{
	if (Utils::IsNonNegativeNumeric(procOrStmtNo)) {
		if (varName == "_") {
			return (!PKB::GetUsedVariables(Utils::StringToInt(procOrStmtNo)).empty());
		}
		else {
			int stmtNo(Utils::StringToInt(procOrStmtNo));
			return PKB::IsUses(stmtNo, varName);
		}
	}
	else if (procOrStmtNo == "_") {
		if (varName == "_") {
			return (PKB::GetNumberOfAssign() > 0);
		}
		else {
			return (!PKB::GetStmtNumberUsing(varName).empty());
		}
	}
	else {
		return PKB::IsUsesProcedure(procOrStmtNo, varName);
	}
}

bool QueryEvaluator::evaluateModifies(Candidate procOrStmtNo, Candidate varName)
{
	if (Utils::IsNonNegativeNumeric(procOrStmtNo)) {
		int stmtNo(Utils::StringToInt(procOrStmtNo));
		if (varName == "_") {
			return (!PKB::GetModifiedVariables(stmtNo).empty());
		}
		return PKB::IsModifies(stmtNo, varName);
	}
	else {
		return PKB::IsModifiesProcedure(procOrStmtNo, varName);
	}
}

bool QueryEvaluator::evaluateParent(Candidate stmt1, Candidate stmt2)
{
	if (stmt1 == "_") {
		if (stmt2 == "_") {
			return (PKB::GetNumberOfIf() + PKB::GetNumberOfWhile() > 0);
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return (!PKB::GetParentsTransitive(stmtNo2).empty());
		}
	}
	else {
		int stmtNo1(Utils::StringToInt(stmt1));
		if (stmt2 == "_") {
			return (!PKB::GetChildrenTransitive(stmtNo1).empty());
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return PKB::IsParent(stmtNo1, stmtNo2);
		}
	}
}

bool QueryEvaluator::evaluateParentStar(Candidate stmt1, Candidate stmt2)
{
	if (stmt1 == "_") {
		if (stmt2 == "_") {
			return (PKB::GetNumberOfIf() + PKB::GetNumberOfWhile() > 0);
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return (!PKB::GetParentsTransitive(stmtNo2).empty());
		}
	}
	else {
		int stmtNo1(Utils::StringToInt(stmt1));
		if (stmt2 == "_") {
			return (!PKB::GetChildrenTransitive(stmtNo1).empty());
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return PKB::IsParentTransitive(stmtNo1, stmtNo2);
		}
	}
}

bool QueryEvaluator::evaluateFollows(Candidate stmt1, Candidate stmt2)
{
	if (stmt1 == "_") {
		if (stmt2 == "_") {
			return true; // TODO
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return (PKB::GetFollows(stmtNo2) > 0);
		}
	}
	else {
		int stmtNo1(Utils::StringToInt(stmt1));
		if (stmt2 == "_") {
			return (PKB::GetFollowing(stmtNo1) > 0);
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return PKB::IsFollows(stmtNo1, stmtNo2);
		}
	}
}

bool QueryEvaluator::evaluateFollowsStar(Candidate stmt1, Candidate stmt2)
{
	if (stmt1 == "_") {
		if (stmt2 == "_") {
			return true; // TODO
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return (PKB::GetFollows(stmtNo2) > 0);
		}
	}
	else {
		int stmtNo1(Utils::StringToInt(stmt1));
		if (stmt2 == "_") {
			return (PKB::GetFollowing(stmtNo1) > 0);
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return PKB::IsFollowsTransitive(stmtNo1, stmtNo2);
		}
	}
}

bool QueryEvaluator::evaluateNext(Candidate stmt1, Candidate stmt2)
{
	int stmtNo1(Utils::StringToInt(stmt1));
	int stmtNo2(Utils::StringToInt(stmt2));
	//return PKB::IsNext(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateNextStar(Candidate stmt1, Candidate stmt2)
{
	int stmtNo1(Utils::StringToInt(stmt1));
	int stmtNo2(Utils::StringToInt(stmt2));
	//return PKB::IsNextTransitive(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateCalls(Candidate proc1, Candidate proc2)
{
	//return PKB::IsCalls(proc1, proc2);
	return false;
}

bool QueryEvaluator::evaluateCallsStar(Candidate proc1, Candidate proc2)
{
	//return PKB::IsCallsTransitive(proc1, proc2);
	return false;
}

bool QueryEvaluator::evaluateAffects(Candidate assign1, Candidate assign2)
{
	int stmtNo1(Utils::StringToInt(assign1));
	int stmtNo2(Utils::StringToInt(assign2));
	//return PKB::IsAffects(stmtNo1, stmtNo2);
	return false;
}

bool QueryEvaluator::evaluateAffectsStar(Candidate assign1, Candidate assign2)
{
	int stmtNo1(Utils::StringToInt(assign1));
	int stmtNo2(Utils::StringToInt(assign2));
	//return PKB::IsAffectsTransitive(stmtNo1, stmtNo2);
	return false;
}


#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include "QueryProcessor/Clause.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryUtils.h"
#include "Utils.h"

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}

std::vector<Candidate> QueryEvaluator::getCandidates(Symbol &synType) {
	switch (synType) {
		case VARIABLE:
			return PKB::GetAllVariableNames();
		case PROCEDURE:
			return PKB::GetAllProcedures();
        case CONSTANT:
            return PKB::GetAllConstantValues();
		case PROGRAM_LINE:
            return Utils::IntsToStrings(PKB::GetSymbolStmtNumbers(STMT));
		case STMT:
		case ASSIGN:
		case IF:
		case WHILE:
			return Utils::IntsToStrings(PKB::GetSymbolStmtNumbers(synType));
		default:
			return std::vector<Candidate>();
	}
}

TotalCombinationList QueryEvaluator::getTotalCandidateList
(std::unordered_map<std::string, Symbol> &varMap, std::vector<Synonym> &synList)
{
	TotalCombinationList totalCandLst;
	for (Synonym &syn : synList) {
		std::vector<Candidate> candLst(getCandidates(varMap[syn]));
		totalCandLst.addSynonym(syn, candLst);
	}
	return totalCandLst;
}


ResultList QueryEvaluator::selectQueryResults(QueryTree &query)
{
	std::vector<Synonym> selectList(query.getResults());
	TotalCombinationList queryResults(getQueryResults(query));
	if (isBoolSelect(selectList)) {
		std::string resultBoolean;
		if (!queryResults.isEmpty()) {
			resultBoolean.append(SYMBOL_TRUE);
		}
		else {
			resultBoolean.append(SYMBOL_FALSE);
		}
		std::vector<std::vector<std::string>> tupleList;
		tupleList.push_back(std::vector<std::string>{resultBoolean});
		ResultList resultList{ selectList, tupleList };
		return resultList;
	}
	else {
		if (queryResults.isEmpty()) {
			ResultList resultList{ selectList, std::vector<std::vector<std::string>>() };
			return resultList;
		}
		else {
			ResultList resultList(getResultsFromCombinationList(queryResults, selectList));
			return resultList;
		}
	}
}

TotalCombinationList QueryEvaluator::getQueryResults(QueryTree &query) {
	if (!getBooleanGroupResult(query.getBooleanClauses())) {
		return TotalCombinationList();
	}
	else {
		
		std::vector<std::pair<std::vector<Synonym>, std::vector<Clause>>> unselectedGroups(query.getUnselectedGroups());
		std::unordered_map<Synonym, Symbol> varMap(query.getVarMap());
		
		for (auto &pair : unselectedGroups) {
			if (!getUnselectedGroupResult(pair.first, varMap, pair.second)) {
				return TotalCombinationList();
			}
		}

		TotalCombinationList result;
		std::vector<Synonym> selectList(query.getResults());
		std::vector<std::pair<std::vector<Synonym>, std::vector<Clause>>> selectedGroups(query.getSelectedGroups());
		
		for (auto &pair : selectedGroups) {
			std::vector<Synonym> &synList(pair.first);
			std::vector<Clause> &group(pair.second);
			TotalCombinationList &tempCombiList(getSelectedGroupResult(synList, varMap, group, selectList));
			result.combine(tempCombiList);

			for (Synonym &syn : synList) {
				std::vector<Synonym>::iterator it(selectList.begin());
				while (it != selectList.end()) {
					if (syn == (*it)) {
						it = selectList.erase(it);
					}
					else {
						++it;
					}
				}
			}
		}

		for (Synonym &syn : selectList) {
			std::vector<Candidate> candList(getCandidates(varMap[syn]));
			result.addSynonym(syn, candList);
		}

		return result;
	}
}

bool QueryEvaluator::getBooleanGroupResult(std::vector<Clause> &clauseGroup) {
	for (Clause clause : clauseGroup) {
		Candidate arg0(QueryUtils::LiteralToCandidate(clause.getArg()[0]));
		Candidate arg1(QueryUtils::LiteralToCandidate(clause.getArg()[1]));
		std::string clauseType(clause.getClauseType());
		if (!evaluateSuchThatClause(clauseType, arg0, arg1)) {
			return false;
		}
	}
	return true;
}

bool QueryEvaluator::getUnselectedGroupResult
(std::vector<Synonym> &synList,
	std::unordered_map<Synonym, Symbol> &varMap,
	std::vector<Clause> &clauseGroup)
{
	TotalCombinationList &combinations(getTotalCandidateList(varMap, synList));
	for (Clause clause : clauseGroup) {
		filterByClause(clause, combinations, varMap);
		if (combinations.isEmpty()) {
			return false;
		}
	}
	return true;
}

TotalCombinationList QueryEvaluator::getSelectedGroupResult
(std::vector<Synonym> &synList,
	std::unordered_map<Synonym, Symbol> &varMap,
	std::vector<Clause> &clauseGroup,
	std::vector<Synonym> &selectList)
{
	TotalCombinationList combinations(getTotalCandidateList(varMap, synList));
	for (Clause clause : clauseGroup) {
		filterByClause(clause, combinations, varMap);
		if (combinations.isEmpty()) {
			return TotalCombinationList();
		}
	}
	combinations.reduceTotalContent(selectList);
	return combinations;
}

void QueryEvaluator::filterByClause(Clause &clause,
	TotalCombinationList &combinations, std::unordered_map<Synonym, Symbol> &varMap)
{
	std::string clauseType(clause.getClauseType());
	if (clauseType == SYMBOL_PATTERN) {
		std::vector<Synonym> args(clause.getArg());
		Synonym lhs(args[1]), rhs(args[2]), assignStmt(args[0]);
		if (QueryUtils::IsStringLiteral(lhs)) {
			filterNoVarPattern(assignStmt, QueryUtils::LiteralToCandidate(lhs), rhs, combinations);
		}
		else {
			filterOneVarPattern(assignStmt, lhs, rhs, combinations);
		}
	}
	else if (clauseType == SYMBOL_WITH) {
		std::vector<Synonym> args(clause.getArg());
		Synonym var0(args[0]), var1(args[1]);
		if (QueryUtils::IsLiteral(var0)) {
			var0 = QueryUtils::LiteralToCandidate(var0);
			if (QueryUtils::IsLiteral(var1)) {
				filterNoVarWith(var0, QueryUtils::LiteralToCandidate(var1), combinations);
			}
			else if (varMap[var1] == CALL) {
				filterNoVarCallWith(var1, var0, combinations);
			}
			else {
				filterSecondVarWith(var0, var1, combinations);
			}
		}
		else if (QueryUtils::IsLiteral(var1)) {
			var1 = QueryUtils::LiteralToCandidate(var1);
			if (varMap[var0] == CALL) {
				filterNoVarCallWith(var0, var1, combinations);
			}
			else {
				filterFirstVarWith(var0, var1, combinations);
			}
		}
		else {
			if (varMap[var0] == CALL) {
				if (varMap[var1] == CALL) filterTwoVarsCallWith(var0, var1, combinations);
				else filterOneVarCallWith(var0, var1, combinations);
			}
			else if (varMap[var1] == CALL) {
				filterOneVarCallWith(var1, var0, combinations);
			}
			else {
				filterTwoVarsWith(var0, var1, combinations);
			}
		}
	}
	else {
		std::vector<Synonym> args(clause.getArg());
		Synonym var0(args[0]), var1(args[1]);
		if (QueryUtils::IsLiteral(var0)) {
			if (QueryUtils::IsLiteral(var1)) {
				filterNoVarClause(clauseType, QueryUtils::LiteralToCandidate(var0), QueryUtils::LiteralToCandidate(var1), combinations);
			}
			else {
				filterSecondVarClause(clauseType, QueryUtils::LiteralToCandidate(var0), var1, combinations);
			}
		}
		else {
			if (QueryUtils::IsLiteral(var1)) {
				filterFirstVarClause(clauseType, var0, QueryUtils::LiteralToCandidate(var1), combinations);
			}
			else {
				filterTwoVarsClause(clauseType, var0, var1, combinations);
			}
		}
	}
}

void QueryEvaluator::filterNoVarPattern(Synonym assignStmt, Candidate lhs,
	Candidate expr, TotalCombinationList &combinations)
{
	auto matchPattern = [=](CandidateCombination &combi) -> bool {
		return evaluatePatternClause(combi[assignStmt], lhs, expr);
	};
	combinations.filter(assignStmt, matchPattern);
}

void QueryEvaluator::filterOneVarPattern(Synonym assignStmt, Synonym lhs,
	Candidate expr, TotalCombinationList &combinations)
{
	auto matchPattern = [=](CandidateCombination combi) -> bool {
		return evaluatePatternClause(combi[assignStmt], combi[lhs], expr);
	};
	combinations.mergeAndFilter(assignStmt, lhs, matchPattern);
}

void QueryEvaluator::filterTwoVarsClause(std::string clauseType,
	Synonym &var0, Synonym &var1, TotalCombinationList &combinations)
{
	auto evaluateClause = [=](CandidateCombination combi) -> bool {
		return evaluateSuchThatClause(clauseType, combi[var0], combi[var1]);
	};
	combinations.mergeAndFilter(var0, var1, evaluateClause);
}

void QueryEvaluator::filterFirstVarClause(std::string clauseType,
 Synonym var, Candidate constant, TotalCombinationList &combinations)
{
	auto evaluateClause = [=](CandidateCombination combi) -> bool {
		return evaluateSuchThatClause(clauseType, combi[var], constant);
	};
	combinations.filter(var, evaluateClause);
}

void QueryEvaluator::filterSecondVarClause(std::string clauseType,
	Candidate constant, Synonym var, TotalCombinationList &combinations)
{
	auto evaluateClause = [=](CandidateCombination combi) -> bool {
		return evaluateSuchThatClause(clauseType, constant, combi[var]);
	};
	combinations.filter(var, evaluateClause);
}

void QueryEvaluator::filterNoVarClause(std::string clauseType,
	Candidate const1, Candidate const2, TotalCombinationList &combinations)
{
	combinations.filter(evaluateSuchThatClause(clauseType, const1, const2));
}

void QueryEvaluator::filterTwoVarsWith(Synonym &var0, Synonym &var1, TotalCombinationList &combinations)
{
	auto evaluateClause = [=](CandidateCombination combi) -> bool {
		return (combi[var0] == combi[var1]);
	};
	combinations.mergeAndFilter(var0, var1, evaluateClause);
}

void QueryEvaluator::filterFirstVarWith(Synonym &var, Candidate constant, TotalCombinationList &combinations)
{
	auto evaluateClause = [=](CandidateCombination combi) -> bool {
		return (combi[var] == constant);
	};
	combinations.filter(var, evaluateClause);
}

void QueryEvaluator::filterSecondVarWith(Candidate constant, Synonym &var, TotalCombinationList &combinations)
{
	auto evaluateClause = [=](CandidateCombination combi) -> bool {
		return (combi[var] == constant);
	};
	combinations.filter(var, evaluateClause);
}

void QueryEvaluator::filterNoVarWith(Candidate const1, Candidate const2, TotalCombinationList &combinations)
{
	combinations.filter(const1 == const2);
}

void QueryEvaluator::filterTwoVarsCallWith(Synonym & call0, Synonym & call1, TotalCombinationList & combinations)
{
}

void QueryEvaluator::filterOneVarCallWith(Synonym & call, Synonym & var, TotalCombinationList & combinations)
{
	PartialCombinationList part(combinations[var]);
	if (part.empty()) {
		combinations.filter(false);
	}
	else {
		Candidate cand(part.front()[var]);
		if (Utils::IsNonNegativeNumeric(cand)) {
			auto comp = [=](CandidateCombination combi) -> bool {
				return (combi[call] == combi[var]);
			};
			combinations.mergeAndFilter(call, var, comp);
		}
		else {
			auto comp = [=](CandidateCombination combi) -> bool {
				return (PKB::GetCallProcedureName(Utils::StringToInt(combi[call])) == combi[var]);
			};
			combinations.mergeAndFilter(call, var, comp);
		}
	}
}

void QueryEvaluator::filterNoVarCallWith(Synonym & call, Candidate cand, TotalCombinationList & combinations)
{
	if (Utils::IsNonNegativeNumeric(cand)) {
		auto comp = [=](CandidateCombination combi) -> bool {
			return (combi[call] == cand);
		};
		combinations.filter(call, comp);
	}
	else {
		auto comp = [=](CandidateCombination combi) -> bool {
			return (PKB::GetCallProcedureName(Utils::StringToInt(combi[call])) == cand);
		};
		combinations.filter(call, comp);
	}
}

ResultList QueryEvaluator::getResultsFromCombinationList
(TotalCombinationList &combinations, std::vector<Synonym> &selectList)
{
	PartialCombinationList
		selectedCombinations(combinations.getCombinationList(selectList));
	std::vector<std::vector<Candidate>> selectedCombinationList;
	for (CandidateCombination combi : selectedCombinations) {
		std::vector<Candidate> candidateTuple;
		for (Synonym syn : selectList) {
			candidateTuple.push_back(combi[syn]);
		}
		selectedCombinationList.push_back(candidateTuple);
	}
	ResultList result{ selectList, selectedCombinationList };
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

bool QueryEvaluator::isBoolSelect(std::vector<std::string> &selectList) {
	if (selectList.size() == 1 && selectList[0] == SYMBOL_BOOLEAN) {
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
	unsigned assignNo(Utils::StringToInt(assignStmt));
	std::string expression(QueryUtils::GetExpression(expr));
	if (expression == "_") {
		if (lhsVar == "_") {
			return true;
		}
		else {
			return PKB::IsModifies(assignNo, lhsVar);
		}
	}
	else if (Utils::StartsWith(expression, '_')) {
		if (lhsVar == "_") {
			return PKB::IsSubExpression(assignNo, QueryUtils::GetSubExpression(expression));
		}
		else {
			return (PKB::IsSubExpression(assignNo, QueryUtils::GetSubExpression(expression)) && PKB::IsModifies(assignNo, lhsVar));
		}
	}
	else {
		if (lhsVar == "_") {
			return PKB::IsExactExpression(assignNo, expression);
		}
		else {
			return (PKB::IsExactExpression(assignNo, expression) && PKB::IsModifies(assignNo, lhsVar));
		}
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
        if (varName == "_") {
            return (!PKB::GetProcedureModifiedVariables(procOrStmtNo).empty());
        }
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
			std::vector<unsigned> vec(PKB::GetSymbolStmtNumbers(STMT));
			for (unsigned cand1 : vec) {
				for (unsigned cand2 : vec) {
					if (PKB::IsFollows(cand1, cand2)) return true;
				}
			}
			return false;
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return (PKB::GetFollows(stmtNo2) > 0);
		}
	}
	else {
		int stmtNo1(Utils::StringToInt(stmt1));
		if (stmt2 == "_") {
			return (!PKB::GetFollowing(stmtNo1).empty());
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
			std::vector<unsigned> vec(PKB::GetSymbolStmtNumbers(STMT));
			for (unsigned cand1 : vec) {
				for (unsigned cand2 : vec) {
					if (PKB::IsFollowsTransitive(cand1, cand2)) return true;
				}
			}
			return false;
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return (PKB::GetFollows(stmtNo2) > 0);
		}
	}
	else {
		int stmtNo1(Utils::StringToInt(stmt1));
		if (stmt2 == "_") {
			return (!PKB::GetFollowing(stmtNo1).empty());
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
	return PKB::IsCalls(proc1, proc2);
}

bool QueryEvaluator::evaluateCallsStar(Candidate proc1, Candidate proc2)
{
	return PKB::IsCallsTransitive(proc1, proc2);
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


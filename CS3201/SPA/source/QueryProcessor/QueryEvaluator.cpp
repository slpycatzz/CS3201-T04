#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Constants.h"
#include "QueryProcessor/Clause.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryUtils.h"
#include "Utils.h"
#include "PKB/PKB.h"

using std::string;
using std::unordered_map;
using std::vector;

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}

void QueryEvaluator::initialize(QueryTree & query)
{
	varMap = query.getVarMap();
	selectList = query.getResults();
	booleanClauses = query.getBooleanClauses();
	unselectedGroups = query.getUnselectedGroups();
	selectedGroups = query.getSelectedGroups();
}

vector<Candidate> QueryEvaluator::getCandidates(Symbol &synType)
{
    switch (synType) {
        case VARIABLE:
            return PKB::GetAllVariableIndexes();
        case PROCEDURE:
            return PKB::GetAllProcedureIndexes();
        case BOOLEAN:
            return vector<Candidate>{ 1 };
        case CONSTANT:
            return Utils::StringsToInts(PKB::GetAllConstantValues());
		case PROGRAM_LINE:
			return PKB::GetSymbolStmtNumbers(STMT);
		case STMTLIST:
			return PKB::GetAllStmtlistsStmtNumber();
		case CALL:
        case STMT:
        case ASSIGN:
        case IF:
        case WHILE:
            return PKB::GetSymbolStmtNumbers(synType);
        default:
            return vector<Candidate>();
    }
}

TotalCombinationList QueryEvaluator::getTotalCandidateList(vector<Synonym> &synList)
{
    TotalCombinationList totalCandLst;
    for (Synonym &syn : synList) {
        vector<Candidate> candLst(getCandidates(varMap[syn]));
        totalCandLst.addSynonym(syn, candLst);
    }

    return totalCandLst;
}

ResultList QueryEvaluator::selectQueryResults(QueryTree &query)
{
	initialize(query);

    TotalCombinationList queryResults(getQueryResults());
    if (isBoolSelect(selectList)) {
		string resultBoolean = (queryResults.getFactorCount() > 0) ? SYMBOL_TRUE : SYMBOL_FALSE;

        vector<vector<string>> tupleList;
        tupleList.push_back(vector<string>{resultBoolean});
        ResultList resultList{ selectList, tupleList };

        return resultList;

    } else {

		log.append("\n");
		log.append("selectList content:");
		for (Synonym syn : selectList) {
			log.append(" " + syn);
		}

        if (queryResults.isEmpty() || queryResults.getFactorCount() == 0) {
            ResultList resultList{ selectList, vector<vector<string>>() };
            return resultList;

        } else {
            ResultList resultList(getResultsFromCombinationList(queryResults));
            return resultList;
        }
    }
}

ResultList QueryEvaluator::getResultsFromCombinationList(TotalCombinationList &combinations)
{
	PartialCombinationList selectedCombinations(combinations.getCombinationList(selectList));
	vector<vector<string>> selectedCombinationList;

	for (CandidateCombination combi : selectedCombinations) {
		vector<string> candidateTuple;

		log.append("\n");

		for (Synonym syn : selectList) {
			string cand;
			switch (varMap[syn]) {
			case VARIABLE:
				cand = PKB::GetVariableName(combi[syn]);
				break;
			case PROCEDURE:
				cand = PKB::GetProcedureName(combi[syn]);
				break;
			default:
				cand = Utils::IntToString(combi[syn]);
			}
			candidateTuple.push_back(cand);
			
			log.append(cand + " ");
		}

		selectedCombinationList.push_back(candidateTuple);
	}

	ResultList result{ selectList, selectedCombinationList };

	return result;
}

bool QueryEvaluator::isBoolSelect(vector<string> &synList)
{
	if (synList.size() == 1 && synList[0] == SYMBOL_BOOLEAN) {
		return true;
	}
	else {
		return false;
	}
}

TotalCombinationList QueryEvaluator::getQueryResults()
{
    if (!getBooleanGroupResult()) {
        return TotalCombinationList();

    } else {

        for (auto &pair : unselectedGroups) {
            if (!getUnselectedGroupResult(pair.first, pair.second)) {
                return TotalCombinationList();
            }
        }

        TotalCombinationList result;
		vector<Synonym> copiedSelectList = selectList;

        for (auto &pair : selectedGroups) {
            vector<Synonym> &synList(pair.first);
            vector<Clause> &group(pair.second);
            TotalCombinationList &tempCombiList(getSelectedGroupResult(synList, group));
			if (tempCombiList.isEmpty() || tempCombiList.getFactorCount() == 0) {
				return TotalCombinationList();
			}
            result.combine(tempCombiList);

            for (Synonym &syn : synList) {
                vector<Synonym>::iterator it(copiedSelectList.begin());
                while (it != copiedSelectList.end()) {
                    if (syn == (*it)) {
                        it = copiedSelectList.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }
		if (result.isEmpty()) {
			return TotalCombinationList();
		}
        for (Synonym &syn : copiedSelectList) {
            vector<Candidate> candList(getCandidates(varMap[syn]));
            result.addSynonym(syn, candList);
        }


		log.append("\n");
		log.append("result content: ");
		for (auto kv : result.getContent()) log.append(kv.first + ":" + Utils::IntToString(kv.second) + " ");
		log.append("\n");
		log.append("result factors: ");
		for (auto kv : result.getFactorList()) {
			log.append(kv.first + ":");
			log.append("<");
			for (CandidateCombination comb : kv.second) {
				log.append(Utils::MapToString(comb));
				log.append(" ");
			}
			log.append("> ");
		}


        return result;
    }
}

bool QueryEvaluator::getBooleanGroupResult()
{
    for (Clause clause : booleanClauses) {
        
		string arg0(clause.getArg()[0]);
        string arg1(clause.getArg()[1]);
        string clauseType(clause.getClauseType());
		
		if (clauseType == SYMBOL_WITH) {
			if (arg0 != arg1) {
				return false;
			}
		}
		else {
			if (!evaluateSuchThatClause(clauseType, arg0, arg1)) {
				return false;
			}
		}

    }

    return true;
}

bool QueryEvaluator::getUnselectedGroupResult(vector<Synonym> &synList, vector<Clause> &clauseGroup)
{
    TotalCombinationList &combinations(getTotalCandidateList(synList));

	log.append("\n");
	log.append("totalCombinationList content: ");
	for (auto kv : combinations.getContent()) log.append(kv.first + ":" + Utils::IntToString(kv.second) + " ");
	log.append("\n");
	log.append("totalCombinationList factors: ");
	for (auto kv : combinations.getFactorList()) {
		log.append(kv.first + ":");
		log.append("<");
		for (CandidateCombination comb : kv.second) {
			log.append(Utils::MapToString(comb));
			log.append(" ");
		}
		log.append("> ");
	}

    for (Clause clause : clauseGroup) {
        filterByClause(clause, combinations);

        if (combinations.isEmpty()) {
            return false;
        }
    }

    return true;
}

TotalCombinationList QueryEvaluator::getSelectedGroupResult(vector<Synonym> &synList, vector<Clause> &clauseGroup)
{
    
	TotalCombinationList combinations(getTotalCandidateList(synList));

	log.append("\n");
	log.append("totalCombinationList content: ");
	for (auto kv : combinations.getContent()) log.append(kv.first + ":" + Utils::IntToString(kv.second) + " ");
	log.append("\n");
	log.append("totalCombinationList factors: ");
	for (auto kv : combinations.getFactorList()) {
		log.append(kv.first + ":");
		log.append("<");
		for (CandidateCombination comb : kv.second) {
			log.append(Utils::MapToString(comb));
			log.append(" ");
		}
		log.append("> ");
	}
	
	log.append("\n");
	log.append("synList: ");
	for (Synonym syn : synList) log.append(syn + " ");
	log.append("\n");
	log.append("selectList: ");
	for (Synonym syn : selectList) log.append(syn + " ");
	log.append("\n");
	log.append("varMap: ");
	for (auto kv : varMap) log.append(kv.first + ":" + Constants::SymbolToString(kv.second) + " ");

    for (Clause clause : clauseGroup) {

        filterByClause(clause, combinations);

        if (combinations.isEmpty()) {
            return TotalCombinationList();
        }
    }

    combinations.reduceTotalContent(selectList);

	log.append("\n");
	log.append("totalCombinationList content: ");
	for (auto kv : combinations.getContent()) log.append(kv.first + ":" + Utils::IntToString(kv.second) + " ");
	log.append("\n");
	log.append("totalCombinationList factors: ");
	for (auto kv : combinations.getFactorList()) {
		log.append(kv.first + ":");
		log.append("<");
		for (CandidateCombination comb : kv.second) {
			log.append(Utils::MapToString(comb));
			log.append(" ");
		}
		log.append("> ");
	}

    return combinations;
}


void QueryEvaluator::filterByClause(Clause &clause, TotalCombinationList &combinations)
{
    string clauseType(clause.getClauseType());
	vector<Synonym> args(clause.getArg());
	
	log.append("\n");
	log.append("clause type: " + clause.getClauseType() + "\n");
	log.append("args: " + clause.getArg()[0] + " " + clause.getArg()[1]);

    
	if (clauseType == SYMBOL_PATTERN) {

        Synonym lhs(args[1]), rhs(args[2]), assignStmt(args[0]);
		
		if (Utils::IsUnderscore(lhs)) {
			filterUnderscorePattern(assignStmt, rhs, combinations);
		}
        else if (QueryUtils::IsStringLiteral(lhs)) {
			filterNoVarPattern(assignStmt, lhs, rhs, combinations);
		}
		else {
            filterOneVarPattern(assignStmt, lhs, rhs, combinations);
        }

    }
	
	else if (clauseType == SYMBOL_WITH) {

        Synonym var0(args[0]), var1(args[1]);

		Symbol type0 = varMap[var0];
		Symbol type1 = varMap[var1];

		if (args[2] == SYMBOL_VARIABLE) {
			if (QueryUtils::IsStringLiteral(var0)) {
				var0 = QueryUtils::LiteralToCandidate(var0);
				if (QueryUtils::IsStringLiteral(var1)) {
					var1 = QueryUtils::LiteralToCandidate(var1);
					filterNoVarWith(var0, var1, combinations);
				}
				else if (type1 == CALL) {
					Candidate proc = PKB::GetProcedureIndex(var0);
					if (proc == 0) {
						combinations.filter(false);
					}
					else {
						filterNoVarCallWith(var1, proc, combinations);
					}
				}
				else {
					Candidate v = (type1 == PROCEDURE) ? PKB::GetProcedureIndex(var0) : PKB::GetVariableIndex(var0);
					if (v == 0) {
						combinations.filter(false);
					}
					else {
						filterSecondVarWith(v, var1, combinations);
					}
				}
			}
			else if (type0 == CALL) {
				if (QueryUtils::IsStringLiteral(var1)) {
					var1 = QueryUtils::LiteralToCandidate(var1);
					Candidate proc = PKB::GetProcedureIndex(var1);
					if (proc == 0) {
						combinations.filter(false);
					}
					else {
						filterNoVarCallWith(var0, proc, combinations);
					}
				}
				else if (type1 == CALL) {
					filterTwoVarsCallWith(var0, var1, combinations);
				}
				else {
					filterOneVarCallWith(var0, var1, combinations);
				}
			}
			else {
				if (QueryUtils::IsStringLiteral(var1)) {
					var1 = QueryUtils::LiteralToCandidate(var1);
					Candidate v = (type0 == PROCEDURE) ? PKB::GetProcedureIndex(var1) : PKB::GetVariableIndex(var1);
					if (v == 0) {
						combinations.filter(false);
					}
					else {
						filterFirstVarWith(var0, v, combinations);
					}
				}
				else if (type1 == CALL) {
					filterOneVarCallWith(var1, var0, combinations);
				}
				else {
					filterTwoVarsWith(var0, var1, combinations);
				}
			}
		}
		else if (args[2] == SYMBOL_CONSTANT) {
			if (Utils::IsNonNegativeNumeric(var0)) {
				Candidate v0 = Utils::StringToInt(var0);
				if (Utils::IsNonNegativeNumeric(var1)) {
					filterNoVarWith(v0, Utils::StringToInt(var1), combinations);
				}
				else {
					filterSecondVarWith(v0, var1, combinations);
				}

			}
			else if (Utils::IsNonNegativeNumeric(var1)) {
				Candidate v1 = Utils::StringToInt(var1);
				filterFirstVarWith(var0, v1, combinations);
			}
			else {
				filterTwoVarsWith(var0, var1, combinations);
			}
		}
    }
	
	else {
		
		Synonym var0(args[0]), var1(args[1]);

		if (QueryUtils::IsLiteral(var1)) {
			if (QueryUtils::IsLiteral(var0)) {
				filterNoVarClause(clauseType, var0, var1, combinations);
			}
			else {
				filterFirstVarClause(clauseType, var0, var1, combinations);
			}

		}
		else {
			if (QueryUtils::IsLiteral(var0)) {
				filterSecondVarClause(clauseType, var0, var1, combinations);
			}
			else {
				filterTwoVarsClause(clauseType, var0, var1, combinations);
			}
		}
	}

	
	log.append("\n");
	log.append("totalCombinationList content: ");
	for (auto kv : combinations.getContent()) log.append(kv.first + ":" + Utils::IntToString(kv.second) + " ");
	log.append("\n");
	log.append("totalCombinationList factors: ");
	for (auto kv : combinations.getFactorList()) {
		log.append(kv.first + ":");
		log.append("<");
		for (CandidateCombination comb : kv.second) {
			log.append(Utils::MapToString(comb));
			log.append(" ");
		}
		log.append("> ");
	}
	log.append("\n");
	log.append("totalCombinationList isEmpty: ");
	string empty = (combinations.isEmpty()) ? "true" : "false";
	log.append(empty);
}

void QueryEvaluator::filterNoVarPattern(Synonym assignStmt, string lhs,
    string expr, TotalCombinationList &combinations)
{
    auto matchPattern = [=](CandidateCombination &combi) -> bool {
        return evaluateNoVarPattern(combi[assignStmt], lhs, expr);
    };
    combinations.filter(assignStmt, matchPattern);
}

void QueryEvaluator::filterUnderscorePattern(Synonym assignStmt, std::string expr,
	TotalCombinationList & combinations)
{
	auto matchPattern = [=](CandidateCombination &combi) -> bool {
		return evaluateUnderscorePattern(combi[assignStmt], expr);
	};
	combinations.filter(assignStmt, matchPattern);
}


void QueryEvaluator::filterOneVarPattern(Synonym assignStmt, Synonym lhs,
    string expr, TotalCombinationList &combinations)
{
    auto matchPattern = [=](CandidateCombination combi) -> bool {
        return evaluateOneVarPattern(combi[assignStmt], combi[lhs], expr);
    };
    combinations.mergeAndFilter(assignStmt, lhs, matchPattern);
}


void QueryEvaluator::filterTwoVarsClause(string clauseType,
	Synonym &var0, Synonym &var1, TotalCombinationList &combinations)
{
	if (varMap[var0] == PROCEDURE && clauseType == SYMBOL_MODIFIES) {
		auto evaluateClause = [=](CandidateCombination combi) -> bool {
			return PKB::IsModifiesProcedure(combi[var0], combi[var1]);
		};
		combinations.mergeAndFilter(var0, var1, evaluateClause);
	}
	else if (varMap[var0] == PROCEDURE && clauseType == SYMBOL_USES) {
		auto evaluateClause = [=](CandidateCombination combi) -> bool {
			return PKB::IsUsesProcedure(combi[var0], combi[var1]);
		};
		combinations.mergeAndFilter(var0, var1, evaluateClause);
	}
	else {
		auto evaluateClause = [=](CandidateCombination combi) -> bool {
			return evaluateSuchThatClause(clauseType, combi[var0], combi[var1]);
		};
		combinations.mergeAndFilter(var0, var1, evaluateClause);
	}
}


void QueryEvaluator::filterFirstVarClause(string clauseType,
	Synonym var, string constant, TotalCombinationList &combinations)
{

	if (varMap[var] == PROCEDURE && clauseType == SYMBOL_MODIFIES) {

		if (Utils::IsUnderscore(constant)) {
			auto evaluateClause = [=](CandidateCombination combi) -> bool {
				return (!PKB::GetProcedureModifiedVariables(combi[var]).empty());
			};
			combinations.filter(var, evaluateClause);
		}
		else {
			Candidate v = PKB::GetVariableIndex(QueryUtils::LiteralToCandidate(constant));
			if (v == 0) {
				combinations.filter(false);
			}
			else {
				auto evaluateClause = [=](CandidateCombination combi) -> bool {
					return PKB::IsModifiesProcedure(combi[var], v);
				};
				combinations.filter(var, evaluateClause);
			}
		}
	}

	else if (varMap[var] == PROCEDURE && clauseType == SYMBOL_USES) {

		if (Utils::IsUnderscore(constant)) {
			auto evaluateClause = [=](CandidateCombination combi) -> bool {
				return (!PKB::GetProcedureUsedVariables(combi[var]).empty());
			};
			combinations.filter(var, evaluateClause);
		}
		else {
			Candidate v = PKB::GetVariableIndex(QueryUtils::LiteralToCandidate(constant));
			if (v == 0) {
				combinations.filter(false);
			}
			else {
				auto evaluateClause = [=](CandidateCombination combi) -> bool {
					return PKB::IsUsesProcedure(combi[var], v);
				};
				combinations.filter(var, evaluateClause);
			}
		}
	}

	else {
		auto evaluateClause = [=](CandidateCombination combi) -> bool {
			return evaluateSuchThatClause(clauseType, combi[var], constant);
		};
		combinations.filter(var, evaluateClause);
	}
}


void QueryEvaluator::filterSecondVarClause(string clauseType,
    string constant, Synonym var, TotalCombinationList &combinations)
{
    auto evaluateClause = [=](CandidateCombination combi) -> bool {
        return evaluateSuchThatClause(clauseType, constant, combi[var]);
    };
    combinations.filter(var, evaluateClause);
}


void QueryEvaluator::filterNoVarClause(string clauseType,
    string const1, string const2, TotalCombinationList &combinations)
{
    combinations.filter(evaluateSuchThatClause(clauseType, const1, const2));
}


void QueryEvaluator::filterTwoVarsWith(Synonym &var0, Synonym &var1, TotalCombinationList &combinations)
{
	if (varMap[var0] == PROCEDURE && varMap[var1] == VARIABLE) {
		auto evaluateClause = [=](CandidateCombination combi) -> bool {
			return (PKB::GetProcedureName(combi[var0]) == PKB::GetVariableName(combi[var1]));
		};
		combinations.mergeAndFilter(var0, var1, evaluateClause);
	}
	else if (varMap[var1] == PROCEDURE && varMap[var0] == VARIABLE) {
		auto evaluateClause = [=](CandidateCombination combi) -> bool {
			return (PKB::GetProcedureName(combi[var1]) == PKB::GetVariableName(combi[var0]));
		};
		combinations.mergeAndFilter(var0, var1, evaluateClause);
	}
	else {
		auto evaluateClause = [=](CandidateCombination combi) -> bool {
			return (combi[var0] == combi[var1]);
		};
		combinations.mergeAndFilter(var0, var1, evaluateClause);
	}
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


void QueryEvaluator::filterNoVarWith(string const1, string const2, TotalCombinationList &combinations)
{
	combinations.filter(const1 == const2);
}


void QueryEvaluator::filterTwoVarsCallWith(Synonym & call0, Synonym & call1, TotalCombinationList & combinations)
{
	auto comp = [=](CandidateCombination combi) -> bool {
		return (PKB::GetCallStmtProcedureIndex(combi[call0])
			== (PKB::GetCallStmtProcedureIndex(combi[call1])));
	};
	combinations.mergeAndFilter(call0, call1, comp);
}


void QueryEvaluator::filterOneVarCallWith(Synonym & call, Synonym & var, TotalCombinationList & combinations)
{
	if (varMap[var] == PROCEDURE) {
		auto comp = [=](CandidateCombination combi) -> bool {
			return (PKB::GetCallStmtProcedureIndex(combi[call]) == combi[var]);
		};
		combinations.mergeAndFilter(call, var, comp);
	}
	else {
		auto comp = [=](CandidateCombination combi) -> bool {
			return (PKB::GetCallStmtProcedureName(combi[call]) == PKB::GetVariableName(combi[var]));
		};
		combinations.mergeAndFilter(call, var, comp);
	}
}


void QueryEvaluator::filterNoVarCallWith(Synonym & call, Candidate cand, TotalCombinationList & combinations)
{
    auto comp = [=](CandidateCombination combi) -> bool {
        return (PKB::GetCallStmtProcedureIndex(combi[call]) == cand);
    };
    combinations.filter(call, comp);
}


bool QueryEvaluator::evaluateNoVarPattern(Candidate stmtNo, string lhsVar, string expr)
{
    // Preprocessor will do the following:
    // expression _"x+y"_ should convert to a string of vectors <x, +, y>
    // preprocessor will get (x+y) after calling Utils::GetPostfixExpression(<x, +, y>)
    // in this method, expr = _"(x+y)"_

    string expression(QueryUtils::GetExpression(expr));

	log.append("\n");
	log.append("lhs: " + lhsVar);
	log.append("\n");
	log.append("expr: " + expression);

	lhsVar = QueryUtils::LiteralToCandidate(lhsVar);
	Candidate lhs = PKB::GetVariableIndex(lhsVar);
	if (lhs == 0) {
		return false;
	}

	else if (Utils::IsUnderscore(expression)) {
		// stmtNo can be type assign/if/while
		Symbol stmtSymbol = PKB::GetStmtSymbol(stmtNo);
		if ((stmtSymbol == ASSIGN)) {
			return PKB::IsModifies(stmtNo, lhs);
		}
		else {
			return PKB::HasControlVariableIndexAtStmtNumber(stmtNo, lhs);
		}

	}

	else if (Utils::StartsWith(expression, CHAR_SYMBOL_UNDERSCORE)) {
        return PKB::IsSubPattern(stmtNo, lhs, QueryUtils::GetSubExpression(expression));
    }

    else {
        return PKB::IsExactPattern(stmtNo, lhs, expression);
    }
}

bool QueryEvaluator::evaluateOneVarPattern(Candidate stmtNo, Candidate lhs, std::string expr)
{
	string expression(QueryUtils::GetExpression(expr));
	if (Utils::IsUnderscore(expression)) {
		// stmtNo can be type assign/if/while
		Symbol stmtSymbol = PKB::GetStmtSymbol(stmtNo);
		if ((stmtSymbol == ASSIGN)) {
			return PKB::IsModifies(stmtNo, lhs);
		}
		else {
			return PKB::HasControlVariableIndexAtStmtNumber(stmtNo, lhs);
		}
	}
	else if (Utils::StartsWith(expression, CHAR_SYMBOL_UNDERSCORE)) {
		return PKB::IsSubPattern(stmtNo, lhs, QueryUtils::GetSubExpression(expression));
	}
	else {
		return PKB::IsExactPattern(stmtNo, lhs, expression);
	}
}

bool QueryEvaluator::evaluateUnderscorePattern(Candidate stmtNo, std::string expr)
{
	string expression(QueryUtils::GetExpression(expr));
	if (Utils::IsUnderscore(expression)) {
		// stmtNo can be type assign/if/while
		return true;
	}
	else if (Utils::StartsWith(expression, CHAR_SYMBOL_UNDERSCORE)) {
		return PKB::IsSubExpression(stmtNo, QueryUtils::GetSubExpression(expression));
	}
	else {
		return PKB::IsExactExpression(stmtNo, expression);
	}
}


bool QueryEvaluator::evaluateSuchThatClause(string clauseType, string var0, string var1)
{
	Candidate v0, v1;
	if (clauseType == SYMBOL_USES) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfUsesRelationship() > 0);
			}
			else if (Utils::IsNonNegativeNumeric(var0)) {
				v0 = Utils::StringToInt(var0);
				return (!PKB::GetUsedVariables(v0).empty());
			}
			else {
				v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
				if (v0 == 0)  return false;
				else return (!PKB::GetProcedureUsedVariables(v0).empty());
			}
		}
		else {
			v1 = PKB::GetVariableIndex(QueryUtils::LiteralToCandidate(var1));
			if (v1 == 0) return false;
			else if (Utils::IsUnderscore(var0)) {
				return (!PKB::GetStmtNumberUsing(v1).empty());
			}
			else if (Utils::IsNonNegativeNumeric(var0)) {
				v0 = Utils::StringToInt(var0);
				return PKB::IsUses(v0, v1);
			}
			else {
				v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
				if (v0 == 0)  return false;
				else return PKB::IsUsesProcedure(v0, v1);
			}
		}
	}
	else if (clauseType == SYMBOL_MODIFIES) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfModifiesRelationship() > 0);
			}
			else if (Utils::IsNonNegativeNumeric(var0)) {
				v0 = Utils::StringToInt(var0);
				return (!PKB::GetModifiedVariables(v0).empty());
			}
			else {
				v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
				if (v0 == 0)  return false;
				else return (!PKB::GetProcedureModifiedVariables(v0).empty());
			}
		}
		else {
			v1 = PKB::GetVariableIndex(QueryUtils::LiteralToCandidate(var1));
			if (v1 == 0) return false;
			else if (Utils::IsUnderscore(var0)) {
				return (!PKB::GetStmtNumberModifying(v1).empty());
			}
			else if (Utils::IsNonNegativeNumeric(var0)) {
				v0 = Utils::StringToInt(var0);
				return PKB::IsModifies(v0, v1);
			}
			else {
				v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
				if (v0 == 0)  return false;
				else return PKB::IsModifiesProcedure(v0, v1);
			}
		}
	}
	else if (clauseType == SYMBOL_PARENT) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfParentRelationship() > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return (!PKB::GetChildren(v0).empty());
			}
		}
		else {
			v1 = Utils::StringToInt(var1);
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetParent(v1) > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return PKB::IsParent(v0, v1);
			}
		}
	}
	else if (clauseType == SYMBOL_PARENT_TRANSITIVE) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfParentRelationship() > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return (!PKB::GetChildren(v0).empty());
			}
		}
		else {
			v1 = Utils::StringToInt(var1);
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetParent(v1) > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return PKB::IsParentTransitive(v0, v1);
			}
		}
	}
	else if (clauseType == SYMBOL_FOLLOWS) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfFollowsRelationship() > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return (PKB::GetFollowing(v0) > 0);
			}
		}
		else {
			v1 = Utils::StringToInt(var1);
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetFollows(v1) > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return PKB::IsFollows(v0, v1);
			}
		}
	}
	else if (clauseType == SYMBOL_FOLLOWS_TRANSITIVE) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfFollowsRelationship() > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return (PKB::GetFollowing(v0) > 0);
			}
		}
		else {
			v1 = Utils::StringToInt(var1);
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetFollows(v1) > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return PKB::IsFollowsTransitive(v0, v1);
			}
		}
	}
	else if (clauseType == (SYMBOL_NEXT)) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfNextRelationship() > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return (!PKB::GetNext(v0).empty());
			}
		}
		else {
			v1 = Utils::StringToInt(var1);
			if (Utils::IsUnderscore(var0)) {
				return (!PKB::GetPrevious(v1).empty());
			}
			else {
				v0 = Utils::StringToInt(var0);
				return PKB::IsNext(v0, v1);
			}
		}
	}
	else if (clauseType == (SYMBOL_NEXT_TRANSITIVE)) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfNextRelationship() > 0);
			}
			else {
				v0 = Utils::StringToInt(var0);
				return (!PKB::GetNext(v0).empty());
			}
		}
		else {
			v1 = Utils::StringToInt(var1);
			if (Utils::IsUnderscore(var0)) {
				return (!PKB::GetPrevious(v1).empty());
			}
			else {
				v0 = Utils::StringToInt(var0);
				return PKB::IsNextTransitive(v0, v1);
			}
		}
	}
	else if (clauseType == (SYMBOL_CALLS)) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfCallsRelationship() > 0);
			}
			else {
				v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
				if (v0 == 0) return false;
				else return (!PKB::GetCalled(v0).empty());
			}
		}
		else {
			v1 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var1));
			if (v1 == 0) return false;
			else if (Utils::IsUnderscore(var0)) {
				return (!PKB::GetCalling(v1).empty());
			}
			else {
				v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
				if (v0 == 0) return false;
				return PKB::IsCalls(v0, v1);
			}
		}
	}
	else if (clauseType == (SYMBOL_CALLS_TRANSITIVE)) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::GetNumberOfCallsRelationship() > 0);
			}
			else {
				v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
				if (v0 == 0) return false;
				else return (!PKB::GetCalled(v0).empty());
			}
		}
		else {
			v1 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var1));
			if (v1 == 0) return false;
			else if (Utils::IsUnderscore(var0)) {
				return (!PKB::GetCalling(v1).empty());
			}
			else {
				v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
				if (v0 == 0) return false;
				return PKB::IsCallsTransitive(v0, v1);
			}
		}
	}
	else if (clauseType == (SYMBOL_AFFECTS)) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::IsAffects());
			}
			else {
				v0 = Utils::StringToInt(var0);
				return (!PKB::GetAffected(v0).empty());
			}
		}
		else {
			v1 = Utils::StringToInt(var1);
			if (v1 == 0) return false;
			else if (Utils::IsUnderscore(var0)) {
				return (!PKB::GetAffecting(v1).empty());
			}
			else {
				v0 = Utils::StringToInt(var0);
				return PKB::IsAffects(v0, v1);
			}
		}
	}
	else if (clauseType == (SYMBOL_AFFECTS_TRANSITIVE)) {
		if (Utils::IsUnderscore(var1)) {
			if (Utils::IsUnderscore(var0)) {
				return (PKB::IsAffects());
			}
			else {
				v0 = Utils::StringToInt(var0);
				return (!PKB::GetAffected(v0).empty());
			}
		}
		else {
			v1 = Utils::StringToInt(var1);
			if (v1 == 0) return false;
			else if (Utils::IsUnderscore(var0)) {
				return (!PKB::GetAffecting(v1).empty());
			}
			else {
				v0 = Utils::StringToInt(var0);
				return PKB::IsAffectsTransitive(v0, v1);
			}
		}
	}

	return false;
}

bool QueryEvaluator::evaluateSuchThatClause(std::string clauseType, Candidate v0, std::string var1)
{
	Candidate v1;
	if (clauseType == SYMBOL_USES) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetUsedVariables(v0).empty());
		}
		else {
			v1 = PKB::GetVariableIndex(QueryUtils::LiteralToCandidate(var1));
			if (v1 == 0) return false;
			else return PKB::IsUses(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_MODIFIES) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetModifiedVariables(v0).empty());
		}
		else {
			v1 = PKB::GetVariableIndex(QueryUtils::LiteralToCandidate(var1));
			if (v1 == 0) return false;
			else return PKB::IsModifies(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_PARENT) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetChildren(v0).empty());
		}
		else {
			v1 = Utils::StringToInt(var1);
			return PKB::IsParent(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_PARENT_TRANSITIVE) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetChildren(v0).empty());
		}
		else {
			v1 = Utils::StringToInt(var1);
			return PKB::IsParentTransitive(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_FOLLOWS) {
		if (Utils::IsUnderscore(var1)) {
			return (PKB::GetFollowing(v0) > 0);
		}
		else {
			v1 = Utils::StringToInt(var1);
			return PKB::IsFollows(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_FOLLOWS_TRANSITIVE) {
		if (Utils::IsUnderscore(var1)) {
			return (PKB::GetFollowing(v0) > 0);
		}
		else {
			v1 = Utils::StringToInt(var1);
			return PKB::IsFollowsTransitive(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_NEXT)) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetNext(v0).empty());
		}
		else {
			v1 = Utils::StringToInt(var1);
			return PKB::IsNext(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_NEXT_TRANSITIVE)) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetNext(v0).empty());
		}
		else {
			v1 = Utils::StringToInt(var1);
			return PKB::IsNextTransitive(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_CALLS)) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetCalled(v0).empty());
		}
		else {
			v1 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var1));
			if (v1 == 0) return false;
			else return PKB::IsCalls(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_CALLS_TRANSITIVE)) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetCalled(v0).empty());
		}
		else {
			v1 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var1));
			if (v1 == 0) return false;
			else return PKB::IsCallsTransitive(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_AFFECTS)) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetAffected(v0).empty());
		}
		else {
			v1 = Utils::StringToInt(var1);
			return PKB::IsAffects(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_AFFECTS_TRANSITIVE)) {
		if (Utils::IsUnderscore(var1)) {
			return (!PKB::GetAffected(v0).empty());
		}
		else {
			v1 = Utils::StringToInt(var1);
			return PKB::IsAffectsTransitive(v0, v1);
		}
	}

	return false;
}

bool QueryEvaluator::evaluateSuchThatClause(std::string clauseType, std::string var0, Candidate v1)
{
	Candidate v0;
	if (clauseType == SYMBOL_USES) {
		if (Utils::IsUnderscore(var0)) {
			return (!PKB::GetStmtNumberUsing(v1).empty());
		}
		else if (Utils::IsNonNegativeNumeric(var0)) {
			v0 = Utils::StringToInt(var0);
			return PKB::IsUses(v0, v1);
		}
		else {
			v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
			if (v0 == 0) return false;
			else return PKB::IsUsesProcedure(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_MODIFIES) {
		if (Utils::IsUnderscore(var0)) {
			return (!PKB::GetStmtNumberModifying(v1).empty());
		}
		else if (Utils::IsNonNegativeNumeric(var0)) {
			v0 = Utils::StringToInt(var0);
			return PKB::IsModifies(v0, v1);
		}
		else {
			v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
			if (v0 == 0) return false;
			else return PKB::IsModifiesProcedure(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_PARENT) {
		if (Utils::IsUnderscore(var0)) {
			return (PKB::GetParent(v1) > 0);
		}
		else {
			v0 = Utils::StringToInt(var0);
			return PKB::IsParent(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_PARENT_TRANSITIVE) {
		if (Utils::IsUnderscore(var0)) {
			return (PKB::GetParent(v1) > 0);
		}
		else {
			v0 = Utils::StringToInt(var0);
			return PKB::IsParentTransitive(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_FOLLOWS) {
		if (Utils::IsUnderscore(var0)) {
			return (PKB::GetFollows(v1) > 0);
		}
		else {
			v0 = Utils::StringToInt(var0);
			return PKB::IsFollows(v0, v1);
		}
	}
	else if (clauseType == SYMBOL_FOLLOWS_TRANSITIVE) {
		if (Utils::IsUnderscore(var0)) {
			return (PKB::GetFollows(v1) > 0);
		}
		else {
			v0 = Utils::StringToInt(var0);
			return PKB::IsFollowsTransitive(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_NEXT)) {
		if (Utils::IsUnderscore(var0)) {
			return (!PKB::GetPrevious(v1).empty());
		}
		else {
			v0 = Utils::StringToInt(var0);
			return PKB::IsNext(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_NEXT_TRANSITIVE)) {
		if (Utils::IsUnderscore(var0)) {
			return (!PKB::GetPrevious(v1).empty());
		}
		else {
			v0 = Utils::StringToInt(var0);
			return PKB::IsNextTransitive(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_CALLS)) {
		if (Utils::IsUnderscore(var0)) {
			return (!PKB::GetCalling(v1).empty());
		}
		else {
			v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
			if (v0 == 0) return false;
			return PKB::IsCalls(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_CALLS_TRANSITIVE)) {
		if (Utils::IsUnderscore(var0)) {
			return (!PKB::GetCalling(v1).empty());
		}
		else {
			v0 = PKB::GetProcedureIndex(QueryUtils::LiteralToCandidate(var0));
			if (v0 == 0) return false;
			return PKB::IsCallsTransitive(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_AFFECTS)) {
		if (Utils::IsUnderscore(var0)) {
			return (!PKB::GetAffecting(v1).empty());
		}
		else {
			v0 = Utils::StringToInt(var0);
			return PKB::IsAffects(v0, v1);
		}
	}
	else if (clauseType == (SYMBOL_AFFECTS_TRANSITIVE)) {
		if (Utils::IsUnderscore(var0)) {
			return (!PKB::GetAffecting(v1).empty());
		}
		else {
			v0 = Utils::StringToInt(var0);
			return PKB::IsAffectsTransitive(v0, v1);
		}
	}

	return false;
}

bool QueryEvaluator::evaluateSuchThatClause(std::string clauseType, Candidate v0, Candidate v1)
{
	if (clauseType == SYMBOL_USES) {
		return PKB::IsUses(v0, v1);
	}
	else if (clauseType == SYMBOL_MODIFIES) {
		return PKB::IsModifies(v0, v1);
	}
	else if (clauseType == SYMBOL_PARENT) {
		return PKB::IsParent(v0, v1);
	}
	else if (clauseType == SYMBOL_PARENT_TRANSITIVE) {
		return PKB::IsParentTransitive(v0, v1);
	}
	else if (clauseType == SYMBOL_FOLLOWS) {
		return PKB::IsFollows(v0, v1);
	}
	else if (clauseType == SYMBOL_FOLLOWS_TRANSITIVE) {
		return PKB::IsFollowsTransitive(v0, v1);
	}
	else if (clauseType == (SYMBOL_NEXT)) {
		return PKB::IsNext(v0, v1);
	}
	else if (clauseType == (SYMBOL_NEXT_TRANSITIVE)) {
		return PKB::IsNextTransitive(v0, v1);
	}
	else if (clauseType == (SYMBOL_CALLS)) {
		return PKB::IsCalls(v0, v1);
	}
	else if (clauseType == (SYMBOL_CALLS_TRANSITIVE)) {
		return PKB::IsCallsTransitive(v0, v1);
	}
	else if (clauseType == (SYMBOL_AFFECTS)) {
		return PKB::IsAffects(v0, v1);
	}
	else if (clauseType == (SYMBOL_AFFECTS_TRANSITIVE)) {
		return PKB::IsAffectsTransitive(v0, v1);
	}

	return false;
}


bool QueryEvaluator::evaluateUses(Candidate stmtNo, Candidate var) {
    if (stmtNo == INT_UNDERSCORE) {
		if (var == INT_UNDERSCORE) {
			return (PKB::GetNumberOfUsesRelationship() > 0);
		}
		else {
			return (!PKB::GetStmtNumberUsing(var).empty());
		}
    }
	else  {
		if (var == INT_UNDERSCORE) {
			return (!PKB::GetUsedVariables(stmtNo).empty());
		}
		else {
			return PKB::IsUses(stmtNo, var);
		}
    }
}

bool QueryEvaluator::evaluateUsesProcedure(Candidate proc, Candidate var) {
	if (proc == INT_UNDERSCORE) {
		if (var == INT_UNDERSCORE) {
			return (PKB::GetNumberOfUsesProcedureRelationship() > 0);
		}
		else {
			return (!PKB::GetProceduresNameUsing(var).empty());
		}
	}
	else {
		if (var == INT_UNDERSCORE) {
			return (!PKB::GetProcedureUsedVariables(proc).empty());
		}
		else {
			return PKB::IsUsesProcedure(proc, var);
		}
	}
}


bool QueryEvaluator::evaluateModifies(Candidate stmtNo, Candidate var) {
	if (stmtNo == INT_UNDERSCORE) {
		if (var == INT_UNDERSCORE) {
			return (PKB::GetNumberOfAssign() > 0);
		}
		else {
			return (!PKB::GetStmtNumberModifying(var).empty());
		}
	}
	else {
        if (var == INT_UNDERSCORE) {
            return (!PKB::GetModifiedVariables(stmtNo).empty());
        }
		else {
			return PKB::IsModifies(stmtNo, var);
		}
    }
}

bool QueryEvaluator::evaluateModifiesProcedure(Candidate proc, Candidate var) {
	if (proc == INT_UNDERSCORE) {
		if (var == INT_UNDERSCORE) {
			return (PKB::GetNumberOfAssign() > 0);
		}
		else {
			return (!PKB::GetProceduresNameModifying(var).empty());
		}
	}
	else {
		if (var == INT_UNDERSCORE) {
			return (!PKB::GetProcedureModifiedVariables(proc).empty());
		}
		else {
			return PKB::IsModifiesProcedure(proc, var);
		}
	}
}


bool QueryEvaluator::evaluateParent(Candidate stmt1, Candidate stmt2) {
    if (stmt1 == INT_UNDERSCORE) {
        if (stmt2 == INT_UNDERSCORE) {
            return (PKB::GetNumberOfContainerStmt() > 0);
        } else {
            return (!PKB::GetParentsTransitive(stmt2).empty());
        }
    }
	else {
        if (stmt2 == INT_UNDERSCORE) {
            return (!PKB::GetChildrenTransitive(stmt1).empty());
        }
		else {
            return PKB::IsParent(stmt1, stmt2);
        }
    }
}


bool QueryEvaluator::evaluateParentStar(Candidate stmt1, Candidate stmt2) {
    if (stmt1 == INT_UNDERSCORE) {
        if (stmt2 == INT_UNDERSCORE) {
            return (PKB::GetNumberOfContainerStmt() > 0);
        } else {
            return (PKB::GetParent(stmt2) > 0);
        }
    }
	else {
        if (stmt2 == INT_UNDERSCORE) {
            return (!PKB::GetChildren(stmt1).empty());
        }
		else {
            return PKB::IsParentTransitive(stmt1, stmt2);
        }
    }
}


bool QueryEvaluator::evaluateFollows(Candidate stmt1, Candidate stmt2) {
    if (stmt1 == INT_UNDERSCORE) {
        if (stmt2 == INT_UNDERSCORE) {
			return (PKB::GetNumberOfFollowsRelationship() > 0);
        } else {
            return (PKB::GetFollows(stmt2) > 0);
        }
    }
	else {
        if (stmt2 == INT_UNDERSCORE) {
            return (PKB::GetFollowing(stmt1) > 0);
        }
		else {
            return PKB::IsFollows(stmt1, stmt2);
        }
    }
}


bool QueryEvaluator::evaluateFollowsStar(Candidate stmt1, Candidate stmt2) {
    if (stmt1 == INT_UNDERSCORE) {
        if (stmt2 == INT_UNDERSCORE) {
			return (PKB::GetNumberOfFollowsRelationship() > 0);
        }
		else {
            return (PKB::GetFollows(stmt2) > 0);
        }
    }
	else {
        if (stmt2 == INT_UNDERSCORE) {
            return (PKB::GetFollowing(stmt1) > 0);
        }
		else {
            return PKB::IsFollowsTransitive(stmt1, stmt2);
        }
    }
}


bool QueryEvaluator::evaluateNext(Candidate stmt1, Candidate stmt2) {
	if (stmt1 == INT_UNDERSCORE) {
		if (stmt2 == INT_UNDERSCORE) {
			return (PKB::GetNumberOfNextRelationship() > 0);
		}
		else {
			return (!PKB::GetPrevious(stmt2).empty());
		}
	}
	else {
		if (stmt2 == INT_UNDERSCORE) {
			return (!PKB::GetNext(stmt1).empty());
		}
		else {
			return PKB::IsNext(stmt1, stmt2);
		}
	}
}


bool QueryEvaluator::evaluateNextStar(Candidate stmt1, Candidate stmt2) {
	if (stmt1 == INT_UNDERSCORE) {
		if (stmt2 == INT_UNDERSCORE) {
			return (PKB::GetNumberOfNextRelationship() > 0);
		}
		else {
			return (!PKB::GetPrevious(stmt2).empty());
		}
	}
	else {
		if (stmt2 == INT_UNDERSCORE) {
			return (!PKB::GetNext(stmt1).empty());
		}
		else {
			return PKB::IsNextTransitive(stmt1, stmt2);
		}
	}
}


bool QueryEvaluator::evaluateCalls(Candidate proc1, Candidate proc2) {
	if (proc1 == INT_UNDERSCORE) {
		if (proc2 == INT_UNDERSCORE) {
			return (PKB::GetNumberOfCallsRelationship() > 0);
		}
		else {
			return (!PKB::GetCalling(proc2).empty());
		}
	}
	else {
		if (proc2 == INT_UNDERSCORE) {
			return (!PKB::GetCalled(proc1).empty());
		}
		else {
			return PKB::IsCalls(proc1, proc2);
		}
	}
}


bool QueryEvaluator::evaluateCallsStar(Candidate proc1, Candidate proc2) {
	if (proc1 == INT_UNDERSCORE) {
		if (proc2 == INT_UNDERSCORE) {
			return (PKB::GetNumberOfCallsRelationship() > 0);
		}
		else {
			return (!PKB::GetCalling(proc2).empty());
		}
	}
	else {
		if (proc2 == INT_UNDERSCORE) {
			return (!PKB::GetCalled(proc1).empty());
		}
		else {
			return PKB::IsCallsTransitive(proc1, proc2);
		}
	}
}


bool QueryEvaluator::evaluateAffects(Candidate assign1, Candidate assign2) {
    if (assign1 == INT_UNDERSCORE) {
        if (assign2 == INT_UNDERSCORE) {
            return PKB::IsAffects();
        }
        else {
            return (!PKB::GetAffecting(assign2).empty());
        }
    }
    else {
        if (assign2 == INT_UNDERSCORE) {
            return (!PKB::GetAffected(assign1).empty());
        }
        else {
            return PKB::IsAffects(assign1, assign2);
        }
    }
}


bool QueryEvaluator::evaluateAffectsStar(Candidate assign1, Candidate assign2) {
    if (assign1 == INT_UNDERSCORE) {
        if (assign2 == INT_UNDERSCORE) {
            return PKB::IsAffects();
        }
        else {
            return (!PKB::GetAffecting(assign2).empty());
        }
    }
    else {
        if (assign2 == INT_UNDERSCORE) {
            return (!PKB::GetAffected(assign1).empty());
        }
        else {
            return PKB::IsAffectsTransitive(assign1, assign2);
        }
    }
}

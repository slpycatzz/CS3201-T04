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

using std::string;
using std::unordered_map;
using std::vector;

QueryEvaluator::QueryEvaluator() {}

QueryEvaluator::~QueryEvaluator() {}

vector<Candidate> QueryEvaluator::getCandidates(Symbol &synType) {
    switch (synType) {
        case VARIABLE:
            return PKB::GetAllVariableNames();
        case PROCEDURE:
            return PKB::GetAllProcedureNames();
        case BOOLEAN:
            return vector<Candidate>{ SYMBOL_TRUE };
        case CONSTANT:
            return PKB::GetAllConstantValues();
		case PROGRAM_LINE:
			return Utils::IntsToStrings(PKB::GetSymbolStmtNumbers(STMT));
		case STMTLIST:
			return Utils::IntsToStrings(PKB::GetAllStmtlistsStmtNumber());
		case CALL:
        case STMT:
        case ASSIGN:
        case IF:
        case WHILE:
            return Utils::IntsToStrings(PKB::GetSymbolStmtNumbers(synType));
        default:
            return vector<Candidate>();
    }
}

TotalCombinationList QueryEvaluator::getTotalCandidateList
(unordered_map<string, Symbol> &varMap, vector<Synonym> &synList) {
    TotalCombinationList totalCandLst;
    for (Synonym &syn : synList) {
        vector<Candidate> candLst(getCandidates(varMap[syn]));
        totalCandLst.addSynonym(syn, candLst);
    }

    return totalCandLst;
}

ResultList QueryEvaluator::selectQueryResults(QueryTree &query) {
    vector<Synonym> selectList(query.getResults());
    TotalCombinationList queryResults(getQueryResults(query));
    if (isBoolSelect(selectList)) {
		string resultBoolean = (queryResults.getFactorCount() > 0) ? SYMBOL_TRUE : SYMBOL_FALSE;

        vector<vector<string>> tupleList;
        tupleList.push_back(vector<string>{resultBoolean});
        ResultList resultList{ selectList, tupleList };

        return resultList;

    } else {
        if (queryResults.isEmpty() || queryResults.getFactorCount() == 0) {
            ResultList resultList{ selectList, vector<vector<string>>() };
            return resultList;

        } else {
            ResultList resultList(getResultsFromCombinationList(queryResults, selectList));
            return resultList;
        }
    }
}

ResultList QueryEvaluator::getResultsFromCombinationList
(TotalCombinationList &combinations, vector<Synonym> &selectList) {
	PartialCombinationList
		selectedCombinations(combinations.getCombinationList(selectList));
	vector<vector<Candidate>> selectedCombinationList;

	for (CandidateCombination combi : selectedCombinations) {
		vector<Candidate> candidateTuple;

		for (Synonym syn : selectList) {
			candidateTuple.push_back(combi[syn]);
		}

		selectedCombinationList.push_back(candidateTuple);
	}

	ResultList result{ selectList, selectedCombinationList };

	return result;
}

bool QueryEvaluator::isBoolSelect(vector<string> &selectList) {
	if (selectList.size() == 1 && selectList[0] == SYMBOL_BOOLEAN) {
		return true;
	}
	else {
		return false;
	}
}

TotalCombinationList QueryEvaluator::getQueryResults(QueryTree &query) {
    if (!getBooleanGroupResult(query.getBooleanClauses())) {
        return TotalCombinationList();

    } else {
        vector<std::pair<vector<Synonym>, vector<Clause>>> unselectedGroups(query.getUnselectedGroups());
        unordered_map<Synonym, Symbol> varMap(query.getVarMap());

        for (auto &pair : unselectedGroups) {
            if (!getUnselectedGroupResult(pair.first, varMap, pair.second)) {
                return TotalCombinationList();
            }
        }

        TotalCombinationList result;
        vector<Synonym> selectList(query.getResults());
        vector<std::pair<vector<Synonym>, vector<Clause>>> selectedGroups(query.getSelectedGroups());

        for (auto &pair : selectedGroups) {
            vector<Synonym> &synList(pair.first);
            vector<Clause> &group(pair.second);
            TotalCombinationList &tempCombiList(getSelectedGroupResult(synList, varMap, group, selectList));
			if (tempCombiList.isEmpty() || tempCombiList.getFactorCount() == 0) {
				return TotalCombinationList();
			}
            result.combine(tempCombiList);

            for (Synonym &syn : synList) {
                vector<Synonym>::iterator it(selectList.begin());
                while (it != selectList.end()) {
                    if (syn == (*it)) {
                        it = selectList.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }
		if (result.isEmpty()) {
			return TotalCombinationList();
		}
        for (Synonym &syn : selectList) {
            vector<Candidate> candList(getCandidates(varMap[syn]));
            result.addSynonym(syn, candList);
        }
        return result;
    }
}

bool QueryEvaluator::getBooleanGroupResult(vector<Clause> &clauseGroup) {
    for (Clause clause : clauseGroup) {
        
		Candidate arg0(QueryUtils::LiteralToCandidate(clause.getArg()[0]));
        Candidate arg1(QueryUtils::LiteralToCandidate(clause.getArg()[1]));
        string clauseType(clause.getClauseType());
		
		if (clauseType == SYMBOL_WITH) {
			if (arg0 != arg1) {
				return false;
			}
		}
        else if (!evaluateSuchThatClause(clauseType, arg0, arg1)) {
            return false;
        }
    }

    return true;
}

bool QueryEvaluator::getUnselectedGroupResult
(vector<Synonym> &synList,
    unordered_map<Synonym, Symbol> &varMap,
    vector<Clause> &clauseGroup) {
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
(vector<Synonym> &synList,
    unordered_map<Synonym, Symbol> &varMap,
    vector<Clause> &clauseGroup,
    vector<Synonym> &selectList) {
    TotalCombinationList combinations(getTotalCandidateList(varMap, synList));

	/*LOG - DELETE AFTER DEBUGGING*/
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

        filterByClause(clause, combinations, varMap);

        if (combinations.isEmpty()) {
            return TotalCombinationList();
        }
    }

    combinations.reduceTotalContent(selectList);

    return combinations;
}

void QueryEvaluator::filterByClause(Clause &clause,
    TotalCombinationList &combinations, unordered_map<Synonym, Symbol> &varMap) {
    string clauseType(clause.getClauseType());


	/*LOG - DELETE AFTER DEBUGGING*/
	log.append("\n");
	log.append("clause type: " + clause.getClauseType() + "\n");
	log.append("args: " + clause.getArg()[0] + " " + clause.getArg()[1]);

    if (clauseType == SYMBOL_PATTERN) {
        vector<Synonym> args(clause.getArg());
        Synonym lhs(args[1]), rhs(args[2]), assignStmt(args[0]);

        if (QueryUtils::IsStringLiteral(lhs)) {
            filterNoVarPattern(assignStmt, QueryUtils::LiteralToCandidate(lhs), rhs, combinations);
        } else {
            filterOneVarPattern(assignStmt, lhs, rhs, combinations);
        }

    }
	
	else if (clauseType == SYMBOL_WITH) {
        vector<Synonym> args(clause.getArg());
        Synonym var0(args[0]), var1(args[1]);

		if (args[2] == SYMBOL_VARIABLE) {
			if (QueryUtils::IsStringLiteral(var0)) {
				var0 = QueryUtils::LiteralToCandidate(var0);
				if (QueryUtils::IsStringLiteral(var1)) {
					var1 = QueryUtils::LiteralToCandidate(var1);
					filterNoVarWith(var0, var1, combinations);
				}
				else if (varMap[var1] == CALL) {
					filterNoVarCallWith(var1, var0, combinations);
				}
				else {
					filterSecondVarWith(var0, var1, combinations);
				}
			}
			else if (varMap[var0] == CALL) {
				if (QueryUtils::IsStringLiteral(var1)) {
					var1 = QueryUtils::LiteralToCandidate(var1);
					filterNoVarCallWith(var0, var1, combinations);
				}
				else if (varMap[var1] == CALL) {
					filterTwoVarsCallWith(var0, var1, combinations);
				}
				else {
					filterOneVarCallWith(var0, var1, combinations);
				}
			}
			else {
				if (QueryUtils::IsStringLiteral(var1)) {
					var1 = QueryUtils::LiteralToCandidate(var1);
					filterFirstVarWith(var0, var1, combinations);
				}
				else if (varMap[var1] == CALL) {
					filterOneVarCallWith(var1, var0, combinations);
				}
				else {
					filterTwoVarsWith(var0, var1, combinations);
				}
			}
		}
		else if (args[2] == SYMBOL_CONSTANT) {
			if (Utils::IsNonNegativeNumeric(var0)) {
				var0 = QueryUtils::LiteralToCandidate(var0);
				if (Utils::IsNonNegativeNumeric(var1)) {
					filterNoVarWith(var0, QueryUtils::LiteralToCandidate(var1), combinations);
				}
				else {
					filterSecondVarWith(var0, var1, combinations);
				}

			}
			else if (Utils::IsNonNegativeNumeric(var1)) {
				var1 = QueryUtils::LiteralToCandidate(var1);
				filterFirstVarWith(var0, var1, combinations);
			}
			else {
				filterTwoVarsWith(var0, var1, combinations);
			}
		}
    }
	
	else {
        vector<Synonym> args(clause.getArg());
        Synonym var0(args[0]), var1(args[1]);
        if (QueryUtils::IsLiteral(var0)) {
            if (QueryUtils::IsLiteral(var1)) {
                filterNoVarClause(clauseType, QueryUtils::LiteralToCandidate(var0), QueryUtils::LiteralToCandidate(var1), combinations);
            } else {
                filterSecondVarClause(clauseType, QueryUtils::LiteralToCandidate(var0), var1, combinations);
            }

        } else {
            if (QueryUtils::IsLiteral(var1)) {
                filterFirstVarClause(clauseType, var0, QueryUtils::LiteralToCandidate(var1), combinations);
            } else {
                filterTwoVarsClause(clauseType, var0, var1, combinations);
            }
        }
    }


	/*LOG - DELETE AFTER DEBUGGING*/
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
}

void QueryEvaluator::filterNoVarPattern(Synonym assignStmt, Candidate lhs,
    Candidate expr, TotalCombinationList &combinations) {
    auto matchPattern = [=](CandidateCombination &combi) -> bool {
        return evaluatePatternClause(combi[assignStmt], lhs, expr);
    };
    combinations.filter(assignStmt, matchPattern);
}

void QueryEvaluator::filterOneVarPattern(Synonym assignStmt, Synonym lhs,
    Candidate expr, TotalCombinationList &combinations) {
    auto matchPattern = [=](CandidateCombination combi) -> bool {
        return evaluatePatternClause(combi[assignStmt], combi[lhs], expr);
    };
    combinations.mergeAndFilter(assignStmt, lhs, matchPattern);
}

void QueryEvaluator::filterTwoVarsClause(string clauseType,
    Synonym &var0, Synonym &var1, TotalCombinationList &combinations) {
    auto evaluateClause = [=](CandidateCombination combi) -> bool {
        return evaluateSuchThatClause(clauseType, combi[var0], combi[var1]);
    };
    combinations.mergeAndFilter(var0, var1, evaluateClause);
}

void QueryEvaluator::filterFirstVarClause(string clauseType,
    Synonym var, Candidate constant, TotalCombinationList &combinations) {
    auto evaluateClause = [=](CandidateCombination combi) -> bool {
        return evaluateSuchThatClause(clauseType, combi[var], constant);
    };
    combinations.filter(var, evaluateClause);
}

void QueryEvaluator::filterSecondVarClause(string clauseType,
    Candidate constant, Synonym var, TotalCombinationList &combinations) {
    auto evaluateClause = [=](CandidateCombination combi) -> bool {
        return evaluateSuchThatClause(clauseType, constant, combi[var]);
    };
    combinations.filter(var, evaluateClause);
}

void QueryEvaluator::filterNoVarClause(string clauseType,
    Candidate const1, Candidate const2, TotalCombinationList &combinations) {
    combinations.filter(evaluateSuchThatClause(clauseType, const1, const2));
}

void QueryEvaluator::filterTwoVarsWith(Synonym &var0, Synonym &var1, TotalCombinationList &combinations) {
    auto evaluateClause = [=](CandidateCombination combi) -> bool {
        return (combi[var0] == combi[var1]);
    };
    combinations.mergeAndFilter(var0, var1, evaluateClause);
}

void QueryEvaluator::filterFirstVarWith(Synonym &var, Candidate constant, TotalCombinationList &combinations) {
    auto evaluateClause = [=](CandidateCombination combi) -> bool {
        return (combi[var] == constant);
    };
    combinations.filter(var, evaluateClause);
}

void QueryEvaluator::filterSecondVarWith(Candidate constant, Synonym &var, TotalCombinationList &combinations) {
    auto evaluateClause = [=](CandidateCombination combi) -> bool {
        return (combi[var] == constant);
    };
    combinations.filter(var, evaluateClause);
}

void QueryEvaluator::filterNoVarWith(Candidate const1, Candidate const2, TotalCombinationList &combinations) {
    combinations.filter(const1 == const2);
}

void QueryEvaluator::filterTwoVarsCallWith(Synonym & call0, Synonym & call1, TotalCombinationList & combinations) {
	auto comp = [=](CandidateCombination combi) -> bool {
		return (PKB::GetCallStmtProcedureName(Utils::StringToInt(combi[call0]), "")
			== (PKB::GetCallStmtProcedureName(Utils::StringToInt(combi[call1]), "")));
	};
	combinations.mergeAndFilter(call0, call1, comp);
}

void QueryEvaluator::filterOneVarCallWith(Synonym & call, Synonym & var, TotalCombinationList & combinations) {
    auto comp = [=](CandidateCombination combi) -> bool {
        return (PKB::GetCallStmtProcedureName(Utils::StringToInt(combi[call]), "") == combi[var]);
    };
    combinations.mergeAndFilter(call, var, comp);
}

void QueryEvaluator::filterNoVarCallWith(Synonym & call, Candidate cand, TotalCombinationList & combinations) {
    auto comp = [=](CandidateCombination combi) -> bool {
        return (PKB::GetCallStmtProcedureName(Utils::StringToInt(combi[call]), "") == cand);
    };
    combinations.filter(call, comp);
}

bool QueryEvaluator::evaluateClause(Clause &clause, CandidateCombination &comb) {
    string type(clause.getClauseType());
    vector<string> args(clause.getArg());

    if (type == SYMBOL_PATTERN) {
        Candidate assignStmt(comb[args[2]]);
        Candidate lhsVar(comb[args[0]]);
        Candidate expr(comb[args[1]]);

        return evaluatePatternClause(assignStmt, lhsVar, expr);

    } else {
        Candidate var0(comb[args[0]]);
        Candidate var1(comb[args[1]]);

        return evaluateSuchThatClause(type, var0, var1);
    }
}

bool QueryEvaluator::evaluatePatternClause(Candidate stmt,
    Candidate lhsVar, string expr) {
    // Preprocessor will do the following:
    // expression _"x+y"_ should convert to a string of vectors <x, +, y>
    // preprocessor will get (x+y) after calling Utils::GetPostfixExpression(<x, +, y>)
    // in this method, expr = _"(x+y)"_

    unsigned stmtNo(Utils::StringToInt(stmt));
    string expression(QueryUtils::GetExpression(expr));
    if (expression == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        if (lhsVar == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return true;

        } else {
            // stmtNo can be type assign/if/while
            Symbol stmtSymbol = PKB::GetStmtSymbol(stmtNo);
            if ((stmtSymbol == ASSIGN)) {
                return PKB::IsModifies(stmtNo, lhsVar);
            } else {
                return PKB::HasControlVariableNameAtStmtNumber(stmtNo, lhsVar);
            }
        }

    } else if (Utils::StartsWith(expression, CHAR_SYMBOL_UNDERSCORE)) {
        if (lhsVar == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return PKB::IsSubExpression(stmtNo, QueryUtils::GetSubExpression(expression));
        } else {
            return PKB::IsSubPattern(stmtNo, lhsVar, QueryUtils::GetSubExpression(expression));
        }

    } else {
        if (lhsVar == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return PKB::IsExactExpression(stmtNo, expression);
        } else {
            return PKB::IsExactPattern(stmtNo, lhsVar, expression);
        }
    }
}

bool QueryEvaluator::evaluateSuchThatClause(
    string clauseType, Candidate var0, Candidate var1) {
    if (clauseType == SYMBOL_USES) {
        return evaluateUses(var0, var1);
    } else if (clauseType == SYMBOL_MODIFIES) {
        return evaluateModifies(var0, var1);
    } else if (clauseType == SYMBOL_PARENT) {
        return evaluateParent(var0, var1);
    } else if (clauseType == SYMBOL_PARENT_TRANSITIVE) {
        return evaluateParentStar(var0, var1);
    } else if (clauseType == SYMBOL_FOLLOWS) {
        return evaluateFollows(var0, var1);
    } else if (clauseType == SYMBOL_FOLLOWS_TRANSITIVE) {
        return evaluateFollowsStar(var0, var1);
    } else if (clauseType == (SYMBOL_NEXT)) {
        return evaluateNext(var0, var1);
    } else if (clauseType == (SYMBOL_NEXT_TRANSITIVE)) {
        return evaluateNextStar(var0, var1);
    } else if (clauseType == (SYMBOL_CALLS)) {
        return evaluateCalls(var0, var1);
    } else if (clauseType == (SYMBOL_CALLS_TRANSITIVE)) {
        return evaluateCallsStar(var0, var1);
    } else if (clauseType == (SYMBOL_AFFECTS)) {
        return evaluateAffects(var0, var1);
    } else if (clauseType == (SYMBOL_AFFECTS_TRANSITIVE)) {
        return evaluateAffectsStar(var0, var1);
    }

    return false;
}

bool QueryEvaluator::evaluateUses(Candidate procOrStmtNo, Candidate varName) {
    if (Utils::IsNonNegativeNumeric(procOrStmtNo)) {
        if (varName == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (!PKB::GetUsedVariables(Utils::StringToInt(procOrStmtNo)).empty());
        }
		else {
            int stmtNo(Utils::StringToInt(procOrStmtNo));
            return PKB::IsUses(stmtNo, varName);
        }
    }
	else if (procOrStmtNo == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        if (varName == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (PKB::GetNumberOfAssign() > 0);
        }
		else {
            return (!PKB::GetStmtNumberUsing(varName).empty());
        }
    }
	else {
        if (varName == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (!PKB::GetProcedureUsedVariables(procOrStmtNo).empty());
        }
        return PKB::IsUsesProcedure(procOrStmtNo, varName);
    }
}

bool QueryEvaluator::evaluateModifies(Candidate procOrStmtNo, Candidate varName) {
    if (Utils::IsNonNegativeNumeric(procOrStmtNo)) {
        int stmtNo(Utils::StringToInt(procOrStmtNo));
        if (varName == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (!PKB::GetModifiedVariables(stmtNo).empty());
        }
		else {
			return PKB::IsModifies(stmtNo, varName);
		}
    }
	else if (procOrStmtNo == string(1, CHAR_SYMBOL_UNDERSCORE)) {
		if (varName == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (PKB::GetNumberOfAssign() > 0);
		}
		else {
			return (!PKB::GetStmtNumberModifying(varName).empty());
		}
	}
	else {
        if (varName == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (!PKB::GetProcedureModifiedVariables(procOrStmtNo).empty());
		}
		else {
			return PKB::IsModifiesProcedure(procOrStmtNo, varName);
		}
    }
}

bool QueryEvaluator::evaluateParent(Candidate stmt1, Candidate stmt2) {
    if (stmt1 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (PKB::GetNumberOfContainerStmt() > 0);
        } else {
            int stmtNo2(Utils::StringToInt(stmt2));
            return (!PKB::GetParentsTransitive(stmtNo2).empty());
        }
    }
	else {
        int stmtNo1(Utils::StringToInt(stmt1));
        if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (!PKB::GetChildrenTransitive(stmtNo1).empty());
        }
		else {
            int stmtNo2(Utils::StringToInt(stmt2));
            return PKB::IsParent(stmtNo1, stmtNo2);
        }
    }
}

bool QueryEvaluator::evaluateParentStar(Candidate stmt1, Candidate stmt2) {
    if (stmt1 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (PKB::GetNumberOfContainerStmt() > 0);
        } else {
            int stmtNo2(Utils::StringToInt(stmt2));
            return (!PKB::GetParent(stmtNo2).empty());
        }
    }
	else {
        int stmtNo1(Utils::StringToInt(stmt1));
        if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (!PKB::GetChildren(stmtNo1).empty());
        }
		else {
            int stmtNo2(Utils::StringToInt(stmt2));
            return PKB::IsParentTransitive(stmtNo1, stmtNo2);
        }
    }
}

bool QueryEvaluator::evaluateFollows(Candidate stmt1, Candidate stmt2) {
    if (stmt1 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (PKB::GetNumberOfFollowsRelationship() > 0);
        } else {
            int stmtNo2(Utils::StringToInt(stmt2));
            return (!PKB::GetFollows(stmtNo2).empty());
        }
    }
	else {
        int stmtNo1(Utils::StringToInt(stmt1));
        if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (!PKB::GetFollowing(stmtNo1).empty());
        }
		else {
            int stmtNo2(Utils::StringToInt(stmt2));
            return PKB::IsFollows(stmtNo1, stmtNo2);
        }
    }
}

bool QueryEvaluator::evaluateFollowsStar(Candidate stmt1, Candidate stmt2) {
    if (stmt1 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (PKB::GetNumberOfFollowsRelationship() > 0);
        }
		else {
            int stmtNo2(Utils::StringToInt(stmt2));
            return (!PKB::GetFollows(stmtNo2).empty());
        }
    }
	else {
        int stmtNo1(Utils::StringToInt(stmt1));
        if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return (!PKB::GetFollowing(stmtNo1).empty());
        }
		else {
            int stmtNo2(Utils::StringToInt(stmt2));
            return PKB::IsFollowsTransitive(stmtNo1, stmtNo2);
        }
    }
}

bool QueryEvaluator::evaluateNext(Candidate stmt1, Candidate stmt2) {
	if (stmt1 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
		if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (PKB::GetNumberOfNextRelationship() > 0);
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return (!PKB::GetPrevious(stmtNo2).empty());
		}
	}
	else {
		int stmtNo1(Utils::StringToInt(stmt1));
		if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (!PKB::GetNext(stmtNo1).empty());
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return PKB::IsNext(stmtNo1, stmtNo2);
		}
	}
}

bool QueryEvaluator::evaluateNextStar(Candidate stmt1, Candidate stmt2) {
	if (stmt1 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
		if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (PKB::GetNumberOfNextRelationship() > 0);
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return (!PKB::GetPrevious(stmtNo2).empty());
		}
	}
	else {
		int stmtNo1(Utils::StringToInt(stmt1));
		if (stmt2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (!PKB::GetNext(stmtNo1).empty());
		}
		else {
			int stmtNo2(Utils::StringToInt(stmt2));
			return PKB::IsNextTransitive(stmtNo1, stmtNo2);
		}
	}
}

bool QueryEvaluator::evaluateCalls(Candidate proc1, Candidate proc2) {
	if (proc1 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
		if (proc2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (PKB::GetNumberOfCallsRelationship() > 0);
		}
		else {
			return (!PKB::GetCalling(proc2).empty());
		}
	}
	else {
		if (proc2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (!PKB::GetCalled(proc1).empty());
		}
		else {
			return PKB::IsCalls(proc1, proc2);
		}
	}
}

bool QueryEvaluator::evaluateCallsStar(Candidate proc1, Candidate proc2) {
	if (proc1 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
		if (proc2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (PKB::GetNumberOfCallsRelationship() > 0);
		}
		else {
			return (!PKB::GetCalling(proc2).empty());
		}
	}
	else {
		if (proc2 == string(1, CHAR_SYMBOL_UNDERSCORE)) {
			return (!PKB::GetCalled(proc1).empty());
		}
		else {
			return PKB::IsCallsTransitive(proc1, proc2);
		}
	}
}

bool QueryEvaluator::evaluateAffects(Candidate assign1, Candidate assign2) {
    int stmtNo1(Utils::StringToInt(assign1));
    int stmtNo2(Utils::StringToInt(assign2));
    // return PKB::IsAffects(stmtNo1, stmtNo2);
    return false;
}

bool QueryEvaluator::evaluateAffectsStar(Candidate assign1, Candidate assign2) {
    int stmtNo1(Utils::StringToInt(assign1));
    int stmtNo2(Utils::StringToInt(assign2));
    // return PKB::IsAffectsTransitive(stmtNo1, stmtNo2);
    return false;
}

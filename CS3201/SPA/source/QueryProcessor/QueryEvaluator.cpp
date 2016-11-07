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

vector<Candidate> QueryEvaluator::getCandidates(Symbol &synType) {
    switch (synType) {
        case VARIABLE:
            return PKB::GetAllVariableIndexes();
        case PROCEDURE:
            return PKB::GetAllProcedureIndexes();
        case BOOLEAN:
            return vector<Candidate>{ 1 };
        case CONSTANT:
            return PKB::GetAllConstantIndexes();
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

TotalCombinationList QueryEvaluator::getTotalCandidateList(vector<Synonym> &synList) {
    TotalCombinationList totalCandLst;
    for (Synonym &syn : synList) {
        vector<Candidate> candLst(getCandidates(varMap[syn]));
        totalCandLst.addSynonym(syn, candLst);
    }

    return totalCandLst;
}

ResultList QueryEvaluator::selectQueryResults(QueryTree &query) {
	varMap = query.getVarMap();
	selectList = query.getResults();
	booleanClauses = query.getBooleanClauses();
	unselectedGroups = query.getUnselectedGroups();
	selectedGroups = query.getSelectedGroups();

    TotalCombinationList queryResults(getQueryResults());
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
            ResultList resultList(getResultsFromCombinationList(queryResults));
            return resultList;
        }
    }
}

ResultList QueryEvaluator::getResultsFromCombinationList(TotalCombinationList &combinations) {
	PartialCombinationList selectedCombinations(combinations.getCombinationList(selectList));
	vector<vector<string>> selectedCombinationList;

	for (CandidateCombination combi : selectedCombinations) {
		vector<string> candidateTuple;

		for (Synonym syn : selectList) {
			string cand;
			if (varMap[syn] == VARIABLE) {
				cand = PKB::GetVariableName(combi[syn]);
			}
			else if (varMap[syn] == PROCEDURE) {
				cand = PKB::GetProcedureName(combi[syn]);
			}
			else {
				cand = Utils::IntToString(combi[syn]);
			}
			candidateTuple.push_back(cand);
		}

		selectedCombinationList.push_back(candidateTuple);
	}

	ResultList result{ selectList, selectedCombinationList };

	return result;
}

bool QueryEvaluator::isBoolSelect(vector<string> &synList) {
	if (synList.size() == 1 && synList[0] == SYMBOL_BOOLEAN) {
		return true;
	}
	else {
		return false;
	}
}

TotalCombinationList QueryEvaluator::getQueryResults() {
    if (!getBooleanGroupResult()) {
        return TotalCombinationList();

    } else {

        for (auto &pair : unselectedGroups) {
            if (!getUnselectedGroupResult(pair.first, pair.second)) {
                return TotalCombinationList();
            }
        }

        TotalCombinationList result;

        for (auto &pair : selectedGroups) {
            vector<Synonym> &synList(pair.first);
            vector<Clause> &group(pair.second);
            TotalCombinationList &tempCombiList(getSelectedGroupResult(synList, group));
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

bool QueryEvaluator::getBooleanGroupResult() {
    for (Clause clause : booleanClauses) {
        
		string arg0(QueryUtils::LiteralToCandidate(clause.getArg()[0]));
        string arg1(QueryUtils::LiteralToCandidate(clause.getArg()[1]));
        string clauseType(clause.getClauseType());
		
		if (clauseType == SYMBOL_WITH) {
			if (arg0 != arg1) {
				return false;
			}
		}
		else if (clauseType == SYMBOL_MODIFIES_PROCEDURE) {
		}
    }

    return true;
}

/*
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

/*
TotalCombinationList QueryEvaluator::getSelectedGroupResult
(vector<Synonym> &synList,
    unordered_map<Synonym, Symbol> &varMap,
    vector<Clause> &clauseGroup,
    vector<Synonym> &selectList) {
    TotalCombinationList combinations(getTotalCandidateList(varMap, synList));

	
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

    return combinations;
}
*/


void QueryEvaluator::filterByClause(Clause &clause,
    TotalCombinationList &combinations) {
    string clauseType(clause.getClauseType());

	
	log.append("\n");
	log.append("clause type: " + clause.getClauseType() + "\n");
	log.append("args: " + clause.getArg()[0] + " " + clause.getArg()[1]);

    
	if (clauseType == SYMBOL_PATTERN) {
        vector<Synonym> args(clause.getArg());
        Synonym lhs(args[1]), rhs(args[2]), assignStmt(args[0]);

        if (QueryUtils::IsStringLiteral(lhs)) {
			Candidate l = PKB::GetVariableIndex(lhs);
			if (l == 0) {
				combinations.filter(false);
			}
			else {
				filterNoVarPattern(assignStmt, l, rhs, combinations);
			}
        } else {
            filterOneVarPattern(assignStmt, lhs, rhs, combinations);
        }

    }
	
	else if (clauseType == SYMBOL_WITH) {
        vector<Synonym> args(clause.getArg());
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
					Candidate v = (type1 == PROCEDURE) ? PKB::GetProcedureIndex(var1) : PKB::GetVariableIndex(var1);
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
        vector<Synonym> args(clause.getArg());
        Synonym var0(args[0]), var1(args[1]);
        if (QueryUtils::IsLiteral(var0)) {
            if (QueryUtils::IsLiteral(var1)) {
                filterNoVarClause(clauseType, Utils::StringToInt(var0), Utils::StringToInt(var1), combinations);
            } else {
                filterSecondVarClause(clauseType, Utils::StringToInt(var0), var1, combinations);
            }

        } else {
            if (QueryUtils::IsLiteral(var1)) {
                filterFirstVarClause(clauseType, var0, Utils::StringToInt(var1), combinations);
            } else {
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
}

void QueryEvaluator::filterNoVarPattern(Synonym assignStmt, Candidate lhs,
    string expr, TotalCombinationList &combinations) {
    auto matchPattern = [=](CandidateCombination &combi) -> bool {
        return evaluatePatternClause(combi[assignStmt], lhs, expr);
    };
    combinations.filter(assignStmt, matchPattern);
}


void QueryEvaluator::filterOneVarPattern(Synonym assignStmt, Synonym lhs,
    string expr, TotalCombinationList &combinations) {
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


void QueryEvaluator::filterNoVarWith(string const1, string const2, TotalCombinationList &combinations) {
	combinations.filter(const1 == const2);
}


void QueryEvaluator::filterTwoVarsCallWith(Synonym & call0, Synonym & call1, TotalCombinationList & combinations) {
	auto comp = [=](CandidateCombination combi) -> bool {
		return (PKB::GetCallStmtProcedureIndex(combi[call0])
			== (PKB::GetCallStmtProcedureIndex(combi[call1])));
	};
	combinations.mergeAndFilter(call0, call1, comp);
}


void QueryEvaluator::filterOneVarCallWith(Synonym & call, Synonym & var, TotalCombinationList & combinations) {
    auto comp = [=](CandidateCombination combi) -> bool {
        return (PKB::GetCallStmtProcedureIndex(combi[call]) == combi[var]);
    };
    combinations.mergeAndFilter(call, var, comp);
}


void QueryEvaluator::filterNoVarCallWith(Synonym & call, Candidate cand, TotalCombinationList & combinations) {
    auto comp = [=](CandidateCombination combi) -> bool {
        return (PKB::GetCallStmtProcedureIndex(combi[call]) == cand);
    };
    combinations.filter(call, comp);
}


bool QueryEvaluator::evaluatePatternClause(Candidate stmtNo, Candidate lhsVar, string expr) {
    // Preprocessor will do the following:
    // expression _"x+y"_ should convert to a string of vectors <x, +, y>
    // preprocessor will get (x+y) after calling Utils::GetPostfixExpression(<x, +, y>)
    // in this method, expr = _"(x+y)"_

    string expression(QueryUtils::GetExpression(expr));
    if (expression == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        if (lhsVar == INT_UNDERSCORE) {
            return true;

        } else {
            // stmtNo can be type assign/if/while
            Symbol stmtSymbol = PKB::GetStmtSymbol(stmtNo);
            if ((stmtSymbol == ASSIGN)) {
                return PKB::IsModifies(stmtNo, lhsVar);
            } else {
                return PKB::HasControlVariableIndexAtStmtNumber(stmtNo, lhsVar);
            }
        }

    } else if (Utils::StartsWith(expression, CHAR_SYMBOL_UNDERSCORE)) {
		if (lhsVar == INT_UNDERSCORE) {
            return PKB::IsSubExpression(stmtNo, QueryUtils::GetSubExpression(expression));
        } else {
            return PKB::IsSubPattern(stmtNo, lhsVar, QueryUtils::GetSubExpression(expression));
        }

    } else {
        if (lhsVar == INT_UNDERSCORE) {
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
	}
	else if (clauseType == SYMBOL_USES_PROCEDURE) {
		return evaluateUsesProcedure(var0, var1);
	}
	else if (clauseType == SYMBOL_MODIFIES) {
		return evaluateModifies(var0, var1);
	}
	else if (clauseType == SYMBOL_MODIFIES_PROCEDURE) {
		return evaluateModifiesProcedure(var0, var1);
	}
	else if (clauseType == SYMBOL_PARENT) {
		return evaluateParent(var0, var1);
	}
	else if (clauseType == SYMBOL_PARENT_TRANSITIVE) {
		return evaluateParentStar(var0, var1);
	}
	else if (clauseType == SYMBOL_FOLLOWS) {
		return evaluateFollows(var0, var1);
	}
	else if (clauseType == SYMBOL_FOLLOWS_TRANSITIVE) {
		return evaluateFollowsStar(var0, var1);
	}
	else if (clauseType == (SYMBOL_NEXT)) {
		return evaluateNext(var0, var1);
	}
	else if (clauseType == (SYMBOL_NEXT_TRANSITIVE)) {
		return evaluateNextStar(var0, var1);
	}
	else if (clauseType == (SYMBOL_CALLS)) {
		return evaluateCalls(var0, var1);
	}
	else if (clauseType == (SYMBOL_CALLS_TRANSITIVE)) {
		return evaluateCallsStar(var0, var1);
	}
	else if (clauseType == (SYMBOL_AFFECTS)) {
		return evaluateAffects(var0, var1);
	}
	else if (clauseType == (SYMBOL_AFFECTS_TRANSITIVE)) {
		return evaluateAffectsStar(var0, var1);
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

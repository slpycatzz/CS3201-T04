#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Constants.h"
#include "Frontend/DesignExtractor.h"
#include "PKB/AST.h"
#include "PKB/PKB.h"
#include "PKB/Table.h"
#include "PKB/TransitiveTable.h"
#include "TreeNode.h"

using std::map;
using std::queue;
using std::set;
using std::string;
using std::vector;

unsigned int PKB::numberOfProcedure_ = 0;
unsigned int PKB::numberOfAssign_    = 0;
unsigned int PKB::numberOfWhile_     = 0;
unsigned int PKB::numberOfIf_        = 0;
unsigned int PKB::numberOfCall_      = 0;

unsigned int PKB::tableMaximumSize_  = 0;

vector<CFGNode> PKB::controlFlowGraphs_;
vector<vector<CFGNode>> PKB::controlFlowGraphsNodes_;

Table<Index, ConstantValue> PKB::constantTable_;
Table<Index, VariableName>  PKB::variableTable_;
Table<Index, ProcedureName> PKB::procedureTable_;

Table<StmtNumber, VariableName>  PKB::controlVariableTable_;
Table<StmtNumber, ProcedureName> PKB::callTable_;
Table<StmtNumber, StmtSymbol>    PKB::stmtTable_;
Table<StmtNumber, StmtSymbol>    PKB::stmtlistTable_;

Table<Priority, TableSymbol> PKB::priorityTable_;

Table<StmtNumber, Expression> PKB::expressionTable_;
Table<StmtNumber, SubExpressions> PKB::subExpressionTable_;

/* Deprecated */
Table<unsigned int, TreeNode*> PKB::assignTable_;

Table<ProcedureName, ProcedureName> PKB::callsTable_;
TransitiveTable<ProcedureName, ProcedureName> PKB::callsTransitiveTable_;

Table<StmtNumber, VariableName> PKB::modifiesTable_;
Table<ProcedureName, VariableName> PKB::modifiesProcedureTable_;

Table<StmtNumber, VariableName> PKB::usesTable_;
Table<ProcedureName, VariableName> PKB::usesProcedureTable_;

Table<StmtNumber, StmtNumber> PKB::parentTable_;
TransitiveTable<StmtNumber, StmtNumber> PKB::parentTransitiveTable_;

Table<StmtNumber, StmtNumber> PKB::followsTable_;
TransitiveTable<StmtNumber, StmtNumber> PKB::followsTransitiveTable_;

Table<StmtNumber, StmtNumber> PKB::nextTable_;
vector<vector<StmtNumber>> PKB::nextTransitiveTable_;

/* START - AST functions */

void PKB::SetASTRoot(TreeNode *root) {
    AST::getInstance()->setRoot(root);
}

TreeNode* PKB::GetASTRoot() {
    return AST::getInstance()->getRoot();
}

TreeNode* PKB::CreateASTNode(Symbol symbol) {
    return AST::getInstance()->createNode(symbol);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, string value) {
    return AST::getInstance()->createNode(symbol, value);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, StmtNumber stmtNumber) {
    return AST::getInstance()->createNode(symbol, stmtNumber);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, StmtNumber stmtNumber, string value) {
    return AST::getInstance()->createNode(symbol, stmtNumber, value);
}

void PKB::PrintASTTree() {
    AST::getInstance()->printTree();
}

/* END   - AST functions */
/* START - Constant table functions */

void PKB::GenerateConstantTable(set<ConstantValue> constants) {
    unsigned int i = 1;
    for (auto &constant : constants) {
        constantTable_.insert(i++, constant);
    }
}

bool PKB::HasConstant(ConstantValue constantValue) {
    return constantTable_.hasValue(constantValue);
}

set<ConstantValue> PKB::GetConstantValue(Index index) {
    return constantTable_.getValues(index);
}

set<Index> PKB::GetConstantIndex(ConstantValue constantValue) {
    return constantTable_.getKeys(constantValue);
}

vector<ConstantValue> PKB::GetAllConstantValues() {
    set<ConstantValue> result = constantTable_.getValues();

    vector<ConstantValue> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintConstantTable() {
    constantTable_.printTable();
}

/* END   - Constant table functions */
/* START - Variable table functions */

void PKB::GenerateVariableTable(set<VariableName> variableNames) {
    unsigned int i = 1;
    for (auto &variableName : variableNames) {
        variableTable_.insert(i++, variableName);
    }
}

bool PKB::HasVariable(VariableName variableName) {
    return variableTable_.hasValue(variableName);
}

set<VariableName> PKB::GetVariableName(Index index) {
    return variableTable_.getValues(index);
}

set<Index> PKB::GetVariableIndex(VariableName variableName) {
    return variableTable_.getKeys(variableName);
}

vector<VariableName> PKB::GetAllVariableNames() {
    set<VariableName> result = variableTable_.getValues();

    vector<VariableName> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintVariableTable() {
    variableTable_.printTable();
}

/* END   - Variable table functions */
/* START - Procedure table functions */

void PKB::GenerateProcedureTable(set<ProcedureName> procedureNames) {
    unsigned int i = 1;
    for (auto &procedureName : procedureNames) {
        numberOfProcedure_++;
        procedureTable_.insert(i++, procedureName);
    }
}

bool PKB::HasProcedure(ProcedureName procedureName) {
    return procedureTable_.hasValue(procedureName);
}

set<ProcedureName> PKB::GetProcedureName(Index index) {
    return procedureTable_.getValues(index);
}

set<Index> PKB::GetProcedureIndex(ProcedureName procedureName) {
    return procedureTable_.getKeys(procedureName);
}

vector<ProcedureName> PKB::GetAllProcedures() {
    set<ProcedureName> result = procedureTable_.getValues();

    vector<ProcedureName> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintProcedureTable() {
    procedureTable_.printTable();
}

/* END   - Procedure table functions */
/* START - Control variable table functions */

void PKB::GenerateControlVariableTable(map<StmtNumber, VariableName> controlVariables) {
    for (auto &controlVariable : controlVariables) {
        controlVariableTable_.insert(controlVariable.first, controlVariable.second);
    }
}

bool PKB::HasControlVariable(VariableName controlVariable) {
    return controlVariableTable_.hasValue(controlVariable);
}

bool PKB::HasControlVariableAtStmtNumber(StmtNumber stmtNumber, VariableName controlVariable) {
    return controlVariableTable_.hasKeyToValue(stmtNumber, controlVariable);
}

set<VariableName> PKB::GetControlVariable(StmtNumber stmtNumber) {
    return controlVariableTable_.getValues(stmtNumber);
}

vector<VariableName> PKB::GetAllControlVariables() {
    set<VariableName> result = controlVariableTable_.getValues();

    vector<VariableName> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintControlVariableTable() {
    controlVariableTable_.printTable();
}

/* END   - Control variable table functions */
/* START - Call table functions */

void PKB::GenerateCallTable(map<StmtNumber, ProcedureName> callStmtNumbers) {
    for (auto &pair : callStmtNumbers) {
        callTable_.insert(pair.first, pair.second);
    }
}

ProcedureName PKB::GetCallProcedureName(StmtNumber stmtNumber) {
    return (callTable_.hasKey(stmtNumber)) ? callTable_.getValue(stmtNumber) : "";
}

void PKB::PrintCallTable() {
    callTable_.printTable();
}

/* END   - Call table functions */
/* START - Stmt table functions */

void PKB::GenerateStmtTable(map<StmtNumber, StmtSymbol> stmts) {
    for (auto &stmt : stmts) {
        switch (Constants::StringToSymbol(stmt.second)) {
            default:
                break;

            case ASSIGN:
                numberOfAssign_++;
                break;

            case WHILE:
                numberOfWhile_++;
                break;

            case IF:
                numberOfIf_++;
                break;

            case CALL:
                numberOfCall_++;
                break;
        }

        stmtTable_.insert(stmt.first, stmt.second);
    }
}

StmtSymbol PKB::GetStmtSymbol(StmtNumber stmtNumber) {
    return (stmtTable_.hasKey(stmtNumber)) ? stmtTable_.getValue(stmtNumber) : "";
}

vector<StmtNumber> PKB::GetSymbolStmtNumbers(StmtSymbol symbol) {
    set<StmtNumber> result = (symbol == SYMBOL_STMT) ? stmtTable_.getKeys() : stmtTable_.getKeys(symbol);

    vector<StmtNumber> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

vector<StmtNumber> PKB::GetSymbolStmtNumbers(Symbol symbol) {
    return GetSymbolStmtNumbers(Constants::SymbolToString(symbol));
}

void PKB::PrintStmtTable() {
    stmtTable_.printTable();
}

/* END   - Stmt table functions */
/* START - Stmtlist table functions */

void PKB::GenerateStmtlistTable(map<StmtNumber, StmtSymbol> stmtlists) {
    for (auto &pair : stmtlists) {
        stmtlistTable_.insert(pair.first, pair.second);
    }
}

vector<StmtNumber> PKB::GetAllStmtlistsStmtNumber() {
    set<StmtNumber> result = stmtlistTable_.getKeys();

    vector<StmtNumber> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintStmtlistTable() {
    stmtlistTable_.printTable();
}

/* END   - Stmtlist table functions */
/* START - Priority table functions */

void PKB::GeneratePriorityTable() {
    vector<std::pair<unsigned int, string>> tablesSize;

    tablesSize.push_back(std::make_pair(callsTable_.getNumberOfValues(),   SYMBOL_CALLS));
    tablesSize.push_back(std::make_pair(followsTable_.getNumberOfValues(), SYMBOL_FOLLOWS));
    tablesSize.push_back(std::make_pair(parentTable_.getNumberOfValues(),  SYMBOL_PARENT));

    tablesSize.push_back(std::make_pair(callsTransitiveTable_.getNumberOfValues(),   SYMBOL_CALLS_TRANSITIVE));
    tablesSize.push_back(std::make_pair(followsTransitiveTable_.getNumberOfValues(), SYMBOL_FOLLOWS_TRANSITIVE));
    tablesSize.push_back(std::make_pair(parentTransitiveTable_.getNumberOfValues(),  SYMBOL_PARENT_TRANSITIVE));

    tablesSize.push_back(std::make_pair(nextTable_.getNumberOfValues(), SYMBOL_NEXT));

    tablesSize.push_back(std::make_pair(modifiesTable_.getNumberOfValues(),          SYMBOL_MODIFIES));
    tablesSize.push_back(std::make_pair(modifiesProcedureTable_.getNumberOfValues(), SYMBOL_MODIFIES_PROCEDURE));
    tablesSize.push_back(std::make_pair(usesTable_.getNumberOfValues(),              SYMBOL_USES));
    tablesSize.push_back(std::make_pair(usesProcedureTable_.getNumberOfValues(),     SYMBOL_USES_PROCEDURE));

    /* Sort the size in ascending order to determine the priority. */
    std::sort(tablesSize.begin(), tablesSize.end(), ComparePairAscending);

    unsigned int i = 1;
    for (; i <= tablesSize.size(); i++) {
        priorityTable_.insert((i * 10), tablesSize[i - 1].second);
    }

    /* Assign lower priority to compute on demand design abstractions. */
    priorityTable_.insert(i + 1, SYMBOL_NEXT_TRANSITIVE);
}

Priority PKB::GetPriority(TableSymbol symbol) {
    return (priorityTable_.hasValue(symbol)) ? priorityTable_.getKey(symbol) : 0;
}

Priority PKB::GetPriority(Symbol symbol) {
    return GetPriority(Constants::SymbolToString(symbol));
}

void PKB::PrintPriorityTable() {
    priorityTable_.printTable();
}

/* END   - Priority table functions */
/* START - Expression table functions */

void PKB::GenerateExpressionTable(map<StmtNumber, Expression> expressions) {
    for (auto &pair : expressions) {
        expressionTable_.insert(pair.first, pair.second);
    }
}

void PKB::GenerateSubExpressionTable(map<StmtNumber, set<SubExpressions>> subExpressions) {
    for (auto &pair : subExpressions) {
        subExpressionTable_.insert(pair.first, pair.second);
    }
}

bool PKB::IsExactPattern(StmtNumber stmtNumber, VariableName variableName, Expression expression) {
    string stmtSymbol = GetStmtSymbol(stmtNumber);

    if (stmtSymbol == SYMBOL_WHILE || stmtSymbol == SYMBOL_IF) {
        return HasControlVariableAtStmtNumber(stmtNumber, variableName);

    } else if (stmtSymbol == SYMBOL_ASSIGN) {
        if (HasControlVariableAtStmtNumber(stmtNumber, variableName)) {
            return IsExactExpression(stmtNumber, expression);
        }
    }

    return false;
}

bool PKB::IsSubPattern(StmtNumber stmtNumber, VariableName variableName, SubExpressions subExpression) {
    string stmtSymbol = GetStmtSymbol(stmtNumber);

    if (stmtSymbol == SYMBOL_WHILE || stmtSymbol == SYMBOL_IF) {
        return HasControlVariableAtStmtNumber(stmtNumber, variableName);

    } else if (stmtSymbol == SYMBOL_ASSIGN) {
        if (HasControlVariableAtStmtNumber(stmtNumber, variableName)) {
            return IsSubExpression(stmtNumber, subExpression);
        }
    }

    return false;
}

bool PKB::HasExactExpression(Expression expression) {
    return expressionTable_.hasValue(expression);
}

bool PKB::HasSubExpression(SubExpressions subExpression) {
    return subExpressionTable_.hasValue(subExpression);
}

bool PKB::IsExactExpression(StmtNumber stmtNumber, Expression expression) {
    return expressionTable_.hasKeyToValue(stmtNumber, expression);
}

bool PKB::IsSubExpression(StmtNumber stmtNumber, SubExpressions subExpression) {
    return subExpressionTable_.hasKeyToValue(stmtNumber, subExpression);
}

void PKB::PrintExactExpressionTable() {
    expressionTable_.printTable();
}

void PKB::PrintSubExpressionTable() {
    subExpressionTable_.printTable();
}

/* END   - Expression table functions */
/* START - Calls table functions */

void PKB::GenerateCallsTable(map<ProcedureName, set<ProcedureName>> calls) {
    for (auto &pair : calls) {
        callsTable_.insert(pair.first, pair.second);
    }

    callsTransitiveTable_.generateKeyToValueTransitiveMap(callsTable_);
    callsTransitiveTable_.generateValueToKeyTransitiveMap(callsTable_);
}

bool PKB::IsCalls(ProcedureName calling, ProcedureName called) {
    return callsTable_.hasKeyToValue(calling, called);
}

bool PKB::IsCalls(ProcedureName calling, set<ProcedureName> called) {
    return callsTable_.hasKeyToValues(calling, called);
}

bool PKB::IsCallsTransitive(ProcedureName calling, ProcedureName called) {
    return callsTransitiveTable_.hasKeyToValue(calling, called);
}

bool PKB::IsCallsTransitive(ProcedureName calling, set<ProcedureName> called) {
    return callsTransitiveTable_.hasKeyToValues(calling, called);
}

set<ProcedureName> PKB::GetCalling(ProcedureName called) {
    return callsTable_.getKeys(called);
}

set<ProcedureName> PKB::GetCalled(ProcedureName calling) {
    return callsTable_.getValues(calling);
}

set<ProcedureName> PKB::GetCallingTransitive(ProcedureName called) {
    return callsTransitiveTable_.getKeys(called);
}

set<ProcedureName> PKB::GetCalledTransitive(ProcedureName calling) {
    return callsTransitiveTable_.getValues(calling);
}

void PKB::PrintCallsTable() {
    callsTable_.printTable();
}

void PKB::PrintCallsTransitiveTable() {
    callsTransitiveTable_.printTable();
}

/* END   - Calls table functions */
/* START - Modifies table functions */

void PKB::GenerateModifiesTable(map<StmtNumber, set<VariableName>> modifies) {
    for (auto &pair : modifies) {
        modifiesTable_.insert(pair.first, pair.second);
    }
}

void PKB::GenerateModifiesProcedureTable(map<ProcedureName, set<VariableName>> modifiesProcedure) {
    for (auto &pair : modifiesProcedure) {
        modifiesProcedureTable_.insert(pair.first, pair.second);
    }
}

bool PKB::IsModifies(StmtNumber stmtNumber, VariableName variableName) {
    return modifiesTable_.hasKeyToValue(stmtNumber, variableName);
}

bool PKB::IsModifies(StmtNumber stmtNumber, set<VariableName> variableNames) {
    return modifiesTable_.hasKeyToValues(stmtNumber, variableNames);
}

bool PKB::IsModifiesProcedure(ProcedureName procedureName, VariableName variableName) {
    return modifiesProcedureTable_.hasKeyToValue(procedureName, variableName);
}

bool PKB::IsModifiesProcedure(ProcedureName procedureName, set<VariableName> variableNames) {
    return modifiesProcedureTable_.hasKeyToValues(procedureName, variableNames);
}

set<VariableName> PKB::GetModifiedVariables(StmtNumber stmtNumber) {
    return modifiesTable_.getValues(stmtNumber);
}

set<StmtNumber> PKB::GetStmtNumberModifying(VariableName variableName) {
    return modifiesTable_.getKeys(variableName);
}

set<VariableName> PKB::GetProcedureModifiedVariables(ProcedureName procedureName) {
    return modifiesProcedureTable_.getValues(procedureName);
}

set<ProcedureName> PKB::GetProceduresNameModifying(VariableName variableName) {
    return modifiesProcedureTable_.getKeys(variableName);
}

void PKB::PrintModifiesTable() {
    modifiesTable_.printTable();
}

void PKB::PrintModifiesProcedureTable() {
    modifiesProcedureTable_.printTable();
}

/* END   - Modifies table functions */
/* START - Uses table functions */

void PKB::GenerateUsesTable(map<StmtNumber, set<VariableName>> uses) {
    for (auto &pair : uses) {
        usesTable_.insert(pair.first, pair.second);
    }
}

void PKB::GenerateUsesProcedureTable(map<ProcedureName, set<VariableName>> usesProcedure) {
    for (auto &pair : usesProcedure) {
        usesProcedureTable_.insert(pair.first, pair.second);
    }
}

bool PKB::IsUses(StmtNumber stmtNumber, VariableName variableName) {
    return usesTable_.hasKeyToValue(stmtNumber, variableName);
}

bool PKB::IsUses(StmtNumber stmtNumber, set<VariableName> variableNames) {
    return usesTable_.hasKeyToValues(stmtNumber, variableNames);
}

bool PKB::IsUsesProcedure(ProcedureName procedureName, VariableName variableName) {
    return usesProcedureTable_.hasKeyToValue(procedureName, variableName);
}

bool PKB::IsUsesProcedure(ProcedureName procedureName, set<VariableName> variableNames) {
    return usesProcedureTable_.hasKeyToValues(procedureName, variableNames);
}

set<VariableName> PKB::GetUsedVariables(StmtNumber stmtNumber) {
    return usesTable_.getValues(stmtNumber);
}

set<StmtNumber> PKB::GetStmtNumberUsing(VariableName variableName) {
    return usesTable_.getKeys(variableName);
}

set<VariableName> PKB::GetProcedureUsedVariables(ProcedureName procedureName) {
    return usesProcedureTable_.getValues(procedureName);
}

set<ProcedureName> PKB::GetProceduresNameUsing(VariableName variableName) {
    return usesProcedureTable_.getKeys(variableName);
}

void PKB::PrintUsesTable() {
    usesTable_.printTable();
}

void PKB::PrintUsesProcedureTable() {
    usesProcedureTable_.printTable();
}

/* END   - Uses table functions */
/* START - Parent table functions */

void PKB::GenerateParentTable(map<StmtNumber, set<StmtNumber>> parent) {
    for (auto &pair : parent) {
        parentTable_.insert(pair.first, pair.second);
    }

    parentTransitiveTable_.generateKeyToValueTransitiveMap(parentTable_);
    parentTransitiveTable_.generateValueToKeyTransitiveMap(parentTable_);
}

bool PKB::IsParent(StmtNumber parent, StmtNumber child) {
    return parentTable_.hasKeyToValue(parent, child);
}

bool PKB::IsParent(StmtNumber parent, set<StmtNumber> children) {
    return parentTable_.hasKeyToValues(parent, children);
}

bool PKB::IsParentTransitive(StmtNumber parent, StmtNumber child) {
    return parentTransitiveTable_.hasKeyToValue(parent, child);
}

bool PKB::IsParentTransitive(StmtNumber parent, set<StmtNumber> children) {
    return parentTransitiveTable_.hasKeyToValues(parent, children);
}

set<StmtNumber> PKB::GetParent(StmtNumber child) {
    return parentTable_.getKeys(child);
}

set<StmtNumber> PKB::GetChildren(StmtNumber parent) {
    return parentTable_.getValues(parent);
}

set<StmtNumber> PKB::GetParentsTransitive(StmtNumber child) {
    return parentTransitiveTable_.getKeys(child);
}

set<StmtNumber> PKB::GetChildrenTransitive(StmtNumber parent) {
    return parentTransitiveTable_.getValues(parent);
}

void PKB::PrintParentTable() {
    parentTable_.printTable();
}

void PKB::PrintParentTransitiveTable() {
    parentTransitiveTable_.printTable();
}

/* END   - Parent table functions */
/* START - Follows table functions */

void PKB::GenerateFollowsTable(map<StmtNumber, StmtNumber> follows) {
    for (auto &pair : follows) {
        followsTable_.insert(pair.first, pair.second);
    }

    followsTransitiveTable_.generateKeyToValueTransitiveMap(followsTable_);
    followsTransitiveTable_.generateValueToKeyTransitiveMap(followsTable_);
}

bool PKB::IsFollows(StmtNumber follows, StmtNumber following) {
    return followsTable_.hasKeyToValue(follows, following);
}

bool PKB::IsFollows(StmtNumber follows, set<StmtNumber> followings) {
    return followsTable_.hasKeyToValues(follows, followings);
}

bool PKB::IsFollowsTransitive(StmtNumber follows, StmtNumber following) {
    return followsTransitiveTable_.hasKeyToValue(follows, following);
}

bool PKB::IsFollowsTransitive(StmtNumber follows, set<StmtNumber> followings) {
    return followsTransitiveTable_.hasKeyToValues(follows, followings);
}

set<StmtNumber> PKB::GetFollows(StmtNumber following) {
    return followsTable_.getKeys(following);
}

set<StmtNumber> PKB::GetFollowing(StmtNumber follows) {
    return followsTable_.getValues(follows);
}

set<StmtNumber> PKB::GetFollowsTransitive(StmtNumber following) {
    return followsTransitiveTable_.getKeys(following);
}

set<StmtNumber> PKB::GetFollowingTransitive(StmtNumber follows) {
    return followsTransitiveTable_.getValues(follows);
}

void PKB::PrintFollowsTable() {
    followsTable_.printTable();
}

void PKB::PrintFollowsTransitiveTable() {
    followsTransitiveTable_.printTable();
}

/* END   - Follows table functions */
/* START - Next table functions */

void PKB::SetControlFlowGraphs(vector<CFGNode> controlFlowGraphs) {
    controlFlowGraphs_ = controlFlowGraphs;

    /* Get all the nodes in the control flow graphs. */
    for (CFGNode rootNode : controlFlowGraphs_) {
        set<CFGNode> visitedNodes;

        rootNode->setVisited(true);
        visitedNodes.insert(rootNode);

        queue<CFGNode> queue;
        queue.push(rootNode);

        while (!queue.empty()) {
            vector<CFGNode> children = queue.front()->getChildren();
            queue.pop();

            for (CFGNode child : children) {
                if (!child->isVisited()) {
                    queue.push(child);

                    child->setVisited(true);
                    visitedNodes.insert(child);
                }
            }
        }

        for (CFGNode node : visitedNodes) {
            node->setVisited(false);
        }

        controlFlowGraphsNodes_.push_back(vector<CFGNode>(visitedNodes.begin(), visitedNodes.end()));
    }
}

void PKB::GenerateNextTable(map<StmtNumber, set<StmtNumber>> next) {
    for (auto &pair : next) {
        nextTable_.insert(pair.first, pair.second);
    }

    /* Initialize next transitive table space. */
    nextTransitiveTable_.resize(tableMaximumSize_, vector<StmtNumber>(tableMaximumSize_, 0));
}

void PKB::GenerateNextTransitiveTable() {
    for (vector<CFGNode> treeNodes : controlFlowGraphsNodes_) {
        /* DFS all the nodes in the control flow graph. */
        for (CFGNode node : treeNodes) {
            vector<CFGNode> visitedNodes;
            queue<CFGNode> queue;

            queue.push(node);
            while (!queue.empty()) {
                CFGNode currentNode = queue.front();
                vector<CFGNode> children = currentNode->getChildren();

                queue.pop();

                for (CFGNode child : children) {
                    if (!child->isVisited()) {
                        child->setVisited(true);

                        nextTransitiveTable_[node->getStmtNumber()][child->getStmtNumber()] = 1;
                        visitedNodes.push_back(child);
                        queue.push(child);
                    }
                }
            }

            for (CFGNode visited : visitedNodes) {
                visited->setVisited(false);
            }
        }
    }
}

bool PKB::IsNext(StmtNumber current, StmtNumber next) {
    return nextTable_.hasKeyToValue(current, next);
}

bool PKB::IsNextTransitive(StmtNumber current, StmtNumber next) {
    if (current > tableMaximumSize_ || current <= 0) {
        return false;
    }

    if (next > tableMaximumSize_ || next <= 0) {
        return false;
    }

    /* Worst case is O(1) time complexity. */
    return (nextTransitiveTable_[current][next] == 1);
}

set<StmtNumber> PKB::GetNext(StmtNumber current) {
    return nextTable_.getValues(current);
}

set<StmtNumber> PKB::GetPrevious(StmtNumber next) {
    return nextTable_.getKeys(next);
}

set<StmtNumber> PKB::GetNextTransitive(StmtNumber current) {
    if (current > tableMaximumSize_ || current <= 0) {
        return set<StmtNumber>();
    }

    set<StmtNumber> nexts;

    /* Worst case is O(V) time complexity. */
    for (unsigned int i = 1; i < nextTransitiveTable_[current].size(); i++) {
        if (nextTransitiveTable_[current][i] == 1) {
            nexts.insert(i);
        }
    }

    return nexts;
}

set<StmtNumber> PKB::GetPreviousTransitive(StmtNumber next) {
    if (next > tableMaximumSize_ || next <= 0) {
        return set<StmtNumber>();
    }

    set<StmtNumber> previouses;

    /* Worst case is O(V) time complexity. */
    for (unsigned int i = 1; i < nextTransitiveTable_.size(); i++) {
        if (nextTransitiveTable_[i][next] == 1) {
            previouses.insert(i);
        }
    }

    return previouses;
}

void PKB::PrintNextTable() {
    nextTable_.printTable();
}

void PKB::PrintNextTransitiveTable() {
    for (unsigned int i = 1; i < nextTransitiveTable_.size(); i++) {
        for (unsigned int k = 1; k < nextTransitiveTable_[i].size(); k++) {
            std::cout << nextTransitiveTable_[i][k];
        }

        std::cout << std::endl;
    }

    std::cout << "=====================" << std::endl;

    for (unsigned int i = 1; i < nextTransitiveTable_.size(); i++) {
        std::cout << i << "-> { ";

        for (unsigned int k = 1; k < nextTransitiveTable_[i].size(); k++) {
            if (nextTransitiveTable_[i][k] == 1) {
                std::cout << k << " ";
            }
        }

        std::cout << "}" << std::endl;
    }
}

/* END   - Next table functions */
/* START - Miscellaneous functions */

unsigned int PKB::GetNumberOfProcedure() {
    return numberOfProcedure_;
}

unsigned int PKB::GetNumberOfAssign() {
    return numberOfAssign_;
}

unsigned int PKB::GetNumberOfWhile() {
    return numberOfWhile_;
}

unsigned int PKB::GetNumberOfIf() {
    return numberOfIf_;
}

unsigned int PKB::GetNumberOfCall() {
    return numberOfCall_;
}

unsigned int PKB::GetNumberOfContainerStmt() {
    return (numberOfWhile_ + numberOfIf_);
}

void PKB::SetTableMaximumSize(unsigned int tableMaximumSize) {
    tableMaximumSize_ = tableMaximumSize;
}

void PKB::Clear() {
    numberOfProcedure_ = 0;
    numberOfAssign_ = 0;
    numberOfWhile_ = 0;
    numberOfIf_ = 0;
    numberOfCall_ = 0;

    tableMaximumSize_ = 0;

    controlFlowGraphs_.clear();
    controlFlowGraphsNodes_.clear();

    constantTable_.clear();
    variableTable_.clear();
    procedureTable_.clear();
    controlVariableTable_.clear();
    callsTable_.clear();
    stmtTable_.clear();
    stmtlistTable_.clear();

    priorityTable_.clear();

    expressionTable_.clear();
    subExpressionTable_.clear();

    /* Deprecated. */
    assignTable_.clear();

    callsTable_.clear();
    callsTransitiveTable_.clear();

    modifiesTable_.clear();
    modifiesProcedureTable_.clear();

    usesTable_.clear();
    usesProcedureTable_.clear();

    parentTable_.clear();
    parentTransitiveTable_.clear();

    followsTable_.clear();
    followsTransitiveTable_.clear();

    nextTable_.clear();
    nextTransitiveTable_.clear();
}

bool PKB::ComparePairAscending(const std::pair<unsigned int, string> &pairOne, const std::pair<unsigned int, string> &pairTwo) {
    return pairOne.first < pairTwo.first;
}

/* START - Miscellaneous functions */
/* START - Deprecated */

void PKB::GenerateAssignTable(map<unsigned int, TreeNode*> assigns) {
    for (auto &assign : assigns) {
        assignTable_.insert(assign.first, assign.second);
    }
}

TreeNode* PKB::GetAssignTreeNode(unsigned int stmtNumber) {
    return (assignTable_.hasKey(stmtNumber)) ? assignTable_.getValue(stmtNumber) : nullptr;
}

vector<TreeNode*> PKB::GetAllAssignTreeNodes() {
    set<TreeNode*> result = assignTable_.getValues();

    vector<TreeNode*> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

bool PKB::IsExactPattern(unsigned int stmtNo, string varName, TreeNode* exprTree) {
    TreeNode* RHS(PKB::GetAssignTreeNode(stmtNo)->getChildren()[1]);

    bool matchLHS(PKB::IsModifies(stmtNo, varName));
    bool matchRHS(Utils::IsSameTree(*RHS, *exprTree));

    return (matchLHS && matchRHS);
}

bool PKB::IsSubPattern(unsigned int stmtNo, string varName, TreeNode* exprTree) {
    TreeNode* RHS(PKB::GetAssignTreeNode(stmtNo)->getChildren()[1]);

    bool matchLHS(PKB::IsModifies(stmtNo, varName));
    bool matchRHS(Utils::IsSubTree(*RHS, *exprTree));

    return (matchLHS && matchRHS);
}

bool PKB::HasExactPattern(TreeNode* exprTree) {
    for (TreeNode* node : GetAllAssignTreeNodes()) {
        if (Utils::IsSameTree(*node->getChildren()[1], *exprTree)) {
            return true;
        }
    }

    return false;
}

bool PKB::HasSubPattern(TreeNode* exprTree) {
    for (TreeNode* node : GetAllAssignTreeNodes()) {
        if (Utils::IsSubTree(*node->getChildren()[1], *exprTree)) {
            return true;
        }
    }

    return false;
}

bool PKB::IsExactRHS(unsigned int stmtNo, TreeNode* exprTree) {
    TreeNode* node(GetAssignTreeNode(stmtNo));
    return Utils::IsSameTree(*node->getChildren()[1], *exprTree);
}

bool PKB::IsSubRHS(unsigned int stmtNo, TreeNode* exprTree) {
    TreeNode* node(GetAssignTreeNode(stmtNo));
    return Utils::IsSubTree(*node->getChildren()[1], *exprTree);
}

/* END - Deprecated */

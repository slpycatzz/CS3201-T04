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

Table<ConstantIndex, ConstantValue> PKB::constantTable_                     = Table<ConstantIndex, ConstantValue>();
Table<VariableIndex, VariableName>  PKB::variableTable_                     = Table<VariableIndex, VariableName>();
Table<ProcedureIndex, ProcedureName> PKB::procedureTable_                   = Table<ProcedureIndex, ProcedureName>();

Table<StmtNumber, VariableIndex>  PKB::controlVariableTable_                = Table<StmtNumber, VariableIndex>();
Table<StmtNumber, ProcedureIndex> PKB::callTable_                           = Table<StmtNumber, ProcedureIndex>();
Table<StmtNumber, Symbol>    PKB::stmtTable_                                = Table<StmtNumber, Symbol>();
Table<StmtNumber, Symbol>    PKB::stmtlistTable_                            = Table<StmtNumber, Symbol>();

Table<Priority, Symbol> PKB::priorityTable_                                 = Table<Priority, Symbol>();

Table<StmtNumber, Expression> PKB::expressionTable_                         = Table<StmtNumber, Expression>();
Table<StmtNumber, SubExpression> PKB::subExpressionTable_                   = Table<StmtNumber, SubExpression>();

Table<ProcedureIndex, ProcedureIndex> PKB::callsTable_                      = Table<ProcedureIndex, ProcedureIndex>();
TransitiveTable<ProcedureIndex, ProcedureIndex> PKB::callsTransitiveTable_  = TransitiveTable<ProcedureIndex, ProcedureIndex>();

Table<StmtNumber, VariableIndex> PKB::modifiesTable_                        = Table<StmtNumber, VariableIndex>();
Table<ProcedureIndex, VariableIndex> PKB::modifiesProcedureTable_           = Table<ProcedureIndex, VariableIndex>();

Table<StmtNumber, VariableIndex> PKB::usesTable_                            = Table<StmtNumber, VariableIndex>();
Table<ProcedureIndex, VariableIndex> PKB::usesProcedureTable_               = Table<ProcedureIndex, VariableIndex>();

vector<vector<StmtNumber>> PKB::parentMatrix_;
vector<vector<StmtNumber>> PKB::parentTransitiveMatrix_;
Table<StmtNumber, StmtNumber> PKB::parentTable_                             = Table<StmtNumber, StmtNumber>();
TransitiveTable<StmtNumber, StmtNumber> PKB::parentTransitiveTable_         = TransitiveTable<StmtNumber, StmtNumber>();

vector<vector<StmtNumber>> PKB::followsMatrix_;
vector<vector<StmtNumber>> PKB::followsTransitiveMatrix_;
Table<StmtNumber, StmtNumber> PKB::followsTable_                            = Table<StmtNumber, StmtNumber>();
TransitiveTable<StmtNumber, StmtNumber> PKB::followsTransitiveTable_        = TransitiveTable<StmtNumber, StmtNumber>();

vector<vector<StmtNumber>> PKB::nextMatrix_;
vector<vector<StmtNumber>> PKB::nextTransitiveMatrix_;
Table<StmtNumber, StmtNumber> PKB::nextTable_                               = Table<StmtNumber, StmtNumber>();
TransitiveTable<StmtNumber, StmtNumber> PKB::nextTransitiveTable_           = TransitiveTable<StmtNumber, StmtNumber>();

/* START - Constant table functions */

void PKB::InsertConstantValue(ConstantIndex constantIndex, ConstantValue constantValue) {
    if (!HasConstantValue(constantValue)) {
        constantTable_.insert(constantIndex, constantValue);
    }
}

bool PKB::HasConstantValue(ConstantValue constantValue) {
    return constantTable_.hasValue(constantValue);
}

ConstantIndex PKB::GetConstantIndex(ConstantValue constantValue) {
    return (constantTable_.hasValue(constantValue)) ? constantTable_.getKey(constantValue) : 0;
}

ConstantValue PKB::GetConstantValue(ConstantIndex constantIndex) {
    return (constantTable_.hasKey(constantIndex)) ? constantTable_.getValue(constantIndex) : "";
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

void PKB::InsertVariableName(VariableIndex variableIndex, VariableName variableName) {
    if (!HasVariableName(variableName)) {
        variableTable_.insert(variableIndex, variableName);
    }
}

bool PKB::HasVariableName(VariableName variableName) {
    return variableTable_.hasValue(variableName);
}

VariableIndex PKB::GetVariableIndex(VariableName variableName) {
    return (variableTable_.hasValue(variableName)) ? variableTable_.getKey(variableName) : 0;
}

VariableName PKB::GetVariableName(VariableIndex variableIndex) {
    return (variableTable_.hasKey(variableIndex)) ? variableTable_.getValue(variableIndex) : "";
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

void PKB::InsertProcedureName(ProcedureIndex procedureIndex, ProcedureName procedureName) {
    if (!HasProcedureName(procedureName)) {
        numberOfProcedure_++;
        procedureTable_.insert(procedureIndex, procedureName);
    }
}

bool PKB::HasProcedureName(ProcedureName procedureName) {
    return procedureTable_.hasValue(procedureName);
}

ProcedureIndex PKB::GetProcedureIndex(ProcedureName procedureName) {
    return (procedureTable_.hasValue(procedureName)) ? procedureTable_.getKey(procedureName) : 0;
}

ProcedureName PKB::GetProcedureName(ProcedureIndex procedureIndex) {
    return (procedureTable_.hasKey(procedureIndex)) ? procedureTable_.getValue(procedureIndex) : "";
}

vector<ProcedureName> PKB::GetAllProcedureNames() {
    set<ProcedureName> result = procedureTable_.getValues();

    vector<ProcedureName> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

vector<ProcedureIndex> PKB::GetAllProcedureIndexes() {
    set<ProcedureIndex> result = procedureTable_.getKeys();

    vector<ProcedureIndex> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintProcedureTable() {
    procedureTable_.printTable();
}

/* END   - Procedure table functions */
/* START - Control variable table functions */

void PKB::InsertControlVariable(StmtNumber stmtNumber, VariableIndex variableIndex) {
    controlVariableTable_.insert(stmtNumber, variableIndex);
}

bool PKB::HasControlVariableIndex(VariableIndex variableIndex) {
    return controlVariableTable_.hasValue(variableIndex);
}

bool PKB::HasControlVariableIndexAtStmtNumber(StmtNumber stmtNumber, VariableIndex variableIndex) {
    return controlVariableTable_.hasKeyToValue(stmtNumber, variableIndex);
}

VariableIndex PKB::GetControlVariableIndex(StmtNumber stmtNumber) {
    return (controlVariableTable_.hasKey(stmtNumber)) ? controlVariableTable_.getValue(stmtNumber) : 0;
}

vector<VariableIndex> PKB::GetAllControlVariableIndexes() {
    set<VariableIndex> result = controlVariableTable_.getValues();

    vector<VariableIndex> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintControlVariableTable() {
    controlVariableTable_.printTable();
}

/* END   - Control variable table functions */
/* START - Call table functions */

void PKB::InsertCallStmt(StmtNumber stmtNumber, ProcedureIndex procedureIndex) {
    callTable_.insert(stmtNumber, procedureIndex);
}

ProcedureIndex PKB::GetCallStmtProcedureIndex(StmtNumber stmtNumber) {
    return (callTable_.hasKey(stmtNumber)) ? callTable_.getValue(stmtNumber) : 0;
}

void PKB::PrintCallTable() {
    callTable_.printTable();
}

/* END   - Call table functions */
/* START - Stmt table functions */

void PKB::InsertStmt(StmtNumber stmtNumber, Symbol stmtSymbol) {
    switch (stmtSymbol) {
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

    stmtTable_.insert(stmtNumber, stmtSymbol);
}

Symbol PKB::GetStmtSymbol(StmtNumber stmtNumber) {
    return (stmtTable_.hasKey(stmtNumber)) ? stmtTable_.getValue(stmtNumber) : INVALID;
}

vector<StmtNumber> PKB::GetSymbolStmtNumbers(Symbol stmtSymbol) {
    set<StmtNumber> result = (stmtSymbol == STMT) ? stmtTable_.getKeys() : stmtTable_.getKeys(stmtSymbol);

    vector<StmtNumber> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintStmtTable() {
    stmtTable_.printTable();
}

/* END   - Stmt table functions */
/* START - Stmtlist table functions */

void PKB::InsertStmtlist(StmtNumber stmtNumber, Symbol stmtSymbol) {
    stmtlistTable_.insert(stmtNumber, stmtSymbol);
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

void PKB::PopulatePriorityTable() {
    vector<std::pair<unsigned int, Symbol>> tablesSize;

    tablesSize.push_back(std::make_pair(callsTable_.getNumberOfRelationship(),   CALLS));
    tablesSize.push_back(std::make_pair(followsTable_.getNumberOfRelationship(), FOLLOWS));
    tablesSize.push_back(std::make_pair(parentTable_.getNumberOfRelationship(),  PARENT));

    tablesSize.push_back(std::make_pair(callsTransitiveTable_.getNumberOfRelationship(),   CALLS_TRANSITIVE));
    tablesSize.push_back(std::make_pair(followsTransitiveTable_.getNumberOfRelationship(), FOLLOWS_TRANSITIVE));
    tablesSize.push_back(std::make_pair(parentTransitiveTable_.getNumberOfRelationship(),  PARENT_TRANSITIVE));

    tablesSize.push_back(std::make_pair(nextTable_.getNumberOfRelationship(), NEXT));

    tablesSize.push_back(std::make_pair(modifiesTable_.getNumberOfRelationship(),          MODIFIES));
    tablesSize.push_back(std::make_pair(modifiesProcedureTable_.getNumberOfRelationship(), MODIFIES_PROCEDURE));
    tablesSize.push_back(std::make_pair(usesTable_.getNumberOfRelationship(),              USES));
    tablesSize.push_back(std::make_pair(usesProcedureTable_.getNumberOfRelationship(),     USES_PROCEDURE));

    /* Sort the size in ascending order to determine the priority. */
    std::sort(tablesSize.begin(), tablesSize.end(), ComparePairAscending);

    unsigned int i = 1;
    for (; i <= tablesSize.size(); i++) {
        priorityTable_.insert((i * 10), tablesSize[i - 1].second);
    }

    /* Assign lower priority to compute on demand design abstractions. */
    priorityTable_.insert(i + 1, NEXT_TRANSITIVE);
}

Priority PKB::GetPriority(Symbol designAbstractionSymbol) {
    return (priorityTable_.hasValue(designAbstractionSymbol)) ? priorityTable_.getKey(designAbstractionSymbol) : 0;
}

void PKB::PrintPriorityTable() {
    priorityTable_.printTable();
}

/* END   - Priority table functions */
/* START - Expression table functions */

void PKB::InsertExpression(StmtNumber stmtNumber, Expression expression) {
    expressionTable_.insert(stmtNumber, expression);
}

void PKB::InsertSubExpression(StmtNumber stmtNumber, SubExpression subExpression) {
    subExpressionTable_.insert(stmtNumber, subExpression);
}

bool PKB::IsExactPattern(StmtNumber stmtNumber, VariableIndex variableIndex, Expression expression) {
    switch (GetStmtSymbol(stmtNumber)) {
        default:
            break;

        case WHILE:
        case IF:
            return HasControlVariableIndexAtStmtNumber(stmtNumber, variableIndex);

        case ASSIGN:
            if (HasControlVariableIndexAtStmtNumber(stmtNumber, variableIndex)) {
                return IsExactExpression(stmtNumber, expression);
            }
    }

    return false;
}

bool PKB::IsSubPattern(StmtNumber stmtNumber, VariableIndex variableIndex, SubExpression subExpression) {
    switch (GetStmtSymbol(stmtNumber)) {
        default:
            break;

        case WHILE:
        case IF:
            return HasControlVariableIndexAtStmtNumber(stmtNumber, variableIndex);

        case ASSIGN:
            if (HasControlVariableIndexAtStmtNumber(stmtNumber, variableIndex)) {
                return IsSubExpression(stmtNumber, subExpression);
            }
    }

    return false;
}

bool PKB::HasExactExpression(Expression expression) {
    return expressionTable_.hasValue(expression);
}

bool PKB::HasSubExpression(SubExpression subExpression) {
    return subExpressionTable_.hasValue(subExpression);
}

bool PKB::IsExactExpression(StmtNumber stmtNumber, Expression expression) {
    return expressionTable_.hasKeyToValue(stmtNumber, expression);
}

bool PKB::IsSubExpression(StmtNumber stmtNumber, SubExpression subExpression) {
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

void PKB::InsertCalls(ProcedureIndex calling, ProcedureIndex called) {
    callsTable_.insert(calling, called);
}

void PKB::PopulateCallsTransitiveTable() {
    callsTransitiveTable_.generateTransitiveTable(callsTable_);
}

bool PKB::IsCalls(ProcedureIndex calling, ProcedureIndex called) {
    return callsTable_.hasKeyToValue(calling, called);
}

bool PKB::IsCallsTransitive(ProcedureIndex calling, ProcedureIndex called) {
    return callsTransitiveTable_.hasKeyToValue(calling, called);
}

set<ProcedureIndex> PKB::GetCalling(ProcedureIndex called) {
    return callsTable_.getKeys(called);
}

set<ProcedureIndex> PKB::GetCalled(ProcedureIndex calling) {
    return callsTable_.getValues(calling);
}

set<ProcedureIndex> PKB::GetCallingTransitive(ProcedureIndex called) {
    return callsTransitiveTable_.getKeys(called);
}

set<ProcedureIndex> PKB::GetCalledTransitive(ProcedureIndex calling) {
    return callsTransitiveTable_.getValues(calling);
}

unsigned int PKB::GetNumberOfCallsRelationship() {
    return callsTable_.getNumberOfRelationship();
}

unsigned int PKB::GetNumberOfCallsTransitiveRelationship() {
    return callsTransitiveTable_.getNumberOfRelationship();
}

void PKB::PrintCallsTable() {
    callsTable_.printTable();
}

void PKB::PrintCallsTransitiveTable() {
    callsTransitiveTable_.printTable();
}

/* END   - Calls table functions */
/* START - Modifies table functions */

void PKB::InsertModifies(StmtNumber stmtNumber, VariableIndex variableIndex) {
    modifiesTable_.insert(stmtNumber, variableIndex);
}

void PKB::InsertModifiesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex) {
    modifiesProcedureTable_.insert(procedureIndex, variableIndex);
}

bool PKB::IsModifies(StmtNumber stmtNumber, VariableIndex variableIndex) {
    return modifiesTable_.hasKeyToValue(stmtNumber, variableIndex);
}

bool PKB::IsModifiesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex) {
    return modifiesProcedureTable_.hasKeyToValue(procedureIndex, variableIndex);
}

set<VariableIndex> PKB::GetModifiedVariables(StmtNumber stmtNumber) {
    return modifiesTable_.getValues(stmtNumber);
}

set<StmtNumber> PKB::GetStmtNumberModifying(VariableIndex variableIndex) {
    return modifiesTable_.getKeys(variableIndex);
}

set<VariableIndex> PKB::GetProcedureModifiedVariables(ProcedureIndex procedureIndex) {
    return modifiesProcedureTable_.getValues(procedureIndex);
}

set<ProcedureIndex> PKB::GetProceduresNameModifying(VariableIndex variableIndex) {
    return modifiesProcedureTable_.getKeys(variableIndex);
}

unsigned int PKB::GetNumberOfModifiesRelationship() {
    return modifiesTable_.getNumberOfRelationship();
}

unsigned int PKB::GetNumberOfModifiesProcedureRelationship() {
    return modifiesProcedureTable_.getNumberOfRelationship();
}

void PKB::PrintModifiesTable() {
    modifiesTable_.printTable();
}

void PKB::PrintModifiesProcedureTable() {
    modifiesProcedureTable_.printTable();
}

/* END   - Modifies table functions */
/* START - Uses table functions */

void PKB::InsertUses(StmtNumber stmtNumber, VariableIndex variableIndex) {
    usesTable_.insert(stmtNumber, variableIndex);
}

void PKB::InsertUsesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex) {
    usesProcedureTable_.insert(procedureIndex, variableIndex);
}

bool PKB::IsUses(StmtNumber stmtNumber, VariableIndex variableIndex) {
    return usesTable_.hasKeyToValue(stmtNumber, variableIndex);
}

bool PKB::IsUsesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex) {
    return usesProcedureTable_.hasKeyToValue(procedureIndex, variableIndex);
}

set<VariableIndex> PKB::GetUsedVariables(StmtNumber stmtNumber) {
    return usesTable_.getValues(stmtNumber);
}

set<StmtNumber> PKB::GetStmtNumberUsing(VariableIndex variableIndex) {
    return usesTable_.getKeys(variableIndex);
}

set<VariableIndex> PKB::GetProcedureUsedVariables(ProcedureIndex procedureIndex) {
    return usesProcedureTable_.getValues(procedureIndex);
}

set<ProcedureIndex> PKB::GetProceduresNameUsing(VariableIndex variableIndex) {
    return usesProcedureTable_.getKeys(variableIndex);
}

unsigned int PKB::GetNumberOfUsesRelationship() {
    return usesTable_.getNumberOfRelationship();
}

unsigned int PKB::GetNumberOfUsesProcedureRelationship() {
    return usesProcedureTable_.getNumberOfRelationship();
}

void PKB::PrintUsesTable() {
    usesTable_.printTable();
}

void PKB::PrintUsesProcedureTable() {
    usesProcedureTable_.printTable();
}

/* END   - Uses table functions */
/* START - Parent table functions */

void PKB::InsertParent(StmtNumber parent, StmtNumber child) {
    parentTable_.insert(parent, child);
    parentMatrix_[parent][child] = 1;
}

void PKB::PopulateParentTransitiveTable() {
    parentTransitiveTable_.generateTransitiveTable(parentTable_, parentTransitiveMatrix_);
}

bool PKB::IsParent(StmtNumber parent, StmtNumber child) {
    if (parent > tableMaximumSize_ || parent <= 0) {
        return false;
    }

    if (child > tableMaximumSize_ || child <= 0) {
        return false;
    }

    return (parentMatrix_[parent][child] == 1);
}

bool PKB::IsParentTransitive(StmtNumber parent, StmtNumber child) {
    if (parent > tableMaximumSize_ || parent <= 0) {
        return false;
    }

    if (child > tableMaximumSize_ || child <= 0) {
        return false;
    }

    return (parentTransitiveMatrix_[parent][child] == 1);
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

unsigned int PKB::GetNumberOfParentRelationship() {
    return parentTable_.getNumberOfRelationship();
}

unsigned int PKB::GetNumberOfParentTransitiveRelationship() {
    return parentTransitiveTable_.getNumberOfRelationship();
}

void PKB::PrintParentTable() {
    parentTable_.printTable();
}

void PKB::PrintParentTransitiveTable() {
    parentTransitiveTable_.printTable();
}

/* END   - Parent table functions */
/* START - Follows table functions */

void PKB::InsertFollows(StmtNumber follows, StmtNumber following) {
    followsTable_.insert(follows, following);
    followsMatrix_[follows][following] = 1;
}

void PKB::PopulateFollowsTransitiveTable() {
    followsTransitiveTable_.generateTransitiveTable(followsTable_, followsTransitiveMatrix_);
}

bool PKB::IsFollows(StmtNumber follows, StmtNumber following) {
    if (follows > tableMaximumSize_ || follows <= 0) {
        return false;
    }

    if (following > tableMaximumSize_ || following <= 0) {
        return false;
    }

    return (followsMatrix_[follows][following] == 1);
}

bool PKB::IsFollowsTransitive(StmtNumber follows, StmtNumber following) {
    if (follows > tableMaximumSize_ || follows <= 0) {
        return false;
    }

    if (following > tableMaximumSize_ || following <= 0) {
        return false;
    }

    return (followsTransitiveMatrix_[follows][following] == 1);
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

unsigned int PKB::GetNumberOfFollowsRelationship() {
    return followsTable_.getNumberOfRelationship();
}

unsigned int PKB::GetNumberOfFollowsTransitiveRelationship() {
    return followsTransitiveTable_.getNumberOfRelationship();
}

void PKB::PrintFollowsTable() {
    followsTable_.printTable();
}

void PKB::PrintFollowsTransitiveTable() {
    followsTransitiveTable_.printTable();
}

/* END   - Follows table functions */
/* START - Next table functions */

void PKB::InsertControlFlowGraph(CFGNode controlFlowGraph) {
    controlFlowGraphs_.push_back(controlFlowGraph);

    /* Get all the nodes in the control flow graph. */
    set<CFGNode> visitedNodes;

    controlFlowGraph->setVisited(true);
    visitedNodes.insert(controlFlowGraph);

    queue<CFGNode> queue;
    queue.push(controlFlowGraph);

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

void PKB::InsertNext(StmtNumber current, StmtNumber next) {
    nextTable_.insert(current, next);
    nextMatrix_[current][next] = 1;
}

void PKB::PopulateNextTransitiveTable() {
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

                        nextTransitiveTable_.insert(node->getStmtNumber(), child->getStmtNumber());
                        nextTransitiveMatrix_[node->getStmtNumber()][child->getStmtNumber()] = 1;
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
    if (current > tableMaximumSize_ || current <= 0) {
        return false;
    }

    if (next > tableMaximumSize_ || next <= 0) {
        return false;
    }

    return (nextMatrix_[current][next] == 1);
}

bool PKB::IsNextTransitive(StmtNumber current, StmtNumber next) {
    if (current > tableMaximumSize_ || current <= 0) {
        return false;
    }

    if (next > tableMaximumSize_ || next <= 0) {
        return false;
    }

    /* Worst case is O(1) time complexity. */
    return (nextTransitiveMatrix_[current][next] == 1);
}

set<StmtNumber> PKB::GetNext(StmtNumber current) {
    return nextTable_.getValues(current);
}

set<StmtNumber> PKB::GetPrevious(StmtNumber next) {
    return nextTable_.getKeys(next);
}

set<StmtNumber> PKB::GetNextTransitive(StmtNumber current) {
    return nextTransitiveTable_.getValues(current);
}

set<StmtNumber> PKB::GetPreviousTransitive(StmtNumber next) {
    return nextTransitiveTable_.getKeys(next);
}

unsigned int PKB::GetNumberOfNextRelationship() {
    return nextTable_.getNumberOfRelationship();
}

unsigned int PKB::GetNumberOfNextTransitiveRelationship() {
    return nextTransitiveTable_.getNumberOfRelationship();
}

void PKB::PrintNextTable() {
    nextTable_.printTable();
}

void PKB::PrintNextTransitiveTable() {
    nextTransitiveTable_.printTable();
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

    parentMatrix_.resize(tableMaximumSize_, vector<StmtNumber>(tableMaximumSize_, 0));
    parentTransitiveMatrix_.resize(tableMaximumSize_, vector<StmtNumber>(tableMaximumSize_, 0));
    followsMatrix_.resize(tableMaximumSize_, vector<StmtNumber>(tableMaximumSize_, 0));
    followsTransitiveMatrix_.resize(tableMaximumSize_, vector<StmtNumber>(tableMaximumSize_, 0));
    nextMatrix_.resize(tableMaximumSize_, vector<StmtNumber>(tableMaximumSize_, 0));
    nextTransitiveMatrix_.resize(tableMaximumSize_, vector<StmtNumber>(tableMaximumSize_, 0));
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

    constantTable_          = Table<ConstantIndex, ConstantValue>();
    procedureTable_         = Table<ProcedureIndex, ProcedureName>();
    variableTable_          = Table<VariableIndex, VariableName>();

    controlVariableTable_   = Table<StmtNumber, VariableIndex>();
    callTable_              = Table<StmtNumber, ProcedureIndex>();
    stmtTable_              = Table<StmtNumber, Symbol>();
    stmtlistTable_          = Table<StmtNumber, Symbol>();

    priorityTable_          = Table<Priority, Symbol>();

    expressionTable_        = Table<StmtNumber, Expression>();
    subExpressionTable_     = Table<StmtNumber, SubExpression>();

    callsTable_             = Table<ProcedureIndex, ProcedureIndex>();
    callsTransitiveTable_   = TransitiveTable<ProcedureIndex, ProcedureIndex>();

    modifiesTable_          = Table<StmtNumber, VariableIndex>();
    modifiesProcedureTable_ = Table<ProcedureIndex, VariableIndex>();

    usesTable_              = Table<StmtNumber, VariableIndex>();
    usesProcedureTable_     = Table<ProcedureIndex, VariableIndex>();

    parentMatrix_.clear();
    parentTransitiveMatrix_.clear();
    parentTable_            = Table<StmtNumber, StmtNumber>();
    parentTransitiveTable_  = TransitiveTable<StmtNumber, StmtNumber>();

    followsMatrix_.clear();
    followsTransitiveMatrix_.clear();
    followsTable_           = Table<StmtNumber, StmtNumber>();
    followsTransitiveTable_ = TransitiveTable<StmtNumber, StmtNumber>();

    nextMatrix_.clear();
    nextTransitiveMatrix_.clear();
    nextTable_              = Table<StmtNumber, StmtNumber>();
    nextTransitiveTable_    = TransitiveTable<StmtNumber, StmtNumber>();
}

void PKB::ClearComputeOnDemandTables() {
    for (vector<StmtNumber> vec : nextTransitiveMatrix_) {
        vec.assign(vec.size(), 0);
    }

    nextTransitiveTable_ = TransitiveTable<StmtNumber, StmtNumber>();
}

bool PKB::ComparePairAscending(const std::pair<unsigned int, Symbol> &pairOne, const std::pair<unsigned int, Symbol> &pairTwo) {
    return pairOne.first < pairTwo.first;
}

/* START - Miscellaneous functions */

/* START - Deprecated functions */
bool PKB::HasControlVariableNameAtStmtNumber(StmtNumber stmtNumber, VariableName controlVariable) {
    return controlVariableTable_.hasKeyToValue(stmtNumber, GetVariableIndex(controlVariable));
}

ProcedureName PKB::GetCallStmtProcedureName(StmtNumber stmtNumber, std::string deprecatedFiller) {
    return (callTable_.hasKey(stmtNumber)) ? PKB::GetProcedureName(callTable_.getValue(stmtNumber)) : 0;
}

bool PKB::IsExactPattern(StmtNumber stmtNumber, VariableName variableName, Expression expression) {
    switch (GetStmtSymbol(stmtNumber)) {
        default:
            break;

        case WHILE:
        case IF:
            return HasControlVariableNameAtStmtNumber(stmtNumber, variableName);

        case ASSIGN:
            if (HasControlVariableNameAtStmtNumber(stmtNumber, variableName)) {
                return IsExactExpression(stmtNumber, expression);
            }
    }

    return false;
}

bool PKB::IsSubPattern(StmtNumber stmtNumber, VariableName variableName, SubExpression subExpression) {
    switch (GetStmtSymbol(stmtNumber)) {
        default:
            break;

        case WHILE:
        case IF:
            return HasControlVariableNameAtStmtNumber(stmtNumber, variableName);

        case ASSIGN:
            if (HasControlVariableNameAtStmtNumber(stmtNumber, variableName)) {
                return IsSubExpression(stmtNumber, subExpression);
            }
    }

    return false;
}

bool PKB::IsCalls(ProcedureName calling, ProcedureName called) {
    return callsTable_.hasKeyToValue(PKB::GetProcedureIndex(calling), PKB::GetProcedureIndex(called));
}

bool PKB::IsCallsTransitive(ProcedureName calling, ProcedureName called) {
    return callsTransitiveTable_.hasKeyToValue(PKB::GetProcedureIndex(calling), PKB::GetProcedureIndex(called));
}

set<ProcedureName> PKB::GetCalling(ProcedureName called) {
    set<ProcedureName> names;
    set<ProcedureIndex> indexes = callsTable_.getKeys(PKB::GetProcedureIndex(called));

    for (ProcedureIndex index : indexes) {
        names.insert(PKB::GetProcedureName(index));
    }

    return names;
}

set<ProcedureName> PKB::GetCalled(ProcedureName calling) {
    set<ProcedureName> names;
    set<ProcedureIndex> indexes = callsTable_.getValues(PKB::GetProcedureIndex(calling));

    for (ProcedureIndex index : indexes) {
        names.insert(PKB::GetProcedureName(index));
    }

    return names;
}

bool PKB::IsModifies(StmtNumber stmtNumber, VariableName variableName) {
    return modifiesTable_.hasKeyToValue(stmtNumber, PKB::GetVariableIndex(variableName));
}

bool PKB::IsModifiesProcedure(ProcedureName procedureName, VariableName variableName) {
    return modifiesProcedureTable_.hasKeyToValue(PKB::GetProcedureIndex(procedureName), PKB::GetVariableIndex(variableName));
}

set<StmtNumber> PKB::GetStmtNumberModifying(VariableName variableName) {
    return modifiesTable_.getKeys(PKB::GetVariableIndex(variableName));
}

set<VariableName> PKB::GetProcedureModifiedVariables(ProcedureName procedureName) {
    set<VariableName> names;
    set<VariableIndex> indexes = modifiesProcedureTable_.getValues(PKB::GetProcedureIndex(procedureName));

    for (VariableIndex index : indexes) {
        names.insert(PKB::GetVariableName(index));
    }

    return names;
}

bool PKB::IsUses(StmtNumber stmtNumber, VariableName variableName) {
    return usesTable_.hasKeyToValue(stmtNumber, PKB::GetVariableIndex(variableName));
}

bool PKB::IsUsesProcedure(ProcedureName procedureName, VariableName variableName) {
    return usesProcedureTable_.hasKeyToValue(PKB::GetProcedureIndex(procedureName), PKB::GetVariableIndex(variableName));
}

set<StmtNumber> PKB::GetStmtNumberUsing(VariableName variableName) {
    return usesTable_.getKeys(PKB::GetVariableIndex(variableName));
}

set<VariableName> PKB::GetProcedureUsedVariables(ProcedureName procedureName) {
    set<VariableName> names;
    set<VariableIndex> indexes = usesProcedureTable_.getValues(PKB::GetProcedureIndex(procedureName));

    for (VariableIndex index : indexes) {
        names.insert(PKB::GetVariableName(index));
    }

    return names;
}
/* END   - Deprecated functions */

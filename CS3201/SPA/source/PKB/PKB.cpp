#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "Frontend/DesignExtractor.h"
#include "PKB/PKB.h"
#include "CFGNode.h"

using std::map;
using std::queue;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

unsigned int PKB::numberOfProcedure_ = 0;
unsigned int PKB::numberOfAssign_    = 0;
unsigned int PKB::numberOfWhile_     = 0;
unsigned int PKB::numberOfIf_        = 0;
unsigned int PKB::numberOfCall_      = 0;

unsigned int PKB::tableMaximumSize_  = 0;

unordered_map<StmtNumber, CFGNode*> PKB::controlFlowGraphNodes_;
unordered_map<StmtNumber, StmtNumber> PKB::procedureFirstAndLastStmtNumber_;

Table<ConstantIndex, ConstantValue> PKB::constantTable_;
Table<VariableIndex, VariableName> PKB::variableTable_;
Table<ProcedureIndex, ProcedureName> PKB::procedureTable_;

VectorTable<StmtNumber, VariableIndex>  PKB::controlVariableTable_ = VectorTable<StmtNumber, ProcedureIndex>();
VectorTable<StmtNumber, ProcedureIndex> PKB::callTable_            = VectorTable<StmtNumber, ProcedureIndex>();
VectorTable<StmtNumber, Symbol> PKB::stmtTable_                    = VectorTable<StmtNumber, Symbol>();
VectorTable<StmtNumber, Symbol> PKB::stmtlistTable_                = VectorTable<StmtNumber, Symbol>();

Table<Priority, Symbol> PKB::priorityTable_;

Table<StmtNumber, Expression> PKB::expressionTable_;
VectorTable<StmtNumber, SubExpression> PKB::subExpressionTable_ = VectorTable<StmtNumber, SubExpression>();

VectorTable<ProcedureIndex, ProcedureIndex> PKB::callsTable_               = VectorTable<ProcedureIndex, ProcedureIndex>();
TransitiveTable<ProcedureIndex, ProcedureIndex> PKB::callsTransitiveTable_ = TransitiveTable<ProcedureIndex, ProcedureIndex>();

VectorTable<StmtNumber, VariableIndex> PKB::modifiesTable_                  = VectorTable<StmtNumber, VariableIndex>();
VectorTable<ProcedureIndex, VariableIndex> PKB::modifiesProcedureTable_     = VectorTable<ProcedureIndex, VariableIndex>();
VectorTable<StmtNumber, VariableIndex> PKB::modifiesProcedureIsolatedTable_ = VectorTable<StmtNumber, VariableIndex>();

VectorTable<StmtNumber, VariableIndex> PKB::usesTable_                  = VectorTable<StmtNumber, VariableIndex>();
VectorTable<ProcedureIndex, VariableIndex> PKB::usesProcedureTable_     = VectorTable<ProcedureIndex, VariableIndex>();
VectorTable<StmtNumber, VariableIndex> PKB::usesProcedureIsolatedTable_ = VectorTable<StmtNumber, VariableIndex>();

Matrix PKB::parentMatrix_;
Matrix PKB::parentTransitiveMatrix_;

VectorTable<StmtNumber, StmtNumber> PKB::parentTable_               = VectorTable<StmtNumber, StmtNumber>();
TransitiveTable<StmtNumber, StmtNumber> PKB::parentTransitiveTable_ = TransitiveTable<StmtNumber, StmtNumber>();

Matrix PKB::followsMatrix_;
Matrix PKB::followsTransitiveMatrix_;

VectorTable<StmtNumber, StmtNumber> PKB::followsTable_ = VectorTable<StmtNumber, StmtNumber>();

Matrix PKB::nextMatrix_;
Matrix PKB::nextTransitiveMatrix_;

VectorTable<StmtNumber, StmtNumber> PKB::nextTable_ = VectorTable<StmtNumber, StmtNumber>();

Matrix PKB::affectsMatrix_;
Matrix PKB::affectsTransitiveMatrix_;

VectorTable<StmtNumber, StmtNumber> PKB::affectsTable_ = VectorTable<StmtNumber, StmtNumber>();

/* START - Constant table functions */

void PKB::InsertConstantValue(ConstantIndex constantIndex, ConstantValue constantValue) {
    if (!constantTable_.hasValue(constantValue)) {
        constantTable_.insert(constantIndex, constantValue);
    }
}

ConstantIndex PKB::GetConstantIndex(ConstantValue constantValue) {
    return (constantTable_.hasValue(constantValue)) ? constantTable_.getKey(constantValue) : 0;
}

ConstantValue PKB::GetConstantValue(ConstantIndex constantIndex) {
    return (constantTable_.hasKey(constantIndex)) ? constantTable_.getValue(constantIndex) : "";
}

vector<ConstantValue> PKB::GetAllConstantValues() {
    return constantTable_.getValues();
}

void PKB::PrintConstantTable() {
    constantTable_.print();
}

/* END   - Constant table functions */
/* START - Variable table functions */

void PKB::InsertVariableName(VariableIndex variableIndex, VariableName variableName) {
    if (!variableTable_.hasValue(variableName)) {
        variableTable_.insert(variableIndex, variableName);
    }
}

VariableIndex PKB::GetVariableIndex(VariableName variableName) {
    return (variableTable_.hasValue(variableName)) ? variableTable_.getKey(variableName) : 0;
}

VariableName PKB::GetVariableName(VariableIndex variableIndex) {
    return (variableTable_.hasKey(variableIndex)) ? variableTable_.getValue(variableIndex) : "";
}

vector<VariableName> PKB::GetAllVariableNames() {
    return variableTable_.getValues();
}

void PKB::PrintVariableTable() {
    variableTable_.print();
}

/* END   - Variable table functions */
/* START - Procedure table functions */

void PKB::InsertProcedureName(ProcedureIndex procedureIndex, ProcedureName procedureName) {
    if (!procedureTable_.hasValue(procedureName)) {
        numberOfProcedure_++;
        procedureTable_.insert(procedureIndex, procedureName);
    }
}

ProcedureIndex PKB::GetProcedureIndex(ProcedureName procedureName) {
    return (procedureTable_.hasValue(procedureName)) ? procedureTable_.getKey(procedureName) : 0;
}

ProcedureName PKB::GetProcedureName(ProcedureIndex procedureIndex) {
    return (procedureTable_.hasKey(procedureIndex)) ? procedureTable_.getValue(procedureIndex) : "";
}

vector<ProcedureName> PKB::GetAllProcedureNames() {
    return procedureTable_.getValues();
}

vector<ProcedureIndex> PKB::GetAllProcedureIndexes() {
    return procedureTable_.getKeys();
}

void PKB::PrintProcedureTable() {
    procedureTable_.print();
}

/* END   - Procedure table functions */
/* START - Control variable table functions */

void PKB::InsertControlVariable(StmtNumber stmtNumber, VariableIndex variableIndex) {
    controlVariableTable_.insert(stmtNumber, variableIndex);
}

bool PKB::HasControlVariableIndexAtStmtNumber(StmtNumber stmtNumber, VariableIndex variableIndex) {
    return controlVariableTable_.hasKeyToValue(stmtNumber, variableIndex);
}

VariableIndex PKB::GetControlVariableIndex(StmtNumber stmtNumber) {
    if (controlVariableTable_.hasKey(stmtNumber)) {
        vector<VariableIndex> variableIndexes = controlVariableTable_.getValues(stmtNumber);

        /* It is expected that one statement has only one control variable. */
        for (VariableIndex variableIndex : variableIndexes) {
            return variableIndex;
        }
    }

    return 0;
}

vector<VariableIndex> PKB::GetAllControlVariableIndexes() {
    return controlVariableTable_.getValues();
}

void PKB::PrintControlVariableTable() {
    controlVariableTable_.print();
}

/* END   - Control variable table functions */
/* START - Call table functions */

void PKB::InsertCallStmt(StmtNumber stmtNumber, ProcedureIndex procedureIndex) {
    callTable_.insert(stmtNumber, procedureIndex);
}

ProcedureIndex PKB::GetCallStmtProcedureIndex(StmtNumber stmtNumber) {
    if (callTable_.hasKey(stmtNumber)) {
        vector<ProcedureIndex> procedureIndexes = callTable_.getValues(stmtNumber);

        /* It is expected that one call statement calls only one procedure. */
        for (ProcedureIndex procedureIndex : procedureIndexes) {
            return procedureIndex;
        }
    }

    return 0;
}

void PKB::PrintCallTable() {
    callTable_.print();
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
    if (stmtTable_.hasKey(stmtNumber)) {
        vector<Symbol> symbols = stmtTable_.getValues(stmtNumber);

        /* It is expected that a statement can only be represented by one symbol. */
        for (Symbol symbol : symbols) {
            return symbol;
        }
    }

    return INVALID;
}

vector<StmtNumber> PKB::GetSymbolStmtNumbers(Symbol stmtSymbol) {
    return (stmtSymbol == STMT) ? stmtTable_.getKeys() : stmtTable_.getKeys(stmtSymbol);
}

void PKB::PrintStmtTable() {
    stmtTable_.print();
}

/* END   - Stmt table functions */
/* START - Stmtlist table functions */

void PKB::InsertStmtlist(StmtNumber stmtNumber, Symbol stmtSymbol) {
    stmtlistTable_.insert(stmtNumber, stmtSymbol);
}

vector<StmtNumber> PKB::GetAllStmtlistsStmtNumber() {
    return stmtlistTable_.getKeys();
}

void PKB::PrintStmtlistTable() {
    stmtlistTable_.print();
}

/* END   - Stmtlist table functions */
/* START - Priority table functions */

void PKB::PopulatePriorityTable() {
    vector<std::pair<unsigned int, Symbol>> tablesSize;

    tablesSize.push_back(std::make_pair(callsTable_.getNumberOfRelationship(),   CALLS));
    tablesSize.push_back(std::make_pair(followsTable_.getNumberOfRelationship(), FOLLOWS));
    tablesSize.push_back(std::make_pair(parentTable_.getNumberOfRelationship(),  PARENT));

    tablesSize.push_back(std::make_pair(callsTransitiveTable_.getNumberOfRelationship(),   CALLS_TRANSITIVE));
    tablesSize.push_back(std::make_pair(followsTransitiveMatrix_.getNumberOfRelationship(), FOLLOWS_TRANSITIVE));
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
    priorityTable_.insert((i + 1) * 10, NEXT_TRANSITIVE);
    priorityTable_.insert((i + 2) * 10, AFFECTS);
    priorityTable_.insert((i + 3) * 10, AFFECTS_TRANSITIVE);
}

Priority PKB::GetPriority(Symbol designAbstractionSymbol) {
    return (priorityTable_.hasValue(designAbstractionSymbol)) ? priorityTable_.getKey(designAbstractionSymbol) : 0;
}

void PKB::PrintPriorityTable() {
    priorityTable_.print();
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
    expressionTable_.print();
}

void PKB::PrintSubExpressionTable() {
    subExpressionTable_.print();
}

/* END   - Expression table functions */
/* START - Calls table functions */

void PKB::InsertCalls(ProcedureIndex calling, ProcedureIndex called) {
    callsTable_.insert(calling, called);
}

void PKB::PopulateCallsTransitive() {
    callsTransitiveTable_.generateTransitiveTable(callsTable_);
}

bool PKB::IsCalls(ProcedureIndex calling, ProcedureIndex called) {
    return callsTable_.hasKeyToValue(calling, called);
}

bool PKB::IsCallsTransitive(ProcedureIndex calling, ProcedureIndex called) {
    return callsTransitiveTable_.hasKeyToValue(calling, called);
}

vector<ProcedureIndex> PKB::GetCalling(ProcedureIndex called) {
    return callsTable_.getKeys(called);
}

vector<ProcedureIndex> PKB::GetCalled(ProcedureIndex calling) {
    return callsTable_.getValues(calling);
}

unsigned int PKB::GetNumberOfCallsRelationship() {
    return callsTable_.getNumberOfRelationship();
}

void PKB::PrintCallsTable() {
    callsTable_.print();
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

void PKB::InsertModifiesProcedureIsolated(StmtNumber firstStmtNumber, VariableIndex variableIndex) {
    modifiesProcedureIsolatedTable_.insert(firstStmtNumber, variableIndex);
}

bool PKB::IsModifies(StmtNumber stmtNumber, VariableIndex variableIndex) {
    return modifiesTable_.hasKeyToValue(stmtNumber, variableIndex);
}

bool PKB::IsModifiesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex) {
    return modifiesProcedureTable_.hasKeyToValue(procedureIndex, variableIndex);
}

vector<VariableIndex> PKB::GetModifiedVariables(StmtNumber stmtNumber) {
    return modifiesTable_.getValues(stmtNumber);
}

vector<StmtNumber> PKB::GetStmtNumberModifying(VariableIndex variableIndex) {
    return modifiesTable_.getKeys(variableIndex);
}

vector<VariableIndex> PKB::GetProcedureModifiedVariables(ProcedureIndex procedureIndex) {
    return modifiesProcedureTable_.getValues(procedureIndex);
}

vector<ProcedureIndex> PKB::GetProceduresNameModifying(VariableIndex variableIndex) {
    return modifiesProcedureTable_.getKeys(variableIndex);
}

vector<StmtNumber> PKB::GetIsolatedProcedureStmtNumberModifying(VariableIndex variableIndex) {
    return modifiesProcedureIsolatedTable_.getKeys(variableIndex);
}

unsigned int PKB::GetNumberOfModifiesRelationship() {
    return modifiesTable_.getNumberOfRelationship();
}

unsigned int PKB::GetNumberOfModifiesProcedureRelationship() {
    return modifiesProcedureTable_.getNumberOfRelationship();
}

void PKB::PrintModifiesTable() {
    modifiesTable_.print();
}

void PKB::PrintModifiesProcedureTable() {
    modifiesProcedureTable_.print();
}

/* END   - Modifies table functions */
/* START - Uses table functions */

void PKB::InsertUses(StmtNumber stmtNumber, VariableIndex variableIndex) {
    usesTable_.insert(stmtNumber, variableIndex);
}

void PKB::InsertUsesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex) {
    usesProcedureTable_.insert(procedureIndex, variableIndex);
}

void PKB::InsertUsesProcedureIsolated(StmtNumber firstStmtNumber, VariableIndex variableIndex) {
    usesProcedureIsolatedTable_.insert(firstStmtNumber, variableIndex);
}

bool PKB::IsUses(StmtNumber stmtNumber, VariableIndex variableIndex) {
    return usesTable_.hasKeyToValue(stmtNumber, variableIndex);
}

bool PKB::IsUsesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex) {
    return usesProcedureTable_.hasKeyToValue(procedureIndex, variableIndex);
}

vector<VariableIndex> PKB::GetUsedVariables(StmtNumber stmtNumber) {
    return usesTable_.getValues(stmtNumber);
}

vector<StmtNumber> PKB::GetStmtNumberUsing(VariableIndex variableIndex) {
    return usesTable_.getKeys(variableIndex);
}

vector<VariableIndex> PKB::GetProcedureUsedVariables(ProcedureIndex procedureIndex) {
    return usesProcedureTable_.getValues(procedureIndex);
}

vector<ProcedureIndex> PKB::GetProceduresNameUsing(VariableIndex variableIndex) {
    return usesProcedureTable_.getKeys(variableIndex);
}

vector<StmtNumber> PKB::GetIsolatedProcedureStmtNumberUsing(VariableIndex variableIndex) {
    return usesProcedureIsolatedTable_.getKeys(variableIndex);
}

unsigned int PKB::GetNumberOfUsesRelationship() {
    return usesTable_.getNumberOfRelationship();
}

unsigned int PKB::GetNumberOfUsesProcedureRelationship() {
    return usesProcedureTable_.getNumberOfRelationship();
}

void PKB::PrintUsesTable() {
    usesTable_.print();
}

void PKB::PrintUsesProcedureTable() {
    usesProcedureTable_.print();
}

/* END   - Uses table functions */
/* START - Parent table functions */

void PKB::InsertParent(StmtNumber parent, StmtNumber child) {
    if (!parentTable_.hasKeyToValue(parent, child)) {
        parentMatrix_.toggleRowColumn(parent, child);
    }

    parentTable_.insert(parent, child);
}

void PKB::PopulateParentTransitive() {
    parentTransitiveTable_.generateTransitiveTable(parentTable_);
    parentTransitiveMatrix_.generateMatrix(parentTable_);
}

bool PKB::IsParent(StmtNumber parent, StmtNumber child) {
    /* Validate if exceed matrix's range. */
    if (parent > tableMaximumSize_ || parent <= 0 || child > tableMaximumSize_ || child <= 0) {
        return false;
    }

    return parentMatrix_.isRowColumnToggled(parent, child);
}

bool PKB::IsParentTransitive(StmtNumber parent, StmtNumber child) {
    /* Validate if exceed matrix's range. */
    if (parent > tableMaximumSize_ || parent <= 0 || child > tableMaximumSize_ || child <= 0) {
        return false;
    }

    return parentTransitiveMatrix_.isRowColumnToggled(parent, child);
}

vector<StmtNumber> PKB::GetParent(StmtNumber child) {
    return parentTable_.getKeys(child);
}

vector<StmtNumber> PKB::GetChildren(StmtNumber parent) {
    return parentTable_.getValues(parent);
}

vector<StmtNumber> PKB::GetParentsTransitive(StmtNumber child) {
    return parentTransitiveTable_.getKeys(child);
}

vector<StmtNumber> PKB::GetChildrenTransitive(StmtNumber parent) {
    return parentTransitiveTable_.getValues(parent);
}

unsigned int PKB::GetNumberOfParentRelationship() {
    return parentTable_.getNumberOfRelationship();
}

void PKB::PrintParentTable() {
    parentTable_.print();
}

void PKB::PrintParentTransitiveTable() {
    parentTransitiveTable_.printTable();
}

/* END   - Parent table functions */
/* START - Follows table functions */

void PKB::InsertFollows(StmtNumber follows, StmtNumber following) {
    if (!followsTable_.hasKeyToValue(follows, following)) {
        followsMatrix_.toggleRowColumn(follows, following);
    }

    followsTable_.insert(follows, following);
}

void PKB::PopulateFollowsTransitive() {
    followsTransitiveMatrix_.generateMatrix(followsTable_);
}

bool PKB::IsFollows(StmtNumber follows, StmtNumber following) {
    /* Validate if exceed matrix's range. */
    if (follows > tableMaximumSize_ || follows <= 0 || following > tableMaximumSize_ || following <= 0) {
        return false;
    }

    return followsMatrix_.isRowColumnToggled(follows, following);
}

bool PKB::IsFollowsTransitive(StmtNumber follows, StmtNumber following) {
    /* Validate if exceed matrix's range. */
    if (follows > tableMaximumSize_ || follows <= 0 || following > tableMaximumSize_ || following <= 0) {
        return false;
    }

    return followsTransitiveMatrix_.isRowColumnToggled(follows, following);
}

vector<StmtNumber> PKB::GetFollows(StmtNumber following) {
    return followsTable_.getKeys(following);
}

vector<StmtNumber> PKB::GetFollowing(StmtNumber follows) {
    return followsTable_.getValues(follows);
}

unsigned int PKB::GetNumberOfFollowsRelationship() {
    return followsTable_.getNumberOfRelationship();
}

void PKB::PrintFollowsTable() {
    followsTable_.print();
}

/* END   - Follows table functions */
/* START - Next table functions */

void PKB::InsertControlFlowGraphNode(StmtNumber stmtNumber, CFGNode* controlFlowGraphNode) {
    controlFlowGraphNodes_.insert(std::make_pair(stmtNumber, controlFlowGraphNode));
}

void PKB::InsertNext(StmtNumber current, StmtNumber next) {
    if (!nextTable_.hasKeyToValue(current, next)) {
        nextMatrix_.toggleRowColumn(current, next);
    }

    nextTable_.insert(current, next);
}

bool PKB::IsNext(StmtNumber current, StmtNumber next) {
    /* Validate if exceed matrix's range. */
    if (current > tableMaximumSize_ || current <= 0 || next > tableMaximumSize_ || next <= 0) {
        return false;
    }

    return nextMatrix_.isRowColumnToggled(current, next);
}

bool PKB::IsNextTransitive(StmtNumber current, StmtNumber next) {
    /* Validate if exceed matrix's range. */
    if (current > tableMaximumSize_ || current <= 0 || next > tableMaximumSize_ || next <= 0) {
        return false;
    }

    /* Validate if "current" and "next" is in same procedure. */
    for (auto &pair : procedureFirstAndLastStmtNumber_) {
        if (current >= pair.first && current <= pair.second && next >= pair.first && next <= pair.second) {
            if (!nextTransitiveMatrix_.isRowPopulated(current)) {
                nextTransitiveMatrix_.setPopulated(current);

                DesignExtractor::getInstance().computeNextTransitive(controlFlowGraphNodes_[current], nextTransitiveMatrix_);
            }

            return nextTransitiveMatrix_.isRowColumnToggled(current, next);
        }
    }

    return false;
}

vector<StmtNumber> PKB::GetNext(StmtNumber current) {
    return nextTable_.getValues(current);
}

vector<StmtNumber> PKB::GetPrevious(StmtNumber next) {
    return nextTable_.getKeys(next);
}

unsigned int PKB::GetNumberOfNextRelationship() {
    return nextTable_.getNumberOfRelationship();
}

void PKB::PrintNextTable() {
    nextTable_.print();
}

/* END   - Next table functions */
/* START - Affects table functions */

bool PKB::IsAffects() {
    /* If nothing is affecting yet, try all those not populated ones. */
    for (auto &pair : controlFlowGraphNodes_) {
        if (affectsTable_.getNumberOfRelationship() > 0) {
            return true;
        }

        GetAffecting(pair.first);
    }

    return false;
}

bool PKB::IsAffects(StmtNumber affecting, StmtNumber affected) {
    /* Validate if exceed matrix's range. */
    if (affecting > tableMaximumSize_ || affecting <= 0 || affected > tableMaximumSize_ || affected <= 0) {
        return false;
    }

    /* Validate if "affecting" and "affected" is in same procedure. */
    for (auto &pair : procedureFirstAndLastStmtNumber_) {
        if (affecting >= pair.first && affecting <= pair.second && affected >= pair.first && affected <= pair.second) {
            /* Validate only assign statements are allowed. */
            if (GetStmtSymbol(affecting) != ASSIGN || GetStmtSymbol(affected) != ASSIGN) {
                return false;
            }

            if (affectsMatrix_.isRowPopulated(affecting)) {
                return affectsMatrix_.isRowColumnToggled(affecting, affected);
            }

            CFGNode* node = controlFlowGraphNodes_[affecting];
            vector<StmtNumber> isolatedProcUses = GetIsolatedProcedureStmtNumberUsing(node->getModify());

            /* Validate if there is any assign in procedure that is using the modify variable. */
            if (isolatedProcUses.empty()) {
                return false;
            }

            if (std::find(isolatedProcUses.begin(), isolatedProcUses.end(), pair.first) == isolatedProcUses.end()) {
                return false;
            }

            affectsMatrix_.setPopulated(affecting);

            DesignExtractor::getInstance().computeAffects(node, affectsMatrix_, affectsTable_);

            return affectsMatrix_.isRowColumnToggled(affecting, affected);
        }
    }

    return false;
}

bool PKB::IsAffectsTransitive(StmtNumber affecting, StmtNumber affected) {
    /* Validate if exceed matrix's range. */
    if (affecting > tableMaximumSize_ || affecting <= 0 || affected > tableMaximumSize_ || affected <= 0) {
        return false;
    }

    /* Validate if "affecting" and "affected" is in same procedure. */
    for (auto &pair : procedureFirstAndLastStmtNumber_) {
        if (affecting >= pair.first && affecting <= pair.second && affected >= pair.first && affected <= pair.second) {
            /* Validate only assign statements are allowed. */
            if (GetStmtSymbol(affecting) != ASSIGN || GetStmtSymbol(affected) != ASSIGN) {
                return false;
            }

            if (affectsTransitiveMatrix_.isRowPopulated(affecting)) {
                return affectsTransitiveMatrix_.isRowColumnToggled(affecting, affected);
            }

            CFGNode* node = controlFlowGraphNodes_[affecting];
            vector<StmtNumber> isolatedProcUses = GetIsolatedProcedureStmtNumberUsing(node->getModify());

            /* Validate if there is any assign in procedure that is using the modify variable. */
            if (isolatedProcUses.empty()) {
                return false;
            }

            if (std::find(isolatedProcUses.begin(), isolatedProcUses.end(), pair.first) == isolatedProcUses.end()) {
                return false;
            }

            affectsMatrix_.setPopulated(affecting);

            DesignExtractor::getInstance().computeAffectsTransitive(node, affectsTransitiveMatrix_);

            return affectsTransitiveMatrix_.isRowColumnToggled(affecting, affected);
        }
    }

    return false;
}

vector<StmtNumber> PKB::GetAffected(StmtNumber affecting) {
    /* Validate if exceed matrix's range. */
    if (affecting > tableMaximumSize_ || affecting <= 0) {
        return vector<StmtNumber>();
    }

    /* Validate only assign statements are allowed. */
    if (GetStmtSymbol(affecting) != ASSIGN) {
        return vector<StmtNumber>();
    }

    if (affectsMatrix_.isRowPopulated(affecting)) {
        return affectsTable_.getValues(affecting);
    }

    CFGNode* node = controlFlowGraphNodes_[affecting];

    /* Validate if there is any assign in procedure that is using the modify variable. */
    for (auto &pair : procedureFirstAndLastStmtNumber_) {
        if (affecting >= pair.first && affecting <= pair.second) {
            vector<StmtNumber> isolatedProcUses = GetIsolatedProcedureStmtNumberUsing(node->getModify());

            if (isolatedProcUses.empty()) {
                return vector<StmtNumber>();
            }

            if (std::find(isolatedProcUses.begin(), isolatedProcUses.end(), pair.first) == isolatedProcUses.end()) {
                return vector<StmtNumber>();
            }

            break;
        }
    }

    affectsMatrix_.setPopulated(affecting);

    DesignExtractor::getInstance().computeAffects(node, affectsMatrix_, affectsTable_);

    return affectsTable_.getValues(affecting);
}

vector<StmtNumber> PKB::GetAffecting(StmtNumber affected) {
    /* Validate if exceed matrix's range. */
    if (affected > tableMaximumSize_ || affected <= 0) {
        return vector<StmtNumber>();
    }

    /* Validate only assign statements are allowed. */
    if (GetStmtSymbol(affected) != ASSIGN) {
        return vector<StmtNumber>();
    }

    /* If already computed. */
    if (affectsTable_.hasValue(affected)) {
        return affectsTable_.getKeys(affected);
    }

    CFGNode* node = controlFlowGraphNodes_[affected];

    /* Validate if uses is empty, nothing can be affecting it. */
    if (node->getUses().empty()) {
        return vector<StmtNumber>();
    }

    for (auto &pair : procedureFirstAndLastStmtNumber_) {
        if (affected >= pair.first && affected <= pair.second) {
            vector<VariableIndex> uses = node->getUses();

            for (VariableIndex variableIndex : uses) {
                vector<StmtNumber> isolatedProcModifies = GetIsolatedProcedureStmtNumberUsing(variableIndex);

                if (isolatedProcModifies.empty()) {
                    continue;
                }

                if (std::find(isolatedProcModifies.begin(), isolatedProcModifies.end(), pair.first) == isolatedProcModifies.end()) {
                    continue;
                }

                if (DesignExtractor::getInstance().computeAffecting(node, variableIndex, affectsMatrix_, affectsTable_)) {
                    return affectsTable_.getKeys(affected);
                }
            }

            break;
        }
    }

    return vector<StmtNumber>();
}

void PKB::PrintAffectsTable() {
    affectsTable_.print();
}

void PKB::PrintAffectsTransitiveMatrix() {
    affectsTransitiveMatrix_.print();
}

/* END   - Affects table functions */
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

    parentMatrix_ = Matrix(tableMaximumSize_);
    parentTransitiveMatrix_ = Matrix(tableMaximumSize_);

    followsMatrix_ = Matrix(tableMaximumSize_);
    followsTransitiveMatrix_ = Matrix(tableMaximumSize_);

    nextMatrix_ = Matrix(tableMaximumSize_);
    nextTransitiveMatrix_ = Matrix(tableMaximumSize_);

    affectsMatrix_ = Matrix(tableMaximumSize_);
    affectsTransitiveMatrix_ = Matrix(tableMaximumSize_);
}

void PKB::SetProcedureFirstAndLastStmtNumber(StmtNumber firstStmtNumber, StmtNumber lastStmtNumber) {
    procedureFirstAndLastStmtNumber_.insert(std::make_pair(firstStmtNumber, lastStmtNumber));
}

void PKB::Clear() {
    DesignExtractor::getInstance().resetInstance();

    numberOfProcedure_ = 0;
    numberOfAssign_ = 0;
    numberOfWhile_ = 0;
    numberOfIf_ = 0;
    numberOfCall_ = 0;

    tableMaximumSize_ = 0;

    controlFlowGraphNodes_.clear();
    procedureFirstAndLastStmtNumber_.clear();

    constantTable_          = Table<ConstantIndex, ConstantValue>();
    procedureTable_         = Table<ProcedureIndex, ProcedureName>();
    variableTable_          = Table<VariableIndex, VariableName>();

    controlVariableTable_   = VectorTable<StmtNumber, VariableIndex>();
    callTable_              = VectorTable<StmtNumber, ProcedureIndex>();
    stmtTable_              = VectorTable<StmtNumber, Symbol>();
    stmtlistTable_          = VectorTable<StmtNumber, Symbol>();

    priorityTable_          = Table<Priority, Symbol>();

    expressionTable_        = Table<StmtNumber, Expression>();
    subExpressionTable_     = VectorTable<StmtNumber, SubExpression>();

    callsTable_             = VectorTable<ProcedureIndex, ProcedureIndex>();
    callsTransitiveTable_   = TransitiveTable<ProcedureIndex, ProcedureIndex>();

    modifiesTable_                  = VectorTable<StmtNumber, VariableIndex>();
    modifiesProcedureTable_         = VectorTable<ProcedureIndex, VariableIndex>();
    modifiesProcedureIsolatedTable_ = VectorTable<StmtNumber, VariableIndex>();

    usesTable_                  = VectorTable<StmtNumber, VariableIndex>();
    usesProcedureTable_         = VectorTable<ProcedureIndex, VariableIndex>();
    usesProcedureIsolatedTable_ = VectorTable<StmtNumber, VariableIndex>();

    parentMatrix_ = Matrix();
    parentTransitiveMatrix_ = Matrix();

    parentTable_            = VectorTable<StmtNumber, StmtNumber>();
    parentTransitiveTable_  = TransitiveTable<StmtNumber, StmtNumber>();

    followsMatrix_ = Matrix();
    followsTransitiveMatrix_ = Matrix();

    followsTable_           = VectorTable<StmtNumber, StmtNumber>();

    nextMatrix_ = Matrix();
    nextTransitiveMatrix_ = Matrix();

    nextTable_ = VectorTable<StmtNumber, StmtNumber>();

    affectsMatrix_ = Matrix();
    affectsTransitiveMatrix_ = Matrix();

    affectsTable_ = VectorTable<StmtNumber, StmtNumber>();
}

void PKB::ClearComputeOnDemands() {
    nextTransitiveMatrix_.clear();

    affectsMatrix_.clear();
    affectsTransitiveMatrix_.clear();

    affectsTable_ = VectorTable<StmtNumber, StmtNumber>();
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
    if (callTable_.hasKey(stmtNumber)) {
        vector<ProcedureIndex> procedureIndexes = callTable_.getValues(stmtNumber);

        /* It is expected that one call statement calls only one procedure. */
        for (ProcedureIndex procedureIndex : procedureIndexes) {
            return PKB::GetProcedureName(procedureIndex);
        }
    }

    return "";
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

vector<ProcedureName> PKB::GetCalling(ProcedureName called) {
    vector<ProcedureName> names;
    vector<ProcedureIndex> indexes = callsTable_.getKeys(PKB::GetProcedureIndex(called));

    for (ProcedureIndex index : indexes) {
        names.push_back(PKB::GetProcedureName(index));
    }

    return names;
}

vector<ProcedureName> PKB::GetCalled(ProcedureName calling) {
    vector<ProcedureName> names;
    vector<ProcedureIndex> indexes = callsTable_.getValues(PKB::GetProcedureIndex(calling));

    for (ProcedureIndex index : indexes) {
        names.push_back(PKB::GetProcedureName(index));
    }

    return names;
}

bool PKB::IsModifies(StmtNumber stmtNumber, VariableName variableName) {
    return modifiesTable_.hasKeyToValue(stmtNumber, PKB::GetVariableIndex(variableName));
}

bool PKB::IsModifiesProcedure(ProcedureName procedureName, VariableName variableName) {
    return modifiesProcedureTable_.hasKeyToValue(PKB::GetProcedureIndex(procedureName), PKB::GetVariableIndex(variableName));
}

vector<StmtNumber> PKB::GetStmtNumberModifying(VariableName variableName) {
    return modifiesTable_.getKeys(PKB::GetVariableIndex(variableName));
}

vector<VariableName> PKB::GetProcedureModifiedVariables(ProcedureName procedureName) {
    vector<VariableName> names;
    vector<VariableIndex> indexes = modifiesProcedureTable_.getValues(PKB::GetProcedureIndex(procedureName));

    for (VariableIndex index : indexes) {
        names.push_back(PKB::GetVariableName(index));
    }

    return names;
}

bool PKB::IsUses(StmtNumber stmtNumber, VariableName variableName) {
    return usesTable_.hasKeyToValue(stmtNumber, PKB::GetVariableIndex(variableName));
}

bool PKB::IsUsesProcedure(ProcedureName procedureName, VariableName variableName) {
    return usesProcedureTable_.hasKeyToValue(PKB::GetProcedureIndex(procedureName), PKB::GetVariableIndex(variableName));
}

vector<StmtNumber> PKB::GetStmtNumberUsing(VariableName variableName) {
    return usesTable_.getKeys(PKB::GetVariableIndex(variableName));
}

vector<VariableName> PKB::GetProcedureUsedVariables(ProcedureName procedureName) {
    vector<VariableName> names;
    vector<VariableIndex> indexes = usesProcedureTable_.getValues(PKB::GetProcedureIndex(procedureName));

    for (VariableIndex index : indexes) {
        names.push_back(PKB::GetVariableName(index));
    }

    return names;
}
/* END   - Deprecated functions */

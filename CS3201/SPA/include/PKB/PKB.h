#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "PKB/Matrix.h"
#include "PKB/Table.h"
#include "PKB/TransitiveTable.h"
#include "PKB/VectorTable.h"
#include "CFGNode.h"
#include "Utils.h"

typedef std::string  Expression, SubExpression;
typedef std::string  ConstantValue, ProcedureName, VariableName;

typedef unsigned int ConstantIndex, ProcedureIndex, VariableIndex;
typedef unsigned int Priority;
typedef unsigned int StmtNumber;

class PKB {
 public:
    static void InsertConstantValue(ConstantIndex constantIndex, ConstantValue constant);
    static ConstantIndex GetConstantIndex(ConstantValue constantValue);
    static ConstantValue GetConstantValue(ConstantIndex constantIndex);
    static std::vector<ConstantValue> GetAllConstantValues();
    static void PrintConstantTable();

    static void InsertVariableName(VariableIndex variableIndex, VariableName variableName);
    static VariableIndex GetVariableIndex(VariableName variableName);
    static VariableName GetVariableName(VariableIndex variableIndex);
    static std::vector<VariableName> GetAllVariableNames();
    static void PrintVariableTable();

    static void InsertProcedureName(ProcedureIndex procedureIndex, ProcedureName procedureName);
    static ProcedureIndex GetProcedureIndex(ProcedureName procedureName);
    static ProcedureName GetProcedureName(ProcedureIndex procedureIndex);
    static std::vector<ProcedureName> GetAllProcedureNames();
    static std::vector<ProcedureIndex> GetAllProcedureIndexes();
    static void PrintProcedureTable();

    static void InsertControlVariable(StmtNumber stmtNumber, VariableIndex variableIndex);
    static bool HasControlVariableIndexAtStmtNumber(StmtNumber stmtNumber, VariableIndex variableIndex);
    static VariableIndex GetControlVariableIndex(StmtNumber stmtNumber);
    static std::vector<VariableIndex> GetAllControlVariableIndexes();
    static void PrintControlVariableTable();

    static void InsertCallStmt(StmtNumber stmtNumber, ProcedureIndex procedureIndex);
    static ProcedureIndex GetCallStmtProcedureIndex(StmtNumber stmtNumber);
    static void PrintCallTable();

    static void InsertStmt(StmtNumber stmtNumber, Symbol stmtSymbol);
    static Symbol GetStmtSymbol(StmtNumber stmtNumber);
    static std::vector<StmtNumber> GetSymbolStmtNumbers(Symbol stmtSymbol);
    static void PrintStmtTable();

    static void InsertStmtlist(StmtNumber stmtNumber, Symbol stmtSymbol);
    static std::vector<StmtNumber> GetAllStmtlistsStmtNumber();
    static void PrintStmtlistTable();

    static void PopulatePriorityTable();
    static Priority GetPriority(Symbol symbol);
    static void PrintPriorityTable();

    static void InsertExpression(StmtNumber stmtNumber, Expression expression);
    static void InsertSubExpression(StmtNumber stmtNumber, SubExpression subExpression);
    static bool IsExactPattern(StmtNumber stmtNumber, VariableIndex variableIndex, Expression expression);
    static bool IsSubPattern(StmtNumber stmtNumber, VariableIndex variableIndex, SubExpression subExpression);
    static bool HasExactExpression(Expression expression);
    static bool HasSubExpression(SubExpression subExpression);
    static bool IsExactExpression(StmtNumber stmtNumber, Expression expression);
    static bool IsSubExpression(StmtNumber stmtNumber, SubExpression subExpression);
    static void PrintExactExpressionTable();
    static void PrintSubExpressionTable();

    static void InsertCalls(ProcedureIndex calling, ProcedureIndex called);
    static void PopulateCallsTransitive();
    static bool IsCalls(ProcedureIndex calling, ProcedureIndex called);
    static bool IsCallsTransitive(ProcedureIndex calling, ProcedureIndex called);
    static std::vector<ProcedureIndex> GetCalling(ProcedureIndex called);
    static std::vector<ProcedureIndex> GetCalled(ProcedureIndex calling);
    static unsigned int GetNumberOfCallsRelationship();
    static void PrintCallsTable();
    static void PrintCallsTransitiveTable();

    static void InsertModifies(StmtNumber stmtNumber, VariableIndex variableIndex);
    static void InsertModifiesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex);
    static void InsertModifiesProcedureIsolated(StmtNumber firstStmtNumber, VariableIndex variableIndex);
    static bool IsModifies(StmtNumber stmtNumber, VariableIndex variableIndex);
    static bool IsModifiesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex);
    static std::vector<VariableIndex> GetModifiedVariables(StmtNumber stmtNumber);
    static std::vector<StmtNumber> GetStmtNumberModifying(VariableIndex variableIndex);
    static std::vector<VariableIndex> GetProcedureModifiedVariables(ProcedureIndex procedureIndex);
    static std::vector<ProcedureIndex> GetProceduresNameModifying(VariableIndex variableIndex);
    static std::vector<StmtNumber> GetIsolatedProcedureStmtNumberModifying(VariableIndex variableIndex);
    static unsigned int GetNumberOfModifiesRelationship();
    static unsigned int GetNumberOfModifiesProcedureRelationship();
    static void PrintModifiesTable();
    static void PrintModifiesProcedureTable();

    static void InsertUses(StmtNumber stmtNumber, VariableIndex variableIndex);
    static void InsertUsesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex);
    static void InsertUsesProcedureIsolated(StmtNumber firstStmtNumber, VariableIndex variableIndex);
    static bool IsUses(StmtNumber stmtNumber, VariableIndex variableIndex);
    static bool IsUsesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex);
    static std::vector<VariableIndex> GetUsedVariables(StmtNumber stmtNumber);
    static std::vector<StmtNumber> GetStmtNumberUsing(VariableIndex variableIndex);
    static std::vector<VariableIndex> GetProcedureUsedVariables(ProcedureIndex procedureIndex);
    static std::vector<ProcedureIndex> GetProceduresNameUsing(VariableIndex variableIndex);
    static std::vector<StmtNumber> GetIsolatedProcedureStmtNumberUsing(VariableIndex variableIndex);
    static unsigned int GetNumberOfUsesRelationship();
    static unsigned int GetNumberOfUsesProcedureRelationship();
    static void PrintUsesTable();
    static void PrintUsesProcedureTable();

    static void InsertParent(StmtNumber parent, StmtNumber child);
    static void PopulateParentTransitive();
    static bool IsParent(StmtNumber parent, StmtNumber child);
    static bool IsParentTransitive(StmtNumber parent, StmtNumber child);
    static std::vector<StmtNumber> GetParent(StmtNumber child);
    static std::vector<StmtNumber> GetChildren(StmtNumber parent);
    static std::vector<StmtNumber> GetParentsTransitive(StmtNumber child);
    static std::vector<StmtNumber> GetChildrenTransitive(StmtNumber parent);
    static unsigned int GetNumberOfParentRelationship();
    static void PrintParentTable();
    static void PrintParentTransitiveTable();

    static void InsertFollows(StmtNumber follows, StmtNumber following);
    static void PopulateFollowsTransitive();
    static bool IsFollows(StmtNumber follows, StmtNumber following);
    static bool IsFollowsTransitive(StmtNumber follows, StmtNumber following);
    static StmtNumber GetFollows(StmtNumber following);
    static StmtNumber GetFollowing(StmtNumber follows);
    static unsigned int GetNumberOfFollowsRelationship();
    static void PrintFollowsTable();

    static void InsertControlFlowGraphNode(StmtNumber stmtNumber, CFGNode* controlFlowGraphNode);
    static void InsertNext(StmtNumber current, StmtNumber next);
    static bool IsNext(StmtNumber current, StmtNumber next);
    static bool IsNextTransitive(StmtNumber current, StmtNumber next);
    static std::vector<StmtNumber> GetNext(StmtNumber current);
    static std::vector<StmtNumber> GetPrevious(StmtNumber next);
    static unsigned int GetNumberOfNextRelationship();
    static void PrintNextTable();

    static bool IsAffects();
    static bool IsAffects(StmtNumber affecting, StmtNumber affected);
    static bool IsAffectsTransitive(StmtNumber affecting, StmtNumber affected);
    static std::vector<StmtNumber> GetAffected(StmtNumber affecting);
    static std::vector<StmtNumber> GetAffecting(StmtNumber affected);
    static void PrintAffectsTable();
    static void PrintAffectsTransitiveMatrix();

    static unsigned int GetNumberOfProcedure();
    static unsigned int GetNumberOfAssign();
    static unsigned int GetNumberOfWhile();
    static unsigned int GetNumberOfIf();
    static unsigned int GetNumberOfCall();
    static unsigned int GetNumberOfContainerStmt();

    static CFGNode* GetCFGNodeByStmtNumber(StmtNumber stmtNumber);

    static void SetTableMaximumSize(unsigned int tableMaximumSize);
    static void SetProcedureFirstAndLastStmtNumber(StmtNumber firstStmtNumber, StmtNumber lastStmtNumber);

    static void Clear();
    static void ClearComputeOnDemands();

    /* START - Deprecated functions */
    static bool HasControlVariableNameAtStmtNumber(StmtNumber stmtNumber, VariableName controlVariable);

    static ProcedureName GetCallStmtProcedureName(StmtNumber stmtNumber, std::string deprecatedFiller);

    static bool IsExactPattern(StmtNumber stmtNumber, VariableName variableName, Expression expression);
    static bool IsSubPattern(StmtNumber stmtNumber, VariableName variableName, SubExpression subExpression);

    static bool IsCalls(ProcedureName calling, ProcedureName called);
    static bool IsCallsTransitive(ProcedureName calling, ProcedureName called);
    static std::vector<ProcedureName> GetCalling(ProcedureName called);
    static std::vector<ProcedureName> GetCalled(ProcedureName calling);

    static bool IsModifies(StmtNumber stmtNumber, VariableName variableName);
    static bool IsModifiesProcedure(ProcedureName procedureName, VariableName variableName);
    static std::vector<StmtNumber> GetStmtNumberModifying(VariableName variableName);
    static std::vector<VariableName> GetProcedureModifiedVariables(ProcedureName procedureName);

    static bool IsUses(StmtNumber stmtNumber, VariableName variableName);
    static bool IsUsesProcedure(ProcedureName procedureName, VariableName variableName);
    static std::vector<StmtNumber> GetStmtNumberUsing(VariableName variableName);
    static std::vector<VariableName> GetProcedureUsedVariables(ProcedureName procedureName);
    /* END   - Deprecated functions */

 private:
    static bool ComparePairAscending(const std::pair<unsigned int, Symbol> &pairOne, const std::pair<unsigned int, Symbol> &pairTwo);

    static unsigned int numberOfProcedure_;
    static unsigned int numberOfAssign_;
    static unsigned int numberOfWhile_;
    static unsigned int numberOfIf_;
    static unsigned int numberOfCall_;

    static unsigned int tableMaximumSize_;

    static std::unordered_map<StmtNumber, CFGNode*> controlFlowGraphNodes_;
    static std::unordered_map<StmtNumber, StmtNumber> procedureFirstAndLastStmtNumber_;

    static Table<ConstantIndex, ConstantValue> constantTable_;
    static Table<VariableIndex, VariableName> variableTable_;
    static Table<ProcedureIndex, ProcedureName> procedureTable_;
    
    static VectorTable<StmtNumber, VariableIndex> controlVariableTable_;
    static VectorTable<StmtNumber, ProcedureIndex> callTable_;
    static VectorTable<StmtNumber, Symbol> stmtTable_;
    static VectorTable<StmtNumber, Symbol> stmtlistTable_;

    static Table<Priority, Symbol> priorityTable_;

    static Table<StmtNumber, Expression> expressionTable_;
    static VectorTable<StmtNumber, SubExpression> subExpressionTable_;

    static VectorTable<ProcedureIndex, ProcedureIndex> callsTable_;
    static TransitiveTable<ProcedureIndex, ProcedureIndex> callsTransitiveTable_;

    static VectorTable<StmtNumber, VariableIndex> modifiesTable_;
    static VectorTable<ProcedureIndex, VariableIndex> modifiesProcedureTable_;
    static VectorTable<StmtNumber, VariableIndex> modifiesProcedureIsolatedTable_;

    static VectorTable<StmtNumber, VariableIndex> usesTable_;
    static VectorTable<ProcedureIndex, VariableIndex> usesProcedureTable_;
    static VectorTable<StmtNumber, VariableIndex> usesProcedureIsolatedTable_;

    static Matrix parentMatrix_;
    static Matrix parentTransitiveMatrix_;

    static VectorTable<StmtNumber, StmtNumber> parentTable_;
    static TransitiveTable<StmtNumber, StmtNumber> parentTransitiveTable_;

    static Matrix followsMatrix_;
    static Matrix followsTransitiveMatrix_;

    static VectorTable<StmtNumber, StmtNumber> followsTable_;

    static Matrix nextMatrix_;
    static Matrix nextTransitiveMatrix_;

    static VectorTable<StmtNumber, StmtNumber> nextTable_;

    static Matrix affectsMatrix_;
    static Matrix affectsTransitiveMatrix_;

    static VectorTable<StmtNumber, StmtNumber> affectsTable_;
};

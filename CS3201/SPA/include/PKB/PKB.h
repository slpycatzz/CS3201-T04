#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Constants.h"
#include "PKB/Table.h"
#include "PKB/TransitiveTable.h"
#include "TreeNode.h"
#include "Utils.h"

typedef TreeNode*    CFGNode;

typedef std::string  Expression, SubExpression;
typedef std::string  ConstantValue, ProcedureName, VariableName;

typedef unsigned int ConstantIndex, ProcedureIndex, VariableIndex;
typedef unsigned int Priority;
typedef unsigned int StmtNumber;

class PKB {
 public:
    static void InsertConstantValue(ConstantIndex constantIndex, ConstantValue constant);
    static bool HasConstantValue(ConstantValue constantValue);
    static ConstantIndex GetConstantIndex(ConstantValue constantValue);
    static ConstantValue GetConstantValue(ConstantIndex constantIndex);
    static std::vector<ConstantValue> GetAllConstantValues();
    static void PrintConstantTable();

    static void InsertVariableName(VariableIndex variableIndex, VariableName variableName);
    static bool HasVariableName(VariableName variableName);
    static VariableIndex GetVariableIndex(VariableName variableName);
    static VariableName GetVariableName(VariableIndex variableIndex);
    static std::vector<VariableName> GetAllVariableNames();
    static void PrintVariableTable();

    static void InsertProcedureName(ProcedureIndex procedureIndex, ProcedureName procedureName);
    static bool HasProcedureName(ProcedureName procedureName);
    static ProcedureIndex GetProcedureIndex(ProcedureName procedureName);
    static ProcedureName GetProcedureName(ProcedureIndex procedureIndex);
    static std::vector<ProcedureName> GetAllProcedureNames();
    static std::vector<ProcedureIndex> GetAllProcedureIndexes();
    static void PrintProcedureTable();

    static void InsertControlVariable(StmtNumber stmtNumber, VariableIndex variableIndex);
    static bool HasControlVariableIndex(VariableIndex variableIndex);
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
    static void PopulateCallsTransitiveTable();
    static bool IsCalls(ProcedureIndex calling, ProcedureIndex called);
    static bool IsCalls(ProcedureIndex calling, std::set<ProcedureIndex> calleds);
    static bool IsCallsTransitive(ProcedureIndex calling, ProcedureIndex called);
    static bool IsCallsTransitive(ProcedureIndex calling, std::set<ProcedureIndex> calleds);
    static std::set<ProcedureIndex> GetCalling(ProcedureIndex called);
    static std::set<ProcedureIndex> GetCalled(ProcedureIndex calling);
    static std::set<ProcedureIndex> GetCallingTransitive(ProcedureIndex called);
    static std::set<ProcedureIndex> GetCalledTransitive(ProcedureIndex calling);
    static unsigned int GetNumberOfCallsRelationship();
    static unsigned int GetNumberOfCallsTransitiveRelationship();
    static void PrintCallsTable();
    static void PrintCallsTransitiveTable();

    static void InsertModifies(StmtNumber stmtNumber, VariableIndex variableIndex);
    static void InsertModifiesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex);
    static bool IsModifies(StmtNumber stmtNumber, VariableIndex variableIndex);
    static bool IsModifies(StmtNumber stmtNumber, std::set<VariableIndex> variableIndexes);
    static bool IsModifiesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex);
    static bool IsModifiesProcedure(ProcedureIndex procedureIndex, std::set<VariableIndex> variableIndexes);
    static std::set<VariableIndex> GetModifiedVariables(StmtNumber stmtNumber);
    static std::set<StmtNumber> GetStmtNumberModifying(VariableIndex variableIndex);
    static std::set<VariableIndex> GetProcedureModifiedVariables(ProcedureIndex procedureIndex);
    static std::set<ProcedureIndex> GetProceduresNameModifying(VariableIndex variableIndex);
    static unsigned int GetNumberOfModifiesRelationship();
    static unsigned int GetNumberOfModifiesProcedureRelationship();
    static void PrintModifiesTable();
    static void PrintModifiesProcedureTable();

    static void InsertUses(StmtNumber stmtNumber, VariableIndex variableIndex);
    static void InsertUsesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex);
    static bool IsUses(StmtNumber stmtNumber, VariableIndex variableIndex);
    static bool IsUses(StmtNumber stmtNumber, std::set<VariableIndex> variableIndexes);
    static bool IsUsesProcedure(ProcedureIndex procedureIndex, VariableIndex variableIndex);
    static bool IsUsesProcedure(ProcedureIndex procedureIndex, std::set<VariableIndex> variableIndexes);
    static std::set<VariableIndex> GetUsedVariables(StmtNumber stmtNumber);
    static std::set<StmtNumber> GetStmtNumberUsing(VariableIndex variableIndex);
    static std::set<VariableIndex> GetProcedureUsedVariables(ProcedureIndex procedureIndex);
    static std::set<ProcedureIndex> GetProceduresNameUsing(VariableIndex variableIndex);
    static unsigned int GetNumberOfUsesRelationship();
    static unsigned int GetNumberOfUsesProcedureRelationship();
    static void PrintUsesTable();
    static void PrintUsesProcedureTable();

    static void InsertParent(StmtNumber parent, StmtNumber child);
    static void PopulateParentTransitiveTable();
    static bool IsParent(StmtNumber parent, StmtNumber child);
    static bool IsParent(StmtNumber parent, std::set<StmtNumber> children);
    static bool IsParentTransitive(StmtNumber parent, StmtNumber child);
    static bool IsParentTransitive(StmtNumber parent, std::set<StmtNumber> children);
    static std::set<StmtNumber> GetParent(StmtNumber child);
    static std::set<StmtNumber> GetChildren(StmtNumber parent);
    static std::set<StmtNumber> GetParentsTransitive(StmtNumber child);
    static std::set<StmtNumber> GetChildrenTransitive(StmtNumber parent);
    static unsigned int GetNumberOfParentRelationship();
    static unsigned int GetNumberOfParentTransitiveRelationship();
    static void PrintParentTable();
    static void PrintParentTransitiveTable();

    static void InsertFollows(StmtNumber follows, StmtNumber following);
    static void PopulateFollowsTransitiveTable();
    static bool IsFollows(StmtNumber follows, StmtNumber following);
    static bool IsFollows(StmtNumber follows, std::set<StmtNumber> followings);
    static bool IsFollowsTransitive(StmtNumber follows, StmtNumber following);
    static bool IsFollowsTransitive(StmtNumber follows, std::set<StmtNumber> followings);
    static std::set<StmtNumber> GetFollows(StmtNumber following);
    static std::set<StmtNumber> GetFollowing(StmtNumber follows);
    static std::set<StmtNumber> GetFollowsTransitive(StmtNumber following);
    static std::set<StmtNumber> GetFollowingTransitive(StmtNumber follows);
    static unsigned int GetNumberOfFollowsRelationship();
    static unsigned int GetNumberOfFollowsTransitiveRelationship();
    static void PrintFollowsTable();
    static void PrintFollowsTransitiveTable();

    static void InsertControlFlowGraph(CFGNode controlFlowGraph);
    static void InsertNext(StmtNumber current, StmtNumber next);
    static void PopulateNextTransitiveTable();
    static bool IsNext(StmtNumber current, StmtNumber next);
    static bool IsNextTransitive(StmtNumber current, StmtNumber next);
    static std::set<StmtNumber> GetNext(StmtNumber current);
    static std::set<StmtNumber> GetPrevious(StmtNumber next);
    static std::set<StmtNumber> GetNextTransitive(StmtNumber current);
    static std::set<StmtNumber> GetPreviousTransitive(StmtNumber next);
    static unsigned int GetNumberOfNextRelationship();
    static unsigned int GetNumberOfNextTransitiveRelationship();
    static void PrintNextTable();
    static void PrintNextTransitiveTable();

    static unsigned int GetNumberOfProcedure();
    static unsigned int GetNumberOfAssign();
    static unsigned int GetNumberOfWhile();
    static unsigned int GetNumberOfIf();
    static unsigned int GetNumberOfCall();
    static unsigned int GetNumberOfContainerStmt();

    static void SetTableMaximumSize(unsigned int tableMaximumSize);

    static void Clear();

    /* START - Deprecated functions */
    static bool HasControlVariableNameAtStmtNumber(StmtNumber stmtNumber, VariableName controlVariable);

    static ProcedureName GetCallStmtProcedureName(StmtNumber stmtNumber, std::string deprecatedFiller);

    static bool IsExactPattern(StmtNumber stmtNumber, VariableName variableName, Expression expression);
    static bool IsSubPattern(StmtNumber stmtNumber, VariableName variableName, SubExpression subExpression);

    static bool IsCalls(ProcedureName calling, ProcedureName called);
    static bool IsCallsTransitive(ProcedureName calling, ProcedureName called);
    static std::set<ProcedureName> GetCalling(ProcedureName called);
    static std::set<ProcedureName> GetCalled(ProcedureName calling);

    static bool IsModifies(StmtNumber stmtNumber, VariableName variableName);
    static bool IsModifiesProcedure(ProcedureName procedureName, VariableName variableName);
    static std::set<StmtNumber> GetStmtNumberModifying(VariableName variableName);
    static std::set<VariableName> GetProcedureModifiedVariables(ProcedureName procedureName);

    static bool IsUses(StmtNumber stmtNumber, VariableName variableName);
    static bool IsUsesProcedure(ProcedureName procedureName, VariableName variableName);
    static std::set<StmtNumber> GetStmtNumberUsing(VariableName variableName);
    static std::set<VariableName> GetProcedureUsedVariables(ProcedureName procedureName);
    /* END   - Deprecated functions */

 private:
    static bool ComparePairAscending(const std::pair<unsigned int, Symbol> &pairOne, const std::pair<unsigned int, Symbol> &pairTwo);

    static unsigned int numberOfProcedure_;
    static unsigned int numberOfAssign_;
    static unsigned int numberOfWhile_;
    static unsigned int numberOfIf_;
    static unsigned int numberOfCall_;

    static unsigned int tableMaximumSize_;

    static std::vector<CFGNode> controlFlowGraphs_;
    static std::vector<std::vector<CFGNode>> controlFlowGraphsNodes_;

    static Table<ConstantIndex, ConstantValue> constantTable_;
    static Table<VariableIndex, VariableName> variableTable_;
    static Table<ProcedureIndex, ProcedureName> procedureTable_;

    static Table<StmtNumber, VariableIndex> controlVariableTable_;
    static Table<StmtNumber, ProcedureIndex> callTable_;
    static Table<StmtNumber, Symbol> stmtTable_;
    static Table<StmtNumber, Symbol> stmtlistTable_;

    static Table<Priority, Symbol> priorityTable_;

    static Table<StmtNumber, Expression> expressionTable_;
    static Table<StmtNumber, SubExpression> subExpressionTable_;

    static Table<ProcedureIndex, ProcedureIndex> callsTable_;
    static TransitiveTable<ProcedureIndex, ProcedureIndex> callsTransitiveTable_;

    static Table<StmtNumber, VariableIndex> modifiesTable_;
    static Table<ProcedureIndex, VariableIndex> modifiesProcedureTable_;

    static Table<StmtNumber, VariableIndex> usesTable_;
    static Table<ProcedureIndex, VariableIndex> usesProcedureTable_;

    static Table<StmtNumber, StmtNumber> parentTable_;
    static TransitiveTable<StmtNumber, StmtNumber> parentTransitiveTable_;

    static Table<StmtNumber, StmtNumber> followsTable_;
    static TransitiveTable<StmtNumber, StmtNumber> followsTransitiveTable_;

    static Table<StmtNumber, StmtNumber> nextTable_;
    static TransitiveTable<StmtNumber, StmtNumber> nextTransitiveTable_;
};

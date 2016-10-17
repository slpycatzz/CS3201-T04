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

typedef std::string  ConstantValue;
typedef std::string  Expression;
typedef std::string  SubExpressions;
typedef std::string  VariableName;
typedef std::string  ProcedureName;
typedef std::string  StmtSymbol;
typedef std::string  TableSymbol;
typedef unsigned int StmtNumber;
typedef unsigned int Index;
typedef unsigned int Priority;

class PKB {
 public:
    static void SetASTRoot(TreeNode *root);
    static TreeNode* GetASTRoot();
    static TreeNode* CreateASTNode(Symbol symbol);
    static TreeNode* CreateASTNode(Symbol, std::string value);
    static TreeNode* CreateASTNode(Symbol symbol, StmtNumber stmtNumber);
    static TreeNode* CreateASTNode(Symbol symbol, StmtNumber stmtNumber, std::string value);
    static void PrintASTTree();

    static void GenerateConstantTable(std::set<ConstantValue> constants);
    static bool HasConstant(ConstantValue constantValue);
    static std::string GetConstantValue(Index index);
    static Index GetConstantIndex(ConstantValue constantValue);
    static std::vector<std::string> GetAllConstantValues();
    static void PrintConstantTable();

    static void GenerateVariableTable(std::set<VariableName> variableNames);
    static bool HasVariable(VariableName variableName);
    static VariableName GetVariableName(Index index);
    static Index GetVariableIndex(VariableName variableName);
    static std::vector<VariableName> GetAllVariableNames();
    static void PrintVariableTable();

    static void GenerateProcedureTable(std::set<ProcedureName> procedureNames);
    static bool HasProcedure(ProcedureName procedureName);
    static ProcedureName GetProcedureName(Index index);
    static Index GetProcedureIndex(ProcedureName procedureName);
    static std::vector<ProcedureName> GetAllProcedures();
    static void PrintProcedureTable();

    static void GenerateControlVariableTable(std::map<StmtNumber, VariableName> controlVariables);
    static bool HasControlVariable(VariableName controlVariable);
    static bool HasControlVariableAtStmtNumber(StmtNumber stmtNumber, VariableName controlVariable);
    static VariableName GetControlVariable(StmtNumber stmtNumber);
    static std::vector<VariableName> GetAllControlVariables();
    static void PrintControlVariableTable();

    static void GenerateCallTable(std::map<StmtNumber, ProcedureName> callStmtNumbers);
    static ProcedureName GetCallProcedureName(StmtNumber stmtNumber);
    static void PrintCallTable();

    static void GenerateStmtTable(std::map<StmtNumber, StmtSymbol> stmts);
    static StmtSymbol GetStmtSymbol(StmtNumber stmtNumber);
    static std::vector<StmtNumber> GetSymbolStmtNumbers(StmtSymbol symbol);
    static std::vector<StmtNumber> GetSymbolStmtNumbers(Symbol symbol);
    static void PrintStmtTable();

    static void GenerateStmtlistTable(std::map<StmtNumber, StmtSymbol> stmtlists);
    static std::vector<StmtNumber> GetAllStmtlistsStmtNumber();
    static void PrintStmtlistTable();

    static void GeneratePriorityTable();
    static Priority GetPriority(TableSymbol symbol);
    static Priority GetPriority(Symbol symbol);
    static void PrintPriorityTable();

    static void GenerateExpressionTable(std::map<StmtNumber, Expression> expressions);
    static void GenerateSubExpressionTable(std::map<StmtNumber, std::set<SubExpressions>> subExpressions);
    static bool IsExactPattern(StmtNumber stmtNumber, VariableName controlVariable, Expression expression);
    static bool IsSubPattern(StmtNumber stmtNumber, VariableName controlVariable, SubExpressions subExpression);
    static bool HasExactExpression(Expression expression);
    static bool HasSubExpression(SubExpressions subExpression);
    static bool IsExactExpression(StmtNumber stmtNumber, Expression expression);
    static bool IsSubExpression(StmtNumber stmtNumber, SubExpressions subExpression);
    static void PrintExactExpressionTable();
    static void PrintSubExpressionTable();

    static void GenerateCallsTable(std::map<ProcedureName, std::set<ProcedureName>> calls);
    static bool IsCalls(ProcedureName calling, ProcedureName called);
    static bool IsCalls(ProcedureName calling, std::set<ProcedureName> called);
    static bool IsCallsTransitive(ProcedureName calling, ProcedureName called);
    static bool IsCallsTransitive(ProcedureName calling, std::set<ProcedureName> called);
    static ProcedureName GetCalling(ProcedureName called);
    static std::set<ProcedureName> GetCalled(ProcedureName calling);
    static std::set<ProcedureName> GetCallingTransitive(ProcedureName called);
    static std::set<ProcedureName> GetCalledTransitive(ProcedureName calling);
    static void PrintCallsTable();
    static void PrintCallsTransitiveTable();

    static void GenerateModifiesTable(std::map<StmtNumber, std::set<VariableName>> modifies);
    static void GenerateModifiesProcedureTable(std::map<ProcedureName, std::set<VariableName>> modifiesProcedure);
    static bool IsModifies(StmtNumber stmtNumber, VariableName variableName);
    static bool IsModifies(StmtNumber stmtNumber, std::set<VariableName> variableNames);
    static bool IsModifiesProcedure(ProcedureName procedureName, VariableName variableName);
    static bool IsModifiesProcedure(ProcedureName procedureName, std::set<VariableName> variableNames);
    static std::set<VariableName> GetModifiedVariables(StmtNumber stmtNumber);
    static std::set<StmtNumber> GetStmtNumberModifying(VariableName variableName);
    static std::set<VariableName> GetProcedureModifiedVariables(ProcedureName procedureName);
    static std::set<ProcedureName> GetProceduresNameModifying(VariableName variableName);
    static void PrintModifiesTable();
    static void PrintModifiesProcedureTable();

    static void GenerateUsesTable(std::map<StmtNumber, std::set<VariableName>> uses);
    static void GenerateUsesProcedureTable(std::map<ProcedureName, std::set<VariableName>> usesProcedure);
    static bool IsUses(StmtNumber stmtNumber, VariableName variableName);
    static bool IsUses(StmtNumber stmtNumber, std::set<VariableName> variableNames);
    static bool IsUsesProcedure(ProcedureName procedureName, VariableName variableName);
    static bool IsUsesProcedure(ProcedureName procedureName, std::set<VariableName> variableNames);
    static std::set<VariableName> GetUsedVariables(StmtNumber stmtNumber);
    static std::set<StmtNumber> GetStmtNumberUsing(VariableName variableName);
    static std::set<VariableName> GetProcedureUsedVariables(ProcedureName procedureName);
    static std::set<ProcedureName> GetProceduresNameUsing(VariableName variableName);
    static void PrintUsesTable();
    static void PrintUsesProcedureTable();

    static void GenerateParentTable(std::map<StmtNumber, std::set<StmtNumber>> parent);
    static bool IsParent(StmtNumber parent, StmtNumber child);
    static bool IsParent(StmtNumber parent, std::set<StmtNumber> children);
    static bool IsParentTransitive(StmtNumber parent, StmtNumber child);
    static bool IsParentTransitive(StmtNumber parent, std::set<StmtNumber> children);
    static StmtNumber GetParent(StmtNumber child);
    static std::set<StmtNumber> GetChildren(StmtNumber parent);
    static std::set<StmtNumber> GetParentsTransitive(StmtNumber child);
    static std::set<StmtNumber> GetChildrenTransitive(StmtNumber parent);
    static void PrintParentTable();
    static void PrintParentTransitiveTable();

    static void GenerateFollowsTable(std::map<StmtNumber, StmtNumber> follows);
    static bool IsFollows(StmtNumber follows, StmtNumber following);
    static bool IsFollows(StmtNumber follows, std::set<StmtNumber> followings);
    static bool IsFollowsTransitive(StmtNumber follows, StmtNumber following);
    static bool IsFollowsTransitive(StmtNumber follows, std::set<StmtNumber> followings);
    static StmtNumber GetFollows(StmtNumber following);
    static StmtNumber GetFollowing(StmtNumber follows);
    static std::set<StmtNumber> GetFollowsTransitive(StmtNumber following);
    static std::set<StmtNumber> GetFollowingTransitive(StmtNumber follows);
    static void PrintFollowsTable();
    static void PrintFollowsTransitiveTable();

    static void SetControlFlowGraphs(std::vector<TreeNode*> controlFlowGraphs);
    static void GenerateNextTable(std::map<StmtNumber, std::set<StmtNumber>> next);
    static void GenerateNextTransitiveTable();
    static bool IsNext(StmtNumber current, StmtNumber next);
    static bool IsNextTransitive(StmtNumber current, StmtNumber next);
    static std::set<StmtNumber> GetNext(StmtNumber current);
    static std::set<StmtNumber> GetPrevious(StmtNumber next);
    static std::set<StmtNumber> GetNextTransitive(StmtNumber current);
    static std::set<StmtNumber> GetPreviousTransitive(StmtNumber next);
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

    /* START - Deprecated */
    static void GenerateAssignTable(std::map<unsigned int, TreeNode*> assigns);
    static TreeNode* GetAssignTreeNode(unsigned int stmtNumber);
    static std::vector<TreeNode*> GetAllAssignTreeNodes();
    static bool IsExactPattern(unsigned int stmtNo, std::string varName, TreeNode* exprTree);
    static bool IsSubPattern(unsigned int stmtNo, std::string varName, TreeNode* exprTree);
    static bool IsExactRHS(unsigned int stmtNo, TreeNode* exprTree);
    static bool IsSubRHS(unsigned int stmtNo, TreeNode* exprTree);
    static bool HasExactPattern(TreeNode* exprTree);
    static bool HasSubPattern(TreeNode* exprTree);
    /* END - Deprecated */

 private:
    static bool ComparePairAscending(const std::pair<unsigned int, std::string> &pairOne, const std::pair<unsigned int, std::string> &pairTwo);

    static unsigned int numberOfProcedure_;
    static unsigned int numberOfAssign_;
    static unsigned int numberOfWhile_;
    static unsigned int numberOfIf_;
    static unsigned int numberOfCall_;

    static unsigned int tableMaximumSize_;

    static std::vector<TreeNode*> controlFlowGraphs_;
    static std::vector<std::vector<TreeNode*>> controlFlowGraphsNodes_;

    static Table<Index, ConstantValue> constantTable_;                              /* map<index, constantValue>      */
    static Table<Index, VariableName> variableTable_;                               /* map<index, variableName>       */
    static Table<Index, ProcedureName> procedureTable_;                             /* map<index, procedureName>      */
    
    static Table<StmtNumber, VariableName> controlVariableTable_;                   /* map<stmtNumber, variableName>  */
    static Table<StmtNumber, ProcedureName> callTable_;                             /* map<stmtNumber, procedureName> */
    static Table<StmtNumber, StmtSymbol> stmtTable_;                                /* map<stmtNumber, symbol string> */
    static Table<StmtNumber, StmtSymbol> stmtlistTable_;                            /* map<stmtNumber, symbol string> */

    static Table<Priority, TableSymbol> priorityTable_;                             /* map<priority, symbol string>   */

    static Table<StmtNumber, Expression> expressionTable_;                          /* map<stmtNumber, exact expression>    */
    static Table<StmtNumber, SubExpressions> subExpressionTable_;                   /* map<stmtNumber, set(subExpressions)> */

    static Table<ProcedureName, ProcedureName> callsTable_;                         /* map<procedureName, set(procedureNames)> */
    static TransitiveTable<ProcedureName, ProcedureName> callsTransitiveTable_;     /* map<procedureName, set(procedureNames)> */

    static Table<StmtNumber, VariableName> modifiesTable_;                          /* map<stmtNumber, set(variableName)>    */
    static Table<ProcedureName, VariableName> modifiesProcedureTable_;              /* map<procedureName, set(variableName)> */

    static Table<StmtNumber, VariableName> usesTable_;                              /* map<stmtNumber, set(variableName)>    */
    static Table<ProcedureName, VariableName> usesProcedureTable_;                  /* map<procedureName, set(variableName)> */

    static Table<StmtNumber, StmtNumber> parentTable_;                              /* map<stmtNumber, set(stmtNumber)> */
    static TransitiveTable<StmtNumber, StmtNumber> parentTransitiveTable_;          /* map<stmtNumber, set(stmtNumber)> */

    static Table<StmtNumber, StmtNumber> followsTable_;                             /* map<stmtNumber, set(stmtNumber)> */
    static TransitiveTable<StmtNumber, StmtNumber> followsTransitiveTable_;         /* map<stmtNumber, set(stmtNumber)> */

    static Table<StmtNumber, StmtNumber> nextTable_;                                /* map<stmtNumber, set(stmtNumber)> */
    static std::vector<std::vector<StmtNumber>> nextTransitiveTable_;               /* vector<vector(stmtNumber)> */

    /* Deprecated. */
    static Table<unsigned int, TreeNode*> assignTable_;                             /* map<stmtNumber, expression string> */
};

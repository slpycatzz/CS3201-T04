#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Constants.h"
#include "PKB/Table.h"
#include "PKB/TransitiveTable.h"
#include "TreeNode.h"
#include "Utils.h"

class PKB {
 public:
    static void SetASTRoot(TreeNode *root);
    static TreeNode* GetASTRoot();
    static TreeNode* CreateASTNode(Symbol symbol);
    static TreeNode* CreateASTNode(Symbol, std::string value);
    static TreeNode* CreateASTNode(Symbol symbol, unsigned int stmtNumber);
    static TreeNode* CreateASTNode(Symbol symbol, unsigned int stmtNumber, std::string value);
    static void PrintASTTree();

    static void GenerateConstantTable(std::set<std::string> constants);
    static bool HasConstant(std::string constantValue);
    static std::string GetConstantValue(unsigned int index);
    static unsigned int GetConstantIndex(std::string constantValue);
    static std::vector<std::string> GetAllConstantValues();
    static void PrintConstantTable();

    static void GenerateVariableTable(std::set<std::string> variableNames);
    static bool HasVariable(std::string variableName);
    static std::string GetVariableName(unsigned int index);
    static unsigned int GetVariableIndex(std::string variableName);
    static std::vector<std::string> GetAllVariableNames();
    static void PrintVariableTable();

    static void GenerateProcedureTable(std::set<std::string> procedureNames);
    static bool HasProcedure(std::string procedureName);
    static std::string GetProcedureName(unsigned int index);
    static unsigned int GetProcedureIndex(std::string procedureName);
    static std::vector<std::string> GetAllProcedures();
    static void PrintProcedureTable();

    static void GenerateControlVariableTable(std::map<unsigned int, std::string> controlVariables);
    static bool HasControlVariable(std::string controlVariable);
    static bool HasControlVariableAtStmtNumber(unsigned int stmtNumber, std::string controlVariable);
    static std::string GetControlVariable(unsigned int stmtNumber);
    static std::vector<std::string> GetAllControlVariables();
    static void PrintControlVariableTable();

    static void GenerateStmtTable(std::map<unsigned int, std::string> stmts);
    static std::string GetStmtSymbol(unsigned int stmtNumber);
    static std::vector<unsigned int> GetSymbolStmtNumbers(std::string symbol);
    static std::vector<unsigned int> GetSymbolStmtNumbers(Symbol symbol);
    static void PrintStmtTable();

    static void GenerateExpressionTable(std::map<unsigned int, std::string> expressions);
    static void GenerateSubExpressionTable(std::map<unsigned int, std::set<std::string>> subExpressions);
    static bool IsExactPattern(unsigned int stmtNumber, std::string controlVariable, std::string expression);
    static bool IsSubPattern(unsigned int stmtNumber, std::string controlVariable, std::string subExpression);
    static bool HasExactExpression(std::string expression);
    static bool HasSubExpression(std::string subExpression);
    static bool IsExactExpression(unsigned int stmtNumber, std::string expression);
    static bool IsSubExpression(unsigned int stmtNumber, std::string subExpression);

    static void GenerateCallsTable(std::map<std::string, std::set<std::string>> calls);
    static bool IsCalls(std::string calling, std::string called);
    static bool IsCalls(std::string calling, std::set<std::string> called);
    static bool IsCallsTransitive(std::string calling, std::string called);
    static bool IsCallsTransitive(std::string calling, std::set<std::string> called);
    static std::string GetCalling(std::string called);
    static std::set<std::string> GetCalled(std::string calling);
    static std::set<std::string> GetCallingTransitive(std::string called);
    static std::set<std::string> GetCalledTransitive(std::string calling);
    static void PrintCallsTable();
    static void PrintCallsTransitiveTable();

    static void GenerateModifiesTable(std::map<unsigned int, std::set<std::string>> modifies);
    static void GenerateModifiesProcedureTable(std::map<std::string, std::set<std::string>> modifiesProcedure);
    static bool IsModifies(unsigned int stmtNumber, std::string variableName);
    static bool IsModifies(unsigned int stmtNumber, std::set<std::string> variableNames);
    static bool IsModifiesProcedure(std::string procedureName, std::string variableName);
    static bool IsModifiesProcedure(std::string procedureName, std::set<std::string> variableNames);
    static std::set<std::string> GetModifiedVariables(unsigned int stmtNumber);
    static std::set<unsigned int> GetStmtNumberModifying(std::string variableName);
    static std::set<std::string> GetProcedureModifiedVariables(std::string procedureName);
    static std::set<std::string> GetProceduresNameModifying(std::string variableName);
    static void PrintModifiesTable();
    static void PrintModifiesProcedureTable();

    static void GenerateUsesTable(std::map<unsigned int, std::set<std::string>> uses);
    static void GenerateUsesProcedureTable(std::map<std::string, std::set<std::string>> usesProcedure);
    static bool IsUses(unsigned int stmtNumber, std::string variableName);
    static bool IsUses(unsigned int stmtNumber, std::set<std::string> variableNames);
    static bool IsUsesProcedure(std::string procedureName, std::string variableName);
    static bool IsUsesProcedure(std::string procedureName, std::set<std::string> variableNames);
    static std::set<std::string> GetUsedVariables(unsigned int stmtNumber);
    static std::set<unsigned int> GetStmtNumberUsing(std::string variableName);
    static std::set<std::string> GetProcedureUsedVariables(std::string procedureName);
    static std::set<std::string> GetProceduresNameUsing(std::string variableName);
    static void PrintUsesTable();
    static void PrintUsesProcedureTable();

    static void GenerateParentTable(std::map<unsigned int, std::set<unsigned int>> parent);
    static bool IsParent(unsigned int parent, unsigned int child);
    static bool IsParent(unsigned int parent, std::set<unsigned int> children);
    static bool IsParentTransitive(unsigned int parent, unsigned int child);
    static bool IsParentTransitive(unsigned int parent, std::set<unsigned int> children);
    static unsigned int GetParent(unsigned int child);
    static std::set<unsigned int> GetChildren(unsigned int parent);
    static std::set<unsigned int> GetParentsTransitive(unsigned int child);
    static std::set<unsigned int> GetChildrenTransitive(unsigned int parent);
    static void PrintParentTable();
    static void PrintParentTransitiveTable();

    static void GenerateFollowsTable(std::map<unsigned int, unsigned int> follows);
    static bool IsFollows(unsigned int follows, unsigned int following);
    static bool IsFollows(unsigned int follows, std::set<unsigned int> followings);
    static bool IsFollowsTransitive(unsigned int follows, unsigned int following);
    static bool IsFollowsTransitive(unsigned int follows, std::set<unsigned int> followings);
    static unsigned int GetFollows(unsigned int following);
    static unsigned int GetFollowing(unsigned int follows);
    static std::set<unsigned int> GetFollowsTransitive(unsigned int following);
    static std::set<unsigned int> GetFollowingTransitive(unsigned int follows);
    static void PrintFollowsTable();
    static void PrintFollowsTransitiveTable();

    static unsigned int GetNumberOfProcedure();
    static unsigned int GetNumberOfAssign();
    static unsigned int GetNumberOfWhile();
    static unsigned int GetNumberOfIf();
    static unsigned int GetNumberOfCall();
    static unsigned int GetNumberOfContainerStmt();

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
    static unsigned int numberOfProcedure_;
    static unsigned int numberOfAssign_;
    static unsigned int numberOfWhile_;
    static unsigned int numberOfIf_;
    static unsigned int numberOfCall_;

    static Table<unsigned int, std::string> constantTable_;                         /* map<index, constantValue> */
    static Table<unsigned int, std::string> variableTable_;                         /* map<index, variableName>  */
    static Table<unsigned int, std::string> procedureTable_;                        /* map<index, procedureName> */
    static Table<unsigned int, std::string> controlVariableTable_;                  /* map<stmtNumber, variableName> */
    static Table<unsigned int, std::string> stmtTable_;                             /* map<stmtNumber, symbol>   */

    static Table<unsigned int, std::string> expressionTable_;                       /* map<stmtNumber, exact expression> */
    static Table<unsigned int, std::string> subExpressionTable_;                    /* map<stmtNumber, set(subExpressions)> */

    static Table<std::string, std::string> callsTable_;                             /* map<procedureName, set(procedureNames)> */
    static TransitiveTable<std::string, std::string> callsTransitiveTable_;         /* map<procedureName, set(procedureNames)> */

    static Table<unsigned int, std::string> modifiesTable_;                         /* map<stmtNumber, set(variableName)>    */
    static Table<std::string, std::string> modifiesProcedureTable_;                 /* map<procedureName, set(variableName)> */

    static Table<unsigned int, std::string> usesTable_;                             /* map<stmtNumber, set(variableName)>    */
    static Table<std::string, std::string> usesProcedureTable_;                     /* map<procedureName, set(variableName)> */

    static Table<unsigned int, unsigned int> parentTable_;                          /* map<stmtNumber, set(stmtNumber)> */
    static TransitiveTable<unsigned int, unsigned int> parentTransitiveTable_;      /* map<stmtNumber, set(stmtNumber)> */

    static Table<unsigned int, unsigned int> followsTable_;                         /* map<stmtNumber, set(stmtNumber)> */
    static TransitiveTable<unsigned int, unsigned int> followsTransitiveTable_;     /* map<stmtNumber, set(stmtNumber)> */

    /* Deprecated. */
    static Table<unsigned int, TreeNode*> assignTable_;                             /* map<stmtNumber, expression string> */
};

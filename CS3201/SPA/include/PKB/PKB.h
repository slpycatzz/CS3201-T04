#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Constants.h"
#include "PKB/Table.h"
#include "PKB/TransitiveTable.h"
#include "TreeNode.h"

class PKB {
 public:
    PKB();
    ~PKB();

    static void SetASTRoot(TreeNode *root);
    static TreeNode* GetASTRoot();
    static TreeNode* CreateASTNode(Symbol symbol);
    static TreeNode* CreateASTNode(Symbol, std::string value);
    static TreeNode* CreateASTNode(Symbol symbol, unsigned int stmtNumber);
    static TreeNode* CreateASTNode(Symbol symbol, unsigned int stmtNumber, std::string value);
    static void PrintASTTree();

    static void GenerateConstantTable(std::vector<std::string> constants);
    static bool HasConstant(std::string constantValue);
    static std::string GetConstantValue(unsigned int index);
    static unsigned int GetConstantIndex(std::string constantValue);
    static std::set<std::string> GetAllConstantValues();
    static void PrintConstantTable();

    static void GenerateVariableTable(std::vector<std::string> variableNames);
    static bool HasVariable(std::string variableName);
    static std::string GetVariableName(unsigned int index);
    static unsigned int GetVariableIndex(std::string variableName);
    static std::set<std::string> GetAllVariableNames();
    static void PrintVariableTable();

    static void GenerateProcedureTable(std::vector<std::string> procedureNames);
    static bool HasProcedure(std::string procedureName);
    static std::string GetProcedureName(unsigned int index);
    static unsigned int GetProcedureIndex(std::string procedureName);
    static std::set<std::string> GetAllProcedures();
    static void PrintProcedureTable();

    static void GenerateStmtTable(std::map<unsigned int, std::string> stmts);
    static std::string GetStmtSymbol(unsigned int stmtNumber);
    static std::set<unsigned int> GetSymbolStmtNumbers(std::string symbol);
    static std::set<unsigned int> GetSymbolStmtNumbers(Symbol symbol);
    static void PrintStmtTable();

    static void GenerateAssignTable(std::map<unsigned int, TreeNode*> assigns);
    static TreeNode* GetAssignTreeNode(unsigned int stmtNumber);
    static std::set<TreeNode*> GetAllAssignTreeNodes();

    static void GenerateModifiesTable(std::map<unsigned int, std::set<std::string>> modifies);
    static void GenerateModifiesProcedureTable(std::map<std::string, std::set<std::string>> modifiesProcedure);
    static bool IsModifies(unsigned int stmtNumber, std::string variableName);
    static bool IsModifiesProcedure(std::string procedureName, std::string variableName);
    static std::set<std::string> GetModifiedVariables(unsigned int stmtNumber);
    static std::set<unsigned int> GetStmtNumberModifying(std::string variableName);
    static std::set<std::string> GetProcedureModifiedVariables(std::string procedureName);
    static std::set<std::string> GetProceduresNameModifying(std::string variableName);
    static void PrintModifiesTable();
    static void PrintModifiesProcedureTable();

    static void GenerateUsesTable(std::map<unsigned int, std::set<std::string>> uses);
    static void GenerateUsesProcedureTable(std::map<std::string, std::set<std::string>> usesProcedure);
    static bool IsUses(unsigned int stmtNumber, std::string variableName);
    static bool IsUsesProcedure(std::string procedureName, std::string variableName);
    static std::set<std::string> GetUsedVariables(unsigned int stmtNumber);
    static std::set<unsigned int> GetStmtNumberUsing(std::string variableName);
    static std::set<std::string> GetProcedureUsedVariables(std::string procedureName);
    static std::set<std::string> GetProceduresNameUsing(std::string variableName);

    static void PrintUsesTable();
    static void PrintUsesProcedureTable();

    static void GenerateParentTable(std::map<unsigned int, std::set<unsigned int>> parent);
    static bool IsParent(unsigned int parent, unsigned int child);
    static bool IsParentTransitive(unsigned int parent, unsigned int child);
    static unsigned int GetParent(unsigned int child);
    static std::set<unsigned int> GetChildren(unsigned int parent);
    static std::set<unsigned int> GetParentsTransitive(unsigned child);
    static std::set<unsigned int> GetChildrenTransitive(unsigned int parent);
    static void PrintParentTable();
    static void PrintParentTransitiveTable();

    static void GenerateFollowsTable(std::map<unsigned int, unsigned int> follows);
    static bool IsFollows(unsigned int follows, unsigned int following);
    static bool IsFollowsTransitive(unsigned int follows, unsigned int following);
    static unsigned int GetFollows(unsigned int following);
    static unsigned int GetFollowing(unsigned int follows);
    static std::set<unsigned int> GetFollowsTransitive(unsigned following);
    static std::set<unsigned int> GetFollowingTransitive(unsigned follows);
    static void PrintFollowsTable();
    static void PrintFollowsTransitiveTable();

    static unsigned int GetNumberOfProcedure();
    static unsigned int GetNumberOfAssign();
    static unsigned int GetNumberOfWhile();
    static unsigned int GetNumberOfIf();
    static unsigned int GetNumberOfCall();

 private:
    static unsigned int numberOfProcedure_;
    static unsigned int numberOfAssign_;
    static unsigned int numberOfWhile_;
    static unsigned int numberOfIf_;
    static unsigned int numberOfCall_;

    static Table<unsigned int, std::string> constantTable_;                         /* map<index, constantValue> */
    static Table<unsigned int, std::string> variableTable_;                         /* map<index, variableName>  */
    static Table<unsigned int, std::string> procedureTable_;                        /* map<index, procedureName> */
    static Table<unsigned int, std::string> stmtTable_;                             /* map<stmtNumber, symbol>   */
    static Table<unsigned int, TreeNode*> assignTable_;                             /* map<stmtNumber, AST pointer> */

    static Table<unsigned int, std::string> modifiesTable_;                         /* map<stmtNumber, set(variableName)>    */
    static Table<std::string, std::string> modifiesProcedureTable_;                 /* map<procedureName, set(variableName)> */

    static Table<unsigned int, std::string> usesTable_;                             /* map<stmtNumber, set(variableName)>    */
    static Table<std::string, std::string> usesProcedureTable_;                     /* map<procedureName, set(variableName)> */

    static Table<unsigned int, unsigned int> parentTable_;                          /* map<stmtNumber, set(stmtNumber)> */
    static TransitiveTable<unsigned int, unsigned int> parentTransitiveTable_;      /* map<stmtNumber, set(stmtNumber)> */

    static Table<unsigned int, unsigned int> followsTable_;                         /* map<stmtNumber, set(stmtNumber)> */
    static TransitiveTable<unsigned int, unsigned int> followsTransitiveTable_;     /* map<stmtNumber, set(stmtNumber)> */
};

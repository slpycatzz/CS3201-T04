#pragma once

#include <string>
#include <vector>
#include "Constants.h"
#include "TreeNode.h"
#include "PKB/Table.h"

class PKB {
 public:
    PKB();
    ~PKB();
    static void SetASTRoot(TreeNode *root);
    static TreeNode* GetASTRoot();
    static TreeNode* CreateASTNode(Symbol symbol);
    static TreeNode* CreateASTNode(Symbol, std::string value);
    static TreeNode* CreateASTNode(Symbol symbol, int lineNumber);
    static TreeNode* CreateASTNode(Symbol symbol, int lineNumber, std::string value);
    static void PrintASTTree();

    static void InsertVariable(std::vector<std::string> variable_names);
    static bool IsVariableExist(std::string variable_name);

    static void InsertProcedure(std::vector<std::string> procedure_names);
    static void InsertProcedureLineNumber(std::string procedure_name, std::vector<int> line_numbers);
    static bool IsProcedureExist(std::string procedure_name);

    static void InsertUses(int line_number, std::vector<std::string> variable_names);
    static void InsertUses(std::string procedure_name, std::vector<std::string> variable_names);
    static std::vector<std::string> UsesVariable(int line_number);
    static std::vector<std::string> UsesVariable(std::string procedure_name);
    static std::vector<int> UsedByLineNumber(std::string variable_name);
    static std::vector<std::string> UsedByProcedure(std::string variable_name);

    static void InsertModifies(int line_number, std::vector<std::string> variable_names);
    static void InsertModifies(std::string procedure_name, std::vector<std::string> variable_names);
    static std::vector<std::string> ModifiesVariable(int line_number);
    static std::vector<std::string> ModifiesVariable(std::string procedure_name);
    static std::vector<int> ModifiedByLineNumber(std::string variable_name);
    static std::vector<std::string> ModifiedByProcedure(std::string variable_name);

    static void InsertCalls(std::string procedure_name1, std::vector<std::string> procedure_names);
    static void InsertCallsTransitive(std::string procedure_name1, std::vector<std::string> procedure_names);
    static bool IsCalls(std::string procedure_name1, std::string procedure_name2);
    static bool IsCallsTransitive(std::string procedure_name1, std::string procedure_name2);
    static void InsertParent(int line_number1, std::vector<int> line_numbers);
    static void InsertParentTransitive(int line_number1, std::vector<int> line_numbers);
    static bool IsParent(int line_number1, int line_number2);
    static bool IsParentTransitive(int line_number1, int line_number2);
    static void InsertFollows(int line_number1, std::vector<int> line_numbers);
    static void InsertFollowsTransitive(int line_number1, std::vector<int> line_numbers);
    static bool IsFollows(int line_number1, int line_number2);
    static bool IsFollowsTransitive(int line_number1, int line_number2);

 private:
  static std::vector<std::string> variable_table;
  static std::vector<std::string> procedure_table;
  static Table<std::string, std::vector<int>> procedure_line_table;
  static Table<int, std::vector<std::string>> uses_table;
  static Table<int, std::vector<std::string>> modifies_table;
  static Table<std::string, std::vector<std::string>> uses_procedure_table;
  static Table<std::string, std::vector<std::string>> modifies_procedure_table;
  static Table<std::string, std::vector<std::string>> calls_table;
  static Table<int, std::vector<int>> parent_table;
  static Table<int, std::vector<int>> follows_table;
  static Table<std::string, std::vector<std::string>> calls_transitive_table;
  static Table<int, std::vector<int>> parent_transitive_table;
  static Table<int, std::vector<int>> follows_transitive_table;
};

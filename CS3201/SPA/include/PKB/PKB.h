#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Constants.h"
#include "PKB/Table.h"
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

    static void generateConstantTable(std::set<std::string> constants);
    static void printConstantTable();

    static void generateVariableTable(std::set<std::string> variableNames);
    static void printVariableTable();

    static void generateProcedureTable(std::set<std::string> procedureNames);
    static void printProcedureTable();

    static void generateStmtTable(std::map<unsigned int, std::string> stmts);
    static void printStmtTable();

    //static void InsertVariable(string variable_name, int line_number);
    //static void InsertVariable(string variable_name, vector<int> line_numbers);
    //static bool IsVariableExist(string variable_name);
    //static void InsertProcedure(string procedure_name, int line_number);
    //static void InsertProcedure(string procedure_name, vector<int> line_numbers);
    //static bool IsProcedureExist(string procedure_name);
    //static void InsertUses(int line_number, string variable_name);
    //static void InsertUses(int line_number, vector<string> variable_names);
    //static void InsertUses(string procedure_name, string variable_name);
    //static void InsertUses(string procedure_name, vector<string> variable_names);
    //static vector<string> UsesVariable(int line_number);
    //static vector<string> UsesVariable(string procedure_name);
    //static vector<int> UsedByLineNumber(string variable_name);
    //static vector<string> UsedByProcedure(string variable_name);
    //static void InsertModifies(int line_number, string variable_name);
    //static void InsertModifies(int line_number, vector<string> variable_names);
    //static void InsertModifies(string procedure_name, string variable_name);
    //static void InsertModifies(string procedure_name, vector<string> variable_names);
    //static vector<string> ModifiesVariable(int line_number);
    //static vector<string> ModifiesVariable(string procedure_name);
    //static vector<int> ModifiedByLineNumber(string variable_name);
    //static vector<string> ModifiedByProcedure(string variable_name);
    //static void InsertCalls(string procedure_name1, string procedure_name2);
    //static void InsertCalls(string procedure_name1, vector<string> procedure_names);
    //static void InsertCallsTransitive(string procedure_name1, string procedure_name2);
    //static void InsertCallsTransitive(string procedure_name1, vector<string> procedure_names);
    //static bool IsCalls(string procedure_name1, string procedure_name2);
    //static bool IsCallsTransitive(string procedure_name1, string procedure_name2);
    //static void InsertParent(int line_number1, int line_number2);
    //static void InsertParent(int line_number1, vector<int> line_numbers);
    //static void InsertParentTransitive(int line_number1, int line_number2);
    //static void InsertParentTransitive(int line_number1, vector<int> line_numbers);
    //static bool IsParent(int line_number1, int line_number2);
    //static bool IsParent(int line_number1, vector<int> line_numbers);
    //static bool IsParentTransitive(int line_number1, int line_number2);
    //static bool IsParentTransitive(int line_number1, vector<int> line_numbers);
    //static void InsertFollows(int line_number1, int line_number2);
    //static void InsertFollows(int line_number1, vector<int> line_numbers);
    //static void InsertFollowsTransitive(int line_number1, int line_number2);
    //static void InsertFollowsTransitive(int line_number1, vector<int> line_numbers);
    //static bool IsFollows(int line_number1, int line_number2);
    //static bool IsFollows(int line_number1, vector<int> line_numbers);
    //static bool IsFollowsTransitive(int line_number1, int line_number2);
    //static bool IsFollowsTransitive(int line_number1, vector<int> line_numbers);

 private:
    static unsigned int numberOfProcedure_;
    static unsigned int numberOfAssign_;
    static unsigned int numberOfWhile_;
    static unsigned int numberOfIf_;
    static unsigned int numberOfCall_;

    static Table<unsigned int, std::string> constantTable_;                      /* Array of constant values. */
    static Table<unsigned int, std::string> variableTable_;                      /* Array of variable names. */
    static Table<unsigned int, std::string> procedureTable_;                     /* Array of procedure names. */
    static Table<unsigned int, std::string> stmtTable_;                   /* Map<stmt number, stmt symbol> */

    //static Table<int, string> uses_table;
    //static Table<string, string> procedure_uses_table;
    //static Table<int, string> modifies_table;
    //static Table<string, string> procedure_modifies_table;
    //static Table<string, string> calls_table;
    //static Table<string, string> calls_transitive_table;
    //static Table<int, int> parent_table;
    //static Table<int, int> parent_transitive_table;
    //static Table<int, int> follows_table;
    //static Table<int, int> follows_transitive_table;
};

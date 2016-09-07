#pragma once

#include <string>
#include <vector>
#include "Constants.h"
#include "TreeNode.h"

using std::string;
using std::vector;

class PKB {
 public:
    PKB();
    ~PKB();
    static void SetASTRoot(TreeNode *root);
    static TreeNode* GetASTRoot();
    static TreeNode* CreateASTNode(Symbol symbol);
    static TreeNode* CreateASTNode(Symbol, string value);
    static TreeNode* CreateASTNode(Symbol symbol, int lineNumber);
    static TreeNode* CreateASTNode(Symbol symbol, int lineNumber, string value);
    static void PrintASTTree();

    static void InsertVariable(string variable_name);
    static void InsertVariable(vector<string> variable_names);
    static bool IsVariableExist(string variable_name);
    static void InsertProcedure(vector<string> procedure_names);
    static void InsertProcedureLineNumber(string procedure_name, int line_number);
    static void InsertProcedureLineNumber(string procedure_name, vector<int> line_numbers);
    static bool IsProcedureExist(string procedure_name);
    static void InsertUses(int line_number, string variable_name);
    static void InsertUses(int line_number, vector<string> variable_names);
    static void InsertUses(string procedure_name, string variable_name);
    static void InsertUses(string procedure_name, vector<string> variable_names);
    static vector<string> UsesVariable(int line_number);
    static vector<string> UsesVariable(string procedure_name);
    static vector<int> UsedByLineNumber(string variable_name);
    static vector<string> UsedByProcedure(string variable_name);
    static void InsertModifies(int line_number, string variable_name);
    static void InsertModifies(int line_number, vector<string> variable_names);
    static void InsertModifies(string procedure_name, string variable_name);
    static void InsertModifies(string procedure_name, vector<string> variable_names);
    static vector<string> ModifiesVariable(int line_number);
    static vector<string> ModifiesVariable(string procedure_name);
    static vector<int> ModifiedByLineNumber(string variable_name);
    static vector<string> ModifiedByProcedure(string variable_name);
    static void InsertCalls(string procedure_name1, string procedure_name2);
    static void InsertCalls(string procedure_name1, vector<string> procedure_names);
    static void InsertCallsTransitive(string procedure_name1, string procedure_name2);
    static void InsertCallsTransitive(string procedure_name1, vector<string> procedure_names);
    static bool IsCalls(string procedure_name1, string procedure_name2);
    static bool IsCallsTransitive(string procedure_name1, string procedure_name2);
    static void InsertParent(int line_number1, int line_number2);
    static void InsertParent(int line_number1, vector<int> line_numbers);
    static void InsertParentTransitive(int line_number1, int line_number2);
    static void InsertParentTransitive(int line_number1, vector<int> line_numbers);
    static bool IsParent(int line_number1, int line_number2);
    static bool IsParent(int line_number1, vector<int> line_numbers);
    static bool IsParentTransitive(int line_number1, int line_number2);
    static bool IsParentTransitive(int line_number1, vector<int> line_numbers);
    static void InsertFollows(int line_number1, int line_number2);
    static void InsertFollows(int line_number1, vector<int> line_numbers);
    static void InsertFollowsTransitive(int line_number1, int line_number2);
    static void InsertFollowsTransitive(int line_number1, vector<int> line_numbers);
    static bool IsFollows(int line_number1, int line_number2);
    static bool IsFollows(int line_number1, vector<int> line_numbers);
    static bool IsFollowsTransitive(int line_number1, int line_number2);
    static bool IsFollowsTransitive(int line_number1, vector<int> line_numbers);
};

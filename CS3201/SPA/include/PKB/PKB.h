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

    static void GenerateConstantTable(std::set<std::string> constants);
    static void PrintConstantTable();

    static void GenerateVariableTable(std::set<std::string> variableNames);
    static void PrintVariableTable();

    static void GenerateProcedureTable(std::set<std::string> procedureNames);
    static void PrintProcedureTable();

    static void GenerateStmtTable(std::map<unsigned int, std::string> stmts);
    static void PrintStmtTable();

    static void GenerateParentTable(std::map<unsigned int, std::set<unsigned int>> parent);
    static void PrintParentTable();
    static void PrintParentTransitiveTable();

    static void GenerateFollowsTable(std::map<unsigned int, unsigned int> follows);
    static void PrintFollowsTable();
    static void PrintFollowsTransitiveTable();

    //static bool IsVariableExist(string variable_name);
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

    //static bool IsParent(int line_number1, int line_number2);
    //static bool IsParent(int line_number1, vector<int> line_numbers);
    //static bool IsParentTransitive(int line_number1, int line_number2);
    //static bool IsParentTransitive(int line_number1, vector<int> line_numbers);

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

    static Table<unsigned int, std::string> constantTable_;                         /* map<index, constantValue> */
    static Table<unsigned int, std::string> variableTable_;                         /* map<index, variableName>  */
    static Table<unsigned int, std::string> procedureTable_;                        /* map<index, procedureName> */
    static Table<unsigned int, std::string> stmtTable_;                             /* map<stmtNumber, symbol>   */

    static Table<unsigned int, std::string> modifiesTable_;                         /* map<stmtNumber, set(variableName)>    */
    static Table<unsigned int, std::string> modifiesProcedureTable_;                /* map<procedureName, set(variableName)> */

    static Table<unsigned int, std::string> usesTable_;                             /* map<stmtNumber, set(variableName)>    */
    static Table<unsigned int, std::string> usesProcedureTable_;                    /* map<procedureName, set(variableName)> */

    static Table<unsigned int, unsigned int> parentTable_;                          /* map<stmtNumber, set(stmtNumber)> */
    static TransitiveTable<unsigned int, unsigned int> parentTransitiveTable_;      /* map<stmtNumber, set(stmtNumber)> */

    static Table<unsigned int, unsigned int> followsTable_;                         /* map<stmtNumber, set(stmtNumber)> */
    static TransitiveTable<unsigned int, unsigned int> followsTransitiveTable_;     /* map<stmtNumber, set(stmtNumber)> */
};

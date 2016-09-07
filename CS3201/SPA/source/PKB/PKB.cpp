#include <string>

#include "Constants.h"
#include "PKB/AST.h"
#include "PKB/PKB.h"
#include "TreeNode.h"
#include "Table.h"

PKB::PKB() {}

PKB::~PKB() {}

void PKB::SetASTRoot(TreeNode *root) {
    AST::getInstance()->setRoot(root);
}

TreeNode* PKB::GetASTRoot() {
    return AST::getInstance()->getRoot();
}

TreeNode* PKB::CreateASTNode(Symbol symbol) {
    return AST::getInstance()->createNode(symbol);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, std::string value) {
    return AST::getInstance()->createNode(symbol, value);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, int lineNumber) {
    return AST::getInstance()->createNode(symbol, lineNumber);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, int lineNumber, std::string value) {
    return AST::getInstance()->createNode(symbol, lineNumber, value);
}

void PKB::PrintASTTree() {
    AST::getInstance()->printTree();
}

static void InsertVariable(std::string variable_names[]) {

}S

static bool IsVariableExist(std::string variable_name) {

}

static void InsertProcedure(std::string procedure_names[]) {
}

static void InsertProcedureLineNumber(std::string procedure_name, int line_numbers[]) {

}

static bool IsProcedureExist(std::string procedure_name) {

}


static void InsertUses(int line_number, std::string variable_names[]) {

}

static void InsertUses(std::string procedure_name, std::string variable_names[]) {

}

static std::string[] UsesVariable(int line_number) {

}

static std::string[] UsesVariable(std::string procedure_name) {

}

static std::string[] UsedByLineNumber(std::string variable_name) {

}

static std::string[] UsedByProcedure(std::string variable_name) {

}

static void InsertModifies(int line_number, std::string variable_names[]) {

}

static void InsertModifies(std::string procedure_name, std::string variable_names[]) {

}

static std::string[] ModifiesVariable(int line_number) {

}

static std::string[] ModifiesVariable(std::string procedure_name) {

}

static std::string[] ModifiedByLineNumber(std::string variable_name) {

}

static std::string[] ModifiedByProcedure(std::string variable_name) {

}

static void InsertCalls(std::string procedure_name1, std::string procedure_names[]) {

}

static void InsertCallsTransitive(std::string procedure_name1, std::string procedure_names[]) {

}

static boolean IsCalls(std::string procedure_name1, std::string procedure_name2) {

}

static boolean IsCallsTransitive(std::string procedure_name1, std::string procedure_name2) {

}

static void InsertParent(int line_number1, int line_number2) {

}

static void InsertParentTransitive(int line_number1, int line_number2) {

}

static boolean IsParent(int line_number1, int line_number2) {

}

static boolean IsParentTransitive(int line_number1, int line_number2) {

}

static void InsertFollows(int line_number1, int line_number2) {

}

static void InsertFollowsTransitive(int line_number1, int line_number2) {

}

static boolean IsFollows(int line_number1, int line_number2) {

}

static boolean IsFollowsTransitive(int line_number1, int line_number2) {

}

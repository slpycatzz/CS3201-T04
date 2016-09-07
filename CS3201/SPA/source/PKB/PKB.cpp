#include <string>
#include <vector>

#include "Constants.h"
#include "PKB/AST.h"
#include "PKB/PKB.h"
#include "TreeNode.h"
#include "PKB/Table.h"

using std::string;
using std::vector;

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

TreeNode* PKB::CreateASTNode(Symbol symbol, string value) {
    return AST::getInstance()->createNode(symbol, value);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, int lineNumber) {
    return AST::getInstance()->createNode(symbol, lineNumber);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, int lineNumber, string value) {
    return AST::getInstance()->createNode(symbol, lineNumber, value);
}

void PKB::PrintASTTree() {
    AST::getInstance()->printTree();
}

void PKB::InsertVariable(string variable_name, int line_number) {
}

void PKB::InsertVariable(string variable_name, vector<int> line_numbers) {
}

bool PKB::IsVariableExist(string variable_name) {
}

void PKB::InsertProcedure(string procedure_name, int line_number) {
}

void PKB::InsertProcedure(string procedure_name, vector<int> line_numbers) {
}

bool PKB::IsProcedureExist(string procedure_name) {
  return false;
}

void PKB::InsertUses(int line_number, string variable_name) {
}

void PKB::InsertUses(int line_number, vector<string> variable_names) {
}

void PKB::InsertUses(string procedure_name, string variable_name) {
}

void PKB::InsertUses(string procedure_name, vector<string> variable_names) {
}

vector<string> PKB::UsesVariable(int line_number) {
}

vector<string> PKB::UsesVariable(string procedure_name) {
}

vector<int> PKB::UsedByLineNumber(string variable_name) {
}

vector<string> PKB::UsedByProcedure(string variable_name) {
}

void PKB::InsertModifies(int line_number, string variable_name) {
}

void PKB::InsertModifies(int line_number, vector<string> variable_names) {
}

void PKB::InsertModifies(string procedure_name, string variable_name) {
}

void PKB::InsertModifies(string procedure_name, vector<string> variable_names) {
}

vector<string> PKB::ModifiesVariable(int line_number) {
}

vector<string> PKB::ModifiesVariable(string procedure_name) {
}

vector<int> PKB::ModifiedByLineNumber(string variable_name) {
}

vector<string> PKB::ModifiedByProcedure(string variable_name) {
}

void PKB::InsertCalls(string procedure_name1, string procedure_name2) {
}

void PKB::InsertCalls(string procedure_name1, vector<string> procedure_names) {
}

void PKB::InsertCallsTransitive(string procedure_name1, string procedure_name2) {
}

void PKB::InsertCallsTransitive(string procedure_name1, vector<string> procedure_names) {
}

bool PKB::IsCalls(string procedure_name1, string procedure_name2) {
  return false;
}

bool PKB::IsCallsTransitive(string procedure_name1, string procedure_name2) {
  return false;
}

void PKB::InsertParent(int line_number1, int line_number2) {
}

void PKB::InsertParent(int line_number1, vector<int> line_numbers) {
}

void PKB::InsertParentTransitive(int line_number1, int line_number2) {
}

void PKB::InsertParentTransitive(int line_number1, vector<int> line_numbers) {
}

bool PKB::IsParent(int line_number1, int line_number2) {
  return false;
}

bool PKB::IsParent(int line_number1, vector<int> line_numbers) {
  return false;
}

bool PKB::IsParentTransitive(int line_number1, int line_number2) {
  return false;
}

bool PKB::IsParentTransitive(int line_number1, vector<int> line_numbers) {
  return false;
}

void PKB::InsertFollows(int line_number1, int line_number2) {
}

void PKB::InsertFollows(int line_number1, vector<int> line_numbers) {
}

void PKB::InsertFollowsTransitive(int line_number1, int line_number2) {
}

void PKB::InsertFollowsTransitive(int line_number1, vector<int> line_numbers) {
}

bool PKB::IsFollows(int line_number1, int line_number2) {
  return false;
}

bool PKB::IsFollows(int line_number1, vector<int> line_numbers) {
  return false;
}

bool PKB::IsFollowsTransitive(int line_number1, int line_number2) {
  return false;
}

bool PKB::IsFollowsTransitive(int line_number1, vector<int> line_numbers) {
  return false;
}

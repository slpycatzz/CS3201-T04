#include <string>
#include <vector>
#include "Constants.h"
#include "PKB/AST.h"
#include "PKB/PKB.h"
#include "TreeNode.h"
#include "PKB/Table.h"

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

void PKB::InsertVariable(std::vector<std::string> variable_names) {
  PKB::variable_table = variable_names;
}

bool PKB::IsVariableExist(std::string variable_name) {
  return false;
}

void PKB::InsertProcedure(std::vector<std::string> procedure_names) {
  PKB::procedure_table = procedure_names;
}

void PKB::InsertProcedureLineNumber(std::string procedure_name, std::vector<int> line_numbers) {
  PKB::procedure_line_table.Insert(procedure_name, line_numbers);
}

bool PKB::IsProcedureExist(std::string procedure_name) {
  return false;
}

void PKB::InsertUses(int line_number, std::vector<std::string> variable_names) {
  PKB::uses_table.Insert(line_number, variable_names);
}

void PKB::InsertUses(std::string procedure_name, std::vector<std::string> variable_names) {
  PKB::uses_procedure_table.Insert(procedure_name, variable_names);
}

std::vector<std::string> PKB::UsesVariable(int line_number) {
  return PKB::uses_table.GetValue(line_number);
}

std::vector<std::string> PKB::UsesVariable(std::string procedure_name) {
  return PKB::uses_procedure_table.GetValue(procedure_name);
}

// TODO(pixelducky): update this function
std::vector<int> PKB::UsedByLineNumber(std::string variable_name) {
  //return PKB::uses_table.GetKey(variable_name);
  return std::vector<int>();
}

// TODO(pixelducky): update this function
std::vector<std::string> PKB::UsedByProcedure(std::string variable_name) {
  //return PKB::uses_procedure_table.GetKey(variable_name);
  return std::vector<std::string>();
}

void PKB::InsertModifies(int line_number, std::vector<std::string> variable_names) {
  PKB::modifies_table.Insert(line_number, variable_names);
}

void PKB::InsertModifies(std::string procedure_name, std::vector<std::string> variable_names) {
  PKB::modifies_procedure_table.Insert(procedure_name, variable_names);
}

std::vector<std::string> PKB::ModifiesVariable(int line_number) {
  return PKB::modifies_table.GetValue(line_number);
}

std::vector<std::string> PKB::ModifiesVariable(std::string procedure_name) {
  return PKB::modifies_procedure_table.GetValue(procedure_name);
}

// TODO(pixelducky): update this function
std::vector<int> PKB::ModifiedByLineNumber(std::string variable_name) {
  //return PKB::modifies_table.GetKey(variable_name);
  return std::vector<int>();
}

// TODO(pixelducky): update this function
std::vector<std::string> PKB::ModifiedByProcedure(std::string variable_name) {
  //return PKB::modifies_procedure_table.GetKey(variable_name);
  return std::vector<std::string>();
}

void PKB::InsertCalls(std::string procedure_name1, std::vector<std::string> procedure_names) {
  PKB::calls_table.Insert(procedure_name1, procedure_names);
}

void PKB::InsertCallsTransitive(std::string procedure_name1, std::vector<std::string> procedure_names) {
  PKB::calls_transitive_table.Insert(procedure_name1, procedure_names);
}

// TODO(pixelducky): update this function
bool PKB::IsCalls(std::string procedure_name1, std::string procedure_name2) {
  return false;
}

// TODO(pixelducky): update this function
bool PKB::IsCallsTransitive(std::string procedure_name1, std::string procedure_name2) {
  return false;
}

void PKB::InsertParent(int line_number1, std::vector<int> line_numbers) {
  PKB::parent_table.Insert(line_number1, line_numbers);
}

void PKB::InsertParentTransitive(int line_number1, std::vector<int> line_numbers) {
  PKB::parent_transitive_table.Insert(line_number1, line_numbers);
}

// TODO(pixelducky): update this function
bool PKB::IsParent(int line_number1, int line_number2) {
  return false;
}

// TODO(pixelducky): update this function
bool PKB::IsParentTransitive(int line_number1, int line_number2) {
  return false;
}

void PKB::InsertFollows(int line_number1, std::vector<int> line_numbers) {
  PKB::follows_table.Insert(line_number1, line_numbers);
}

void PKB::InsertFollowsTransitive(int line_number1, std::vector<int> line_numbers) {
  PKB::follows_transitive_table.Insert(line_number1, line_numbers);
}

// TODO(pixelducky): update this function
bool PKB::IsFollows(int line_number1, int line_number2) {
  return false;
}

// TODO(pixelducky): update this function
bool PKB::IsFollowsTransitive(int line_number1, int line_number2) {
  return false;
}

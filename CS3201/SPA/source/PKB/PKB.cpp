#include <string>

#include "Constants.h"
#include "PKB/AST.h"
#include "PKB/PKB.h"
#include "TreeNode.h"

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

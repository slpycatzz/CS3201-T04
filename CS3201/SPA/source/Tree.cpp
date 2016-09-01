#include <string>

#include "Tree.h"

Tree::Tree() {}

Tree::~Tree() {}

void Tree::setRoot(TreeNode *root) {
    this->root_ = root;
}

TreeNode* Tree::getRoot() {
    return root_;
}

TreeNode* Tree::createNode(Symbol symbol) {
    return new TreeNode(symbol);
}

TreeNode* Tree::createNode(Symbol symbol, std::string value) {
    return new TreeNode(symbol, value);
}

TreeNode* Tree::createNode(Symbol symbol, int lineNumber) {
    return new TreeNode(symbol, lineNumber);
}

TreeNode* Tree::createNode(Symbol symbol, int lineNumber, std::string value) {
    return new TreeNode(symbol, lineNumber, value);
}

#include <string>

#include "Constants.h"
#include "Tree.h"
#include "TreeNode.h"

using std::string;

Tree::Tree() {}

Tree::~Tree() {}

void Tree::setRoot(TreeNode *root) {
    root_ = root;
}

TreeNode* Tree::getRoot() {
    return root_;
}

TreeNode* Tree::createNode(Symbol symbol) {
    return new TreeNode(symbol);
}

TreeNode* Tree::createNode(Symbol symbol, string value) {
    return new TreeNode(symbol, value);
}

TreeNode* Tree::createNode(Symbol symbol, int lineNumber) {
    return new TreeNode(symbol, lineNumber);
}

TreeNode* Tree::createNode(Symbol symbol, int lineNumber, string value) {
    return new TreeNode(symbol, lineNumber, value);
}

void Tree::printTree() {
    root_->printTreeNode(0);
}

#include <iomanip>
#include <iostream>
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
    printTreeNode(root_, 0);
}

void Tree::printTreeNode(TreeNode* node, unsigned int indent) {
    if (node != NULL) {
        std::cout << std::setw(indent) << ' ';

        if (node->getStmtNumber() != -1) {
            std::cout << node->getStmtNumber() << ". ";
        }

        if (node->getValue() != "") {
            std::cout << node->getValue() << ':';
        }

        std::cout << Constants::SymbolToString(node->getSymbol()) << std::endl;
        for (const auto &tempNode : node->getChildren()) {
            printTreeNode(tempNode, (indent + 4));
        }
    }
}

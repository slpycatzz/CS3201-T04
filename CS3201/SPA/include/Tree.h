#pragma once

#include "TreeNode.h"

class Tree {
 public:
    Tree();
    ~Tree();
    void setRoot(TreeNode *root);
    TreeNode* getRoot();
    TreeNode* createNode(Symbol symbol);
    TreeNode* createNode(Symbol symbol, std::string value);
    TreeNode* createNode(Symbol symbol, int lineNumber);
    TreeNode* createNode(Symbol symbol, int lineNumber, std::string value);
    void printTree();

 private:
    TreeNode *root_;
    void printTreeNode(TreeNode* node, unsigned int indent);
};


#pragma once

#include <string>
#include <vector>

#include "Constants.h"

class TreeNode {
 public:
    TreeNode(Symbol symbol);
    TreeNode(Symbol symbol, std::string value);
    TreeNode(Symbol symbol, unsigned int stmtNumber);
    TreeNode(Symbol symbol, unsigned int stmtNumber, std::string value);
    ~TreeNode();

    Symbol getSymbol();

    unsigned int getStmtNumber();

    void setValue(std::string value);
    std::string getValue();

    std::vector<TreeNode*> getChildren();
    unsigned int getNumberOfChildren();
    void addChild(TreeNode *child);

    void printTreeNode(unsigned int indent);

 private:
    Symbol symbol_;
    unsigned int stmtNumber_;
    std::string value_;
    std::vector<TreeNode*> children_;
};

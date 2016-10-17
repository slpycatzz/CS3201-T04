#pragma once

#include <string>
#include <vector>

#include "Constants.h"

class TreeNode {
 public:
    TreeNode(Symbol symbol);
    TreeNode(unsigned int stmtNumber);
    TreeNode(Symbol symbol, std::string value);
    TreeNode(Symbol symbol, unsigned int stmtNumber);
    TreeNode(Symbol symbol, unsigned int stmtNumber, std::string value);
    ~TreeNode();

    Symbol getSymbol();
    unsigned int getStmtNumber();
    std::string getValue();
    bool isVisited();
    std::vector<TreeNode*> getChildren();
    unsigned int getNumberOfChildren();

    void setValue(std::string value);
    void setVisited(bool visited);
    void addChild(TreeNode *child);

    void printTreeNode(unsigned int indent);

 private:
    Symbol symbol_;
    unsigned int stmtNumber_;
    std::string value_;
    bool visited_;
    std::vector<TreeNode*> children_;
};

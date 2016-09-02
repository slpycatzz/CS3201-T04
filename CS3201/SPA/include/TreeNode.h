#pragma once

#include <string>
#include <vector>

#include "Constants.h"

class TreeNode {
 public:
    TreeNode(Symbol symbol);
    TreeNode(Symbol symbol, std::string value);
    TreeNode(Symbol symbol, int lineNumber);
    TreeNode(Symbol symbol, int lineNumber, std::string value);
    ~TreeNode();
    Symbol getSymbol();
    int getLineNumber();
    void setValue(std::string value);
    std::string getValue();
    std::vector<TreeNode*> getChildren();
    int getNumberOfChildren();
    void addChild(TreeNode *child);

 private:
    Symbol symbol_;
    int lineNumber_;
    std::string value_;
    std::vector<TreeNode*> children_;
};

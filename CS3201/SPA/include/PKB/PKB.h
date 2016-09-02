#pragma once

#include <string>

#include "Constants.h"
#include "TreeNode.h"

class PKB {
 public:
    PKB();
    ~PKB();
    static void SetASTRoot(TreeNode *root);
    static TreeNode* GetASTRoot();
    static TreeNode* CreateASTNode(Symbol symbol);
    static TreeNode* CreateASTNode(Symbol, std::string value);
    static TreeNode* CreateASTNode(Symbol symbol, int lineNumber);
    static TreeNode* CreateASTNode(Symbol symbol, int lineNumber, std::string value);
    static void PrintASTTree();
};

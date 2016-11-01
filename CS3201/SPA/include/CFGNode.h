#pragma once

#include <set>
#include <string>
#include <vector>

#include "Constants.h"

class CFGNode {
 public:
    CFGNode(Symbol symbol);
    CFGNode(unsigned int stmtNumber);
    CFGNode(Symbol symbol, unsigned int stmtNumber);
    ~CFGNode();

    void setSymbol(Symbol symbol);
    Symbol getSymbol();

    void setStmtNumber(unsigned int stmtNumber);
    unsigned int getStmtNumber();

    void setVisited(bool visited);
    bool isVisited();

    void addChild(CFGNode *child);
    std::vector<CFGNode*> getChildren();
    unsigned int getNumberOfChildren();

    void setModifies(unsigned int modifies);
    unsigned int getModifies();

    void setUses(std::set<unsigned int> uses);
    std::set<unsigned int> getUses();

    void printCFGNode(unsigned int indent);

 private:
    Symbol symbol_;
    unsigned int stmtNumber_;
    bool visited_;
    std::vector<CFGNode*> children_;

    unsigned int modifies_;
    std::set<unsigned int> uses_;
};

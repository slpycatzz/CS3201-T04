#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "PKB/CFGNode.h"

using std::string;
using std::vector;

CFGNode::CFGNode(Symbol symbol) {
    symbol_ = symbol;
    stmtNumber_ = 0;
    visited_ = false;
    modify_ = 0;
}

CFGNode::CFGNode(unsigned int stmtNumber) {
    symbol_ = INVALID;
    stmtNumber_ = stmtNumber;
    visited_ = false;
    modify_ = 0;
}

CFGNode::CFGNode(Symbol symbol, unsigned int stmtNumber) {
    symbol_ = symbol;
    stmtNumber_ = stmtNumber;
    visited_ = false;
    modify_ = 0;
}

CFGNode::~CFGNode() {}

void CFGNode::setSymbol(Symbol symbol) {
    symbol_ = symbol;
}

Symbol CFGNode::getSymbol() {
    return symbol_;
}

void CFGNode::setStmtNumber(unsigned int stmtNumber) {
    stmtNumber_ = stmtNumber;
}

unsigned int CFGNode::getStmtNumber() {
    return stmtNumber_;
}

void CFGNode::setVisited(bool visited) {
    visited_ = visited;
}

bool CFGNode::isVisited() {
    return visited_;
}

void CFGNode::addChild(CFGNode* child) {
    children_.push_back(child);
}

vector<CFGNode*> CFGNode::getChildren() {
    return children_;
}

unsigned int CFGNode::getNumberOfChildren() {
    return children_.size();
}

void CFGNode::addParent(CFGNode* parent) {
    parents_.push_back(parent);
}

vector<CFGNode*> CFGNode::getParents() {
    return parents_;
}

unsigned int CFGNode::getNumberOfParents() {
    return parents_.size();
}

void CFGNode::setModify(unsigned int modify) {
    modify_ = modify;
    modifies_.push_back(modify);
}

unsigned int CFGNode::getModify() {
    return modify_;
}

void CFGNode::setModifies(vector<unsigned int> modifies) {
    modifies_ = modifies;
}

vector<unsigned int> CFGNode::getModifies() {
    return modifies_;
}

void CFGNode::setUses(vector<unsigned int> uses) {
    uses_ = uses;
}

vector<unsigned int> CFGNode::getUses() {
    return uses_;
}

void CFGNode::printCFGNode(unsigned int indent) {

    std::cout << std::setw(indent) << ' ';

    if (stmtNumber_ != 0) {
        std::cout << stmtNumber_ << ". ";
    }

    if (!modifies_.empty()) {
        std::cout << "Modifies : { ";

        for (unsigned int variableIndex : modifies_) {
            std::cout << variableIndex << " ";
        }

        std::cout << "}";
    }

    if (!uses_.empty()) {
        std::cout << "Uses : { ";
        
        for (unsigned int variableIndex : uses_) {
            std::cout << variableIndex << " ";
        }

        std::cout << "}";
    }

    if (symbol_ != INVALID) {
        std::cout << Constants::SymbolToString(symbol_);
    }

    std::cout << std::endl;
    
    if (!visited_) {
            visited_ = true;
        for (const auto &tempNode : children_) {
            tempNode->printCFGNode(indent + 4);
        }
    }

    visited_ = false;
}

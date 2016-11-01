#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "CFGNode.h"

using std::set;
using std::string;
using std::vector;

CFGNode::CFGNode(Symbol symbol) {
    symbol_ = symbol;
    stmtNumber_ = 0;
    visited_ = false;
    modifies_ = 0;
}

CFGNode::CFGNode(unsigned int stmtNumber) {
    symbol_ = INVALID;
    stmtNumber_ = stmtNumber;
    visited_ = false;
    modifies_ = 0;
}

CFGNode::CFGNode(Symbol symbol, unsigned int stmtNumber) {
    symbol_ = symbol;
    stmtNumber_ = stmtNumber;
    visited_ = false;
    modifies_ = 0;
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

void CFGNode::addChild(CFGNode *child) {
    children_.push_back(child);
}

vector<CFGNode*> CFGNode::getChildren() {
    return children_;
}

unsigned int CFGNode::getNumberOfChildren() {
    return children_.size();
}

void CFGNode::setModifies(unsigned int modifies) {
    modifies_ = modifies;
}

unsigned int CFGNode::getModifies() {
    return modifies_;
}

void CFGNode::setUses(set<unsigned int> uses) {
    uses_ = uses;
}

set<unsigned int> CFGNode::getUses() {
    return uses_;
}

void CFGNode::printCFGNode(unsigned int indent) {

    std::cout << std::setw(indent) << ' ';

    if (stmtNumber_ != 0) {
        std::cout << stmtNumber_ << ". ";
    }

    if (modifies_ != 0) {
        std::cout << "Modifies: { " << modifies_ << " } ";
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

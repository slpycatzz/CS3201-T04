#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "TreeNode.h"

using std::string;
using std::vector;

TreeNode::TreeNode(Symbol symbol) {
    symbol_ = symbol;
    stmtNumber_ = -1;
    value_ = "";
    visited_ = false;
}

TreeNode::TreeNode(unsigned int stmtNumber) {
    symbol_ = INVALID;
    stmtNumber_ = stmtNumber;
    value_ = "";
    visited_ = false;
}

TreeNode::TreeNode(Symbol symbol, string value) {
    symbol_ = symbol;
    stmtNumber_ = -1;
    value_ = value;
    visited_ = false;
}

TreeNode::TreeNode(Symbol symbol, unsigned int stmtNumber) {
    symbol_ = symbol;
    stmtNumber_ = stmtNumber;
    value_ = "";
    visited_ = false;
}

TreeNode::TreeNode(Symbol symbol, unsigned int stmtNumber, string value) {
    symbol_ = symbol;
    stmtNumber_ = stmtNumber;
    value_ = value;
    visited_ = false;
}

TreeNode::~TreeNode() {}

Symbol TreeNode::getSymbol() {
    return symbol_;
}

unsigned int TreeNode::getStmtNumber() {
    return stmtNumber_;
}

string TreeNode::getValue() {
    return value_;
}

bool TreeNode::isVisited() {
    return visited_;
}

vector<TreeNode*> TreeNode::getChildren() {
    return children_;
}

unsigned int TreeNode::getNumberOfChildren() {
    return children_.size();
}

void TreeNode::setValue(string value) {
    value_ = value;
}

void TreeNode::setVisited(bool visited) {
    visited_ = visited;
}

void TreeNode::addChild(TreeNode *child) {
    children_.push_back(child);
}

void TreeNode::printTreeNode(unsigned int indent) {

    std::cout << std::setw(indent) << ' ';

    if (stmtNumber_ != -1) {
        std::cout << stmtNumber_ << ". ";
    }

    if (value_ != "") {
        std::cout << value_ << ':';
    }

    if (symbol_ != INVALID) {
        std::cout << Constants::SymbolToString(symbol_);
    }

    std::cout << std::endl;
    
    if (!visited_) {
            visited_ = true;
        for (const auto &tempNode : children_) {
            tempNode->printTreeNode(indent + 4);
        }
    }

    visited_ = false;
}

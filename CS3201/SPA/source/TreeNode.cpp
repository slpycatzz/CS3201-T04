#include <string>
#include <vector>

#include "TreeNode.h"

using std::string;
using std::vector;

TreeNode::TreeNode(Symbol symbol) {
    this->symbol_ = symbol;
    this->stmtNumber_ = -1;
    this->value_ = "";
}

TreeNode::TreeNode(Symbol symbol, string value) {
    this->symbol_ = symbol;
    this->stmtNumber_ = -1;
    this->value_ = value;
}

TreeNode::TreeNode(Symbol symbol, unsigned int stmtNumber) {
    this->symbol_ = symbol;
    this->stmtNumber_ = stmtNumber;
    this->value_ = "";
}

TreeNode::TreeNode(Symbol symbol, unsigned int stmtNumber, string value) {
    this->symbol_ = symbol;
    this->stmtNumber_ = stmtNumber;
    this->value_ = value;
}

TreeNode::~TreeNode() {}

Symbol TreeNode::getSymbol() {
    return symbol_;
}

unsigned int TreeNode::getStmtNumber() {
    return stmtNumber_;
}

void TreeNode::setValue(string value) {
    this->value_ = value;
}

string TreeNode::getValue() {
    return value_;
}

vector<TreeNode*> TreeNode::getChildren() {
    return children_;
}

unsigned int TreeNode::getNumberOfChildren() {
    return children_.size();
}

void TreeNode::addChild(TreeNode *child) {
    children_.push_back(child);
}

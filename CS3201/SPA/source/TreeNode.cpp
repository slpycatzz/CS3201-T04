#include <string>
#include <vector>

#include "TreeNode.h"

using std::string;
using std::vector;

TreeNode::TreeNode(Symbol symbol) {
    this->symbol_ = symbol;
    this->lineNumber_ = -1;
    this->value_ = "";
}

TreeNode::TreeNode(Symbol symbol, string value) {
    this->symbol_ = symbol;
    this->lineNumber_ = -1;
    this->value_ = value;
}

TreeNode::TreeNode(Symbol symbol, int lineNumber) {
    this->symbol_ = symbol;
    this->lineNumber_ = lineNumber;
    this->value_ = "";
}

TreeNode::TreeNode(Symbol symbol, int lineNumber, string value) {
    this->symbol_ = symbol;
    this->lineNumber_ = lineNumber;
    this->value_ = value;
}

TreeNode::~TreeNode() {}

Symbol TreeNode::getSymbol() {
    return symbol_;
}

int TreeNode::getLineNumber() {
    return lineNumber_;
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

int TreeNode::getNumberOfChildren() {
    return children_.size();
}

void TreeNode::addChild(TreeNode *child) {
    children_.push_back(child);
}

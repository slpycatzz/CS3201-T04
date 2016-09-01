#include <string>

#include "Constants.h"
#include "TreeNode.h"

TreeNode::TreeNode(Symbol symbol) {
    this->symbol_ = symbol;
}

TreeNode::TreeNode(Symbol symbol, std::string value) {
    this->symbol_ = symbol;
    this->value_ = value;
}

TreeNode::TreeNode(Symbol symbol, int lineNumber) {
    this->symbol_ = symbol;
    this->lineNumber_ = lineNumber;
}

TreeNode::TreeNode(Symbol symbol, int lineNumber, std::string value) {
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

void TreeNode::setValue(std::string value) {
    this->value_ = value;
}

std::string TreeNode::getValue() {
    return value_;
}

std::vector<TreeNode*> TreeNode::getChildren() {
    return children_;
}

int TreeNode::getNumberOfChildren() {
    return children_.size();
}

void TreeNode::addChild(TreeNode *child) {
    children_.push_back(child);
}

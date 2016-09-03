#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Constants.h"
#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "PKB/PKB.h"
#include "TreeNode.h"
#include "Utils.h"

FrontendParser::FrontendParser() {
    this->tokensIndex_ = 0;
    this->lineNumber_ = 1;
}

FrontendParser::~FrontendParser() {}

void FrontendParser::parseProgram(std::string filePath) {
    std::ifstream fileStream(filePath);

    /* Validate if file exists. */
    if (!fileStream.is_open()) {
        throw FileNotFoundException();
    }

    splitProgramLinesIntoTokens(preprocessProgramLines(fileStream));

    PKB::SetASTRoot(callProgramRecognizer());

    // TODO(YH): Recursive call check
    // TODO(YH): Invalid procedure call check (procedure does not exist)
}

std::vector<std::string> FrontendParser::preprocessProgramLines(std::ifstream& fileStream) {
    std::string currentLine;
    std::vector<std::string> programLines;

    while (std::getline(fileStream, currentLine)) {
        int position;

        /* If there are any comments in the program line, remove it. */
        position = currentLine.find(SYMBOL_COMMENT);
        if (position != std::string::npos) {
            currentLine = currentLine.substr(0, position);
        }

        currentLine = Utils::TrimLeadingSpaces(currentLine);
        currentLine = Utils::TrimTrailingSpaces(currentLine);

        /* Ignore empty lines. */
        if (currentLine.empty()) {
            continue;
        }

        /* Append leading and trailing whitespace to char symbols. */
        for (unsigned int i = 0; i < currentLine.size(); i++) {
            char currentChar = currentLine.at(i);

            switch (currentChar) {
                default:
                    continue;

                case CHAR_SYMBOL_EQUAL:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_EQUAL));
                    break;

                case CHAR_SYMBOL_MINUS:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_MINUS));
                    break;

                case CHAR_SYMBOL_PLUS:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_PLUS));
                    break;

                case CHAR_SYMBOL_MULTIPLY:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_MULTIPLY));
                    break;

                case CHAR_SYMBOL_SEMICOLON:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_SEMICOLON));
                    break;

                case CHAR_SYMBOL_OPENCURLYBRACKET:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_OPENCURLYBRACKET));
                    break;

                case CHAR_SYMBOL_CLOSECURLYBRACKET:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_CLOSECURLYBRACKET));
                    break;

                case CHAR_SYMBOL_OPENBRACKET:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_OPENBRACKET));
                    break;

                case CHAR_SYMBOL_CLOSEBRACKET:
                    currentLine = currentLine.replace(i, 1, Constants::GetPreprocessFormat(CHAR_SYMBOL_CLOSEBRACKET));
                    break;
            }

            i += 2;
        }

        programLines.push_back(currentLine);
    }

    fileStream.close();

    return programLines;
}

void FrontendParser::splitProgramLinesIntoTokens(std::vector<std::string> programLines) {
    for (unsigned int i = 0; i < programLines.size(); i++) {
        std::string str;
        std::istringstream stringStream;

        stringStream.str(programLines[i]);
        while (stringStream >> str) {
            this->tokens_.push_back(str);
        }
    }
}

TreeNode* FrontendParser::callProgramRecognizer() {
    /* Node construction. */
    TreeNode* programNode = PKB::CreateASTNode(PROGRAM);
    while (this->tokensIndex_ < this->tokens_.size()) {
        programNode->addChild(callProcedureRecognizer());
    }

    return programNode;
}

TreeNode* FrontendParser::callProcedureRecognizer() {
    expect(SYMBOL_PROCEDURE);

    /* Validate if procedure name follows naming convention. */
    if (!Utils::IsValidNamingConvention(peekTokens())) {
        throw ProgramSyntaxErrorException();
    }

    std::string procedureName = getToken();

    /* Node construction. */
    TreeNode* procedureNode = PKB::CreateASTNode(PROCEDURE, procedureName);
    procedureNode->addChild(callStmtListRecognizer());

    return procedureNode;
}

TreeNode* FrontendParser::callStmtListRecognizer() {
    expect(CHAR_SYMBOL_OPENCURLYBRACKET);

    /* Node construction. */
    TreeNode* stmtListNode = PKB::CreateASTNode(STMTLIST);
    while (peekTokens() != std::string(1, CHAR_SYMBOL_CLOSECURLYBRACKET)) {
        stmtListNode->addChild(callStmtRecognizer());
    }

    expect(CHAR_SYMBOL_CLOSECURLYBRACKET);

    /* Validate if statement list is empty. */
    if (stmtListNode->getNumberOfChildren() == 0) {
        throw ProgramLogicErrorException();
    }

    return stmtListNode;
}

TreeNode* FrontendParser::callStmtRecognizer() {
    TreeNode* stmtNode;

    /* To catch special cases of assign statement where the variable name is same as certain symbols. */
    if (peekForwardTokens(1) == std::string(1, CHAR_SYMBOL_EQUAL)) {
        stmtNode = callAssignRecognizer();
    } else if (accept(SYMBOL_WHILE)) {
        stmtNode = callWhileRecognizer();
    } else if (accept(SYMBOL_IF)) {
        stmtNode = callIfRecognizer();
    } else if (accept(SYMBOL_CALL)) {
        stmtNode = callCallRecognizer();
    } else {
        throw ProgramSyntaxErrorException();
    }

    return stmtNode;
}

TreeNode* FrontendParser::callWhileRecognizer() {
    TreeNode* whileNode = PKB::CreateASTNode(WHILE, lineNumber_++);

    std::string controlVariableName = getToken();

    whileNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));
    whileNode->addChild(callStmtListRecognizer());

    return whileNode;
}

TreeNode* FrontendParser::callIfRecognizer() {
    TreeNode* ifNode = PKB::CreateASTNode(IF, lineNumber_++);

    std::string controlVariableName = getToken();

    ifNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));

    expect(SYMBOL_IF_THEN);

    ifNode->addChild(callStmtListRecognizer());

    expect(SYMBOL_IF_ELSE);

    ifNode->addChild(callStmtListRecognizer());

    return ifNode;
}

TreeNode* FrontendParser::callCallRecognizer() {
    std::string procedureName = getToken();

    TreeNode* callNode = PKB::CreateASTNode(CALL, lineNumber_++, procedureName);

    expect(CHAR_SYMBOL_SEMICOLON);

    return callNode;
}

TreeNode* FrontendParser::callAssignRecognizer() {
    TreeNode* assignNode = PKB::CreateASTNode(ASSIGN, lineNumber_++);

    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(peekTokens())) {
        throw ProgramSyntaxErrorException();
    }

    std::string controlVariableName = getToken();

    assignNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));

    expect(CHAR_SYMBOL_EQUAL);

    assignNode->addChild(callExpressionRecognizer());

    expect(CHAR_SYMBOL_SEMICOLON);

    return assignNode;
}

TreeNode*  FrontendParser::callExpressionRecognizer() {
    TreeNode* expressionNode = callTermRecognizer();

    while (true) {
        TreeNode* node;

        if (accept(CHAR_SYMBOL_PLUS)) {
            node = PKB::CreateASTNode(PLUS);
        } else if (accept(CHAR_SYMBOL_MINUS)) {
            node = PKB::CreateASTNode(MINUS);
        } else {
            break;
        }

        node->addChild(expressionNode);
        node->addChild(callTermRecognizer());

        expressionNode = node;
    }

    return expressionNode;
}

TreeNode* FrontendParser::callTermRecognizer() {
    TreeNode* termNode = callFactorRecognizer();

    while (accept(CHAR_SYMBOL_MULTIPLY)) {
        TreeNode* multiplyNode = PKB::CreateASTNode(MULTIPLY);
        multiplyNode->addChild(termNode);
        multiplyNode->addChild(callFactorRecognizer());

        termNode = multiplyNode;
    }

    return termNode;
}

TreeNode* FrontendParser::callFactorRecognizer() {
    TreeNode* factorNode;

    /* Higher precedence for expressions in (). */
    if (accept(CHAR_SYMBOL_OPENBRACKET)) {
        factorNode = callExpressionRecognizer();
        expect(CHAR_SYMBOL_CLOSEBRACKET);

    /* Variable. */
    } else if (Utils::IsValidNamingConvention(peekTokens())) {
        factorNode = PKB::CreateASTNode(VARIABLE, getToken());

    /* Constant. */
    } else if (Utils::IsNonNegativeNumeric(peekTokens())) {
        factorNode = PKB::CreateASTNode(CONSTANT, getToken());

    } else {
        throw ProgramSyntaxErrorException();
    }

    return factorNode;
}

void FrontendParser::expect(std::string token) {
    if (!accept(token)) {
        throw ProgramSyntaxErrorException();
    }
}

void FrontendParser::expect(char token) {
    if (!accept(token)) {
        throw ProgramSyntaxErrorException();
    }
}

bool FrontendParser::accept(std::string token) {
    if (peekTokens() == token) {
        getToken();
        return true;
    }

    return false;
}

bool FrontendParser::accept(char token) {
    if (peekTokens() == std::string(1, token)) {
        getToken();
        return true;
    }

    return false;
}

std::string FrontendParser::peekTokens() {
    return peekForwardTokens(0);
}

std::string FrontendParser::peekForwardTokens(unsigned int index) {
    unsigned int forwardIndex = this->tokensIndex_ + index;

    return (forwardIndex >= this->tokens_.size()) ? "" : this->tokens_[forwardIndex];
}

std::string FrontendParser::getToken() {
    return this->tokens_[this->tokensIndex_++];
}

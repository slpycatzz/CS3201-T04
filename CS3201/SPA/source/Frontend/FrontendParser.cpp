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

using std::string;
using std::vector;

FrontendParser::FrontendParser() {
    this->tokensIndex_ = 0;
    this->stmtNumber_ = 1;
}

FrontendParser::~FrontendParser() {}

void FrontendParser::parseProgram(string filePath) {
    std::ifstream fileStream(filePath);

    /* Validate if file exists. */
    if (!fileStream.is_open()) {
        throw FileNotFoundException();
    }

    vector<string> programLines = preprocessProgramLines(fileStream);

    /* Split program lines into tokens. */
    for (const auto &programLine : programLines) {
        Utils::SplitAndIgnoreEmpty(programLine, ' ', this->tokens_);
    }

    PKB::SetASTRoot(callProgramRecognizer());

    /* Generic tables generation. */
    PKB::GenerateConstantTable(constants_);
    PKB::GenerateVariableTable(variableNames_);
    PKB::GenerateProcedureTable(procedureNames_);
    PKB::GenerateStmtTable(stmts_);



    // TODO(YH): Recursive call check
    // TODO(YH): Invalid procedure call check (procedure does not exist)
}

vector<string> FrontendParser::preprocessProgramLines(std::ifstream& fileStream) {
    string currentLine;
    vector<string> programLines;

    while (std::getline(fileStream, currentLine)) {
        int position;

        /* If there are any comments in the program line, remove it. */
        position = currentLine.find(SYMBOL_COMMENT);
        if (position != string::npos) {
            currentLine = currentLine.substr(0, position);
        }

        currentLine = Utils::TrimSpaces(currentLine);

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

TreeNode* FrontendParser::callProgramRecognizer() {
    /* Node generation. */
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

    string procedureName = getToken();

    /* For PKB procedure table generation. */
    this->procedureNames_.insert(procedureName);

    /* TreeNode generation. */
    TreeNode* procedureNode = PKB::CreateASTNode(PROCEDURE, procedureName);
    procedureNode->addChild(callStmtListRecognizer());

    return procedureNode;
}

TreeNode* FrontendParser::callStmtListRecognizer() {
    expect(CHAR_SYMBOL_OPENCURLYBRACKET);

    /* TreeNode generation. */
    TreeNode* stmtListNode = PKB::CreateASTNode(STMTLIST);
    while (peekTokens() != string(1, CHAR_SYMBOL_CLOSECURLYBRACKET)) {
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

    string stmt = "";
    int stmtNumber = stmtNumber_;

    /* To catch special cases of assign statement where the variable name is same as certain symbols. */
    if (peekForwardTokens(1) == string(1, CHAR_SYMBOL_EQUAL)) {
        stmt = SYMBOL_ASSIGN;
        stmtNode = callAssignRecognizer();

    } else if (accept(SYMBOL_WHILE)) {
        stmt = SYMBOL_WHILE;
        stmtNode = callWhileRecognizer();

    } else if (accept(SYMBOL_IF)) {
        stmt = SYMBOL_IF;
        throw ProgramSyntaxErrorException(); // stmtNode = callIfRecognizer();

    } else if (accept(SYMBOL_CALL)) {
        stmt = SYMBOL_CALL;
        throw ProgramSyntaxErrorException(); // stmtNode = callCallRecognizer();

    } else {
        throw ProgramSyntaxErrorException();
    }

    /* For PKB stmt table generation. */
    this->stmts_.emplace(stmtNumber, stmt);

    return stmtNode;
}

TreeNode* FrontendParser::callWhileRecognizer() {
    TreeNode* whileNode = PKB::CreateASTNode(WHILE, stmtNumber_++);

    string controlVariableName = getToken();

    whileNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));
    whileNode->addChild(callStmtListRecognizer());

    return whileNode;
}

TreeNode* FrontendParser::callIfRecognizer() {
    TreeNode* ifNode = PKB::CreateASTNode(IF, stmtNumber_++);

    string controlVariableName = getToken();

    ifNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));

    expect(SYMBOL_IF_THEN);

    ifNode->addChild(callStmtListRecognizer());

    expect(SYMBOL_IF_ELSE);

    ifNode->addChild(callStmtListRecognizer());

    return ifNode;
}

TreeNode* FrontendParser::callCallRecognizer() {
    string procedureName = getToken();

    TreeNode* callNode = PKB::CreateASTNode(CALL, stmtNumber_++, procedureName);

    expect(CHAR_SYMBOL_SEMICOLON);

    return callNode;
}

TreeNode* FrontendParser::callAssignRecognizer() {
    TreeNode* assignNode = PKB::CreateASTNode(ASSIGN, stmtNumber_++);

    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(peekTokens())) {
        throw ProgramSyntaxErrorException();
    }

    string controlVariableName = getToken();

    /* For PKB variable table generation. */
    this->variableNames_.insert(controlVariableName);

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
        string variableName = getToken();

        /* For PKB variable table generation. */
        this->variableNames_.insert(variableName);

        factorNode = PKB::CreateASTNode(VARIABLE, variableName);

    /* Constant. */
    } else if (Utils::IsNonNegativeNumeric(peekTokens())) {
        string constant = getToken();

        /* For PKB constant table generation. */
        this->constants_.insert(constant);

        factorNode = PKB::CreateASTNode(CONSTANT, constant);

    } else {
        throw ProgramSyntaxErrorException();
    }

    return factorNode;
}

void FrontendParser::expect(string token) {
    if (!accept(token)) {
        throw ProgramSyntaxErrorException();
    }
}

void FrontendParser::expect(char token) {
    if (!accept(token)) {
        throw ProgramSyntaxErrorException();
    }
}

bool FrontendParser::accept(string token) {
    if (peekTokens() == token) {
        getToken();
        return true;
    }

    return false;
}

bool FrontendParser::accept(char token) {
    if (peekTokens() == string(1, token)) {
        getToken();
        return true;
    }

    return false;
}

string FrontendParser::peekTokens() {
    return peekForwardTokens(0);
}

string FrontendParser::peekForwardTokens(unsigned int index) {
    unsigned int forwardIndex = this->tokensIndex_ + index;

    return (forwardIndex >= this->tokens_.size()) ? "" : this->tokens_[forwardIndex];
}

string FrontendParser::getToken() {
    return this->tokens_[this->tokensIndex_++];
}

#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Constants.h"
#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "PKB/PKB.h"
#include "TreeNode.h"
#include "Utils.h"

using std::map;
using std::set;
using std::string;
using std::vector;

FrontendParser::FrontendParser() {
    stmtNumber_ = 1;
    tokensIndex_ = 0;

    currentTreeLevel_ = 0;
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
        Utils::SplitAndIgnoreEmpty(programLine, ' ', tokens_);
    }

    PKB::SetASTRoot(callProgramRecognizer());

    /* Generic tables generation. */
    PKB::GenerateConstantTable(constants_);
    PKB::GenerateVariableTable(variableNames_);
    PKB::GenerateProcedureTable(procedureNames_);
    PKB::GenerateStmtTable(stmts_);
    PKB::GenerateAssignTable(assigns_);

    /* Generate data for design abstraction table generation. */
    setModifies();
    setUses();
    setParent();
    setFollows();

    /* Design abstraction tables generation. */
    PKB::GenerateModifiesTable(modifies_);
    PKB::GenerateModifiesProcedureTable(modifiesProcedure_);
    PKB::GenerateUsesTable(uses_);
    PKB::GenerateUsesProcedureTable(usesProcedure_);
    PKB::GenerateParentTable(parent_);
    PKB::GenerateFollowsTable(follows_);

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
    while (tokensIndex_ < tokens_.size()) {
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
    procedureNames_.push_back(procedureName);

    proceduresFirstStmt_.push_back(stmtNumber_);

    /* TreeNode generation. */
    TreeNode* procedureNode = PKB::CreateASTNode(PROCEDURE, procedureName);
    procedureNode->addChild(callStmtListRecognizer());

    return procedureNode;
}

TreeNode* FrontendParser::callStmtListRecognizer() {
    expect(CHAR_SYMBOL_OPENCURLYBRACKET);
    currentTreeLevel_++;

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

    currentTreeLevel_--;
    return stmtListNode;
}

TreeNode* FrontendParser::callStmtRecognizer() {
    TreeNode* stmtNode;

    string stmt = "";
    unsigned int stmtNumber = stmtNumber_;

    /* For PKB parent table generation. Preorder storing. */
    stmtsLevels_.push_back(currentTreeLevel_);

    /* To catch special cases of assign statement where the variable name is same as certain symbols. */
    if (peekForwardTokens(1) == string(1, CHAR_SYMBOL_EQUAL)) {
        stmt = SYMBOL_ASSIGN;
        stmtNode = callAssignRecognizer();

    } else if (accept(SYMBOL_WHILE)) {
        stmt = SYMBOL_WHILE;
        stmtNode = callWhileRecognizer();

    } else if (accept(SYMBOL_IF)) {
        stmt = SYMBOL_IF;
        /*throw ProgramSyntaxErrorException();  */ stmtNode = callIfRecognizer();

    } else if (accept(SYMBOL_CALL)) {
        stmt = SYMBOL_CALL;
        throw ProgramSyntaxErrorException();  // stmtNode = callCallRecognizer();

    } else {
        throw ProgramSyntaxErrorException();
    }

    /* For PKB stmt table generation. */
    stmts_.emplace(stmtNumber, stmt);

    return stmtNode;
}

TreeNode* FrontendParser::callWhileRecognizer() {
    TreeNode* whileNode = PKB::CreateASTNode(WHILE, stmtNumber_++);

    string controlVariableName = getToken();

    /* For PKB variable table generation. */
    variableNames_.push_back(controlVariableName);

    /* For PKB uses table generation. */
    uses_[stmtNumber_ - 1].insert(controlVariableName);

    whileNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));
    whileNode->addChild(callStmtListRecognizer());

    return whileNode;
}

TreeNode* FrontendParser::callIfRecognizer() {
    TreeNode* ifNode = PKB::CreateASTNode(IF, stmtNumber_++);

    string controlVariableName = getToken();

    /* For PKB variable table generation. */
    variableNames_.push_back(controlVariableName);

    /* For PKB uses table generation. */
    uses_[stmtNumber_ - 1].insert(controlVariableName);

    ifNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));

    expect(SYMBOL_IF_THEN);

    ifNode->addChild(callStmtListRecognizer());

    thenLastStmt_.push_back(stmtNumber_ - 1);

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
    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(peekTokens())) {
        throw ProgramSyntaxErrorException();
    }

    string controlVariableName = getToken();

    /* For PKB variable table generation. */
    variableNames_.push_back(controlVariableName);

    /* For PKB modifies table generation. */
    modifies_[stmtNumber_].insert(controlVariableName);

    TreeNode* assignNode = PKB::CreateASTNode(ASSIGN, stmtNumber_++);

    /* For PKB assign table generation. */
    assigns_.insert(std::make_pair(stmtNumber_ - 1, assignNode));

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
        variableNames_.push_back(variableName);

        /* For PKB uses table generation. */
        uses_[stmtNumber_ - 1].insert(variableName);

        factorNode = PKB::CreateASTNode(VARIABLE, variableName);

    /* Constant. */
    } else if (Utils::IsNonNegativeNumeric(peekTokens())) {
        string constant = getToken();

        /* For PKB constant table generation. */
        constants_.push_back(constant);

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
    unsigned int forwardIndex = tokensIndex_ + index;

    return (forwardIndex >= tokens_.size()) ? "" : tokens_[forwardIndex];
}

string FrontendParser::getToken() {
    return tokens_[tokensIndex_++];
}

void FrontendParser::setModifies() {
    map<unsigned int, set<string>> modifiesContainers;

    for (const auto &pair : modifies_) {
        unsigned int stmtNumber = pair.first;
        set<string> variableNames = pair.second;

        if (stmtsLevels_.at(stmtNumber - 1) != 1) {
            unsigned int parentStmtNumber = getParentOfStmtNumber(stmtNumber);

            while (parentStmtNumber > 0) {
                modifiesContainers[parentStmtNumber].insert(variableNames.begin(), variableNames.end());
                parentStmtNumber = getParentOfStmtNumber(parentStmtNumber);
            }
        }

        for (int i = proceduresFirstStmt_.size() - 1; i >= 0; i--) {
            if (proceduresFirstStmt_[i] < stmtNumber) {
                modifiesProcedure_[procedureNames_[i]].insert(variableNames.begin(), variableNames.end());
                break;
            }
        }
    }

    for (auto &pair : modifiesContainers) {
        set<string> variableNames = pair.second;
        modifies_[pair.first].insert(variableNames.begin(), variableNames.end());
    }
}

void FrontendParser::setUses() {
    map<unsigned int, set<string>> usesContainers;

    for (const auto &pair : uses_) {
        unsigned int stmtNumber = pair.first;
        set<string> variableNames = pair.second;

        if (stmtsLevels_.at(stmtNumber - 1) != 1) {
            unsigned int parentStmtNumber = getParentOfStmtNumber(stmtNumber);

            while (parentStmtNumber > 0) {
                usesContainers[parentStmtNumber].insert(variableNames.begin(), variableNames.end());
                parentStmtNumber = getParentOfStmtNumber(parentStmtNumber);
            }
        }

        for (int i = proceduresFirstStmt_.size() - 1; i >= 0; i--) {
            if (proceduresFirstStmt_[i] < stmtNumber) {
                usesProcedure_[procedureNames_[i]].insert(variableNames.begin(), variableNames.end());
                break;
            }
        }
    }

    for (auto &pair : usesContainers) {
        set<string> variableNames = pair.second;
        uses_[pair.first].insert(variableNames.begin(), variableNames.end());
    }
}

void FrontendParser::setParent() {
    for (unsigned int i = 0; i < stmtsLevels_.size(); i++) {
        unsigned int parentStmtNumber = getParentOfStmtNumber(i + 1);

        if (parentStmtNumber > 0) {
            parent_[parentStmtNumber].insert(i + 1);
        }
    }
}

void FrontendParser::setFollows() {
    for (unsigned int i = 0; i < stmtsLevels_.size(); i++) {
        unsigned int stmtNumber = getFollowOfStmtNumber(i + 1);

        if (stmtNumber > 0) {
            follows_[stmtNumber] = (i + 1);
        }
    }
}

int FrontendParser::getParentOfStmtNumber(unsigned int stmtNumber) {
    if (stmtNumber == 0) {
        return -1;
    }

    unsigned int stmtLevel = stmtsLevels_.at(stmtNumber - 1);

    /* Root node level, no parent. */
    if (stmtLevel == 1) {
        return 0;
    }

    /* Search for the nearest (stmtLevel - 1) value; that, will be the parent. */
    unsigned int i = stmtNumber - 1;
    while (i > 0, i--) {
        if (stmtsLevels_.at(i) == (stmtLevel - 1)) {
            return (i + 1);
        }
    }

    return -1;
}

int FrontendParser::getFollowOfStmtNumber(unsigned int stmtNumber) {
    if (stmtNumber == 0) {
        return -1;
    }

    /* Nothing is before the first statement of a procedure. */
    if (Utils::VectorContains(proceduresFirstStmt_, stmtNumber)) {
        return 0;
    }

    unsigned int stmtLevel = stmtsLevels_.at(stmtNumber - 1);

    /* Get stmtNumber's procedure's first statement position. */
    unsigned int j;
    for (int i = proceduresFirstStmt_.size() - 1; i >= 0; i--) {
        if (proceduresFirstStmt_[i] < stmtNumber) {
            j = proceduresFirstStmt_[i];
            break;
        }
    }

    unsigned int k = stmtNumber - 1;
    while (k > j, k--) {
        if (stmtsLevels_.at(k) == (stmtLevel - 1)) {
            return 0;
        }

        /* Catch if statement being in same level but different statement list. */
        if (Utils::VectorContains(thenLastStmt_, (k + 1))) {
            return 0;
        }

        if (stmtsLevels_.at(k) == stmtLevel) {
            return (k + 1);
        }
    }

    return -1;
}

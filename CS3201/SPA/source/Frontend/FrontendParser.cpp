#include <fstream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
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
using std::queue;
using std::set;
using std::stack;
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

    /* Validate non-existent calls, no procedure names that does not exist. */
    validateNonExistentCall();

    /* Validate no recursive call in program. */
    validateRecursiveCall();

    /* Generic tables generation. */
    PKB::GenerateConstantTable(constants_);
    PKB::GenerateVariableTable(variableNames_);
    PKB::GenerateProcedureTable(procedureNames_);
    PKB::GenerateStmtTable(stmts_);

    /* Generate expressions for assign table. */
    for (const auto &pair : expressions_) {
        unsigned int stmtNumber = pair.first;
        queue<string> postfixExpression = Utils::GetPostfixExpression(pair.second);

        exactExpressions_.insert(std::make_pair(stmtNumber, Utils::GetExactExpressionWithBrackets(postfixExpression)));
        subExpressions_.insert(std::make_pair(stmtNumber, Utils::GetSubExpressionsWithBrackets(postfixExpression)));
    }

    PKB::GenerateExpressionTable(exactExpressions_);
    PKB::GenerateSubExpressionTable(subExpressions_);

    /* Calls table generation. Have to generate this first as other tables have dependencies on it. */
    PKB::GenerateCallsTable(calls_);

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

    currentProcedureName_ = getToken();

    /* Validate if procedure name follows naming convention. */
    if (!Utils::IsValidNamingConvention(currentProcedureName_)) {
        throw ProgramSyntaxErrorException(MESSAGE_PROCEDURENAME_INVALID);
    }

    /* Validate if procedure name already exist. */
    if (procedureNames_.count(currentProcedureName_) == 1) {
        throw ProgramSyntaxErrorException(MESSAGE_PROCEDURENAME_EXIST);
    }

    /* For PKB procedure table generation. */
    procedureNames_.insert(currentProcedureName_);

    proceduresFirstStmt_.push_back(std::make_pair(stmtNumber_, currentProcedureName_));

    /* TreeNode generation. */
    TreeNode* procedureNode = PKB::CreateASTNode(PROCEDURE, currentProcedureName_);
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
        throw ProgramLogicErrorException(MESSAGE_EMPTY_STMTLIST);
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
        stmtNode = callIfRecognizer();

    } else if (accept(SYMBOL_CALL)) {
        stmt = SYMBOL_CALL;
        stmtNode = callCallRecognizer();

    } else {
        throw ProgramSyntaxErrorException();
    }

    /* For PKB stmt table generation. */
    stmts_.insert(std::make_pair(stmtNumber, stmt));

    return stmtNode;
}

TreeNode* FrontendParser::callWhileRecognizer() {
    unsigned int stmtNumber = stmtNumber_;
    TreeNode* whileNode = PKB::CreateASTNode(WHILE, stmtNumber_++);

    string controlVariableName = getToken();

    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(controlVariableName)) {
        throw ProgramSyntaxErrorException(MESSAGE_VARIABLENAME_INVALID);
    }

    /* For PKB variable table generation. */
    variableNames_.insert(controlVariableName);

    /* For PKB uses table generation. */
    uses_[stmtNumber].insert(controlVariableName);

    whileNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));
    whileNode->addChild(callStmtListRecognizer());

    return whileNode;
}

TreeNode* FrontendParser::callIfRecognizer() {
    unsigned int stmtNumber = stmtNumber_;
    string controlVariableName = getToken();

    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(controlVariableName)) {
        throw ProgramSyntaxErrorException(MESSAGE_VARIABLENAME_INVALID);
    }

    TreeNode* ifNode = PKB::CreateASTNode(IF, stmtNumber_++);

    /* For PKB variable table generation. */
    variableNames_.insert(controlVariableName);

    /* For PKB uses table generation. */
    uses_[stmtNumber].insert(controlVariableName);

    ifNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));

    expect(SYMBOL_IF_THEN);

    ifNode->addChild(callStmtListRecognizer());

    thenLastStmt_.push_back(stmtNumber);

    expect(SYMBOL_IF_ELSE);

    ifNode->addChild(callStmtListRecognizer());

    return ifNode;
}

TreeNode* FrontendParser::callCallRecognizer() {
    unsigned int stmtNumber = stmtNumber_;
    string procedureName = getToken();

    /* Validate if procedure name follows naming convention. */
    if (!Utils::IsValidNamingConvention(currentProcedureName_)) {
        throw ProgramSyntaxErrorException(MESSAGE_PROCEDURENAME_INVALID);
    }

    TreeNode* callNode = PKB::CreateASTNode(CALL, stmtNumber_++, procedureName);

    /* For PKB calls table generation. */
    calls_[currentProcedureName_].insert(procedureName);

    /* For PKB modifies and uses table generation. */
    callStmtNumbers_.insert(std::make_pair(stmtNumber, procedureName));
    modifies_[stmtNumber] = uses_[stmtNumber] = {};

    expect(CHAR_SYMBOL_SEMICOLON);

    return callNode;
}

TreeNode* FrontendParser::callAssignRecognizer() {
    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(peekTokens())) {
        throw ProgramSyntaxErrorException(MESSAGE_VARIABLENAME_INVALID);
    }

    unsigned int stmtNumber = stmtNumber_;
    string controlVariableName = getToken();

    /* For PKB variable table generation. */
    variableNames_.insert(controlVariableName);

    /* For PKB modifies table generation. */
    modifies_[stmtNumber].insert(controlVariableName);

    TreeNode* assignNode = PKB::CreateASTNode(ASSIGN, stmtNumber_++);
    assigns_.insert(std::make_pair(stmtNumber, assignNode));

    assignNode->addChild(PKB::CreateASTNode(VARIABLE, controlVariableName));

    expect(CHAR_SYMBOL_EQUAL);

    assignNode->addChild(callExpressionRecognizer());

    expect(CHAR_SYMBOL_SEMICOLON);

    /* For PKB assign table generation. */
    expressions_.insert(std::make_pair(stmtNumber, expression_));
    expression_.clear();

    return assignNode;
}

TreeNode*  FrontendParser::callExpressionRecognizer() {
    TreeNode* expressionNode = callTermRecognizer();

    while (true) {
        TreeNode* node;

        if (accept(CHAR_SYMBOL_PLUS)) {
            expression_.push_back(string(1, CHAR_SYMBOL_PLUS));
            node = PKB::CreateASTNode(PLUS);

        } else if (accept(CHAR_SYMBOL_MINUS)) {
            expression_.push_back(string(1, CHAR_SYMBOL_MINUS));
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
        expression_.push_back(string(1, CHAR_SYMBOL_MULTIPLY));

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
        expression_.push_back(string(1, CHAR_SYMBOL_OPENBRACKET));

        factorNode = callExpressionRecognizer();
        expect(CHAR_SYMBOL_CLOSEBRACKET);

        expression_.push_back(string(1, CHAR_SYMBOL_CLOSEBRACKET));

    /* Variable. */
    } else if (Utils::IsValidNamingConvention(peekTokens())) {
        string variableName = getToken();

        /* For PKB variable table generation. */
        variableNames_.insert(variableName);

        /* For PKB uses table generation. */
        uses_[stmtNumber_ - 1].insert(variableName);

        expression_.push_back(variableName);
        factorNode = PKB::CreateASTNode(VARIABLE, variableName);

    /* Constant. */
    } else if (Utils::IsNonNegativeNumeric(peekTokens())) {
        string constant = getToken();

        /* For PKB constant table generation. */
        constants_.insert(constant);

        expression_.push_back(constant);
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
    /* Populate modifies that are procedure local into the modifies procedure table. */
    for (const auto &pair : modifies_) {
        set<string> variableNames = pair.second;

        for (int i = proceduresFirstStmt_.size() - 1; i >= 0; i--) {
            if (proceduresFirstStmt_[i].first <= pair.first) {
                modifiesProcedure_[proceduresFirstStmt_[i].second].insert(variableNames.begin(), variableNames.end());
                break;
            }
        }
    }

    /* Populate modifies that are from other procedures. */
    for (const auto &pair : modifiesProcedure_) {
        set<string> procedures = PKB::GetCalled(pair.first);

        /* Procedure does not have any call. */
        if (procedures.empty()) {
            continue;
        }

        for (const auto &procedure : procedures) {
            if (modifiesProcedure_.count(procedure) != 1) {
                continue;
            }

            set<string> variableNames = modifiesProcedure_[procedure];
            modifiesProcedure_[pair.first].insert(variableNames.begin(), variableNames.end());
        }
    }

    /* Populate the container statements. */
    map<unsigned int, set<string>> modifiesContainers;
    for (auto iter = modifies_.begin(); iter != modifies_.end();) {
        unsigned int stmtNumber = iter->first;
        set<string> variableNames;

        /* If call statement, get variables from modifies procedure table. */
        if (callStmtNumbers_.count(stmtNumber) == 1) {
            string procedure = callStmtNumbers_[stmtNumber];

            if (modifiesProcedure_.count(procedure) != 1) {
                iter = modifies_.erase(iter);
                continue;
            }

            variableNames = modifiesProcedure_[procedure];

            /* Populate the call statements. */
            modifies_[stmtNumber].insert(variableNames.begin(), variableNames.end());
        } else {
            variableNames = iter->second;
        }

        if (stmtsLevels_.at(stmtNumber - 1) != 1) {
            unsigned int parentStmtNumber = getParentOfStmtNumber(stmtNumber);

            while (parentStmtNumber > 0) {
                modifiesContainers[parentStmtNumber].insert(variableNames.begin(), variableNames.end());
                parentStmtNumber = getParentOfStmtNumber(parentStmtNumber);
            }
        }

        iter++;
    }

    for (auto &pair : modifiesContainers) {
        set<string> variableNames = pair.second;
        modifies_[pair.first].insert(variableNames.begin(), variableNames.end());
    }
}

void FrontendParser::setUses() {
    /* Populate uses that are procedure local into the uses procedure table. */
    for (const auto &pair : uses_) {
        set<string> variableNames = pair.second;

        for (int i = proceduresFirstStmt_.size() - 1; i >= 0; i--) {
            if (proceduresFirstStmt_[i].first <= pair.first) {
                usesProcedure_[proceduresFirstStmt_[i].second].insert(variableNames.begin(), variableNames.end());
                break;
            }
        }
    }

    /* Populate uses that are from other procedures. */
    for (const auto &pair : usesProcedure_) {
        set<string> procedures = PKB::GetCalled(pair.first);

        /* Procedure does not have any call. */
        if (procedures.empty()) {
            continue;
        }

        for (const auto &procedure : procedures) {
            if (usesProcedure_.count(procedure) != 1) {
                continue;
            }

            set<string> variableNames = usesProcedure_[procedure];
            usesProcedure_[pair.first].insert(variableNames.begin(), variableNames.end());
        }
    }

    map<unsigned int, set<string>> usesContainers;
    for (auto iter = uses_.begin(); iter != uses_.end();) {
        unsigned int stmtNumber = iter->first;
        set<string> variableNames;

        /* If call statement, get variables from uses procedure table. */
        if (callStmtNumbers_.count(stmtNumber) == 1) {
            string procedure = callStmtNumbers_[stmtNumber];

            if (usesProcedure_.count(procedure) != 1) {
                iter = uses_.erase(iter);
                continue;
            }

            variableNames = usesProcedure_[procedure];

            /* Populate the call statements. */
            uses_[stmtNumber].insert(variableNames.begin(), variableNames.end());
        } else {
            variableNames = iter->second;
        }

        if (stmtsLevels_.at(stmtNumber - 1) != 1) {
            unsigned int parentStmtNumber = getParentOfStmtNumber(stmtNumber);

            while (parentStmtNumber > 0) {
                usesContainers[parentStmtNumber].insert(variableNames.begin(), variableNames.end());
                parentStmtNumber = getParentOfStmtNumber(parentStmtNumber);
            }
        }

        iter++;
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

    /* Get stmtNumber's procedure's first statement position. */
    unsigned int j;
    for (int i = proceduresFirstStmt_.size() - 1; i >= 0; i--) {
        unsigned int number = proceduresFirstStmt_[i].first;

        /* Nothing is before the first statement of a procedure. */
        if (number == stmtNumber) {
            return 0;
        }

        if (number < stmtNumber) {
            j = number;
            break;
        }
    }

    unsigned int stmtLevel = stmtsLevels_.at(stmtNumber - 1);

    unsigned int k = stmtNumber - 1;
    while (k > j, k--) {
        if (stmtsLevels_.at(k) == (stmtLevel - 1)) {
            return 0;
        }

        /* Catch "if" statement being in same level but different statement list. */
        if (Utils::VectorContains(thenLastStmt_, (k + 1))) {
            return 0;
        }

        if (stmtsLevels_.at(k) == stmtLevel) {
            return (k + 1);
        }
    }

    return -1;
}

void FrontendParser::validateNonExistentCall() {
    set<string> procedures;
    for (const auto &pair : calls_) {
        procedures.insert(pair.second.begin(), pair.second.end());
    }

    for (const auto &procedure : procedures) {
        if (procedureNames_.count(procedure) != 1) {
            throw ProgramSyntaxErrorException(MESSAGE_NONEXISTENTCALL_EXIST);
        }
    }
}

void FrontendParser::validateRecursiveCall() {
    if (calls_.empty()) {
        return;
    }

    /* Perform DFS to check for recursive call. */
    for (const auto &pair : calls_) {
        stack<string> stack;
        stack.push(pair.first);

        set<string> uniqueCalls;
        while (!stack.empty()) {
            string call = stack.top();

            uniqueCalls.insert(call);
            stack.pop();

            /* End of the transitive closure of the call. */
            if (calls_.count(call) != 1) {
                continue;
            }

            set<string> calls = calls_[call];

            if (calls.count(pair.first) == 1) {
                throw ProgramLogicErrorException(MESSAGE_RECURSIVECALL_EXIST);
            }

            for (const auto &value : calls) {
                if (uniqueCalls.count(value) != 1) {
                    stack.push(value);
                }
            }
        }
    }
}

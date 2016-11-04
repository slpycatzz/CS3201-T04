#include <fstream>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "Constants.h"
#include "Exceptions.h"
#include "Frontend/DesignExtractor.h"
#include "Frontend/FrontendParser.h"
#include "Utils.h"

using std::set;
using std::stack;
using std::string;
using std::vector;

FrontendParser::FrontendParser() {
    stmtNumber_ = 1;
    tokensIndex_ = 0;
    currentTreeLevel_ = 0;

    designExtractor = DesignExtractor::getInstance();
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
        Utils::SplitAndIgnoreEmpty(programLine, " \t", tokens_);
    }

    callProgramRecognizer();

    /* Validate non-existent calls, no procedure names that does not exist. */
    validateNonExistentCall();

    /* Validate no recursive call in program. */
    validateRecursiveCall();

    designExtractor.populatePKB(stmtsLevels_, stmtNumber_ - 1);
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

void FrontendParser::callProgramRecognizer() {
    while (tokensIndex_ < tokens_.size()) {
        callProcedureRecognizer();
    }
}

void FrontendParser::callProcedureRecognizer() {
    expect(SYMBOL_PROCEDURE);

    unsigned int stmtNumber = stmtNumber_;
    currentProcedureName_ = getToken();

    /* Validate if procedure name follows naming convention. */
    if (!Utils::IsValidNamingConvention(currentProcedureName_)) {
        throw ProgramSyntaxErrorException(MESSAGE_PROCEDURENAME_INVALID);
    }

    /* Validate if procedure name already exist. */
    if (procedureNames_.count(currentProcedureName_) == 1) {
        throw ProgramSyntaxErrorException(MESSAGE_PROCEDURENAME_EXIST);
    }

    callStmtListRecognizer(PROCEDURE);

    procedureNames_.insert(currentProcedureName_);
    designExtractor.setProcedureData(currentProcedureName_, stmtNumber, stmtNumber_ - 1);
}

void FrontendParser::callStmtListRecognizer(Symbol stmtSymbol) {
    expect(CHAR_SYMBOL_OPENCURLYBRACKET);
    currentTreeLevel_++;

    unsigned int stmtNumber = stmtNumber_;

    bool hasChildren = (peekTokens() != string(1, CHAR_SYMBOL_CLOSECURLYBRACKET));

    /* Validate if statement list is empty. */
    if (!hasChildren) {
        throw ProgramLogicErrorException(MESSAGE_EMPTY_STMTLIST);
    }

    while (peekTokens() != string(1, CHAR_SYMBOL_CLOSECURLYBRACKET)) {
        callStmtRecognizer();
    }

    expect(CHAR_SYMBOL_CLOSECURLYBRACKET);
    currentTreeLevel_--;

    designExtractor.setStmtListData(stmtSymbol, stmtNumber);
}

void FrontendParser::callStmtRecognizer() {
    unsigned int stmtNumber = stmtNumber_;

    /* For PKB parent table generation. Preorder storing. */
    stmtsLevels_.insert(std::make_pair(stmtNumber, currentTreeLevel_));

    /* To catch special cases of assign statement where the variable name is same as certain symbols. */
    if (peekForwardTokens(1) == string(1, CHAR_SYMBOL_EQUAL)) {
        callAssignRecognizer();

    } else if (accept(SYMBOL_WHILE)) {
        callWhileRecognizer();

    } else if (accept(SYMBOL_IF)) {
        callIfRecognizer();

    } else if (accept(SYMBOL_CALL)) {
        callCallRecognizer();

    } else {
        throw ProgramSyntaxErrorException(MESSAGE_STMT_INVALID);
    }
}

void FrontendParser::callWhileRecognizer() {
    unsigned int stmtNumber = stmtNumber_++;
    string controlVariableName = getToken();

    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(controlVariableName)) {
        throw ProgramSyntaxErrorException(MESSAGE_VARIABLENAME_INVALID);
    }

    callStmtListRecognizer(WHILE);

    designExtractor.setContainerStmtData(WHILE, stmtNumber, controlVariableName);
    designExtractor.setWhileStmtData(stmtNumber, stmtNumber_);
}

void FrontendParser::callIfRecognizer() {
    unsigned int stmtNumber = stmtNumber_++;
    unsigned int thenLastStmtNumber = 0, elseFirstStmtNumber = 0;
    string controlVariableName = getToken();

    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(controlVariableName)) {
        throw ProgramSyntaxErrorException(MESSAGE_VARIABLENAME_INVALID);
    }

    expect(SYMBOL_IF_THEN);

    callStmtListRecognizer(IF_THEN);

    for (unsigned int i = stmtNumber_ - 1; i > stmtNumber; i--) {
        if (stmtsLevels_[i] == (stmtsLevels_[stmtNumber] + 1)) {
            thenLastStmtNumber = i;
            break;
        }
    }

    expect(SYMBOL_IF_ELSE);

    elseFirstStmtNumber = stmtNumber_;

    callStmtListRecognizer(IF_ELSE);

    designExtractor.setContainerStmtData(IF, stmtNumber, controlVariableName);
    designExtractor.setIfStmtData(stmtNumber, thenLastStmtNumber, elseFirstStmtNumber);
}

void FrontendParser::callCallRecognizer() {
    unsigned int stmtNumber = stmtNumber_++;
    string procedureName = getToken();

    /* Validate if procedure name follows naming convention. */
    if (!Utils::IsValidNamingConvention(procedureName)) {
        throw ProgramSyntaxErrorException(MESSAGE_PROCEDURENAME_INVALID);
    }

    expect(CHAR_SYMBOL_SEMICOLON);

    calls_[currentProcedureName_].insert(procedureName);
    designExtractor.setCallStmtData(stmtNumber, currentProcedureName_, procedureName);
}

void FrontendParser::callAssignRecognizer() {
    unsigned int stmtNumber = stmtNumber_++;
    string controlVariableName = getToken();

    /* Validate if variable name follows naming convention. */
    if (!Utils::IsValidNamingConvention(controlVariableName)) {
        throw ProgramSyntaxErrorException(MESSAGE_VARIABLENAME_INVALID);
    }

    expect(CHAR_SYMBOL_EQUAL);

    callExpressionRecognizer();

    expect(CHAR_SYMBOL_SEMICOLON);

    designExtractor.setAssignStmtData(ASSIGN, stmtNumber, controlVariableName);
    designExtractor.setAssignExpressionData(stmtNumber, expression_);

    expression_.clear();
}

void FrontendParser::callExpressionRecognizer() {
    callTermRecognizer();

    while (true) {
        if (accept(CHAR_SYMBOL_PLUS)) {
            expression_.push_back(string(1, CHAR_SYMBOL_PLUS));

        } else if (accept(CHAR_SYMBOL_MINUS)) {
            expression_.push_back(string(1, CHAR_SYMBOL_MINUS));

        } else {
            break;
        }

        callTermRecognizer();
    }
}

void FrontendParser::callTermRecognizer() {
    callFactorRecognizer();

    while (accept(CHAR_SYMBOL_MULTIPLY)) {
        expression_.push_back(string(1, CHAR_SYMBOL_MULTIPLY));

        callFactorRecognizer();
    }
}

void FrontendParser::callFactorRecognizer() {
    /* Higher precedence for expressions in (). */
    if (accept(CHAR_SYMBOL_OPENBRACKET)) {
        expression_.push_back(string(1, CHAR_SYMBOL_OPENBRACKET));

        callExpressionRecognizer();
        expect(CHAR_SYMBOL_CLOSEBRACKET);

        expression_.push_back(string(1, CHAR_SYMBOL_CLOSEBRACKET));

    /* Variable. */
    } else if (Utils::IsValidNamingConvention(peekTokens())) {
        string variableName = getToken();

        expression_.push_back(variableName);

        designExtractor.setAssignStmtData(VARIABLE, stmtNumber_ - 1, variableName);

    /* Constant. */
    } else if (Utils::IsNonNegativeNumeric(peekTokens())) {
        string constantValue = getToken();

        expression_.push_back(constantValue);

        designExtractor.setAssignStmtData(CONSTANT, stmtNumber_ - 1, constantValue);

    } else {
        throw ProgramSyntaxErrorException(MESSAGE_ASSIGN_STMT_INVALID);
    }
}

void FrontendParser::expect(string token) {
    if (!accept(token)) {
        throw ProgramSyntaxErrorException(MESSAGE_TOKEN_INVALID);
    }
}

void FrontendParser::expect(char token) {
    if (!accept(token)) {
        throw ProgramSyntaxErrorException(MESSAGE_TOKEN_INVALID);
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

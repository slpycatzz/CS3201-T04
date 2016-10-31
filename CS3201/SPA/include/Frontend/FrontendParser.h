#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Frontend/DesignExtractor.h"

class FrontendParser {
 public:
    FrontendParser();
    ~FrontendParser();
    void parseProgram(std::string filePath);

 private:
    std::vector<std::string> preprocessProgramLines(std::ifstream& fileStream);

    void callProgramRecognizer();
    void callProcedureRecognizer();
    void callStmtListRecognizer(Symbol stmtSymbol);
    void callStmtRecognizer();
    void callWhileRecognizer();
    void callIfRecognizer();
    void callCallRecognizer();
    void callAssignRecognizer();
    void callExpressionRecognizer();
    void callTermRecognizer();
    void callFactorRecognizer();

    void expect(std::string token);
    void expect(char token);
    bool accept(std::string token);
    bool accept(char token);

    std::string peekTokens();
    std::string peekForwardTokens(unsigned int index);
    std::string getToken();

    void validateNonExistentCall();
    void validateRecursiveCall();

    /* Parser helper variables. */
    unsigned int stmtNumber_;
    unsigned int tokensIndex_;
    std::vector<std::string> tokens_;

    /* For validation usage. */
    std::string currentProcedureName_;
    std::set<std::string> procedureNames_;
    std::map<std::string, std::set<std::string>> calls_;

    DesignExtractor designExtractor;

    /* For design extractor usage. */
    unsigned int currentTreeLevel_;
    std::map<unsigned int, unsigned int> stmtsLevels_;          /* map<stmtNumber, stmtLevel> */
    std::vector<std::string> expression_;
};

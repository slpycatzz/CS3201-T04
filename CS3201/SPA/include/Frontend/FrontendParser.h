#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "TreeNode.h"

class FrontendParser {
 public:
    FrontendParser();
    ~FrontendParser();
    void parseProgram(std::string filePath);

 private:
    std::vector<std::string> preprocessProgramLines(std::ifstream& fileStream);

    TreeNode* callProgramRecognizer();
    TreeNode* callProcedureRecognizer();
    TreeNode* callStmtListRecognizer();
    TreeNode* callStmtRecognizer();
    TreeNode* callWhileRecognizer();
    TreeNode* callIfRecognizer();
    TreeNode* callCallRecognizer();
    TreeNode* callAssignRecognizer();
    TreeNode* callExpressionRecognizer();
    TreeNode* callTermRecognizer();
    TreeNode* callFactorRecognizer();

    void expect(std::string token);
    void expect(char token);
    bool accept(std::string token);
    bool accept(char token);

    std::string peekTokens();
    std::string peekForwardTokens(unsigned int index);
    std::string getToken();

    void setParent();

    int getParentOfStmtNumber(unsigned int stmtNumber);

    /* Parser helper variables. */
    unsigned int stmtNumber_;
    unsigned int tokensIndex_;
    std::vector<std::string> tokens_;

    /* PKB tables helper variables. */
    unsigned int currentTreeLevel_;
    std::vector<unsigned int> stmtsLevels_;

    /* For PKB generic table generation. */
    std::set<std::string> constants_;
    std::set<std::string> variableNames_;
    std::set<std::string> procedureNames_;
    std::map<unsigned int, std::string> stmts_;

    /* For PKB design abstraction table generation. */
    std::map<unsigned int, std::string> modifies_;
    std::map<unsigned int, std::string> uses_;
    std::map<unsigned int, std::set<unsigned int>> parent_;
};

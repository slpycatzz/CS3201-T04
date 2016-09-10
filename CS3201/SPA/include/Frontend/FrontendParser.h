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

    void setModifies();
    void setUses();
    void setParent();
    void setFollows();

    int getParentOfStmtNumber(unsigned int stmtNumber);
    int getFollowOfStmtNumber(unsigned int stmtNumber);


    /* Parser helper variables. */
    unsigned int stmtNumber_;
    unsigned int tokensIndex_;
    std::vector<std::string> tokens_;

    /* PKB tables helper variables. */
    unsigned int currentTreeLevel_;
    std::vector<unsigned int> stmtsLevels_;
    std::vector<unsigned int> proceduresFirstStmt_;
    std::vector<unsigned int> thenLastStmt_;

    /* For PKB generic table generation. */
    std::vector<std::string> constants_;
    std::vector<std::string> variableNames_;
    std::vector<std::string> procedureNames_;
    std::map<unsigned int, std::string> stmts_;
    std::map<unsigned int, TreeNode*> assigns_;

    /* For PKB design abstraction table generation. */
    std::map<unsigned int, std::set<std::string>> modifies_;
    std::map<std::string, std::set<std::string>> modifiesProcedure_;
    std::map<unsigned int, std::set<std::string>> uses_;
    std::map<std::string, std::set<std::string>> usesProcedure_;
    std::map<unsigned int, std::set<unsigned int>> parent_;
    std::map<unsigned int, unsigned int> follows_;
};

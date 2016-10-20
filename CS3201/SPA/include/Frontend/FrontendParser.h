#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>
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
    void setNext();

    int getParentOfStmtNumber(unsigned int stmtNumber);
    int getFollowOfStmtNumber(unsigned int stmtNumber);
    int getFollowingOfStmtNumber(unsigned int stmtNumber);
    std::set<unsigned int> getNextStmtNumbers(unsigned int stmtNumber);

    void validateNonExistentCall();
    void validateRecursiveCall();

    /* Parser helper variables. */
    unsigned int stmtNumber_;
    unsigned int tokensIndex_;
    std::vector<std::string> tokens_;

    /* PKB tables helper variables. */
    std::string currentProcedureName_;
    unsigned int currentTreeLevel_;
    std::map<unsigned int, unsigned int> stmtsLevels_;                  /* map<stmtNumber, stmtLevel> */
    std::map<unsigned int, std::string> proceduresLastStmt_;
    std::map<unsigned int, unsigned int> thenLastStmt_;                 /* map<if stmtNumber, thenLast stmtNumber> */

    /* For PKB generic table generation. */
    std::set<std::string> constants_;
    std::set<std::string> variableNames_;
    std::set<std::string> procedureNames_;
    std::map<unsigned int, std::string> controlVariables_;
    std::map<unsigned int, std::string> callStmtNumbers_;
    std::map<unsigned int, std::string> stmts_;
    std::map<unsigned int, std::string> stmtlists_;
    std::map<unsigned int, std::string> proceduresFirstStmt_;

    /* For PKB expression tables generation. */
    std::vector<std::string> expression_;
    std::map<unsigned int, std::vector<std::string>> expressions_;
    std::map<unsigned int, std::string> exactExpressions_;
    std::map<unsigned int, std::set<std::string>> subExpressions_;

    /* For PKB design abstraction table generation. */
    std::map<std::string, std::set<std::string>> calls_;
    std::map<unsigned int, std::set<std::string>> modifies_;
    std::map<std::string, std::set<std::string>> modifiesProcedure_;
    std::map<unsigned int, std::set<std::string>> uses_;
    std::map<std::string, std::set<std::string>> usesProcedure_;
    std::map<unsigned int, std::set<unsigned int>> parent_;
    std::map<unsigned int, unsigned int> follows_;

    /* For PKB next design abstraction tables generation. */
    std::map<unsigned int, std::set<unsigned int>> next_;
    std::vector<TreeNode*> controlFlowGraphs_;
};

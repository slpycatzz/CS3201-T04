#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "QueryProcessor/QueryTree.h"
#include "QueryProcessor/RelationTable.h"

const int QUERY_NUM_OF_LINES = 5;

class QueryPreprocessor {
 public:
    QueryPreprocessor();
    ~QueryPreprocessor();

    /*  query is size n, [first to n - 1 will be declaration, n will be all the clauses] */
    /*  e.g. "assign a,a2;while w1; select a such that uses(a,"x")" */
    void preprocessQuery(std::string query);
    QueryTree getQueryTree();

 private:
     bool processDeclaration(std::string declaration);
     bool processQuery(std::string query);

     bool parseSelect(std::vector<std::string> queryList);

     bool parseSuchThat(std::vector<std::string> suchThat);
     bool parseRelation(std::string clauseType, std::string relType, std::vector<std::string> &varList);
     bool parsePattern(std::vector<std::string> pattern);

     bool isVarExist(std::string var);
     bool isValidVarName(std::string varName);
     bool isValidVarType(std::string varName);
     bool isConstantVar(std::string varName);

     std::vector<std::string> getNextToken(std::vector<std::string> queryList);

     std::string getVarType(std::string var);

     std::vector<std::string> queries;
     std::unordered_map<std::string, std::string> varMap;
     std::unordered_map<std::string, std::string> relMap;
     std::unordered_map<std::string, std::string> entMap;
     RelationTable r;
     QueryTree qt;

     std::string out;
     std::string testMethodOut();
};

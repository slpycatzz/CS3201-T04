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
    std::string testMethodOut();
    std::vector<std::string> unitTestStubGetParams(std::string filePath, bool isInput);

    // usage: e.g. "Select <a,a1> such that Uses(a1,"x")"
    bool processDeclaration(std::string declaration);

 private:
     void expect(std::string word);
     void expect(char token);
     int accept(std::string word);
     int accept(char token);

     bool QueryPreprocessor::accept(std::string &var, char token);
     bool QueryPreprocessor::accept(std::string &var, Symbol token);

     bool QueryPreprocessor::expect(std::string &var, char token);

     /* return 1 if token found, do nothing after */
     int accept(Symbol varType);
     std::string peek();

     std::string getVar();
     std::string getVar(std::string word);

     // bool processDeclaration(std::string declaration);
     bool processQuery(std::string query);

     void parseSelect();
     void parseSuchThat();
     void parsePattern();
     void parseAnd(Symbol);
     void parseWith();

     bool isAttributeValid(std::string var, std::string varAttr);
     bool isAttributeValid(std::string var, std::string varAttr, std::string var2, std::string varAttr2);
     bool isVarExist(std::string var);
     bool isValidVarName(std::string varName);
     bool isValidVarType(std::string varName);
     bool isConstantVar(std::string varName);

     bool isConstantVarTerm(std::string &var);
     void callExpressionRecognizer(std::string &var);
     void callFactorRecognizer(std::string &var);

     std::vector<std::string> getNextToken(std::vector<std::string> queryList);
     Symbol getVarType(std::string var);
     std::string getPatternExpression();
     Symbol getAttributeType(std::string var, std::string varAttr);

     void mergeSeparatedClauses();
     std::string removeWhitespaces(std::string);
     // global variables
     std::vector<std::string> queries;
     std::vector<std::string> queryList;
     std::vector<std::string> patternList;
     Symbol prevClause;
     Symbol prevRelation;
     int cur;

     std::unordered_map<std::string, Symbol> varSymbolMap;
     std::unordered_map<std::string, bool> varAttrMap;

     RelationTable r;
     QueryTree qt;

     std::string toLower(std::string);
     std::string out;
};

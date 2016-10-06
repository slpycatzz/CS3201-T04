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
	
	//Temporarily made public to test the optimizer
    /*
    bool parseSuchThat(std::vector<std::string> suchThat);
    bool parsePattern(std::vector<std::string> pattern);
    bool parseSelect(std::vector<std::string> queryList);
    bool parseRelation(std::string clauseType, std::string relType, std::vector<std::string>& varList);
    */
    bool processDeclaration(std::string declaration);

 private:
     void expect(std::string);
     void expect(char);
     int accept(std::string);
     int accept(char);
     int accept(Symbol);
     std::string peek();

     std::string getVar();

     // bool processDeclaration(std::string declaration);
     bool processQuery(std::string query);

     void parseSelect();
     void parseSuchThat();
     void parsePattern();
     void parseAnd(Symbol);
     void parseWith();

     Symbol varAttributeToSymbol(std::string varAttr);

     bool isVarExist(std::string var);
     bool isValidVarName(std::string varName);
     bool isValidVarType(std::string varName);
     bool isConstantVar(std::string varName);

     std::vector<std::string> getNextToken(std::vector<std::string> queryList);

     std::string getVarType(std::string var);

     std::vector<std::string> queries;
     std::vector<std::string> queryList;
     Symbol prevClause;
     Symbol prevRelation;
     int cur;

     std::unordered_map<std::string, std::string> varMap;
     std::unordered_map<std::string, Symbol> varSymbolMap;
     RelationTable r;
     QueryTree qt;

     std::string toLower(std::string);
     std::string out;
};

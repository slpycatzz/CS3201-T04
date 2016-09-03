#pragma once

#include <string>
#include <vector>

#include "QueryProcessor/RelationTable.h"

const int QUERY_NUM_OF_LINES = 5;

class QueryPreprocessor {
 public:
    QueryPreprocessor();
    ~QueryPreprocessor();
    void preprocess(std::string filePath);

 private:
     void preprocessFile(std::ifstream& fileStream);
     bool processDeclaration(std::string declaration);
     bool processQuery(std::string query);

     bool parseSuchThat(std::string suchThat);
     bool parseSuchThatRelation(std::string relType, std::vector<std::string> &varList, std::vector<std::string> &varTypes);
     bool parsePattern(std::string pattern);
     bool isVarExist(std::string var);
     bool isValidVarName(std::string varName);
     bool isValidVarType(std::string varName);
     std::string getVarType(std::string var);

     std::vector<std::vector<std::string>> queries;
     std::vector<std::string> declareVar;
     std::vector<std::string> declareVarType;
     std::vector<std::string> selectVar;
     RelationTable r;
};

#pragma once

#include <string>
#include <vector>

const int QUERY_NUM_OF_LINES = 5;

class QueryPreprocessor {
 public:
    QueryPreprocessor();
    ~QueryPreprocessor();
    void preprocess(std::string filePath);
    void preprocessFile(std::ifstream& fileStream);

 private:
     std::vector<std::vector<std::string>> queries;

};

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class RelationTable {
 public:
    RelationTable();
    ~RelationTable();
    bool isArgValid(std::string, std::string, int);
    bool isRelationValid(std::string);

 private:
    bool isArgFound(std::vector<std::string>, std::string);
    void initTable();
    std::unordered_map<std::string, std::vector<std::string>> clauseMap1;
    std::unordered_map<std::string, std::vector<std::string>> clauseMap2;
};


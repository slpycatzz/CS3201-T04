#pragma once

#include <string>

class RelationTable {
 public:
    RelationTable();
    ~RelationTable();
    bool isRelationValid(std::string relation, std::vector<std::string>, std::vector<std::string>);
    bool isArgValid(std::string relation, std::string arg1, std::string arg2);
};


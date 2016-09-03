#pragma once
#include <string>

class RelationTable
{
public:
    RelationTable();
    ~RelationTable();
    bool isRelValid(std::string relation);
    bool isArgValid(std::string relation, std::string arg1, std::string arg2);
};


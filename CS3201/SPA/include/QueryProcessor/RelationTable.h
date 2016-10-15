#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Constants.h"

class RelationTable {
 public:
    RelationTable();
    ~RelationTable();

    /* checks if e.g. [a1("x",_)] "x"(VARIABLE) is valid for a1(ASSIGN)*/
    bool isArgValid(Symbol relation, std::string argumentType, int i);
    /* checks if e.g. [pattern a1() ] a1(ASSIGN) is valid for pattern */
    bool isRelationValid(Symbol relation,Symbol clause);

 private:
    bool isArgFound(std::vector<Symbol>, Symbol);
    void initTable();
    std::unordered_map<Symbol, std::vector<Symbol>> clauseMap1;
    std::unordered_map<Symbol, std::vector<Symbol>> clauseMap2;
};


#pragma once
#include <vector>
#include "Constants.h"
class Clause {
 public:
     Clause();
     ~Clause();
     bool setClauseType();
     bool setArg();
     bool getClauseType();
     bool getArg();
     bool getArgCount();
 private:
     Symbol clauseType;
     std::vector<std::string> argList;
};

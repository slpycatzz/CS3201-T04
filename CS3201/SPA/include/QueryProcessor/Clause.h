#pragma once
#include <string>
#include <vector>

#include "Constants.h"

class Clause {
 public:
     Clause();
     ~Clause();
     bool setClauseType(std::string);
     bool setArg(std::vector<std::string>);
     std::string getClauseType();
     std::vector<std::string> getArg();
     int getArgCount();
 private:
     Symbol clauseType;
     std::string clauseName;
     std::vector<std::string> argList;
};

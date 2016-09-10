#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "QueryProcessor/Clause.h"

Clause::Clause() {}

Clause::~Clause() {}

bool Clause::setClauseType(std::string clauseType) {
    clauseName = clauseType;
    return true;
}
bool Clause::setArg(std::vector<std::string> argListInput) {
    argList = argListInput;
    return true;
}
std::string Clause::getClauseType() {
    return clauseName;
}
std::vector<std::string> Clause::getArg() {
    return argList;
}
int Clause::getArgCount() {
    return argList.size();
}
#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "QueryProcessor/Clause.h"
#include "Utils.h"

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
std::string Clause::toString() {
    std::string sb = getClauseType() + " ";
    
    for (std::string arg : getArg()) {
        sb += arg + " ";
        if (Utils::IsNonNegativeNumeric(arg)) {
            sb += "constant ";
        }
        else if (arg == "_") {
            sb += "underscore ";
        }
        else if (Utils::StartsWith(arg, CHAR_SYMBOL_DOUBLEQUOTES) && Utils::EndsWith(arg, CHAR_SYMBOL_DOUBLEQUOTES)) {
            sb += "constant ";
        }
        else if (Utils::StartsWith(arg, CHAR_SYMBOL_UNDERSCORE) && Utils::EndsWith(arg, CHAR_SYMBOL_UNDERSCORE)) {
            sb += "constant ";
        }
        else {
            sb += "synonym ";
        }
    }
    sb += "\n";
    
    return sb;
}

std::vector<std::string> Clause::getSynonyms() {
    std::vector<std::string> synonyms;

    for (std::string arg : getArg()) {
        if (!Utils::IsNonNegativeNumeric(arg) &&                                                                        //not a number
            !(arg == "_") &&                                                                                            //not an underscore
            !(Utils::StartsWith(arg, CHAR_SYMBOL_DOUBLEQUOTES) && Utils::EndsWith(arg, CHAR_SYMBOL_DOUBLEQUOTES)) &&    //not in quotes
            !(Utils::StartsWith(arg, CHAR_SYMBOL_UNDERSCORE) && Utils::EndsWith(arg, CHAR_SYMBOL_UNDERSCORE))) {        //not surrounded by underscores
            synonyms.push_back(arg);                                                                                            //then it is a synonym
        }
    }

    return synonyms;
}
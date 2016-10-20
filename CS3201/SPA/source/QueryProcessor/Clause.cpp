#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "QueryProcessor/Clause.h"
#include "Utils.h"

using std::string;
using std::vector;

Clause::Clause() {}

Clause::~Clause() {}

bool Clause::setClauseType(string clauseType) {
    clauseName = clauseType;
    return true;
}
bool Clause::setArg(vector<string> argListInput) {
    argList = argListInput;
    return true;
}
string Clause::getClauseType() {
    return clauseName;
}
vector<string> Clause::getArg() {
    return argList;
}
int Clause::getArgCount() {
    return argList.size();
}
string Clause::toString() {
    string sb;

    if (Constants::StringToSymbol(getClauseType()) == PATTERN) {
        sb += getClauseType() + " " + getArg()[0] + "(";

        for (int i = 1; i < getArg().size(); i++) {
            sb += getArg()[i];
            if (i < getArg().size() - 1) {
                sb += ",";
            }
        }
        sb += ")";
    }
    else if (Constants::StringToSymbol(getClauseType()) == WITH) {
        sb += getClauseType() + " " + getArg()[0] + "=" + getArg()[1];
    }
    else {
        sb += getClauseType() + "(";
        int i = 0;
        for (string arg : getArg()) {
            sb += arg;
            i++;
            if (i < getArg().size()) {
                sb += ",";
            }
        }
        sb += ")";
    }

    return sb;
}

vector<string> Clause::getSynonyms() {
    vector<string> synonyms;

    for (string arg : getArg()) {
        if (!Utils::IsNonNegativeNumeric(arg) &&                                                                        //not a number
            !(arg == string(1, CHAR_SYMBOL_UNDERSCORE)) &&                                                                                            //not an underscore
            !(Utils::StartsWith(arg, CHAR_SYMBOL_DOUBLEQUOTES) && Utils::EndsWith(arg, CHAR_SYMBOL_DOUBLEQUOTES)) &&    //not in quotes
            !(Utils::StartsWith(arg, CHAR_SYMBOL_UNDERSCORE) && Utils::EndsWith(arg, CHAR_SYMBOL_UNDERSCORE))) {        //not surrounded by underscores
            synonyms.push_back(arg);                                                                                            //then it is a synonym
        }
    }

    return synonyms;
}
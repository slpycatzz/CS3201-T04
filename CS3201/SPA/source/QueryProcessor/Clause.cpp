#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "QueryProcessor/Clause.h"
#include "QueryProcessor/QueryUtils.h"
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

    if (getClauseType() == SYMBOL_PATTERN) {
        sb += getClauseType() + " " + getArg()[0] + "(";

        for (int i = 1; i < getArg().size(); i++) {
            sb += getArg()[i];
            if (i < getArg().size() - 1) {
                sb += ",";
            }
        }
        sb += ")";
    }
    else if (getClauseType() == SYMBOL_WITH) {
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

    if (getClauseType() == SYMBOL_WITH) {
        for (unsigned int i = 0; i < 2; i++) {
            string arg = getArg()[i];
            if (QueryUtils::IsSynonym(arg)) {
                synonyms.push_back(arg);
            }
        }
    }
    else {
        for (string arg : getArg()) {
            if (QueryUtils::IsSynonym(arg)) {
                synonyms.push_back(arg);
            }
        }
    }

    return synonyms;
}
#include <string>
#include <vector>
#include <unordered_map>

#include "Constants.h"
#include "QueryProcessor/QueryUtils.h"
#include "Utils.h"

using std::string;
using std::stringstream;
using std::vector;
using std::unordered_map;

string QueryUtils::LiteralToCandidate(string stringLiteral) {
    return (IsStringLiteral(stringLiteral)) ? GetValueFromStringLiteral(stringLiteral) : stringLiteral;
}

string QueryUtils::GetValueFromStringLiteral(string stringLiteral) {
    /* Returns underscore. */
    if (stringLiteral == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        return stringLiteral;
    }

    /* Remove starting and ending double quotes. */
    return stringLiteral.substr(1, stringLiteral.length() - 2);
}

bool QueryUtils::IsLiteral(string str) {
    return (Utils::IsNonNegativeNumeric(str) || IsStringLiteral(str));
}

bool QueryUtils::IsSynonym(std::string str) {
    if (!IsLiteral(str) && !(Utils::StartsWith(str, CHAR_SYMBOL_UNDERSCORE) && Utils::EndsWith(str, CHAR_SYMBOL_UNDERSCORE))) {
        return true;
    }
    else {
        return false;
    }
}

string QueryUtils::GetExpression(string expression) {
    stringstream res;
    for (char c : expression) {
        if ((c != CHAR_SYMBOL_SPACE) && (c != CHAR_SYMBOL_DOUBLEQUOTES)) {
            res << c;
        }
    }
    return res.str();
}

string QueryUtils::GetSubExpression(string expression) {
    stringstream res;
    for (char c : expression) {
        if ((c != CHAR_SYMBOL_SPACE) && (c != CHAR_SYMBOL_UNDERSCORE) && (c != CHAR_SYMBOL_DOUBLEQUOTES)) {
            res << c;
        }
    }
    return res.str();
}

bool QueryUtils::IsStringLiteral(string str) {
    if (Utils::StartsWith(str, CHAR_SYMBOL_DOUBLEQUOTES) && Utils::EndsWith(str, CHAR_SYMBOL_DOUBLEQUOTES)) {
        return true;
    } else if (str == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        return true;
    }

    return false;
}

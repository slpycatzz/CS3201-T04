#include <string>
#include <vector>
#include <unordered_map>

#include "Constants.h"
#include "QueryProcessor/QueryUtils.h"
#include "TreeNode.h"
#include "Utils.h"

using std::string;
using std::stringstream;
using std::vector;
using std::unordered_map;

TreeNode* QueryUtils::BuildExpressionTree(string expr) {
    unsigned i = expr.find_first_not_of(CHAR_SYMBOL_UNDERSCORE);
    unsigned j = expr.find_last_not_of(CHAR_SYMBOL_UNDERSCORE);
    string temp(LiteralToCandidate(expr.substr(i, j - i + 1)));

    if (Utils::IsNonNegativeNumeric(temp)) {
        return new TreeNode(CONSTANT, temp);
    } else {
        return new TreeNode(VARIABLE, temp);
    }
}

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

string QueryUtils::GetExpression(string expr) {
    stringstream res;
    for (char c : expr) {
        if ((c != ' ') && (c != '\"')) {
            res << c;
        }
    }
    return res.str();
}

string QueryUtils::GetSubExpression(string expr) {
    stringstream res;
    for (char c : expr) {
        if ((c != ' ') && (c != '_') && (c != '\"')) {
            res << c;
        }
    }
    return res.str();
}

unordered_map<string, string>
QueryUtils::GetSubMap(unordered_map<string, string>& map, const vector<string>& keyList) {
    unordered_map<string, string> result;
    for (string key : keyList) {
        result.insert_or_assign(key, map.at(key));
    }
    return result;
}

bool QueryUtils::IsStringLiteral(string str) {
    if (Utils::StartsWith(str, CHAR_SYMBOL_DOUBLEQUOTES) && Utils::EndsWith(str, CHAR_SYMBOL_DOUBLEQUOTES)) {
        return true;
    } else if (str == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        return true;
    }

    return false;
}

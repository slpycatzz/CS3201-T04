#include <string>

#include "Constants.h"
#include "QueryProcessor/QueryUtils.h"
#include "TreeNode.h"
#include "Utils.h"

using std::string;

TreeNode* QueryUtils::BuildExpressionTree(std::string expr) {
    unsigned i = expr.find_first_not_of('_');
    unsigned j = expr.find_last_not_of('_');
    std::string temp(LiteralToCandidate(expr.substr(i, j - i + 1)));

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

bool QueryUtils::IsStringLiteral(string str) {
    if (Utils::StartsWith(str, CHAR_SYMBOL_DOUBLEQUOTES) && Utils::EndsWith(str, CHAR_SYMBOL_DOUBLEQUOTES)) {
        return true;
    } else if (str == string(1, CHAR_SYMBOL_UNDERSCORE)) {
        return true;
    }

    return false;
}

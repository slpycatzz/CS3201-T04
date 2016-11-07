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
	if (IsStringLiteral(stringLiteral)) {
		return GetValueFromStringLiteral(stringLiteral);
	}
	else if (Utils::IsNonNegativeNumeric(stringLiteral)) {
		return Utils::IntToString(Utils::StringToInt(stringLiteral));
	}
	else {
		return stringLiteral;
	}
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

bool QueryUtils::IsStringLiteral(string str) {
	if (Utils::StartsWith(str, CHAR_SYMBOL_DOUBLEQUOTES) && Utils::EndsWith(str, CHAR_SYMBOL_DOUBLEQUOTES)) {
		return true;
	}

	return false;
}

bool QueryUtils::IsNonNegativeInt(std::string str)
{
	if (str == string(1, CHAR_SYMBOL_UNDERSCORE)) return true;
	
	for (unsigned int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}

	return !str.empty();
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

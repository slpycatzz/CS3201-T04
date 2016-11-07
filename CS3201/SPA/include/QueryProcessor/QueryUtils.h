#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

class QueryUtils {
 public:
    static std::string LiteralToCandidate(std::string literalstringLiteral);
    static std::string GetValueFromStringLiteral(std::string stringLiteral);
    static bool IsStringLiteral(std::string str);
    static bool IsLiteral(std::string str);
    static bool IsSynonym(std::string str);
	static bool IsNonNegativeInt(std::string str);

    static std::string GetExpression(std::string expression);
    static std::string GetSubExpression(std::string expression);
};

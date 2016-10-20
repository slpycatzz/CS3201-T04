#pragma once

#include <sstream>
#include <string>
#include <unordered_map>

#include "TreeNode.h"

class QueryUtils {
 public:
    static TreeNode* QueryUtils::BuildExpressionTree(std::string expression);

    static std::string LiteralToCandidate(std::string literalstringLiteral);
    static std::string GetValueFromStringLiteral(std::string stringLiteral);
    static bool IsStringLiteral(std::string str);
    static bool IsLiteral(std::string str);

	static std::string GetExpression(std::string expr);
	static std::string GetSubExpression(std::string expr);

	static std::unordered_map<std::string, std::string>
		GetSubMap(std::unordered_map<std::string, std::string> &map, const std::vector<std::string> &keyList);
};

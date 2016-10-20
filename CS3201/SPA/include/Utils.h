#pragma once

#include <queue>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include "TreeNode.h"

class Utils {
 public:
    static std::vector<std::vector<std::string>>
        Flatten(std::unordered_map<std::string, std::vector<std::string>> &map,
        std::vector<std::string> &list, unsigned int start, unsigned int end);

    static std::vector<std::vector<std::string>> Zip(std::vector<std::string> list1, std::vector<std::string> list2);

    static bool IsSameTree(TreeNode &node1, TreeNode &node2);
    static bool IsSubTree(TreeNode &node, TreeNode &root);

    static bool VectorContains(std::vector<unsigned int> vec, unsigned int i);
    static bool VectorContains(std::vector<std::string> vec, std::string str);

    static std::string VectorToString(std::vector<std::string> &vec);
    static std::vector<std::string> VectorToStrings(std::vector<std::vector<std::string>> &vec);

    static std::unordered_map<std::string, std::string> MergeMap(
        std::unordered_map<std::string, std::string> &map1,
        std::unordered_map<std::string, std::string> &map2);

    static std::unordered_map<std::string, std::string> ReduceMap(
        std::unordered_map<std::string, std::string> &map,
        std::vector<std::string> &selections);

    static std::unordered_map<std::string, std::string> GetSubMap(
        std::unordered_map<std::string, std::string> &map,
        const std::vector<std::string> &keyList);

    static std::string MapToString(std::unordered_map<std::string, std::string> &map);
    static std::string MapToValueString(std::unordered_map<std::string, std::string> &map);

    static std::string IntToString(unsigned int i);
    static std::vector<std::string> IntsToStrings(std::vector<unsigned int> vec);

    static unsigned StringToInt(std::string str);
    static std::vector<unsigned int> StringsToInts(std::vector<std::string> vec);

    static void Split(std::string str, char delimiter, std::vector<std::string> &target);
    static std::vector<std::string> Split(std::string str, char delimiter);
    static void SplitAndIgnoreEmpty(std::string str, char delimiter, std::vector<std::string> &target);
    static std::vector<std::string> SplitAndIgnoreEmpty(std::string str, char delimiter);

    static std::string TrimSpaces(std::string str);
    static std::string TrimLeadingSpaces(std::string str);
    static std::string TrimTrailingSpaces(std::string str);

    static bool IsValidNamingConvention(std::string str);
    static bool IsAlphanumeric(std::string str);
    static bool IsNonNegativeNumeric(std::string str);

    static bool StartsWith(std::string str, char c);
    static bool StartsWith(std::string str, std::string substr);
    static bool StartsWithAnAlphabet(std::string str);
    static bool EndsWith(std::string str, char c);
    static bool EndsWith(std::string str, std::string substr);

    static std::queue<std::string> GetPostfixExpression(std::vector<std::string> expression);
    static std::string GetExactExpressionWithBrackets(std::queue<std::string> postfixExpression);
    static std::set<std::string> GetSubExpressionsWithBrackets(std::queue<std::string> postfixExpression);

 private:
    static bool IsOperator(std::string operator_);
    static unsigned int GetOperatorPrecedence(std::string operator_);
};

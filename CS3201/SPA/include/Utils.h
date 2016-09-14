#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Utils {
 public:
    //template<class K, class V>
    static std::vector<std::vector<std::string>> Flatten(std::unordered_map<std::string, std::vector<std::string>> &map, std::vector<std::string> &list, unsigned int start, unsigned int end);

    template<class T>
    static std::vector<std::vector<T>> Zip(std::vector<T> list1, std::vector<T> list2);

    //template<class K, class V>
    static std::unordered_map<std::string, std::string> MergeMap(std::unordered_map<std::string, std::string> &map1, std::unordered_map<std::string, std::string> &map2);

    //template<class T>
    static bool VectorContains(std::vector<unsigned int> vec, unsigned int i);

    static std::string VectorToString(std::vector<std::string> &vec);
    static std::vector<std::string> VectorToString(std::vector<std::vector<std::string>> &vec);

    static std::string IntToString(unsigned int i);
    static std::vector<std::string> IntToString(std::vector<unsigned int> vec);

    static unsigned StringToInt(std::string str);
    static std::vector<unsigned int> StringToInt(std::vector<std::string> vec);

    static void Split(std::string str, char delimiter, std::vector<std::string> &target);
    static std::vector<std::string> Split(std::string str, char delimiter);
    static void SplitAndIgnoreEmpty(std::string str, char delimiter, std::vector<std::string> &target);
    static std::vector<std::string> SplitAndIgnoreEmpty(std::string str, char delimiter);

    static std::string TrimSpaces(std::string str);
    static std::string TrimLeadingSpaces(std::string str);
    static std::string TrimTrailingSpaces(std::string str);

    static bool IsValidNamingConvention(std::string str);
    static bool IsAlphanumeric(std::string str);
	static bool IsStringLiteral(std::string str);
	static bool IsLiteral(std::string str);
    static bool IsNonNegativeNumeric(std::string str);
    static bool StartsWithAlphabet(std::string str);
};

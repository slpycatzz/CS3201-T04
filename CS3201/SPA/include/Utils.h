#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Utils {
 public:
    static std::vector<std::string> Split(std::string str, char delimiter);
    static std::vector<std::string> SplitAndIgnoreEmpty(std::string str, char delimiter);
    static void Split(std::string str, char delimiter, std::vector<std::string> &target);
    static void SplitAndIgnoreEmpty(std::string str, char delimiter, std::vector<std::string> &target);
    
    static std::string TrimSpaces(std::string str);
    static std::string TrimLeadingSpaces(std::string str);
    static std::string TrimTrailingSpaces(std::string str);
    
    static bool IsValidNamingConvention(std::string str);
    static bool IsAlphanumeric(std::string str);
    static bool IsNonNegativeNumeric(std::string str);
    static bool StartsWithAlphabet(std::string str);

    static bool VectorContains(std::vector<unsigned int> vec, unsigned int i);
	
	template<class K, class V> static std::vector<std::vector<V>> Flatten(std::unordered_map<K, V>, std::vector<K>);
	template<class T> static std::vector<std::vector<T>> Zip(std::vector<T>, std::vector<T>);
	template<class K, class V> static std::unordered_map<K,V> MergeMap(std::unordered_map<K,V>&, std::unordered_map<K,V>&);
};

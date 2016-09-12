#include <sstream>
#include <string>
#include <vector>

#include "Utils.h"
#include "Constants.h"

using std::string;
using std::vector;
using std::istringstream;

template<class K, class V>
std::vector<std::vector<V>> Utils::Flatten(std::unordered_map<K, std::vector<V>> &map,
	std::vector<K> &selectList, unsigned start, unsigned end)
{
	using V_vector = std::vector<V>;
	std::vector<V_vector> result;
	if (start > end) {
		result.push_back(std::vector<V>());
	}
	else {
		K firstKey = selectList[start];
		V_vector firstVList = map[firstKey];
		std::vector<V_vector> recursiveList = Flatten(map, selectList, start + 1, end);
		for (V value : firstVList) {
			for (V_vector : recursiveList) {
				V_vector vv;
				vv = recursiveList;
				vv.insert(vv.begin(), value);
				result.push_back(vv);
			}
		}
	}
	return result;
}

template<class T>
std::vector<std::vector<T>> Utils::Zip(std::vector<T> list1, std::vector<T> list2) {
	std::vector<std::vector<T>> result;
	for (T item1 : list1) {
		for (T item2 : list2) {
			std::vector<T> item();
			item.push_back(item1);
			item.push_back(item2);
			result.push_back<item>;
		}
	}
}

template<class K, class V>
std::unordered_map<K, V> Utils::MergeMap(std::unordered_map<K, V> &map1, std::unordered_map<K, V> &map2) {
	map1.insert(map2.begin(), map2.end());
	return map1;
}

std::string Utils::VectorToString(std::vector<std::string> &vs) {
	std::stringstream ss;
	ss << "<";
	std::vector<std::string>::iterator itr(vs.begin());
	while (itr != --vs.end()) {
		ss << *itr << CHAR_SYMBOL_COMMA;
	}
	ss << *itr << ">";
}

std::vector<std::string> Utils::VectorToString(std::vector<std::vector<std::string>> &vv) {
	std::vector<std::string> result;
	for (std::vector<std::string> item : vv) {
		result.push_back(VectorToString(item));
	}
	return result;
}

vector<string> Utils::Split(string str, char delimiter) {
    vector<string> result;
    
    Utils::Split(str, delimiter, result);

    return result;
}

vector<string> Utils::SplitAndIgnoreEmpty(string str, char delimiter) {
    vector<string> result;

    Utils::SplitAndIgnoreEmpty(str, delimiter, result);

    return result;
}

void Utils::Split(string str, char delimiter, vector<string> &target) {
    string temp;
    istringstream stringStream;

    stringStream.str(str);
    while (getline(stringStream, temp, delimiter)) {
        target.push_back(temp);
    }
}

void Utils::SplitAndIgnoreEmpty(string str, char delimiter, vector<string> &target) {
    string temp;
    istringstream stringStream;

    stringStream.str(str);
    while (getline(stringStream, temp, delimiter)) {
        if (!temp.empty()) {
            target.push_back(temp);
        }
    }
}

string Utils::TrimSpaces(string str) {
    str = Utils::TrimLeadingSpaces(str);
    str = Utils::TrimTrailingSpaces(str);

    return str;
}

string Utils::TrimLeadingSpaces(string str) {
    int position = str.find_first_not_of(" \t");
    if (position == string::npos) {
        return (str.find_last_of(" \t") == (str.size() - 1)) ? "" : str;
    }

    return str.substr(position);
}

string Utils::TrimTrailingSpaces(string str) {
    int position = str.find_last_not_of(" \t");
    if (position == string::npos) {
        return (str.find_last_of(" \t") == (str.size() - 1)) ? "" : str;
    }

    return str.substr(0, position + 1);
}

std::string Utils::IntToString(unsigned i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

unsigned Utils::StringToInt(std::string str)
{
	return std::stoi(str);
}

std::vector<std::string> Utils::IntToString(std::vector<unsigned> vi)
{
	std::vector<std::string> result;
	for (unsigned i : vi) {
		result.push_back(IntToString(i));
	}
	return result;
}

std::vector<unsigned> Utils::StringToInt(std::vector<std::string> vs)
{
	std::vector<unsigned> result;
	for (std::string str : vs) {
		result.push_back(StringToInt(str));
	}
	return result;
}

bool Utils::IsValidNamingConvention(string str) {
    return StartsWithAlphabet(str) && IsAlphanumeric(str);
}

bool Utils::IsAlphanumeric(string str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }

    return true;
}

bool Utils::IsNonNegativeNumeric(string str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool Utils::StartsWithAlphabet(string str) {
    return (isalpha(str[0])) ? true : false;
}

bool Utils::VectorContains(vector<unsigned int> vec, unsigned int i) {
    return (std::find(vec.begin(), vec.end(), i) != vec.end());
}

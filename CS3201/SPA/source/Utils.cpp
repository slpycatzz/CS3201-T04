#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "Utils.h"

using std::istringstream;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

template<class K, class V>
vector<vector<V>> Utils::Flatten(unordered_map<K, vector<V>> &map, vector<K> &list, unsigned int start, unsigned int end) {
    using V_vector = vector<V>;

    vector<V_vector> result;
    if (start > end) {
        return vector<V>();
    }

    V_vector firstVList = map[selectList[start]];
    std::vector<V_vector> recursiveList = Flatten(map, selectList, start + 1, end);

    for (V value : firstVList) {
        for (V_vector : recursiveList) {
            V_vector vec = recursiveList;
            vec.insert(vec.begin(), value);

            result.push_back(vec);
        }
    }

    return result;
}

template<class T>
vector<vector<T>> Utils::Zip(vector<T> list1, vector<T> list2) {
    vector<vector<T>> result;

    for (T item1 : list1) {
        for (T item2 : list2) {
            vector<T> item;
            item.push_back(item1);
            item.push_back(item2);

            result.push_back<item>;
        }
    }

    return result;
}

template<class K, class V>
unordered_map<K, V> Utils::MergeMap(unordered_map<K, V> &map1, unordered_map<K, V> &map2) {
    map1.insert(map2.begin(), map2.end());

    return map1;
}

//template<class T>
bool Utils::VectorContains(vector<unsigned int> vec, unsigned int value) {
    return (std::find(vec.begin(), vec.end(), value) != vec.end());
}

string Utils::VectorToString(vector<string> &vec) {
    stringstream ss;
    ss << "<";

    vector<string>::iterator itr(vec.begin());
    while (itr != --vec.end()) {
        ss << *itr << CHAR_SYMBOL_COMMA;
    }

    ss << *itr << ">";

    return ss.str();
}

vector<string> Utils::VectorToString(vector<vector<string>> &vec) {
    vector<string> result;
    for (vector<string> item : vec) {
        result.push_back(VectorToString(item));
    }

    return result;
}

std::string Utils::IntToString(unsigned int i) {
    return std::to_string(i);
}

vector<string> Utils::IntToString(vector<unsigned int> vec) {
    vector<string> result;
    for (unsigned int i : vec) {
        result.push_back(IntToString(i));
    }

    return result;
}

unsigned int Utils::StringToInt(string str) {
    return std::stoi(str);
}

vector<unsigned int> Utils::StringToInt(vector<string> vec) {
    vector<unsigned> result;
    for (string str : vec) {
        result.push_back(StringToInt(str));
    }

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

vector<string> Utils::Split(string str, char delimiter) {
    vector<string> result;

    Utils::Split(str, delimiter, result);

    return result;
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

vector<string> Utils::SplitAndIgnoreEmpty(string str, char delimiter) {
    vector<string> result;

    Utils::SplitAndIgnoreEmpty(str, delimiter, result);

    return result;
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

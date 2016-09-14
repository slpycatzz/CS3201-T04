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

//template<class K, class V>
vector<vector<string>> Utils::Flatten(unordered_map<string, vector<string>> &map, vector<string> &list, unsigned int start, unsigned int end) {
    using V_vector = vector<string>;

    vector<vector<string>> result;
    if (start > end) {
        return vector<vector<string>>();
    }

    vector<string> firstVList = map[list[start]];
    std::vector<vector<string>> recursiveList = Flatten(map, list, start + 1, end);

    for (string value : firstVList) {
        for (vector<string> vec : recursiveList) {
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

//template<class K, class V>
unordered_map<string, string> Utils::MergeMap(unordered_map<string, string> &map1, unordered_map<string, string> &map2) {
    map1.insert(map2.begin(), map2.end());

    return map1;
}

TreeNode* Utils::buildExprTree(std::string expr) {
	unsigned i = expr.find_first_not_of('_');
	unsigned j = expr.find_last_not_of('_');
	std::string temp(expr.substr(i, j-i+1));
	return &TreeNode(VARIABLE, temp);
}

bool Utils::IsSameTreeNode(TreeNode &node1, TreeNode &node2) {
	return ((node1.getSymbol == node2.getSymbol)
		&& (node1.getValue.compare(node2.getValue));
}

bool Utils::IsSameTree(TreeNode &tree1, TreeNode &tree2) {
	if (!IsSameTreeNode(tree1, tree2)) {
		return false;
	}
	else {
		std::vector<TreeNode*> childrenLst1(tree1.getChildren());
		std::vector<TreeNode*> childrenLst2(tree2.getChildren());
		if (childrenLst1.size() != childrenLst2.size()) return false;
		for (unsigned i = 0; i < childrenLst1.size(); i++) {
			if (!IsSameTree(*childrenLst1[i], *childrenLst2[i])) return false;
		}
		return true;
	}
}

bool Utils::IsSubTree(TreeNode &tree1, TreeNode &tree2) {
	if (IsSameTree(tree1, tree2)) {
		return true;
	}
	else {
		std::vector<TreeNode*> childrenLst1(tree1.getChildren());
		for (unsigned i = 0; i < childrenLst1.size(); i++) {
			if (IsSubTree(*childrenLst1[i], tree2)) return true;
		}
		return false;
	}
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

bool Utils::IsStringLiteral(std::string str) {
	return ((str[0] == '\"') && (str[1] == '\"'));
}

bool Utils::IsLiteral(std::string str) {
	return (IsNonNegativeNumeric(str) || IsStringLiteral(str));
}

bool Utils::StartsWithAlphabet(string str) {
    return (isalpha(str[0])) ? true : false;
}

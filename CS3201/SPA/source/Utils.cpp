#include <sstream>
#include <string>
#include <vector>

#include "Utils.h"

using std::string;
using std::vector;

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
    std::istringstream stringStream;

    stringStream.str(str);
    while (getline(stringStream, temp, delimiter)) {
        target.push_back(temp);
    }
}

void Utils::SplitAndIgnoreEmpty(string str, char delimiter, vector<string> &target) {
    string temp;
    std::istringstream stringStream;

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

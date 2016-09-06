#include <sstream>
#include <string>
#include <vector>

#include "Utils.h"

std::vector<std::string> Utils::Split(std::string str, char delimiter) {
    std::vector<std::string> result;
    
    Utils::Split(str, delimiter, result);

    return result;
}

std::vector<std::string> Utils::SplitAndIgnoreEmpty(std::string str, char delimiter) {
    std::vector<std::string> result;

    Utils::SplitAndIgnoreEmpty(str, delimiter, result);

    return result;
}

void Utils::Split(std::string str, char delimiter, std::vector<std::string> &target) {
    std::string temp;
    std::istringstream stringStream;

    stringStream.str(str);
    while (getline(stringStream, temp, delimiter)) {
        target.push_back(temp);
    }
}

void Utils::SplitAndIgnoreEmpty(std::string str, char delimiter, std::vector<std::string> & target) {
    std::string temp;
    std::istringstream stringStream;

    stringStream.str(str);
    while (getline(stringStream, temp, delimiter)) {
        if (!temp.empty()) {
            target.push_back(temp);
        }
    }
}

std::string Utils::TrimSpaces(std::string str) {
    str = Utils::TrimLeadingSpaces(str);
    str = Utils::TrimTrailingSpaces(str);

    return str;
}

std::string Utils::TrimLeadingSpaces(std::string str) {
    int position = str.find_first_not_of(" \t");
    if (position == std::string::npos) {
        return (str.find_last_of(" \t") == (str.size() - 1)) ? "" : str;
    }

    return str.substr(position);
}

std::string Utils::TrimTrailingSpaces(std::string str) {
    int position = str.find_last_not_of(" \t");
    if (position == std::string::npos) {
        return (str.find_last_of(" \t") == (str.size() - 1)) ? "" : str;
    }

    return str.substr(0, position + 1);
}

bool Utils::IsValidNamingConvention(std::string str) {
    return StartsWithAlphabet(str) && IsAlphanumeric(str);
}

bool Utils::IsAlphanumeric(std::string str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }

    return true;
}

bool Utils::IsNonNegativeNumeric(std::string str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool Utils::StartsWithAlphabet(std::string str) {
    return (isalpha(str[0])) ? true : false;
}

#include <string>

#include "Utils.h"

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
    return isalpha(str[0]);
}

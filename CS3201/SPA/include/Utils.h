#pragma once

#include <string>

class Utils {
 public:
    static std::string TrimLeadingSpaces(std::string str);
    static std::string TrimTrailingSpaces(std::string str);
    static bool IsValidNamingConvention(std::string str);
    static bool IsAlphanumeric(std::string str);
    static bool IsNonNegativeNumeric(std::string str);
    static bool StartsWithAlphabet(std::string str);
};

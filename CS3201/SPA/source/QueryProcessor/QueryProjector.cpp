#include <string>
#include <vector>
#include "QueryProcessor/QueryProjector.h"

QueryProjector::QueryProjector() {}

QueryProjector::~QueryProjector() {}

//Expect a map of tuple of synonyms -> vector of tuples of string values from evaluator

std::list<std::string> QueryProjector::formatResult(std::vector<std::string> resultList) {
    std::list<std::string> result;

    for (std::string s : resultList) {
        result.emplace_back(s.substr(1, s.length() - 2));
    }

    return result;
}

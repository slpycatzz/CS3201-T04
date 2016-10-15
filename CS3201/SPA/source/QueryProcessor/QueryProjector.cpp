#include <string>
#include <vector>
#include "QueryProcessor/QueryProjector.h"

QueryProjector::QueryProjector() {}

QueryProjector::~QueryProjector() {}

//Expect a map of tuple of synonyms -> vector of tuples of string values from evaluator

std::list<std::string> QueryProjector::formatResult(ResultList resultList) {
    std::list<std::string> result;

    for (std::vector<std::string> v : (*resultList.begin()).second) {
		std::string s = Utils::VectorToString(v);
        result.emplace_back(s.substr(1, s.length() - 2));
    }

    return result;
}

#include <string>
#include <vector>
#include "QueryProcessor/QueryProjector.h"

QueryProjector::QueryProjector() {}

QueryProjector::~QueryProjector() {}

std::list<std::string> QueryProjector::formatResult(std::vector<std::string> resultList) {
    std::list<std::string> result;

    for (std::string s : resultList) {
        result.emplace_back(s.substr(1, s.length() - 2));
    }

    return result;
}

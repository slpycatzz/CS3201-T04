#pragma once

#include <string>
#include <list>
#include <vector>
#include "QueryProcessor/QueryEvaluator.h"

class QueryProjector {
public:
    QueryProjector();
    ~QueryProjector();
    void projectResult(std::list<std::string>& results, std::unordered_map<std::string, bool> resultsInfo, ResultList resultList);

private:
};

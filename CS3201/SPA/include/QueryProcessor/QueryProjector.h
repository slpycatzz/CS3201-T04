#pragma once

#include <string>
#include <list>
#include <vector>
#include "QueryProcessor/QueryEvaluator.h"

class QueryProjector {
public:
    QueryProjector();
    ~QueryProjector();
    std::list<std::string> formatResult(ResultList resultList);

private:
};

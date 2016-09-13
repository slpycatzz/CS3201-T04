#pragma once

#include <string>
#include <list>
#include <vector>

class QueryProjector {
public:
    QueryProjector();
    ~QueryProjector();
    std::list<std::string> formatResult(std::vector<std::string> resultList);

private:
};

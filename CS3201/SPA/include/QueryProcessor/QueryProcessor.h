#pragma once
#include <string>
#include <list>

class QueryProcessor
{
public:
    QueryProcessor();
    ~QueryProcessor();

    void process(std::string query, std::list<std::string>& results);
};


#pragma once
#include <vector>

class Clause {
 public:
    static std::vector<Clause*> getClauses();

 private:
    Clause();
    ~Clause();
    static Clause *clause;
};

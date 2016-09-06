#pragma once
#include <vector>

class Clause {
 public:
     Clause();
     ~Clause();
    static std::vector<Clause*> getClauses();

 private:
    static Clause *clause;
};

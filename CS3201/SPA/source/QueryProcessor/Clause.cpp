#include "Clause.h"
#include <vector>

std::vector<Clause*> clauses;

Clause::Clause() {
}

Clause::~Clause() {
}

std::vector<Clause*> Clause::getClauses() {
    return clauses;
}

#include <unordered_map>
#include <set>
#include <string>

#include "Frontend/DesignExtractor.h"
#include "PKB/PKB.h"
#include "PKB/Table.h"

using std::unordered_map;
using std::set;
using std::string;

bool DesignExtractor::IsNextTransitive(unsigned int current, unsigned int next) {
    unordered_map<unsigned int, set<string>> procedureFirstStmtMap = PKB::GetProcedureFirstStmtMap();
    unordered_map<unsigned int, set<unsigned int>> ifNextMap = PKB::GetIfNextMap();
    unordered_map<unsigned int, set<unsigned int>> whileNextMap = PKB::GetWhileNextMap();
    
    /* Check if both is in the same procedure. */
    bool isInSameProcedure = false;
    for (const auto &pair : procedureFirstStmtMap) {
        unsigned int firstStmtNumber = pair.first;

        if (current < firstStmtNumber && next >= firstStmtNumber) {
            return false;
        }

        if (current >= firstStmtNumber && next < firstStmtNumber) {
            return false;
        }
    }

    /* Check if it is a while next. */
    if (whileNextMap.count(current) == 1) {
        if (whileNextMap[current].count(next) == 1) {
            return true;
        }
    }

    /* Check if it is a if next. */
    if (ifNextMap.count(current) == 1) {
        if (ifNextMap[current].count(next) == 1) {
            return false;
        }
    }

    /* Check normal next. */
    if (current >= next) {
        return false;
    }

    return true;
}

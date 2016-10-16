#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Clause.h"
#include "Constants.h"

class QueryTree {
 public:
    QueryTree();
    ~QueryTree();

    /* returns: varMap[varName] = varType e.g. varMap[a1] = ASSIGN */
    std::unordered_map<std::string, Symbol> getVarMap();
    std::vector<std::string> getResults();
    std::unordered_map<std::string, bool> getResultsInfo();
    // returns all clauses (order: st->pattern->with)
    std::vector<Clause> QueryTree::getClauses();
    std::vector<Clause> QueryTree::getClauses(std::vector<Symbol> clauseType);

    bool insertDeclaration(std::unordered_map<std::string, Symbol>);
    bool insert(Symbol type, std::string argType, std::vector<std::string> argList);
    bool insert(Symbol type, std::string argType, std::unordered_map<std::string, bool> argList);

    // Used by optimizer
    std::vector<Clause> getBooleanClauses();                    // List of clauses that have no synonyms
    std::vector<std::vector<Clause>> getUnselectedGroups();     // Groups of list of clauses that are not selected by query, length > 0 is sufficient when evaluting
    std::vector<std::vector<Clause>> getSelectedGroups();       // Groups of list of clauses that are selected by query
    void setBooleanClauses(std::vector<Clause> booleanClauses);
    void setUnselectedGroups(std::vector<std::vector<Clause>> unselectedGroups);
    void setSelectedGroups(std::vector<std::vector<Clause>> selectedGroups);
    void printGroups();

 private:
     /* container for list of declared variableMap to Symbol*/
     std::unordered_map<std::string, Symbol> varMap;
     std::unordered_map<std::string, bool> varAttrMap;      // true if varAttr is selected
     std::vector<std::string> varList;
     std::vector<Clause> suchThatList;
     std::vector<Clause> patternList;
     std::vector<Clause> withList;

     std::vector<Clause> booleanClauses;
     std::vector<std::vector<Clause>> unselectedGroups;
     std::vector<std::vector<Clause>> selectedGroups;
};

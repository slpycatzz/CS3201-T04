#include <iostream>
#include <set>

#include "QueryProcessor/QueryOptimizer.h"
#include "QueryProcessor/Clause.h"
#include "Utils.h"

using std::string;
using std::vector;

QueryOptimizer::QueryOptimizer() {}

QueryOptimizer::~QueryOptimizer() {}

QueryTree QueryOptimizer::optimize(QueryTree qt) {
    this->queryTree = qt;

    vector<Clause> booleanClauses;
    vector<vector<Clause>> unselectedGroup;
    vector<vector<Clause>> selectedGroup;

    vector<Clause> allClauses(queryTree.getClauses("suchThat pattern"));

    for (unsigned i = 0; i < allClauses.size(); i++) {
        Clause clause = allClauses.at(i);
        
        if (clause.getSynonyms().size() == 0) {
            booleanClauses.push_back(clause);
            allClauses.erase(allClauses.begin() + i);
            i--;
        }
    }

    vector<vector<Clause>> genericGroups;
    
    //Insert first clause into group if group is empty
    if (genericGroups.empty() && !allClauses.empty()) {
        vector<Clause> group;
        group.push_back(allClauses.at(0));
        genericGroups.push_back(group);
        allClauses.erase(allClauses.begin() + 0);
    }
    
    //First common synonym detection loop
    //For every clause, compare with clauses that are already in groups. If common synonyms are found, add the clause into the group
    for (vector<Clause>::iterator it1 = allClauses.begin(); it1 != allClauses.end();) {
        Clause clause = *it1;
        bool isInGroup = false;
        for (vector<vector<Clause>>::iterator it2 = genericGroups.begin(); it2 != genericGroups.end(); ++it2) {
            vector<Clause> group = *it2;
            for (Clause c : group) {
                bool match = false;
                vector<string> args = clause.getArg();
                for (string arg : c.getArg()) {
                    if (clause.getClauseType() == "pattern") {
                        if (arg == args.at(0) || arg == args.at(1) || arg == args.at(2)) {
                            match = true;
                        }
                    }
                    else {
                        if (arg == args.at(0) || arg == args.at(1)) {
                            match = true;
                        }
                    }
                }
                if (match) {
                    (*it2).push_back(clause);
                    isInGroup = true;
                    break;
                }
            }
            if (isInGroup) {
                break;
            }
        }
        if (!isInGroup) {
            vector<Clause> newGroup;
            newGroup.push_back(clause);
            genericGroups.push_back(newGroup);
            isInGroup = true;
        }
        
        if (isInGroup) {
            it1 = allClauses.erase(it1);
        }
        else {
            ++it1;
        }
    }
    
    //Second common synonym detection loop
    //Combine groups that have common synonyms, that may not be detected in the first loop
    for (vector<vector<Clause>>::iterator it1 = genericGroups.begin(); it1 != genericGroups.end(); ++it1) {
        vector<Clause> group1 = *it1;
        std::set<string> synonyms;

        for (Clause clause : group1) {
            for (string synonym : clause.getSynonyms()) {
                synonyms.insert(synonym);
            }
        }

        for (vector<vector<Clause>>::iterator it2 = it1 + 1; it2 != genericGroups.end();) {
            vector<Clause> group2 = *it2;
            bool isCombined = false;
            for (Clause c : group2) {
                bool match = false;
                for (string args : c.getArg()) {
                    std::set<string>::const_iterator it3 = synonyms.find(args);
                    if (it3 != synonyms.end()) {
                        //found matching synonym
                        match = true;
                        break;
                    }
                }
                if (match) {
                    (*it1).insert((*it1).end(), (*it2).begin(), (*it2).end()); //copy all clauses from group 2 into group 1
                    isCombined = true;
                    break;
                }
            }
            if (isCombined) {
                it2 = genericGroups.erase(it2);
            }
            else {
                ++it2;
            }
        }
    }
    
    //Check synonyms used in groups against those that are selected by query
    //If none, insert to unselected groups
    //If selected, insert to selected groups
    for (vector<vector<Clause>>::iterator it1 = genericGroups.begin(); it1 != genericGroups.end();) {
        vector<Clause> group = *it1;
        std::set<string> synonyms;

        for (Clause clause : group) {
            for (string synonym : clause.getSynonyms()) {
                synonyms.insert(synonym);
            }
        }
        
        bool isSelected = false;
        for (string var : queryTree.getResults()) {
            std::set<string>::const_iterator it2 = synonyms.find(var);
            if (it2 != synonyms.end()) {
                //at least one of the synonyms are selected by the query
                isSelected = true;
                break;
            }
        }
        if (isSelected) {
            selectedGroup.push_back(*it1);
            it1 = genericGroups.erase(it1);
        }
        else {
            ++it1;
        }
        
    }

    //Add weights to clauses and sort
    
    queryTree.setBooleanClauses(booleanClauses);
    queryTree.setUnselectedGroups(genericGroups);
    queryTree.setSelectedGroups(selectedGroup);

    return queryTree;
}


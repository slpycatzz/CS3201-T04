#include <map>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "QueryProcessor/TotalCombinationList.h"
#include "Utils.h"

using std::string;
using std::vector;

/* Constructor */
TotalCombinationList::TotalCombinationList() {
    empty = true;
    factorCounter = 0;
}

/* Destructor */
TotalCombinationList::~TotalCombinationList() {}

/* Content Manipulation */

void TotalCombinationList::addSynonym(Synonym &syn, unsigned index) {
    if (factorList.find(index) != factorList.end()) {
        content.insert_or_assign(syn, index);
    }
}

void TotalCombinationList::addSynonym(const char* &syn, unsigned index) {
    addSynonym(string(syn), index);
}

void TotalCombinationList::addSynonym(const char syn[], unsigned index) {
    addSynonym(string(syn), index);
}

void TotalCombinationList::addSynonym(Synonym &syn, vector<Candidate> &candidateList) {
    PartialCombinationList partList(makePartialCombiList(syn, candidateList));
    addSynonym(syn, partList);
}

void TotalCombinationList::addSynonym(const char* &syn, vector<Candidate> &candidateList) {
    addSynonym(string(syn), candidateList);
}

void TotalCombinationList::addSynonym(const char syn[], vector<Candidate> &candidateList) {
    addSynonym(string(syn), candidateList);
}

void TotalCombinationList::addSynonym(Synonym &syn, PartialCombinationList &partList) {
    factorList.insert_or_assign(factorCounter, partList);
    content.insert_or_assign(syn, factorCounter);
    factorCounter++;
    empty = partList.empty();
}

void TotalCombinationList::addSynonym(const char* &syn, PartialCombinationList &partList) {
    addSynonym(string(syn), partList);
}

void TotalCombinationList::addSynonym(const char syn[], PartialCombinationList &partList) {
    addSynonym(string(syn), partList);
}

void TotalCombinationList::merge(Synonym &syn1, Synonym &syn2) {
    unsigned index1(content[syn1]);
    unsigned index2(content[syn2]);
    if (index1 == index2) {
        return;
    } else {
        PartialCombinationList candidateList1(factorList[content[syn1]]);
        PartialCombinationList candidateList2(factorList[content[syn2]]);
        PartialCombinationList &mergedList = cartesianProduct(candidateList1, candidateList2);

        factorList.erase(index1);
        factorList.erase(index2);
        factorList.insert_or_assign(factorCounter, mergedList);

        for (auto kv : content) {
            if ((kv.second == index1) || (kv.second == index2)) {
                content.insert_or_assign(kv.first, factorCounter);
            }
        }
        factorCounter++;
    }
}

void TotalCombinationList::combine(TotalCombinationList &combiList) {
    std::unordered_map<Synonym, unsigned>& combiListContent(combiList.getContent());
	std::map<unsigned, PartialCombinationList>& combiListFactors(combiList.getFactorList());
    for (std::pair<Synonym, unsigned> kv : combiListContent) {
		unsigned index = kv.second + factorCounter;
		content.insert_or_assign(kv.first, index);
		factorList.insert_or_assign(index, combiListFactors[kv.second]);
		//addSynonym(kv.first, combiList[kv.first]);
    }
	factorCounter += combiList.getFactorCount();
	empty = (empty && combiList.isEmpty());
}

// template<typename Filterer>
void TotalCombinationList::filter(PartialCombinationList &candidateList, std::function<bool(CandidateCombination)> filterer) {
    PartialCombinationList::iterator iter = candidateList.begin();
    while (iter != candidateList.end()) {
        if (filterer(*iter)) {
            iter++;
        } else {
            iter = candidateList.erase(iter);
        }
    }
    empty = candidateList.empty();
}

void TotalCombinationList::filter(bool expression) {
    if (!expression) {
        empty = true;
    }
}

// template<typename Filterer>
void TotalCombinationList::filter(Synonym &syn, std::function<bool(CandidateCombination)> filterer) {
    PartialCombinationList &candidateList(factorList[content[syn]]);
    filter(candidateList, filterer);
}

// template<typename Filterer>
void TotalCombinationList::mergeAndFilter(Synonym &syn1, Synonym &syn2, std::function<bool(CandidateCombination)> filterer) {
    unsigned index1(content[syn1]);
    unsigned index2(content[syn2]);
    if (index1 == index2) {
        filter(syn1, filterer);
    } else {
        PartialCombinationList &candidateList1(factorList[index1]);
        PartialCombinationList &candidateList2(factorList[index2]);
        PartialCombinationList &mergedList = cartesianProduct(candidateList1, candidateList2, filterer);

        factorList.erase(index1);
        factorList.erase(index2);
        factorList.insert_or_assign(factorCounter, mergedList);

        for (auto kv : content) {
            if ((kv.second == index1) || (kv.second == index2)) {
                content.insert_or_assign(kv.first, factorCounter);
            }
        }
        factorCounter++;
        empty = mergedList.empty();
    }
}

/* Content accessors */

std::unordered_map<Synonym, unsigned>& TotalCombinationList::getContent() {
    return content;
}

std::map<unsigned, PartialCombinationList>& TotalCombinationList::getFactorList() {
    return factorList;
}

unsigned TotalCombinationList::getFactorCount() {
	return factorCounter;
}

bool TotalCombinationList::contains(Synonym &syn) {
    return (content.find(syn) != content.end());
}
bool TotalCombinationList::contains(const char* &syn) {
    return contains(string(syn));
}
bool TotalCombinationList::contains(const char syn[]) {
    return contains(string(syn));
}

PartialCombinationList& TotalCombinationList::operator[](Synonym &syn) {
    return factorList[content[syn]];
}

PartialCombinationList& TotalCombinationList::operator[](const char* &syn) {
    return factorList[content[string(syn)]];
}

PartialCombinationList& TotalCombinationList::operator[](const char syn[]) {
    return factorList[content[string(syn)]];
}

bool TotalCombinationList::isEmpty() {
    return empty;
}

void TotalCombinationList::reduceSingleFactor(vector<Synonym> &synList, PartialCombinationList &candidateList) {
    combinationComp comp(synList);
    std::set<CandidateCombination, combinationComp> resultSet(comp);
    for (CandidateCombination &combination : candidateList) {
        CandidateCombination selectedCombi(Utils::GetSubMap(combination, synList));
        resultSet.insert(selectedCombi);
    }
    candidateList.clear();
    for (CandidateCombination comb : resultSet) {
        candidateList.push_back(comb);
    }
}

void TotalCombinationList::reduceTotalContent(vector<Synonym> &synList) {
    for (auto &kv : factorList) {
        vector<Synonym> subSynList;
        for (Synonym syn : synList) {
			std::unordered_map<Synonym, unsigned>::iterator it = content.find(syn);
			if ((it != content.end()) && ((*it).second == kv.first)) {
                subSynList.push_back(syn);
            }
        }
        reduceSingleFactor(subSynList, kv.second);
    }
}


PartialCombinationList& TotalCombinationList::getSingleFactor(vector<Synonym> &synList, PartialCombinationList &candidateList) {
    reduceSingleFactor(synList, candidateList);
    return candidateList;
}

PartialCombinationList& TotalCombinationList::getCombinationList(vector<Synonym> &synList) {
    reduceTotalContent(synList);
    PartialCombinationList &result = (*factorList.begin()).second;
    for (auto &kv : factorList) {
        result = cartesianProduct(result, kv.second);
    }
    return result;
}

PartialCombinationList TotalCombinationList::cartesianProduct(PartialCombinationList &list1, PartialCombinationList &list2) {
    if (&list1 == &list2) {
        return list1;
    }
	else {
        PartialCombinationList::iterator iter1 = list1.begin();
        while (iter1 != list1.end()) {
            CandidateCombination combi1 = *iter1;
            iter1 = list1.erase(iter1);
            for (CandidateCombination combi2 : list2) {
                CandidateCombination mergedCombi = Utils::MergeMap(combi1, combi2);
                list1.insert(iter1, mergedCombi);
            }
        }
        return list1;
    }
}

// template<typename Filterer>
PartialCombinationList TotalCombinationList::cartesianProduct(PartialCombinationList &list1,
    PartialCombinationList &list2, std::function<bool(CandidateCombination)> filterer) {
    if (&list1 == &list2) {
        filter(list1, filterer);
        return list1;
    } else {
        PartialCombinationList::iterator iter1 = list1.begin();
        while (iter1 != list1.end()) {
            CandidateCombination combi1 = *iter1;
            iter1 = list1.erase(iter1);
            for (CandidateCombination combi2 : list2) {
                CandidateCombination mergedCombi = Utils::MergeMap(combi1, combi2);
                if (!filterer(mergedCombi)) continue;
                list1.insert(iter1, mergedCombi);
            }
        }
        return list1;
    }
}

PartialCombinationList TotalCombinationList::makePartialCombiList(Synonym syn, vector<Candidate> &vec) {
    PartialCombinationList result;
    for (Candidate &cand : vec) {
        CandidateCombination combi{ { syn, cand } };
        result.push_back(combi);
    }
    return result;
}

string TotalCombinationList::toString() {
    string s("<");

    for (auto kv : factorList) {
        s.append("<");
        for (CandidateCombination comb : kv.second) {
            s.append(Utils::MapToString(comb));
            s.append(",");
        }
        s.append(">");
    }

    return s;
}

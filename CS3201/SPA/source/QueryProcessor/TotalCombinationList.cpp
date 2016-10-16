#include <set>
#include <vector>
#include "QueryProcessor/TotalCombinationList.h"
#include "Utils.h"

/* Constructor */
TotalCombinationList::TotalCombinationList() {
	empty = true;
}

/* Destructor */
TotalCombinationList::~TotalCombinationList() {}

/* Content Manipulation */

void TotalCombinationList::addSynonym(const Synonym &syn, std::vector<Candidate> &candidateList) {
	PartialCombinationList partList(makePartialCombiList(syn, candidateList));
	addSynonym(syn, partList);
}

void TotalCombinationList::addSynonym(const Synonym &syn, PartialCombinationList &partList) {
	content.insert_or_assign(syn, partList);
	factorList.insert(&partList);
	empty = partList.empty();
}

void TotalCombinationList::merge(Synonym &syn1, Synonym &syn2) {
	PartialCombinationList candidateList1 = content[syn1];
	PartialCombinationList candidateList2 = content[syn2];
	if (&candidateList1 == &candidateList2) {
		return;
	}
	else {
		PartialCombinationList &mergedList = cartesianProduct(candidateList1, candidateList2);
		for (auto kv : content) {
			if ((&content[kv.first] == &candidateList1) || (&content[kv.first] == &candidateList2)) {
				content.insert_or_assign(kv.first, mergedList);
			}
		}
		factorList.erase(&candidateList1);
		factorList.erase(&candidateList2);
		factorList.insert(&mergedList);
	}
}

void TotalCombinationList::combine(TotalCombinationList &combiList) {
	std::unordered_map<Synonym, PartialCombinationList> combiListContent(combiList.getContent());
	for (auto kv : combiListContent) {
		addSynonym(kv.first, kv.second);
	}
}

//template<typename Filterer>
void TotalCombinationList::filter(PartialCombinationList &candidateList, std::function<bool(CandidateCombination)> filterer) {
	PartialCombinationList::iterator iter = candidateList.begin();
	while (iter != candidateList.end()) {
		if (filterer(*iter)) {
			iter++;
		}
		else {
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

//template<typename Filterer>
void TotalCombinationList::filter(Synonym &syn, std::function<bool(CandidateCombination)> filterer) {
	PartialCombinationList &candidateList = content[syn];
	filter(candidateList, filterer);
}

//template<typename Filterer>
void TotalCombinationList::mergeAndFilter(Synonym &syn1, Synonym &syn2, std::function<bool(CandidateCombination)> filterer) {
	PartialCombinationList &candidateList1 = content[syn1];
	PartialCombinationList &candidateList2 = content[syn2];
	if (&candidateList1 == &candidateList2) {
		filter(syn1, filterer);
	}
	else {
		PartialCombinationList &mergedList = cartesianProduct(candidateList1, candidateList2, filterer);
		for (auto kv : content) {
			if ((&content[kv.first] == &candidateList1) || (&content[kv.first] == &candidateList2)) {
				content.insert_or_assign(kv.first, mergedList);
			}
		}
		factorList.erase(&candidateList1);
		factorList.erase(&candidateList2);
		factorList.insert(&mergedList);
		empty = mergedList.empty();
	}
}

/* Content accessors */

const std::unordered_map<Synonym, PartialCombinationList> TotalCombinationList::getContent() {
	return content;
}

const PartialCombinationList TotalCombinationList::operator[](Synonym &syn) {
	return content[syn];
}

bool TotalCombinationList::isEmpty() {
	return empty;
}

void TotalCombinationList::reduceSingleFactor(const std::vector<Synonym> &synList, PartialCombinationList &candidateList) {
	combinationComp comp(synList);
	std::set<CandidateCombination, combinationComp> resultSet(comp);
	for (CandidateCombination &combination : candidateList) {
		CandidateCombination selectedCombi(QueryUtils::GetSubMap(combination, synList));
		resultSet.insert(selectedCombi);
	}
	candidateList.clear();
	for (CandidateCombination comb : resultSet) {
		candidateList.push_back(comb);
	}
}

void TotalCombinationList::reduceTotalContent(const std::vector<Synonym> &synList) {
	for (PartialCombinationList* factorPointer : factorList) {
		PartialCombinationList &combiList = *factorPointer;
		std::vector<Synonym> subSynList;
		for (Synonym syn : synList) {
			if (&content[syn] == &combiList) {
				subSynList.push_back(syn);
			}
		}
		reduceSingleFactor(subSynList, combiList);
	}
}


PartialCombinationList TotalCombinationList::getSingleFactor(const std::vector<Synonym> &synList, PartialCombinationList &candidateList) {
	reduceSingleFactor(synList, candidateList);
	return candidateList;
}

PartialCombinationList TotalCombinationList::getCombinationList(const std::vector<Synonym> &synList) {
	reduceTotalContent(synList);
	PartialCombinationList result = *(*factorList.begin());
	for (PartialCombinationList* factorPointer : factorList) {
		PartialCombinationList factor = *factorPointer;
		result = cartesianProduct(result, factor);
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

//template<typename Filterer>
PartialCombinationList TotalCombinationList::cartesianProduct(PartialCombinationList &list1,
	PartialCombinationList &list2, std::function<bool(CandidateCombination)> filterer) {
	if (&list1 == &list2) {
		filter(list1, filterer);
		return list1;
	}
	else {
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

PartialCombinationList TotalCombinationList::makePartialCombiList(Synonym syn, std::vector<Candidate> &vec) {
	PartialCombinationList result;
	for (Candidate &cand : vec) {
		CandidateCombination combi{ { syn, cand } };
		result.push_back(combi);
	}
	return result;
}

std::string TotalCombinationList::toString() {
    for (auto kv : content) {
        std::string s("<");
        for (CandidateCombination comb : kv.second) {
            s.append(Utils::MapToString(comb));
            s.append(",");
        }
        s.append(">");
        return s;
    }
}

#pragma once
#include "QueryUtils.h"
#include "QueryTree.h"

typedef std::vector<std::string> ResultList;
typedef std::string Candidate;
typedef std::string Synonym;
typedef int Cand;
typedef std::unordered_map<Synonym, Candidate> CandidateCombination;
typedef std::list<CandidateCombination> PartialCombinationList;

class TotalCombinationList {

public:

	/* Constructor and destructor */

	TotalCombinationList();
	~TotalCombinationList();

	/* Content manipulation */

	void addSynonym(Synonym &syn, PartialCombinationList &candidateList);
	void merge(Synonym &syn1, Synonym &syn2);
	void filter(Synonym &syn, bool (*filterer)(CandidateCombination &combination));
	void mergeAndFilter(Synonym &syn1, Synonym &syn2, bool(*filterer)(CandidateCombination &combination));
	
	/* Content accessors */

	bool isEmpty();
	void reduceSingleFactor(std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	void reduceTotalContent(std::vector<Synonym> &synList);
	PartialCombinationList getSingleFactor(std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	PartialCombinationList getCombinationList(std::vector<Synonym> &synList);

private:
	
	/* Utillities on PartialCombinationList */
	void filter(PartialCombinationList &list, bool(*filterer)(CandidateCombination &combination));
	PartialCombinationList cartesianProduct(PartialCombinationList &list1, PartialCombinationList &list2);
	PartialCombinationList cartesianProduct(PartialCombinationList &list1,
		PartialCombinationList &list2, bool(*filterer)(CandidateCombination &combination));

	std::unordered_map<Synonym, PartialCombinationList> content;
	std::set<PartialCombinationList*> factorList;
	bool empty;
};
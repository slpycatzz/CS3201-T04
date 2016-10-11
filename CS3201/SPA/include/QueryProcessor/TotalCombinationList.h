#pragma once
#include "QueryUtils.h"
#include "QueryTree.h"

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

	void addSynonym(const Synonym &syn, PartialCombinationList &candidateList);
	void merge(Synonym &syn1, Synonym &syn2);

	void filter(bool expression);

	template<typename Filterer>
	void filter(Synonym &syn, Filterer filterer);

	template<typename Filterer>
	void mergeAndFilter(Synonym &syn1, Synonym &syn2, Filterer filterer);
	
	/* Content accessors */

	PartialCombinationList operator[](Synonym &syn);
	bool isEmpty();
	void reduceSingleFactor(std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	void reduceTotalContent(std::vector<Synonym> &synList);
	PartialCombinationList getSingleFactor(std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	PartialCombinationList getCombinationList(std::vector<Synonym> &synList);

private:
	
	/* Utillities on PartialCombinationList */
	template<typename Filterer>
	void filter(PartialCombinationList &list, Filterer filterer);

	PartialCombinationList cartesianProduct(PartialCombinationList &list1, PartialCombinationList &list2);

	template<typename Filterer>
	PartialCombinationList cartesianProduct(PartialCombinationList &list1, PartialCombinationList &list2, Filterer filterer);

	std::unordered_map<Synonym, PartialCombinationList> content;
	std::set<PartialCombinationList*> factorList;
	bool empty;
};
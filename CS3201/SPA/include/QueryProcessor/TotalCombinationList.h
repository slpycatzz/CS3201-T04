#pragma once
#include "QueryUtils.h"
#include "QueryTree.h"
#include <functional>

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

	void addSynonym(const Synonym &syn, std::vector<Candidate> &candidateList);
	
	void addSynonym(const Synonym &syn, PartialCombinationList &partList);
	
	void merge(Synonym &syn1, Synonym &syn2);

	void filter(bool expression);

	//template<typename Filterer>
	void filter(Synonym &syn, std::function<bool(CandidateCombination)> filterer);

	//template<typename Filterer>
	void mergeAndFilter(Synonym &syn1, Synonym &syn2, std::function<bool(CandidateCombination)> filterer);
	
	/* Content accessors */

	PartialCombinationList operator[](Synonym &syn);
	bool isEmpty();
	void reduceSingleFactor(const std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	void reduceTotalContent(const std::vector<Synonym> &synList);
	PartialCombinationList getSingleFactor(const std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	PartialCombinationList getCombinationList(const std::vector<Synonym> &synList);
	std::string toString();

	/* Comparator for CandidateCombination */
	struct combinationComp {
	public:
		combinationComp(const std::vector<Synonym> &synList) {
			this->synList = synList;
		}

		bool operator() (const CandidateCombination &comb1, const CandidateCombination &comb2) {
			for (Synonym syn: synList) {
				if (comb1.at(syn) < comb2.at(syn)) return true;
			}
			return false;
		}
	private:
		std::vector<Synonym> synList;
	};

private:
	
	/* Utillities on PartialCombinationList */
	//template<typename Filterer>
	void filter(PartialCombinationList &list, std::function<bool(CandidateCombination)> filterer);

	PartialCombinationList cartesianProduct(PartialCombinationList &list1, PartialCombinationList &list2);

	//template<typename Filterer>
	PartialCombinationList cartesianProduct(PartialCombinationList &list1,
		PartialCombinationList &list2, std::function<bool(CandidateCombination)> filterer);

	PartialCombinationList makePartialCombiList(Synonym syn, std::vector<Candidate> &vec);

	std::unordered_map<Synonym, PartialCombinationList> content;
	std::set<PartialCombinationList*> factorList;
	bool empty;
};
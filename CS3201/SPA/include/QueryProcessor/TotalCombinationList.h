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

	void addSynonym(Synonym &syn, std::vector<Candidate> &candidateList);

	void addSynonym(const char* &syn, std::vector<Candidate> &candidateList);

	void addSynonym(const char syn[], std::vector<Candidate> &candidateList);
	
	void addSynonym(Synonym &syn, PartialCombinationList &partList);

	void addSynonym(const char* &syn, PartialCombinationList &partList);

	void addSynonym(const char syn[], PartialCombinationList &partList);
	
	void merge(Synonym &syn1, Synonym &syn2);

	void filter(bool expression);

	void combine(TotalCombinationList &combiList);

	//template<typename Filterer>
	void filter(Synonym &syn, std::function<bool(CandidateCombination)> filterer);

	//template<typename Filterer>
	void mergeAndFilter(Synonym &syn1, Synonym &syn2, std::function<bool(CandidateCombination)> filterer);
	
	/* Content accessors */

	std::unordered_map<Synonym, PartialCombinationList>& getContent();
	std::set<PartialCombinationList*>& getFactorList();
	
	PartialCombinationList& operator[](Synonym &syn);
	PartialCombinationList& operator[](const char* &syn);
	PartialCombinationList& operator[](const char syn[]);
	
	bool isEmpty();
	
	void reduceSingleFactor(const std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	void reduceTotalContent(const std::vector<Synonym> &synList);
	
	PartialCombinationList& getSingleFactor(std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	PartialCombinationList& getCombinationList(std::vector<Synonym> &synList);
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

	PartialCombinationList cartesianProduct(PartialCombinationList &list1, PartialCombinationList &list2);

private:
	
	/* Utillities on PartialCombinationList */
	//template<typename Filterer>
	void filter(PartialCombinationList &list, std::function<bool(CandidateCombination)> filterer);

	/* TODO: Paste cartesianProduct back here after testing done */

	//template<typename Filterer>
	PartialCombinationList cartesianProduct(PartialCombinationList &list1,
		PartialCombinationList &list2, std::function<bool(CandidateCombination)> filterer);

	PartialCombinationList makePartialCombiList(Synonym syn, std::vector<Candidate> &vec);

	std::unordered_map<Synonym, PartialCombinationList> content;
	std::set<PartialCombinationList*> factorList;
	bool empty;
};
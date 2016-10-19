#pragma once
#include "QueryUtils.h"
#include "QueryTree.h"
#include <functional>
#include <map>

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

	void addSynonym(Synonym &syn, unsigned index);

	void addSynonym(const char* &syn, unsigned index);

	void addSynonym(const char syn[], unsigned index);

	void addSynonym(Synonym &syn, std::vector<Candidate> &candidateList);

	void addSynonym(const char* &syn, std::vector<Candidate> &candidateList);

	void addSynonym(const char syn[], std::vector<Candidate> &candidateList);
	
	void addSynonym(Synonym &syn, PartialCombinationList &partList);

	void addSynonym(const char* &syn, PartialCombinationList &partList);

	void addSynonym(const char syn[], PartialCombinationList &partList);
	
	void merge(Synonym &syn1, Synonym &syn2);

	void filter(bool expression);

	void combine(TotalCombinationList &combiList);

	void filter(Synonym &syn, std::function<bool(CandidateCombination)> filterer);

	void mergeAndFilter(Synonym &syn1, Synonym &syn2, std::function<bool(CandidateCombination)> filterer);
	
	/* Content accessors */

	std::unordered_map<Synonym, unsigned>& getContent();
	std::map<unsigned, PartialCombinationList>& getFactorList();

	bool contains(Synonym &syn);
	bool contains(const char* &syn);
	bool contains(const char syn[]);
	
	PartialCombinationList& operator[](Synonym &syn);
	PartialCombinationList& operator[](const char* &syn);
	PartialCombinationList& operator[](const char syn[]);
	
	bool isEmpty();
	
	void reduceSingleFactor(std::vector<Synonym> &synList, PartialCombinationList &candidateList);
	void reduceTotalContent(std::vector<Synonym> &synList);
	
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

	void filter(PartialCombinationList &list, std::function<bool(CandidateCombination)> filterer);

	PartialCombinationList cartesianProduct(PartialCombinationList &list1, PartialCombinationList &list2);

	PartialCombinationList cartesianProduct(PartialCombinationList &list1,
		PartialCombinationList &list2, std::function<bool(CandidateCombination)> filterer);

	PartialCombinationList makePartialCombiList(Synonym syn, std::vector<Candidate> &vec);

private:

	std::unordered_map<Synonym, unsigned> content;
	std::map<unsigned, PartialCombinationList> factorList;
	unsigned factorCounter;
	bool empty;
};
#include <string>
#include <vector>
#include <stdlib.h>

#include "stdafx.h"
#include "targetver.h"

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "PKB/PKB.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "Constants.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryOptimizer.h"
#include "QueryProcessor/QueryProjector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(QueryEvaluatorUnitTest) {
		
public:
	std::string PartialToString(PartialCombinationList &partialCombi) {
		std::string actual("<");
		PartialCombinationList::iterator it(partialCombi.begin());
		while (it != partialCombi.end()) {
			std::string str = Utils::MapToString(*it);
			actual.append(str);
			it++;
			if (it != partialCombi.end()) {
				actual.append(",");
			}
		}
		actual.append(">");
		return actual;
	}

	void parse(std::string filePath) {
		FrontendParser frontendParser = FrontendParser();

		try {
			frontendParser.parseProgram(filePath);

		}
		catch (std::exception& ex) {
			Logger::WriteMessage("Parse Error");
			Logger::WriteMessage(ex.what());
			exit(EXIT_FAILURE);
		}
	}

	void getSampleProgram() {
		std::string fileName = "..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt";
		PKB::Clear();
		parse(fileName);
	}

	QueryTree getQueryTree(std::string query) {
		QueryPreprocessor qp;
		qp.preprocessQuery(query);
		QueryTree qt(qp.getQueryTree());
		QueryOptimizer qo;
		return qo.optimize(qt);
	}

	std::vector<std::string> resultToString(ResultList &result) {
		std::vector<std::vector<std::string>> &list = result.second;
		std::vector<std::string> res;
		for (std::vector<std::string> combi : list) {
			res.push_back(Utils::VectorToString(combi));
		}
		return res;
	}

	TEST_METHOD(GetCandidatesTest) {
		getSampleProgram();
		QueryTree qt(getQueryTree("stmt a; Select a such that Modifies(a, \"a\")"));
		QueryEvaluator qe;

		TotalCombinationList total(qe.getTotalCandidateList(qt.getVarMap(), qt.getResults()));
		PartialCombinationList partial(total["a"]);
		std::string actual(PartialToString(partial));
		std::string expected;
		for (unsigned i : PKB::GetSymbolStmtNumbers(STMT)) {
			expected.append(std::to_string(i));
			expected.append(",");
		}
		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(GetBooleanGroupResultTest) {
		getSampleProgram();
		QueryTree qt(getQueryTree("stmt a; Select a such that Modifies(a, \"a\")"));
		QueryEvaluator qe;

		bool res(qe.getBooleanGroupResult(qt.getBooleanClauses()));
		Assert::IsTrue(res);
	}

	TEST_METHOD(GetUnselectedGroupResultTest) {
		getSampleProgram();
		QueryTree qt(getQueryTree("stmt a; Select a such that Modifies(a, \"a\")"));
		QueryEvaluator qe;

		bool res = true;
		for (auto &pair : qt.getUnselectedGroups()) {
			if (!qe.getUnselectedGroupResult(pair.first, qt.getVarMap(), pair.second)) {
				res = false;
				break;
			}
		}
		Assert::IsTrue(res);
	}

	TEST_METHOD(GetClausesTest) {
		getSampleProgram();
		std::string sampleQuery("stmt s1,s2; assign a1; Select s1 such that Modifies(a1, \"a\") and Uses(5, \"c\") and Parent(s1, s2)");
		QueryTree qt(getQueryTree(sampleQuery));
		QueryEvaluator qe;

		std::vector<Clause> clauses(qt.getClauses());
		Assert::IsTrue(clauses.size() == 3);
	}

	TEST_METHOD(GetBooleanClausestTest) {
		getSampleProgram();
		std::string sampleQuery("stmt a; assign a1; Select a such that Modifies(a, \"a\") and Uses(5, \"c\") and Modifies(a1, \"a\")");
		QueryTree qt(getQueryTree(sampleQuery));
		QueryEvaluator qe;

		std::vector<Clause> unselectedGroups(qt.getBooleanClauses());
		Assert::IsFalse(unselectedGroups.empty());
	}

	TEST_METHOD(GetSelectedGroupsTest) {

		PKB::Clear();
		getSampleProgram();
		std::string sampleQuery("stmt a; assign a1; Select a such that Modifies(a, \"a\") and Uses(5, \"c\") and Modifies(a1, \"a\")");
		QueryTree qt(getQueryTree(sampleQuery));
		QueryEvaluator qe;

		std::vector<std::pair<std::vector<Synonym>, std::vector<Clause>>> selectedGroups(qt.getSelectedGroups());
		Assert::IsFalse(selectedGroups.empty());
	}

	TEST_METHOD(GetUnSelectedGroupstTest) {
		getSampleProgram();
		std::string sampleQuery("stmt a; assign a1; Select a such that Modifies(a, \"a\") and Uses(5, \"c\") and Modifies(a1, \"a\")");
		QueryTree qt(getQueryTree(sampleQuery));
		QueryEvaluator qe;

		std::vector<std::pair<std::vector<Synonym>, std::vector<Clause>>> unselectedGroups(qt.getUnselectedGroups());
		Assert::IsFalse(unselectedGroups.empty());
	}

	TEST_METHOD(GetSelectedGroupResultTest) {
		getSampleProgram();
		QueryTree qt(getQueryTree("stmt a; Select a such that Modifies(a, \"a\")"));
		QueryEvaluator qe;
		
		std::vector<std::pair<std::vector<Synonym>, std::vector<Clause>>> selectedGroups(qt.getSelectedGroups());
		Assert::IsFalse(selectedGroups.empty());

		std::unordered_map<Synonym, Symbol> &varMap(qt.getVarMap());
		std::vector<Clause> &clauseGroup(selectedGroups.at(0).second);
		std::vector<Synonym> &selectList(qt.getResults());
		std::vector<Synonym> &synList(selectedGroups.at(0).first);

		TotalCombinationList total(qe.getSelectedGroupResult(synList, varMap, clauseGroup, selectList));
		PartialCombinationList partial(total["a"]);
		std::string actual(PartialToString(partial));
		std::string expected;
		for (unsigned i : PKB::GetSymbolStmtNumbers(STMT)) {
			expected.append(std::to_string(i));
			expected.append(",");
		}
		Assert::AreEqual(expected, actual);
	}

	};
}
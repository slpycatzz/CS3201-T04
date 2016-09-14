#include <string>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "Utils.h"
#include "QueryProcessor/QueryEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(QueryEvaluatorUnitTest) {
public:
	TEST_METHOD(TestMergeMap) {
		CandidateCombination comb1({ {"a", "1"} , {"b", "x"}, {"c", "4"} });
		CandidateCombination comb2({ {"d", "y"} , {"e", "6"} });
		CandidateCombination comb(Utils::MergeMap(comb1, comb2));
		std::string expected;
		for (auto kv : comb) {
			expected.append("<");
			expected.append(kv.first + ",");
			expected.append(kv.second + ">");
			expected.append(" ");
		}
		std::string actual("<a,1> <b,x> <c,4> <d,y> <e,6> ");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestVectorToString) {
		std::vector<std::string> vt;
		vt.push_back("1");
		vt.push_back("2");
		vt.push_back("3");
		std::string actual(Utils::VectorToString(vt));
		std::string expected("<1,2,3>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestVectorToString2) {
		std::vector<std::vector<std::string>> vt;
		vt.push_back(std::vector<std::string>({ "1", "2", "3" }));
		vt.push_back(std::vector<std::string>({ "a", "b", "c" }));
		std::string actual(Utils::VectorToString(Utils::VectorToString(vt)));
		std::string expected("<<1,2,3>,<a,b,c>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestFlatten) {
		std::unordered_map<VarName, std::vector<Candidate>> map;
		map.insert_or_assign("a", std::vector<Candidate>({ "1", "2" }));
		map.insert_or_assign("b", std::vector<Candidate>({ "3", "4", "5" }));
		std::vector<std::vector<std::string>>
			result(Utils::Flatten(map, std::vector<VarName>({ "a", "b" }), 0, 1));
		std::string actual(Utils::VectorToString(Utils::VectorToString(result)));
		std::string expected("<<1,3>,<1,4>,<1,5>,<2,3>,<2,4>,<2,5>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestGetSelectMap) {
		std::unordered_map<std::string, std::string> candidateCombi1({ { "a", "1" },{ "b", "2" } }); 
		std::unordered_map<std::string, std::string> candidateCombi2({ { "a", "2" }, { "b", "3" } });
		std::vector<std::unordered_map<std::string, std::string>> partialCombi{ candidateCombi1, candidateCombi2 };
		std::unordered_map<std::string, std::vector<std::unordered_map<std::string, std::string>>> totalCombi;
		totalCombi.insert_or_assign("a", partialCombi);
		totalCombi.insert_or_assign("b", partialCombi);
		totalCombi.insert_or_assign("c", partialCombi);
		std::unordered_map<std::string, Symbol> selectList{ { "a", ASSIGN },{ "b", ASSIGN } };

		QueryEvaluator qe = QueryEvaluator();
		std::vector<std::string> vt;
		std::unordered_map<VarName, std::vector<Candidate>>
			selectMap(qe.getSelectMap(selectList, totalCombi));
		/**
		for (auto kv : selectMap) {
			std::string msg(kv.first + " : " + Utils::VectorToString(kv.second));
			vt.push_back(msg);
		}
		std::string expected("<a : <1,2>,b : <2,3>>");
		Assert::AreEqual(expected, Utils::VectorToString(vt));
		**/

		std::vector<std::string> varList;
		for (auto kv : selectMap) {
			varList.push_back(kv.first);
			Logger::WriteMessage(kv.first.c_str());
		}
		std::vector<std::vector<Candidate>>
			result = Utils::Flatten(selectMap, varList, 0, varList.size() - 1);
		std::string actual = Utils::VectorToString(Utils::VectorToString(result));
		Logger::WriteMessage(actual.c_str());
		std::string expected("<<1,2>,<1,3>,<2,2>,<2,3>>");
		Assert::AreEqual(expected, actual);
	}
    TEST_METHOD(TestGetResultsFromCombinationList) {
		std::unordered_map<std::string, std::string> candidateCombi1({ { "a", "1" },{ "b", "2" } });
		std::unordered_map<std::string, std::string> candidateCombi2({ { "a", "2" },{ "b", "3" } });
		std::vector<std::unordered_map<std::string, std::string>> partialCombi{ candidateCombi1, candidateCombi2 };
		std::unordered_map<std::string, std::vector<std::unordered_map<std::string, std::string>>> totalCombi;
        totalCombi.insert_or_assign("a", partialCombi);
        totalCombi.insert_or_assign("b", partialCombi);
        totalCombi.insert_or_assign("c", partialCombi);
        std::unordered_map<std::string, Symbol> selectList{ {"a", ASSIGN},{ "b", ASSIGN } };

        QueryEvaluator qe = QueryEvaluator();
        std::vector<std::string> result(qe.getResultsFromCombinationList(totalCombi, selectList));
        std::string actual = Utils::VectorToString(result);
        Logger::WriteMessage(actual.c_str());
		std::string expected("<<1,2>,<1,3>,<2,2>,<2,3>>");
		Assert::AreEqual(expected, actual);
    }
	};
}
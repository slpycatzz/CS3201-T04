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
	TEST_METHOD(TestFlatten) {
		QueryEvaluator qe;
		TotalCombinationList combList;
		PartialCombinationList partList1;
		partList1.push_back({ {"a", "1"}, {"b", "2"} });
		partList1.push_back({ {"a", "2"}, {"b", "3"} });
		partList1.push_back({ {"a", "3"}, {"b", "1"} });
		combList.insert_or_assign("a", partList1);
		combList.insert_or_assign("b", partList1);
		PartialCombinationList partList2;
		partList2.push_back({ {"c", "1"} });
		partList2.push_back({ {"c", "2"} });
		combList.insert_or_assign("c", partList2);
		std::unordered_map<VarName, Symbol> list({ {"a", ASSIGN}, {"b", ASSIGN}, {"c", ASSIGN} });
		std::vector<std::string> result(qe.getResultsFromCombinationList(combList, list));
		std::string actual(Utils::VectorToString(result));
		std::string expected("<<1,2,1>,<1,2,2>,<2,3,1>,<2,3,2>,<3,1,1>,<3,1,2>>");
		Assert::AreEqual(expected, actual);
	}
	};
}
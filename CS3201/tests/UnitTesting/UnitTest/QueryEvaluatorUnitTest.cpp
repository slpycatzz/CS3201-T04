#include <string>
#include <vector>
#include <sstream>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "Utils.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryUtils.h"

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
		std::string actual(Utils::VectorToString(Utils::VectorToStrings(vt)));
		std::string expected("<<1,2,3>,<a,b,c>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestFlatten) {
		std::unordered_map<Synonym, std::vector<Candidate>> map;
		map.insert_or_assign("a", std::vector<Candidate>({ "1", "2" }));
		map.insert_or_assign("b", std::vector<Candidate>({ "3", "4", "5" }));
		std::vector<std::vector<std::string>>
			result(Utils::Flatten(map, std::vector<Synonym>({ "a", "b" }), 0, 1));
		std::string actual(Utils::VectorToString(Utils::VectorToStrings(result)));
		std::string expected("<<1,3>,<1,4>,<1,5>,<2,3>,<2,4>,<2,5>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestGetSelectCombinations) {
		
		CandidateCombination candidateCombi1({ { "a", "1" },{ "b", "2" } }); 
		CandidateCombination candidateCombi2({ { "a", "2" }, { "b", "3" } });
		PartialCombinationList partialCombi1{ candidateCombi1, candidateCombi2 };
		
		CandidateCombination candidateCombi3({ {"c", "4"} });
		CandidateCombination candidateCombi4({ {"c", "5"} });
		PartialCombinationList partialCombi2{ candidateCombi3, candidateCombi4 };

		TotalCombinationList totalCombi;

		totalCombi.insert_or_assign("a", partialCombi1);
		totalCombi.insert_or_assign("b", partialCombi1);
		totalCombi.insert_or_assign("c", partialCombi2);
		std::vector<std::string> selectList{ "a", "b" , "c" };

		QueryEvaluator qe = QueryEvaluator();
		std::stringstream actual;
		actual << "<";
		PartialCombinationList
			selectedCombs(qe.getSelectedCombinations(totalCombi, selectList));
		PartialCombinationList::iterator it(selectedCombs.begin());
		while (true) {
			std::string str = Utils::MapToString(*it);
			actual << str;
			it++;
			if (it == selectedCombs.end()) {
				actual << ">";
				break;
			}
			else {
				actual << ",";
			}
		}
		
		std::string expected("<<a:1,b:2,c:4>,<a:1,b:2,c:5>,<a:2,b:3,c:4>,<a:2,b:3,c:5>>");
		Assert::AreEqual(expected, actual.str());
	}
    TEST_METHOD(TestGetResultsFromCombinationList) {
		std::unordered_map<std::string, std::string> candidateCombi1({ { "a", "1" },{ "b", "2" } });
		std::unordered_map<std::string, std::string> candidateCombi2({ { "a", "2" },{ "b", "3" } });
		std::vector<std::unordered_map<std::string, std::string>> partialCombi{ candidateCombi1, candidateCombi2 };
		std::unordered_map<std::string, std::vector<std::unordered_map<std::string, std::string>>> totalCombi;
        totalCombi.insert_or_assign("a", partialCombi);
        totalCombi.insert_or_assign("b", partialCombi);
        totalCombi.insert_or_assign("c", partialCombi);
        std::vector<std::string> selectList{ "a", "b" };

        QueryEvaluator qe = QueryEvaluator();
        std::vector<std::string> result(qe.getResultsFromCombinationList(totalCombi, selectList));
        std::string actual = Utils::VectorToString(result);
        Logger::WriteMessage(actual.c_str());
		std::string expected("<<1,2>,<2,3>>");
		Assert::AreEqual(expected, actual);
    }
	TEST_METHOD(TestIsLiteral) {
		std::string s("\"x\"");
		Assert::IsTrue(s[0] == '\"');
	}
	TEST_METHOD(TestLiteralToCandidate) {
		std::string s("\"x\"");
		Assert::AreEqual(QueryUtils::LiteralToCandidate(s), std::string("x"));
	}
	TEST_METHOD(TestIsSameTreeNode) {
		TreeNode root1(TreeNode(ASSIGN, "a"));
		TreeNode root2(TreeNode(ASSIGN, "a"));
		Assert::IsTrue(Utils::IsSameTreeNode(root1, root2));
	}
	TEST_METHOD(TestIsSameTree) {
		TreeNode root1(TreeNode(ASSIGN, "a"));
		TreeNode root2(TreeNode(ASSIGN, "a"));

		root1.addChild(&TreeNode(ASSIGN, "b"));
		root2.addChild(&TreeNode(ASSIGN, "b"));

		root1.addChild(&TreeNode(ASSIGN, "c"));
		root2.addChild(&TreeNode(ASSIGN, "c"));

		Assert::IsTrue(Utils::IsSameTree(root1, root2));
	}
	TEST_METHOD(TestBuildExprTree) {
		TreeNode* root(QueryUtils::BuildExpressionTree("_\"x\"_"));
		Assert::AreEqual(std::string("x"), root->getValue());
	}
	};
}
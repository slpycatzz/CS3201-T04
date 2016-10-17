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

namespace UnitTesting {
	TEST_CLASS(QueryEvaluatorUnitTest) {
public:
	std::string PartialToString(PartialCombinationList &partialCombi) {
		std::string actual("<");
		PartialCombinationList::iterator it(partialCombi.begin());
		while (true) {
			std::string str = Utils::MapToString(*it);
			actual.append(str);
			it++;
			if (it == partialCombi.end()) {
				actual.append(">");
				break;
			}
			else {
				actual.append(",");
			}
		}
		return actual;
	}
	TEST_METHOD(MergeMapTest) {
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
	TEST_METHOD(VectorToStringTest) {
		std::vector<std::string> vt;
		vt.push_back("1");
		vt.push_back("2");
		vt.push_back("3");
		std::string actual(Utils::VectorToString(vt));
		std::string expected("<1,2,3>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(VectorToString2Test) {
		std::vector<std::vector<std::string>> vt;
		vt.push_back(std::vector<std::string>({ "1", "2", "3" }));
		vt.push_back(std::vector<std::string>({ "a", "b", "c" }));
		std::string actual(Utils::VectorToString(Utils::VectorToStrings(vt)));
		std::string expected("<<1,2,3>,<a,b,c>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(FlattenTest) {
		std::unordered_map<Synonym, std::vector<Candidate>> map;
		map.insert_or_assign("a", std::vector<Candidate>({ "1", "2" }));
		map.insert_or_assign("b", std::vector<Candidate>({ "3", "4", "5" }));
		std::vector<std::vector<std::string>>
			result(Utils::Flatten(map, std::vector<Synonym>({ "a", "b" }), 0, 1));
		std::string actual(Utils::VectorToString(Utils::VectorToStrings(result)));
		std::string expected("<<1,3>,<1,4>,<1,5>,<2,3>,<2,4>,<2,5>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(IsLiteralTest) {
		std::string s("\"x\"");
		Assert::IsTrue(s[0] == '\"');
	}
	TEST_METHOD(LiteralToCandidateTest) {
		std::string s("\"x\"");
		Assert::AreEqual(QueryUtils::LiteralToCandidate(s), std::string("x"));
	}
	TEST_METHOD(IsSameTreeNodeTest) {
		TreeNode root1(TreeNode(ASSIGN, "a"));
		TreeNode root2(TreeNode(ASSIGN, "a"));
		Assert::IsTrue(Utils::IsSameTreeNode(root1, root2));
	}
	TEST_METHOD(IsSameTreeTest) {
		TreeNode root1(TreeNode(ASSIGN, "a"));
		TreeNode root2(TreeNode(ASSIGN, "a"));

		root1.addChild(&TreeNode(ASSIGN, "b"));
		root2.addChild(&TreeNode(ASSIGN, "b"));

		root1.addChild(&TreeNode(ASSIGN, "c"));
		root2.addChild(&TreeNode(ASSIGN, "c"));

		Assert::IsTrue(Utils::IsSameTree(root1, root2));
	}
	TEST_METHOD(BuildExprTreeTest) {
		TreeNode* root(QueryUtils::BuildExpressionTree("_\"x\"_"));
		Assert::AreEqual(std::string("x"), root->getValue());
	}

	TEST_METHOD(GetSubMapTest) {
		CandidateCombination combi{ {"a", "1"}, {"b", "2"}, {"c", "3"} };
		std::vector<Synonym> keyList{ "a", "b" };
		CandidateCombination subCombi(QueryUtils::GetSubMap(combi, keyList));
		std::string actual(Utils::MapToString(subCombi));
		std::string expected("<a:1,b:2>");
		Assert::AreEqual(expected, actual);
	}
	};
}
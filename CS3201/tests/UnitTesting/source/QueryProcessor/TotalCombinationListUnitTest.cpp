#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryUtils.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::map;
using std::stringstream;
using std::string;
using std::unordered_map;
using std::vector;

namespace UnitTesting {
    TEST_CLASS(TotalCombinationListUnitTest) {
        string PartialToString(PartialCombinationList &partialCombi) {
            string actual("<");
            PartialCombinationList::iterator it(partialCombi.begin());
            while (it != partialCombi.end()) {
                string str = Utils::MapToString(*it);
                actual.append(str);
                it++;
                if (it != partialCombi.end()) {
                    actual.append(",");
                }
            }
            actual.append(">");
            return actual;
        }

        TotalCombinationList getSampleList_1() {
            TotalCombinationList totalCombi;
            totalCombi.addSynonym("a", vector<Candidate>{ "1" });
            totalCombi.addSynonym("b", vector<Candidate>{"2", "3"});
            totalCombi.addSynonym("c", vector<Candidate>{"4", "5"});
            return totalCombi;
        }

        TotalCombinationList getSampleList_2() {
            CandidateCombination candidateCombi1({ { "a", "1" }, { "b", "2" } });
            CandidateCombination candidateCombi2({ { "a", "1" }, { "b", "3" } });
            PartialCombinationList partialCombi1{ candidateCombi1, candidateCombi2 };

            CandidateCombination candidateCombi3({ { "c", "4" } });
            CandidateCombination candidateCombi4({ { "c", "5" } });
            PartialCombinationList partialCombi2{ candidateCombi3, candidateCombi4 };

            TotalCombinationList totalCombi;

            totalCombi.addSynonym("a", partialCombi1);
            totalCombi.addSynonym("b", 0);
            totalCombi.addSynonym("c", partialCombi2);

            return totalCombi;
        }
     public:
        TEST_METHOD(TotalCombinationList_ReduceSingleFactorTest) {
            TotalCombinationList totalCombi(getSampleList_2());
            vector<Synonym> synList{ "a" };
            totalCombi.reduceSingleFactor(synList, totalCombi["a"]);

            string actual(PartialToString(totalCombi["a"]));
            string expected("<<a:1>>");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TotalCombinationList_CartesianProductTest) {
            TotalCombinationList totalCombi(getSampleList_2());
            PartialCombinationList product(totalCombi.cartesianProduct(totalCombi["a"], totalCombi["c"]));

            string actual(PartialToString(product));
            string expected("<<a:1,b:2,c:4>,<a:1,b:2,c:5>,<a:1,b:3,c:4>,<a:1,b:3,c:5>>");
            Assert::AreEqual(expected, actual);
        }

		TEST_METHOD(TotalCombinationList_MergeTest_1) {
			TotalCombinationList totalCombi(getSampleList_2());
			totalCombi.merge(string("a"), string("c"));
			Assert::IsTrue(totalCombi.getFactorList().size() == 1);
			PartialCombinationList product(totalCombi["a"]);

			string actual(PartialToString(product));
			string expected("<<a:1,b:2,c:4>,<a:1,b:2,c:5>,<a:1,b:3,c:4>,<a:1,b:3,c:5>>");
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TotalCombinationList_CartesianProductFilterTest) {
			TotalCombinationList totalCombi(getSampleList_2());

			auto f = [](CandidateCombination combi)->bool {
				return (combi["a"] == "1" && combi["c"] == "5");
			};

			PartialCombinationList product(totalCombi.cartesianProduct(totalCombi["a"], totalCombi["c"], f));

			string actual(PartialToString(product));
			string expected("<<a:1,b:2,c:5>,<a:1,b:3,c:5>>");
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TotalCombinationList_MergeFilterTest) {
			TotalCombinationList totalCombi(getSampleList_2());

			auto f = [](CandidateCombination combi)->bool {
				return (combi["a"] == "1" && combi["c"] == "5");
			};

			totalCombi.mergeAndFilter(string("a"), string("c"), f);
			Assert::IsTrue(totalCombi.getFactorList().size() == 1);
			PartialCombinationList product(totalCombi["a"]);

			string actual(PartialToString(product));
			string expected("<<a:1,b:2,c:5>,<a:1,b:3,c:5>>");
			Assert::AreEqual(expected, actual);
		}

        TEST_METHOD(TotalCombinationList_GetSelectCombinationsTest_One) {
            TotalCombinationList totalCombi(getSampleList_2());
            vector<string> selectList{ "a", "b" , "c" };

            PartialCombinationList &selectedCombs(totalCombi.getCombinationList(selectList));

            string actual(PartialToString(selectedCombs));

            string expected("<<a:1,b:2,c:4>,<a:1,b:2,c:5>,<a:1,b:3,c:4>,<a:1,b:3,c:5>>");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TotalCombinationList_GetSelectCombinationsTest_Two) {
            TotalCombinationList totalCombi(getSampleList_2());
            vector<string> selectList{ "a", "c" };

            PartialCombinationList &selectedCombs(totalCombi.getCombinationList(selectList));

            string actual(PartialToString(selectedCombs));

            string expected("<<a:1,c:4>,<a:1,c:5>>");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TotalCombinationList_GetContentTest) {
            TotalCombinationList totalCombi(getSampleList_2());
            unordered_map<Synonym, unsigned> content(totalCombi.getContent());

            stringstream ss;
            ss << "<";
            unordered_map<Synonym, unsigned>::iterator it(content.begin());
            while (it != content.end()) {
                ss << (*it).first << ":" << (*it).second;
                it++;
                if (it != content.end()) ss << ",";
            }
            ss << ">";

            string actual(ss.str());
            string expected("<a:0,b:0,c:1>");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TotalCombinationList_OperatorLookupTest) {
            TotalCombinationList totalCombi(getSampleList_2());
            PartialCombinationList factor(totalCombi["a"]);
            string actual(PartialToString(factor));
            string expected("<<a:1,b:2>,<a:1,b:3>>");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TotalCombinationList_GetFactorListTest) {
            TotalCombinationList totalCombi(getSampleList_2());
            vector<string> selectList{ "a", "b" };

            map<unsigned, PartialCombinationList> factorList(totalCombi.getFactorList());

            string actual("{");
            for (auto kv : factorList) {
                actual.append(PartialToString(kv.second));
                actual.append(";");
            }
            actual.append("}");
            string expected("{<<a:1,b:2>,<a:1,b:3>>;<<c:4>,<c:5>>;}");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TotalCombinationList_ReduceTotalContentTest) {
            TotalCombinationList totalCombi(getSampleList_2());
            vector<string> selectList{ "a", "b" };

            totalCombi.reduceTotalContent(selectList);
            PartialCombinationList result(totalCombi["a"]);
            // result = totalCombi.cartesianProduct(result, result);
            // PartialCombinationList result(totalCombi.getCombinationList(selectList));
            // totalCombi.reduceSingleFactor(selectList, result);

            string actual(PartialToString(result));
            string expected("<<a:1,b:2>,<a:1,b:3>>");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(TotalCombinationList_MergeTest_2) {
            TotalCombinationList totalCombi(getSampleList_1());
            string actual;

            map<unsigned, PartialCombinationList>& factorList(totalCombi.getFactorList());
            unordered_map<Synonym, unsigned>& content(totalCombi.getContent());
            Synonym syn1("a"); unsigned index1(content[syn1]);
            Synonym syn2("b"); unsigned index2(content[syn2]);

            totalCombi.merge(syn1, syn2);

            actual = PartialToString(totalCombi[syn1]);
            string expected("<<a:1,b:2>,<a:1,b:3>>");
            Assert::AreEqual(expected, actual);
        }
    };
}

#include <string>
#include <unordered_map>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "QueryProcessor/QueryEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::string;
using std::unordered_map;
using std::vector;

namespace UnitTesting {
    TEST_CLASS(UtilsUnitTest) {
     public:
         TEST_METHOD(Utils_FlattenTest) {
             unordered_map<Synonym, vector<Candidate>> map;
             map.insert_or_assign("a", vector<Candidate>({ "1", "2" }));
             map.insert_or_assign("b", vector<Candidate>({ "3", "4", "5" }));
             vector<vector<string>>
                 result(Utils::Flatten(map, vector<Synonym>({ "a", "b" }), 0, 1));
             string actual(Utils::VectorToString(Utils::VectorToStrings(result)));
             string expected("<<1,3>,<1,4>,<1,5>,<2,3>,<2,4>,<2,5>>");
             Assert::AreEqual(expected, actual);
         }

        TEST_METHOD(Utils_MergeMapTest) {
            CandidateCombination comb1({ { "a", "1" } , { "b", "x" }, { "c", "4" } });
            CandidateCombination comb2({ { "d", "y" } , { "e", "6" } });
            CandidateCombination comb(Utils::MergeMap(comb1, comb2));
            string expected;
            for (auto kv : comb) {
                expected.append("<");
                expected.append(kv.first + ",");
                expected.append(kv.second + ">");
                expected.append(" ");
            }
            string actual("<a,1> <b,x> <c,4> <d,y> <e,6> ");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(Utils_VectorToStringTest) {
            vector<string> vt;
            vt.push_back("1");
            vt.push_back("2");
            vt.push_back("3");
            string actual(Utils::VectorToString(vt));
            string expected("<1,2,3>");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(Utils_VectorToStringTestTwo) {
            vector<vector<string>> vt;
            vt.push_back(vector<string>({ "1", "2", "3" }));
            vt.push_back(vector<string>({ "a", "b", "c" }));
            string actual(Utils::VectorToString(Utils::VectorToStrings(vt)));
            string expected("<<1,2,3>,<a,b,c>>");
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryUtils_GetSubMapTest) {
            CandidateCombination combi{ { "a", "1" }, { "b", "2" }, { "c", "3" } };
            vector<Synonym> keyList{ "a", "b" };
            CandidateCombination subCombi(Utils::GetSubMap(combi, keyList));
            string actual(Utils::MapToString(subCombi));
            string expected("<a:1,b:2>");
            Assert::AreEqual(expected, actual);
        }
    };
}

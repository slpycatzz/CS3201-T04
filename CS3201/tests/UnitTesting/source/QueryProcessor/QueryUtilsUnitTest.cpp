#include <string>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryUtils.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::string;

namespace UnitTesting {
    TEST_CLASS(QueryUtilsUnitTest) {
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

     public:
        TEST_METHOD(QueryUtils_IsLiteralTest) {
            string s("\"x\"");
            Assert::IsTrue(s[0] == '\"');
        }

        TEST_METHOD(QueryUtils_LiteralToCandidateTest) {
            string s("\"x\"");
            Assert::AreEqual(QueryUtils::LiteralToCandidate(s), string("x"));
        }
    };
}

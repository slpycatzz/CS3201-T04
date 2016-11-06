#include <list>
#include <string>
#include <vector>
#include <unordered_map>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "QueryProcessor/QueryProjector.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::list;
using std::string;
using std::unordered_map;

namespace UnitTesting {
    TEST_CLASS(QueryProjectorUnitTest) {
     public:
        TEST_METHOD(QueryProjector_ResultListTuple) {
            QueryProjector projector;
            string expected, actual;

            list<string> results;
            ResultList resultList({ { "a", "s", "c" }, { { "1", "2", "3" }, { "4", "5", "6" } } });

            unordered_map<string, bool> varAttrMap;

            projector.projectResult(results, varAttrMap, resultList);

            // expected = "1 2 3, 4 5 6";
            //
            // for (string s : results) {
            //     actual += s + ", ";
            // }
            // actual = actual.substr(0, actual.length() - 2);

            // Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryProjector_ResultListBooleanTrue) {
            QueryProjector projector;
            string expected, actual;

            list<string> results;
            ResultList resultList({ { "BOOLEAN" }, { { SYMBOL_TRUE } } });

            unordered_map<string, bool> varAttrMap;

            projector.projectResult(results, varAttrMap, resultList);

            expected = SYMBOL_TRUE;

            for (string s : results) {
                actual += s + ", ";
            }
            actual = actual.substr(0, actual.length() - 2);

            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryProjector_ResultListBooleanFalse) {
            QueryProjector projector;
            string expected, actual;

            list<string> results;
            ResultList resultList({ { "BOOLEAN" }, { { SYMBOL_FALSE } } });

            unordered_map<string, bool> varAttrMap;

            projector.projectResult(results, varAttrMap, resultList);

            expected = SYMBOL_FALSE;

            for (string s : results) {
                actual += s + ", ";
            }
            actual = actual.substr(0, actual.length() - 2);

            Assert::AreEqual(expected, actual);
        }
    };
}

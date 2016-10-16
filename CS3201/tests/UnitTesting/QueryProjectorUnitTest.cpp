#include <string>
#include <vector>
#include <unordered_map>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "QueryProcessor/QueryProjector.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(QueryProjectorTest) {
public:
    TEST_METHOD(QueryProjectorResultListTupleOne) {
        QueryProjector projector;
        std::string expected, actual;

        std::list<std::string> results;
        ResultList resultList({ { "a", "s", "c" },{ { "1","2","3" },{ "4","5","6" } } });

        std::unordered_map<std::string, bool> varAttrMap;

        projector.projectResult(results, varAttrMap, resultList);
        
        expected = "1 2 3, 4 5 6";
        
        for (std::string s : results) {
            actual += s + ", ";
        }
        actual = actual.substr(0, actual.length() - 2);

        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryProjectorResultListBooleanTrue) {
        QueryProjector projector;
        std::string expected, actual;

        std::list<std::string> results;
        ResultList resultList({ { "BOOLEAN" },{ { SYMBOL_TRUE } } });

        std::unordered_map<std::string, bool> varAttrMap;

        projector.projectResult(results, varAttrMap, resultList);

        expected = "TRUE";

        for (std::string s : results) {
            actual += s + ", ";
        }
        actual = actual.substr(0, actual.length() - 2);

        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryProjectorResultListBooleanFalse) {
        QueryProjector projector;
        std::string expected, actual;

        std::list<std::string> results;
        ResultList resultList({ { "BOOLEAN" },{ { SYMBOL_FALSE } } });

        std::unordered_map<std::string, bool> varAttrMap;

        projector.projectResult(results, varAttrMap, resultList);

        expected = "FALSE";

        for (std::string s : results) {
            actual += s + ", ";
        }
        actual = actual.substr(0, actual.length() - 2);

        Assert::AreEqual(expected, actual);
    }
    };
}   // namespace UnitTest

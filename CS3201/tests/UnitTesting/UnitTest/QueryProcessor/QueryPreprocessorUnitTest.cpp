#include <string>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/RelationTable.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
    TEST_CLASS(QueryPreprocessorTest) {
public:
    TEST_METHOD(QueryParserSelect) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        qp.preprocessQuery("assign a,a1; while w1,w2;select a");
        qt = qp.getQueryTree();
        std::vector<std::string> varList;

        varList = qt.getResults();
        expected = "a ";
        for (unsigned int i = 0; i < varList.size(); i++) {
            actual += varList[i] + " ";
        }
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryParserSuchThat) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        std::string query;
        query = "assign a,a1; while w1,w2; variable x;";
        query += "select a such that uses(a1, x)";
        qp.preprocessQuery(query);
        qt = qp.getQueryTree();
        std::vector<Clause> resList;

        resList = qt.getSuchThat();
        expected = "uses a1 x ";
        for (unsigned int i = 0; i < resList.size(); i++) {
            actual += resList[i].getClauseType() + " ";
            for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                actual += resList[i].getArg()[j] + " ";
            }
        }
        // actual = qp.testMethodOut();
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryParserPattern) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        std::string query;
        query = "assign a,a1; while w1,w2;";
        query += "select a pattern a1(\"x\",_)";
        qp.preprocessQuery(query);

        qt = qp.getQueryTree();
        std::vector<Clause> resList;

        resList = qt.getPattern();
        expected = "pattern \"x\" _ a1 ";
        for (unsigned int i = 0; i < resList.size(); i++) {
            actual += resList[i].getClauseType() + " ";
            for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                actual += resList[i].getArg()[j] + " ";
            }
        }
        // actual = qp.testMethodOut();
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryParserInvalidQuery) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        std::string query;
        query = "assign a,a1; while w1,w2; variable x;";
        query += "select a such taht uses(a1, x)";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), "Query parser encountered a syntax error in the query.");
        }
        // actual = qp.testMethodOut();

    }
    };
}   // namespace UnitTest

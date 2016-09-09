#include <string>
#include <vector>

#include "CppUnitTest.h"
#include "stdafx.h"

#include "../../../SPA/include/Constants.h"
#include "../../../SPA/include/QueryProcessor/QueryPreprocessor.h"
#include "../../../SPA/include/QueryProcessor/RelationTable.h"
#include "../../../SPA/include/Utils.h"

// using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
    TEST_CLASS(QueryPreprocessorTest) {
    public:
        TEST_METHOD(QueryParserSelectTestMethod) {
            std::string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            bool isSuccess = qp.processDeclaration("assign a,a1; while w1,w2;");
            isSuccess = qp.processQuery("select a,w1");
            qt = qp.getQueryTree();
            std::vector<std::string> varList;

            varList = qt.getResults();
            expected = "a w1 ";
            for (unsigned int i = 0; i < varList.size(); i++) {
                actual += varList[i] + " ";
            }
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryParserSuchThatTestMethod) {
            std::string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            bool isSuccess = qp.processDeclaration("assign a,a1; while w1,w2;");
            isSuccess = qp.processQuery("select a,w1 such that uses(a1,w1)");
            qt = qp.getQueryTree();
            std::vector<Clause> resList;

            resList = qt.getSuchThat();
            expected = "uses a1 w1 ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j]+ " ";
                }
            }
            // actual = qp.testMethodOut();
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(expected, actual);
        }
    };
}   // namespace UnitTest

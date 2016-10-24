#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "PKB/PKB.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryOptimizer.h"
#include "QueryProcessor/QueryProjector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::string;
using std::unordered_map;
using std::vector;

namespace UnitTesting {
    TEST_CLASS(QueryEvaluatorUnitTest)  {
        FrontendParser frontendParser;
        string filePath = "..\\tests\\UnitTesting\\testcases\\QueryProcessor\\QueryEvaluatorUnitTest.txt";

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

        QueryTree getQueryTree(string query) {
            QueryPreprocessor qp;
            qp.preprocessQuery(query);
            QueryTree qt(qp.getQueryTree());
            QueryOptimizer qo;
            return qo.optimize(qt);
        }

        vector<string> resultToString(ResultList &result) {
            vector<vector<string>> &list = result.second;
            vector<string> res;
            for (vector<string> combi : list) {
                res.push_back(Utils::VectorToString(combi));
            }
            return res;
        }

     public:
        TEST_METHOD_INITIALIZE(Initialize) {
            PKB::Clear();
            frontendParser = FrontendParser();

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception& ex) {
                Logger::WriteMessage("Frontend Parser Error");
                Logger::WriteMessage(ex.what());

                exit(EXIT_FAILURE);
            }
        }

        TEST_METHOD(QueryEvaluator_GetCandidatesTest) {
            QueryTree qt(getQueryTree("assign a; while w; Select a such that Follows(a, w)"));
            QueryEvaluator qe;

            TotalCombinationList total(qe.getTotalCandidateList(qt.getVarMap(), qt.getResults()));
            PartialCombinationList partial(total["a"]);
            string actual(PartialToString(partial));
            string expected;

            expected.append("<");
            for (unsigned i : PKB::GetSymbolStmtNumbers(ASSIGN)) {
                expected.append("<a:");
                expected.append(std::to_string(i));
                expected.append(">");
                expected.append(",");
            }
            expected = expected.substr(0, expected.length() - 1);
            expected.append(">");

            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryEvaluator_GetBooleanGroupResultTest) {
            QueryTree qt(getQueryTree("assign a; while w; Select a such that Follows(5, 6)"));
            QueryEvaluator qe;

            bool res(qe.getBooleanGroupResult(qt.getBooleanClauses()));
            Assert::IsTrue(res);
        }

        TEST_METHOD(QueryEvaluator_GetUnselectedGroupResultTest) {
            QueryTree qt(getQueryTree("assign s, a; while w; Select s such that Follows(a, w)"));
            QueryEvaluator qe;

            bool res = true;
            for (auto &pair : qt.getUnselectedGroups()) {
                if (!qe.getUnselectedGroupResult(pair.first, qt.getVarMap(), pair.second)) {
                    res = false;
                    break;
                }
            }

            Assert::IsTrue(res);
        }

        TEST_METHOD(QueryEvaluator_GetClausesTest) {
            string sampleQuery("stmt s1,s2; assign a1; Select s1 such that Modifies(a1, \"a\") and Uses(5, \"c\") and Parent(s1, s2)");
            QueryTree qt(getQueryTree(sampleQuery));
            QueryEvaluator qe;

            vector<Clause> clauses(qt.getClauses());
            Assert::IsTrue(clauses.size() == 3);
        }

        TEST_METHOD(QueryEvaluator_GetBooleanClausestTest) {
            string sampleQuery("stmt a; assign a1; Select a such that Modifies(a, \"a\") and Uses(5, \"c\") and Modifies(a1, \"a\")");
            QueryTree qt(getQueryTree(sampleQuery));
            QueryEvaluator qe;

            vector<Clause> unselectedGroups(qt.getBooleanClauses());
            Assert::IsFalse(unselectedGroups.empty());
        }

        TEST_METHOD(QueryEvaluator_GetSelectedGroupsTest) {
            string sampleQuery("stmt a; assign a1; Select a such that Modifies(a, \"a\") and Uses(5, \"c\") and Modifies(a1, \"a\")");
            QueryTree qt(getQueryTree(sampleQuery));
            QueryEvaluator qe;

            vector<std::pair<vector<Synonym>, vector<Clause>>> selectedGroups(qt.getSelectedGroups());
            Assert::IsFalse(selectedGroups.empty());
        }

        TEST_METHOD(QueryEvaluator_GetUnSelectedGroupstTest) {
            string sampleQuery("stmt a; assign a1; Select a such that Modifies(a, \"a\") and Uses(5, \"c\") and Modifies(a1, \"a\")");
            QueryTree qt(getQueryTree(sampleQuery));
            QueryEvaluator qe;

            vector<std::pair<vector<Synonym>, vector<Clause>>> unselectedGroups(qt.getUnselectedGroups());
            Assert::IsFalse(unselectedGroups.empty());
        }

        TEST_METHOD(QueryEvaluator_GetSelectedGroupResultTest) {
            QueryTree qt(getQueryTree("assign a; variable v; while w; Select a such that Modifies(a, \"a\")"));

            QueryEvaluator qe;

            vector<std::pair<vector<Synonym>, vector<Clause>>> selectedGroups(qt.getSelectedGroups());

            unordered_map<Synonym, Symbol> &varMap(qt.getVarMap());
            vector<Clause> &clauseGroup(selectedGroups.at(0).second);
            vector<Synonym> &selectList(qt.getResults());
            vector<Synonym> &synList(selectedGroups.at(0).first);
            TotalCombinationList total(qe.getSelectedGroupResult(synList, varMap, clauseGroup, selectList));
            // Assert::IsTrue(total.isEmpty());
            PartialCombinationList partial(total.getFactorList()[0]);
            // Assert::IsTrue(partial.empty());

            string actual(PartialToString(partial));
            string expected("<<a:1>,<a:10>,<a:16>,<a:23>,<a:29>,<a:8>,<a:9>>");

            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryEvaluator_getQueryResultsTest) {
            QueryTree query(getQueryTree("assign a; variable v; while w; Select a such that Follows(3, 4)"));

            QueryEvaluator qe;

            Assert::IsTrue(qe.getBooleanGroupResult(query.getBooleanClauses()));

            vector<std::pair<vector<Synonym>, vector<Clause>>> unselectedGroups(query.getUnselectedGroups());
            unordered_map<Synonym, Symbol> varMap(query.getVarMap());

            for (auto &pair : unselectedGroups) {
                Assert::IsTrue(qe.getUnselectedGroupResult(pair.first, varMap, pair.second));
            }

            TotalCombinationList result;
            vector<Synonym> selectList(query.getResults());
            vector<std::pair<vector<Synonym>, vector<Clause>>> selectedGroups(query.getSelectedGroups());

            for (auto &pair : selectedGroups) {
                TotalCombinationList &tempCombiList(qe.getSelectedGroupResult(pair.first, varMap, pair.second, selectList));
                Assert::IsFalse(tempCombiList.isEmpty());
                result.combine(tempCombiList);
            }

            Assert::IsTrue(result.isEmpty() || result.getFactorCount() == 0);
        }
    };
}

#include <string>
#include <vector>
#include <unordered_map>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/RelationTable.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::string;
using std::unordered_map;
using std::vector;

namespace UnitTesting {
    TEST_CLASS(QueryPreprocessorUnitTest) {
     public:

        TEST_METHOD(QueryPreprocessor_aSelect) {
            string expected, actual, query;
            QueryPreprocessor qp;
            QueryTree qt;

            query = "assign a,a1; while w1,w2;call c;Select c.procName";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<string> varList;

            varList = qt.getResults();
            expected = "c ";
            for (unsigned int i = 0; i < varList.size(); i++) {
                actual += varList[i] + " ";
            }
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_bSuchThat) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; while w1,w2; variable x;";
            query += "Select a such that Uses(a1, x)";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Uses a1 x ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_cSuchThatWithStringConstant) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(a1, \"x\")";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Uses a1 \"x\" ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_dSuchThatWithNumericConstant) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(1, x)";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Uses 1 x ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_eSuchThatTwoVars) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(a1, x)";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Uses a1 x ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
        }
        TEST_METHOD(QueryPreprocessor_fPattern) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1;";
            query += "Select a pattern a1(\"x\",_)";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "pattern a1 \"x\" _ ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_gPatternWhile) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "while w, w1; variable v;";
            query += "Select <w,w1> pattern w(v,_) and w1(\"v\", _) such that Parent*(w, w1)";

            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Parent* w w1 pattern w v _ pattern w1 \"v\" _ ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_hPatternWhile) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "while w, w1; ";
            query += "Select <w, w1> pattern w(_, _) and w1(_, _)";

            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "pattern w _ _ pattern w1 _ _ ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_iPatternSelectTwo) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1;";
            query += "Select a pattern a1(\"x\",_)";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "pattern a1 \"x\" _ ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_10MultipleAnd) {
            string expected, actual, query;
            QueryPreprocessor qp;
            QueryTree qt;

            query = "assign a1,a2,a3; stmt s1,s2,s3; variable v1,v2,v3;";
            query += "Select <s1,s2,v2> such that Uses(s3,v1) and Modifies(s3,\"x\") and ";
            query += "Follows(s1,s2) and Parent(s3,s1) such that Uses(s2,v1) and Uses(5,\"y\") and ";
            query += "Follows(3,4) pattern a1(v2,\"x+y\") and a3(\"z\",_) such that Modifies(a3,v3)";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<string> varList;

            varList = qt.getResults();
            for (unsigned int i = 0; i < varList.size(); i++) {
                actual += varList[i] + " ";
            }
            vector<Clause> clauseList = qt.getClauses();

            expected = "s1 s2 v2 ";
            expected += "Uses s3 v1 Modifies s3 \"x\" Follows s1 s2 Parent s3 s1 Uses s2 v1 Uses 5 \"y\" ";
            expected += "Follows 3 4 Modifies a3 v3 ";
            expected += "pattern a1 v2 pattern a3 \"z\" ";
            for (Clause c : clauseList) {
                actual += c.getClauseType() + " ";
                actual += c.getArg()[0] + " ";
                actual += c.getArg()[1] + " ";
            }
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_InvalidQuery) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            bool isValid;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that uses(\"1a\", x)";

            try {
                qp.preprocessQuery(query);
                isValid = true;
            } catch (std::exception& ex) {
                isValid = false;
            }
            Assert::AreEqual(isValid, false);
        }

        TEST_METHOD(QueryPreprocessor_With) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query += "procedure p; Select p with \"Mudkip\" = p.procName";

            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "with \"Mudkip\" p variable ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_ExtractQueryTreeOneClause) {
            string query = "assign a; Select a pattern a(\"a\", _)";
            QueryPreprocessor queryPreprocessor = QueryPreprocessor();
            queryPreprocessor.preprocessQuery(query);
            QueryTree queryTree = queryPreprocessor.getQueryTree();

            string actual, expected;

            unordered_map<string, Symbol> varMap = queryTree.getVarMap();
            for (auto kv : varMap) {
                actual += Constants::SymbolToString(kv.second) + " " + kv.first + "; ";
            }

            vector<string> selectList = queryTree.getResults();
            for (auto varName : selectList) {
                actual += Constants::SymbolToString(varMap[varName]) + " " + varName + "; ";
            }

            vector<Clause> clauses = queryTree.getClauses();
            for (Clause c : clauses) {
                actual += c.getClauseType() + " ";
                for (int i = 0; i < c.getArgCount(); i++) {
                    actual += c.getArg()[i] + " ";
                }
            }

            expected = "assign a; assign a; pattern a \"a\" _ ";  // declaration; select a; pattern
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_ExtractQueryTreeTwoClauses) {
            string query = "assign a, a1; Select a pattern a(\"e\", _) such that Follows(a, a1)";
            QueryPreprocessor queryPreprocessor = QueryPreprocessor();
            queryPreprocessor.preprocessQuery(query);
            QueryTree queryTree = queryPreprocessor.getQueryTree();

            string actual, expected;

            unordered_map<string, Symbol> varMap = queryTree.getVarMap();
            for (auto kv : varMap) {
                actual += Constants::SymbolToString(kv.second) + " " + kv.first + "; ";
            }

            vector<string> selectList = queryTree.getResults();
            for (auto varName : selectList) {
                actual += Constants::SymbolToString(varMap[varName]) + " " + varName + "; ";
            }

            vector<Clause> clauses = queryTree.getClauses();
            for (Clause c : clauses) {
                actual += c.getClauseType() + " ";
                for (int i = 0; i < c.getArgCount(); i++) {
                    actual += c.getArg()[i] + " ";
                }
            }

            expected = "assign a; assign a1; assign a; Follows a a1 pattern a \"e\" _ ";  // declaration; select a; Follows
            Assert::AreEqual(actual, expected);
        }

        TEST_METHOD(QueryPreprocessor_7xmlSelectAll) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "7-Queries.txt", true);
            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "out7-Queries.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            if (queryList.size() == 0) {
                actual = "no file found";
            }
            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }

        TEST_METHOD(QueryPreprocessor_8xmlSelectAll) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "7-Queries.txt", true);
            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "out7-Queries.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            if (queryList.size() == 0) {
                actual = "no file found";
            }
            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }

        TEST_METHOD(QueryPreprocessor_10xmlSelectAll) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "10-Queries.txt", true);
            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "out10-Queries.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            if (queryList.size() == 0) {
                actual = "no file found";
            }
            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
        TEST_METHOD(QueryPreprocessor_11xmlSelectAll) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "11-Queries.txt", true);
            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "out11-Queries.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            size_t empty = 0;
            Assert::AreNotEqual(empty, queryList.size());

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
        TEST_METHOD(QueryPreprocessor_01xmlClauseTesting) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\clauseTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\clauseTesting\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "Calls.txt", true);

            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "outCalls.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            size_t empty = 0;
            Assert::AreNotEqual(empty, queryList.size());

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
        TEST_METHOD(QueryPreprocessor_02xmlClauseTestingCallsStar) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\clauseTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\clauseTesting\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "CallsStar.txt", true);

            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "outCallsStar.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            size_t empty = 0;
            Assert::AreNotEqual(empty, queryList.size());

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
        TEST_METHOD(QueryPreprocessor_03xmlClauseTestingCustom) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\clauseTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\clauseTesting\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "Custom.txt", true);

            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "outCustom.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            size_t empty = 0;
            Assert::AreNotEqual(empty, queryList.size());

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
        TEST_METHOD(QueryPreprocessor_04xmlClauseTestingFollows) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\clauseTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\clauseTesting\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "Follows.txt", true);

            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "outFollows.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            size_t empty = 0;
            Assert::AreNotEqual(empty, queryList.size());

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
        TEST_METHOD(QueryPreprocessor_05xmlClauseTestingFollowsStar) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\clauseTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\clauseTesting\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "FollowsStar.txt", true);

            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "outFollowsStar.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            size_t empty = 0;
            Assert::AreNotEqual(empty, queryList.size());

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
        TEST_METHOD(QueryPreprocessor_06xmlClauseTestingNext) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\clauseTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\clauseTesting\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "Next.txt", true);

            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "outNext.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            size_t empty = 0;
            Assert::AreNotEqual(empty, queryList.size());

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
        TEST_METHOD(QueryPreprocessor_08xmlClauseTestingNextStar) {
            string expected, actual;
            QueryPreprocessor qpStub;
            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries10.txt
            string dirPath = "..\\tests\\SystemTesting\\clauseTesting\\";
            string dirPath1 = "..\\tests\\UnitTesting\\testcases\\QueryPreprocessor\\clauseTesting\\";

            queryList = qpStub.unitTestStubGetParams(dirPath + "NextStar.txt", true);

            expectedList = qpStub.unitTestStubGetParams(dirPath1 + "outNextStar.txt", false);

            Assert::AreEqual(expectedList.size(), queryList.size());

            size_t empty = 0;
            Assert::AreNotEqual(empty, queryList.size());

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                } catch (std::exception& ex) {
                    actual = ex.what();
                    Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                    i++;
                    continue;
                }
                qt = qp.getQueryTree();
                vector<string> varList;

                varList = qt.getResults();
                for (unsigned int ii = 0; ii < varList.size(); ii++) {
                    actual += varList[ii] + " ";
                }
                vector<Clause> clauseList = qt.getClauses();

                for (Clause c : clauseList) {
                    actual += c.getClauseType() + " ";
                    int num = c.getArgCount();
                    if (c.getClauseType() == "with") {
                        num--;
                    }
                    for (int n = 0; n < num; n++) {
                        actual += c.getArg()[n] + " ";
                    }
                }
                Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                i++;
            }
        }
    };
}  // namespace UnitTesting


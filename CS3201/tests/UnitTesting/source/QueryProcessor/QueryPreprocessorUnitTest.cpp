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
         
        TEST_METHOD(QueryPreprocessor_1Select) {
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

        TEST_METHOD(QueryPreprocessor_2SuchThat) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; while w1,w2; variable x;";
            query += "Select a such that Uses(a1, x)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
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
        TEST_METHOD(QueryPreprocessor_3SuchThatWithStringConstant) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(a1, \"x\")";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
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

        TEST_METHOD(QueryPreprocessor_4SuchThatWithNumericConstant) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(1, x)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
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
        TEST_METHOD(QueryPreprocessor_5SuchThatTwoVars) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(a1, x)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
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
        TEST_METHOD(QueryPreprocessor_6Pattern) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1;";
            query += "Select a pattern a1(\"x\",_)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
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
        TEST_METHOD(QueryPreprocessor_7PatternWhile) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "while w, w1; variable v;";
            query += "Select <w,w1> pattern w(v,_) and w1(\"v\", _) such that Parent*(w, w1)";

            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
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
        TEST_METHOD(QueryPreprocessor_8PatternWhile) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "while w, w1; ";
            query += "Select <w, w1> pattern w(_, _) and w1(_, _)";

            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
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
        TEST_METHOD(QueryPreprocessor_9PatternSelectTwo) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1;";
            query += "Select a pattern a1(\"x\",_)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
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
            }
            catch (std::exception& ex) {
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

            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries7.txt
            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 3 ");
            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 12 ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 17 ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 22 ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 29 ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 5) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 14) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 16) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 23) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 30) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (4, s) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (10, s) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (12, s) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (28, s) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 5) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 17) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 18) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 22) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 30) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (s, 5) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (s, 12) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (s, 21) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 2) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 12) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 14) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 18) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 23) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 30) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 1) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 4) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 10) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 11) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 19) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 22) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 1) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 4) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 10) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 11) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 19) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 22) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Follows* (s, w) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Follows* (w, s) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 2) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 11) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 12) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 17) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 22) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select ifstat such that Follows* (ifstat, 11) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select ifstat such that Follows* (ifstat, 18) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select ifstat such that Follows* (ifstat, 26) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select ifstat such that Follows* (ifstat, 28) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 5) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 8) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 12) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 18) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 28) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (4, v) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (6, v) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (10, v) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (12, v) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Modifies (w, \"i\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Modifies (w, \"t\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (\"Mama\", v) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Uses (s,\"x\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Uses (s, \"t\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Uses (s, \"i\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Uses (19, v) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Uses (20, v) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Uses (28, v) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Uses (a, v)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, \"i\") and Uses (a, \"i\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, a) and Uses (a, a) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, \"x\") and Uses (a,\"x\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, \"a\") and Parent (w, a)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Parent (w, a) and Modifies (a, \"a\")  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, \"a\") such that Parent (w, a)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select p such that Calls* (p, \"Zehzeh\")  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select p such that Calls (\"Papa\", p) and Modifies (p, \"i\") and Uses (p, \"c\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select p such that Calls* (\"Papa\", p) and Modifies (p, \"t\")  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (\"t\", _)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (\"a\", \"2 * c\")  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (\"t\", _) such that Follows (w, a)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_,\"t + 1\")  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_,\"t * 3 + 2\")  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"2 * c\") ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"t + b * x\"_)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"t + a\"_)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"t + b\"_)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"3 + 2\"_)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"x + a\"_)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"c * a\"_)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a1 (_, _\"t + a\"_)  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s with s.stmt# = c.value  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN pattern ifstat (\"i\", _, _) with c.value = 2  ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (1, 2) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (7,9) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (9, 10) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (11, 12) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (12, 13) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (13, 12) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (14, 15) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (18, 11) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (18, 19) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (21, 27) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (27, 30) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select n such that Next* (11, n) and Next* (n, 10) ");

            queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select n such that Next* (n, n)");

            expectedList.push_back("s Parent s s1 with s1 3 ");

            expectedList.push_back("s Parent s s1 with s1 12 ");

            expectedList.push_back("s Parent s s1 with s1 17 ");

            expectedList.push_back("s Parent s s1 with s1 22 ");

            expectedList.push_back("s Parent s s1 with s1 29 ");

            expectedList.push_back("s Parent s 5 ");

            expectedList.push_back("s Parent s 14 ");

            expectedList.push_back("s Parent s 16 ");

            expectedList.push_back("s Parent s 23 ");

            expectedList.push_back("s Parent s 30 ");

            expectedList.push_back("s Parent 4 s ");

            expectedList.push_back("s Parent 10 s ");

            expectedList.push_back("s Parent 12 s ");

            expectedList.push_back("s Parent 28 s ");

            expectedList.push_back("s Parent* s 5 ");

            expectedList.push_back("s Parent* s 17 ");

            expectedList.push_back("s Parent* s 18 ");

            expectedList.push_back("s Parent* s 22 ");

            expectedList.push_back("s Parent* s 30 ");

            expectedList.push_back("w Parent* s 5 ");

            expectedList.push_back("w Parent* s 12 ");

            expectedList.push_back("w Parent* s 21 ");

            expectedList.push_back("w Parent* w 2 ");

            expectedList.push_back("w Parent* w 12 ");

            expectedList.push_back("w Parent* w 14 ");

            expectedList.push_back("w Parent* w 18 ");

            expectedList.push_back("w Parent* w 23 ");

            expectedList.push_back("w Parent* w 30 ");

            expectedList.push_back("s Follows s 1 ");

            expectedList.push_back("s Follows s 4 ");

            expectedList.push_back("s Follows s 10 ");

            expectedList.push_back("s Follows s 11 ");

            expectedList.push_back("s Follows s 19 ");

            expectedList.push_back("s Follows s 22 ");

            expectedList.push_back("a Follows a 1 ");

            expectedList.push_back("a Follows a 4 ");

            expectedList.push_back("a Follows a 10 ");

            expectedList.push_back("a Follows a 11 ");

            expectedList.push_back("a Follows a 19 ");

            expectedList.push_back("a Follows a 22 ");

            expectedList.push_back("w Follows* s w ");

            expectedList.push_back("w Follows* w s ");

            expectedList.push_back("s Follows* s 2 ");

            expectedList.push_back("s Follows* s 11 ");

            expectedList.push_back("s Follows* s 12 ");

            expectedList.push_back("s Follows* s 17 ");

            expectedList.push_back("s Follows* s 22 ");

            expectedList.push_back("ifstat Follows* ifstat 11 ");

            expectedList.push_back("ifstat Follows* ifstat 18 ");

            expectedList.push_back("ifstat Follows* ifstat 26 ");

            expectedList.push_back("ifstat Follows* ifstat 28 ");

            expectedList.push_back("a Follows* a 5 ");

            expectedList.push_back("a Follows* a 8 ");

            expectedList.push_back("a Follows* a 12 ");

            expectedList.push_back("a Follows* a 18 ");

            expectedList.push_back("a Follows* a 28 ");

            expectedList.push_back("v Modifies 4 v ");

            expectedList.push_back("v Modifies 6 v ");

            expectedList.push_back("v Modifies 10 v ");

            expectedList.push_back("v Modifies 12 v ");

            expectedList.push_back("w Modifies w \"i\" ");

            expectedList.push_back("w Modifies w \"t\" ");

            expectedList.push_back("v Modifies \"Mama\" v ");

            expectedList.push_back("s Uses s \"x\" ");

            expectedList.push_back("s Uses s \"t\" ");

            expectedList.push_back("s Uses s \"i\" ");

            expectedList.push_back("v Uses 19 v ");

            expectedList.push_back("v Uses 20 v ");

            expectedList.push_back("v Uses 28 v ");

            expectedList.push_back("v Uses a v ");

            expectedList.push_back("a Modifies a \"i\" Uses a \"i\" ");

            expectedList.push_back(querySyntaxErrorMsg + "10");

            expectedList.push_back("a Modifies a \"x\" Uses a \"x\" ");

            expectedList.push_back("a Modifies a \"a\" Parent w a ");

            expectedList.push_back("a Parent w a Modifies a \"a\" ");

            expectedList.push_back("a Modifies a \"a\" Parent w a ");

            expectedList.push_back("p Calls* p \"Zehzeh\" ");

            expectedList.push_back("p Calls \"Papa\" p Modifies p \"i\" Uses p \"c\" ");

            expectedList.push_back("p Calls* \"Papa\" p Modifies p \"t\" ");

            expectedList.push_back("a pattern a \"t\" _ ");

            expectedList.push_back("a pattern a \"a\" \"(2*c)\" ");

            expectedList.push_back("a Follows w a pattern a \"t\" _ ");

            expectedList.push_back("a pattern a _ \"(t+1)\" ");

            expectedList.push_back("a pattern a _ \"((t*3)+2)\" ");

            expectedList.push_back(querySyntaxErrorMsg + "30");

            expectedList.push_back("a pattern a _ _\"(t+(b*x))\"_ ");

            expectedList.push_back("a pattern a _ _\"(t+a)\"_ ");

            expectedList.push_back("a pattern a _ _\"(t+b)\"_ ");

            expectedList.push_back("a pattern a _ _\"(3+2)\"_ ");

            expectedList.push_back("a pattern a _ _\"(x+a)\"_ ");

            expectedList.push_back("a pattern a _ _\"(c*a)\"_ ");

            expectedList.push_back("a pattern a1 _ _\"(t+a)\"_ ");

            expectedList.push_back("s with s c ");

            expectedList.push_back("BOOLEAN pattern ifstat \"i\" _ _ with c 2 ");

            expectedList.push_back("BOOLEAN Next 1 2 ");

            expectedList.push_back("BOOLEAN Next 7 9 ");

            expectedList.push_back("BOOLEAN Next 9 10 ");

            expectedList.push_back("BOOLEAN Next 11 12 ");

            expectedList.push_back("BOOLEAN Next 12 13 ");

            expectedList.push_back("BOOLEAN Next 13 12 ");

            expectedList.push_back("BOOLEAN Next 14 15 ");

            expectedList.push_back("BOOLEAN Next 18 11 ");

            expectedList.push_back("BOOLEAN Next 18 19 ");

            expectedList.push_back("BOOLEAN Next 21 27 ");

            expectedList.push_back("BOOLEAN Next 27 30 ");

            expectedList.push_back("n Next* 11 n Next* n 10 ");

            expectedList.push_back("n Next* n n ");


            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                }
                catch (std::exception& ex) {
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

            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries8.txt
            queryList.push_back("procedure p;variable v;Select p with p.procName = v.varName");

            queryList.push_back("constant c;stmt s;Select c with c.value = s.stmt#");

            queryList.push_back("constant c; if ifstmt;Select c with c.value = ifstmt.stmt#");

            queryList.push_back("constant c; assign a;Select c with c.value = a.stmt#");

            queryList.push_back("constant c; prog_line n;Select c with c.value = n");

            queryList.push_back("procedure p;Select p with p.procName = \"Pikachu\"");

            queryList.push_back("variable x;Select x with x.varName = \"i\"");

            queryList.push_back("constant c;Select c with c.value = 3");

            queryList.push_back("constant c;Select c with c.value = 7");

            queryList.push_back("stmt s;Select s with s.stmt# = 8");

            queryList.push_back("stmt s;Select s with s.stmt# = 20");

            queryList.push_back("procedure p;Select p with \"Mudkip\" = p.procName");

            queryList.push_back("variable v;Select v with \"q\" = v.varName");

            queryList.push_back("if ifstmt;Select ifstmt with \"Pikachu\" = \"Mewtwo\"");

            queryList.push_back("call c;Select c with \"z\" = \"z\"");

            queryList.push_back("constant c;Select c with 9 = c.value");

            queryList.push_back("stmt s;Select s with 18 = s.stmt#");

            queryList.push_back("prog_line n;Select n with 9 = n");

            queryList.push_back("prog_line n;Select n with 0 = n");

            expectedList.push_back("p with p v ");

            expectedList.push_back("c with c s ");

            expectedList.push_back("c with c ifstmt ");

            expectedList.push_back("c with c a ");

            expectedList.push_back("c with c n ");

            expectedList.push_back("p with p \"Pikachu\" ");

            expectedList.push_back("x with x \"i\" ");

            expectedList.push_back("c with c 3 ");

            expectedList.push_back("c with c 7 ");

            expectedList.push_back("s with s 8 ");

            expectedList.push_back("s with s 20 ");

            expectedList.push_back("p with \"Mudkip\" p ");

            expectedList.push_back("v with \"q\" v ");

            expectedList.push_back("ifstmt with \"Pikachu\" \"Mewtwo\" ");

            expectedList.push_back("c with \"z\" \"z\" ");

            expectedList.push_back("c with 9 c ");

            expectedList.push_back("s with 18 s ");

            expectedList.push_back("n with 9 n ");

            expectedList.push_back("n with 0 n ");

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                }
                catch (std::exception& ex) {
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
            
            queryList = qpStub.unitTestStubGetParams(dirPath+"10-Queries.txt", true);
            expectedList = qpStub.unitTestStubGetParams(dirPath1+"out10-Queries.txt", false);
            
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
                }
                catch (std::exception& ex) {
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

            vector<string> queryList;
            vector<string> expectedList;
            string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
            // queries11.txt
            queryList.push_back("procedure p;variable v;Select p such that Calls(p,\"p4\") and Modifies(p,v) with v.varName = \"cho\"");

            queryList.push_back("assign a; if i1; prog_line n;variable v;Select i1 such that Parent(i1,a) and Next(i1,n) and Uses(a,v) and Modifies(a,v) with v.varName = \"heir\"");

            queryList.push_back("procedure p1;procedure p2; assign a1;stmt s; variable v;Select p1 such that Uses(p1,v) and Calls(p1,p2) and Modifies(p2,v) and Parent(s,a1)");

            queryList.push_back("procedure p1;procedure p2;variable v;stmt s1; stmt s2;Select s2 such that Calls*(p1,p2) and Modifies(p1,v) and Modifies(s1,v) and Parent(s2,s1)");

            queryList.push_back("variable v1;variable v2;assign a;assign b;stmt s;Select <v1,v2> such that Modifies(a,v1) and Uses(b,v2) and Parent*(s,a)");

            queryList.push_back("variable v;procedure p1;procedure p2;Select v such that Calls*(p1,p2) and Modifies(p2,v)");

            queryList.push_back("variable v1; variable v2;variable v3;assign a;assign b;procedure p1;Select <v1,v2,v3> such that Modifies(a,v1) and Uses(b,v2) and Modifies(p1,v3) and Uses(a,v3)");

            queryList.push_back("assign a;assign b;assign c;variable v1; variable v2;stmt s1;stmt s2;procedure p1;procedure p2;prog_line n1;prog_line n2;Select <a,b,c> pattern b(\"ghost\",_) such that Uses(a,v1) and Parent*(s1,b) and Follows*(s1,s2) and Modifies(p1,v2) and Calls*(p1,p2) and Uses(c,v2) and Next*(n1,n2) with n1 = 26 and v2.varName = \"psy\"");

            queryList.push_back("assign a;Select <a,a> such that Modifies(a,\"ghost\")");

            queryList.push_back("assign a;Select a such that Parent(a,a)");

            queryList.push_back("assign a;Select a such that Follows(a,a)");

            queryList.push_back("assign a;Select a such that Follows*(a,a)");

            queryList.push_back("assign a,b;Select a pattern a(\"cho\",_) and a(_,_\"3 * psy + 2\"_)");

            queryList.push_back("stmt s;while w;variable v; prog_line n1,n2; assign a;Select <w,a> such that Follows*(w,s) and Parent(w,a) and Uses(a,v) and Next*(n1,n2) and Parent*(n1,n2) with v.varName = \"psy\" pattern w(\"man\",_) ");
            
            expectedList.push_back("p Calls p \"p4\" Modifies p v with v \"cho\" ");

            expectedList.push_back("i1 Parent i1 a Next i1 n Uses a v Modifies a v with v \"heir\" ");

            expectedList.push_back("p1 Uses p1 v Calls p1 p2 Modifies p2 v Parent s a1 ");

            expectedList.push_back("s2 Calls* p1 p2 Modifies p1 v Modifies s1 v Parent s2 s1 ");

            expectedList.push_back("v1 v2 Modifies a v1 Uses b v2 Parent* s a ");

            expectedList.push_back("v Calls* p1 p2 Modifies p2 v ");

            expectedList.push_back("v1 v2 v3 Modifies a v1 Uses b v2 Modifies p1 v3 Uses a v3 ");

            expectedList.push_back("a b c Uses a v1 Parent* s1 b Follows* s1 s2 Modifies p1 v2 Calls* p1 p2 Uses c v2 Next* n1 n2 pattern b \"ghost\" _ with n1 26 with v2 \"psy\" ");

            expectedList.push_back("a a Modifies a \"ghost\" ");

            expectedList.push_back(querySyntaxErrorMsg+"10");

            expectedList.push_back("a Follows a a ");

            expectedList.push_back("a Follows* a a ");

            expectedList.push_back("a pattern a \"cho\" _ pattern a _ _\"((3*psy)+2)\"_ ");

            expectedList.push_back("w a Follows* w s Parent w a Uses a v Next* n1 n2 Parent* n1 n2 pattern w \"man\" _ with v \"psy\" ");

            int i = 0;
            for (string query : queryList) {
                QueryPreprocessor qp;
                QueryTree qt;
                actual = "";
                try {
                    qp.preprocessQuery(query);
                }
                catch (std::exception& ex) {
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


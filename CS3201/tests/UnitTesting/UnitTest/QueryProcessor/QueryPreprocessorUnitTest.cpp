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
        std::string expected, actual, query;
        QueryPreprocessor qp;
        QueryTree qt;

        query = "assign a,a1; while w1,w2;Select a";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            actual = ex.what();
        }
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
        query += "Select a such that Uses(a1, x)";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            actual = ex.what();
        }
        qt = qp.getQueryTree();
        std::vector<Clause> resList;

        resList = qt.getSuchThat();
        expected = "Uses a1 x ";
        for (unsigned int i = 0; i < resList.size(); i++) {
            actual += resList[i].getClauseType() + " ";
            for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                actual += resList[i].getArg()[j] + " ";
            }
        }
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryParserPatternSelectTwo) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        std::string query;
        query = "assign a,a1;";
        query += "Select a pattern a1(\"x\",_)";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            actual = ex.what();
        }
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
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryParserSuchThatWithStringConstant) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        std::string query;
        query = "assign a,a1; variable x;";
        query += "Select a such that Uses(a1, \"x\")";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            actual = ex.what();
        }
        qt = qp.getQueryTree();
        std::vector<Clause> resList;

        resList = qt.getSuchThat();
        expected = "Uses a1 \"x\" ";
        for (unsigned int i = 0; i < resList.size(); i++) {
            actual += resList[i].getClauseType() + " ";
            for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                actual += resList[i].getArg()[j] + " ";
            }
        }
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryParserSuchThatWithNumericConstant) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        std::string query;
        query = "assign a,a1; variable x;";
        query += "Select a such that Uses(1, x)";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            actual = ex.what();
        }
        qt = qp.getQueryTree();
        std::vector<Clause> resList;

        resList = qt.getSuchThat();
        expected = "Uses 1 x ";
        for (unsigned int i = 0; i < resList.size(); i++) {
            actual += resList[i].getClauseType() + " ";
            for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                actual += resList[i].getArg()[j] + " ";
            }
        }
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryParserInvalidQuery) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        bool isValid;

        std::string query;
        query = "assign a,a1; variable x;";
        query += "Select a such that uses(\"1a\", x)";

        try {
            qp.preprocessQuery(query);
            isValid = true;
        }
        catch (std::exception& ex) {
            isValid = false;
        }
        Assert::AreEqual(isValid, false);
    }
    TEST_METHOD(QueryParserSelectOneVars) {
        std::string expected, actual, query;
        QueryPreprocessor qp;
        QueryTree qt;

        query = "assign a,a1; while w1,w2;Select a";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            actual = ex.what();
        }
        qt = qp.getQueryTree();
        std::vector<std::string> varList;

        varList = qt.getResults();
        expected = "a ";
        for (unsigned int i = 0; i < varList.size(); i++) {
            actual += varList[i] + " ";
        }
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(QueryParserSuchThatTwoVars) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        std::string query;
        query = "assign a,a1; variable x;";
        query += "Select a such that Uses(a1, x)";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            actual = ex.what();
        }
        qt = qp.getQueryTree();
        std::vector<Clause> resList;

        resList = qt.getSuchThat();
        expected = "Uses a1 x ";
        for (unsigned int i = 0; i < resList.size(); i++) {
            actual += resList[i].getClauseType() + " ";
            for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                actual += resList[i].getArg()[j] + " ";
            }
        }
    }
    TEST_METHOD(QueryParserPattern) {
        std::string expected, actual;
        QueryPreprocessor qp;
        QueryTree qt;

        std::string query;
        query = "assign a,a1;";
        query += "Select a pattern a1(\"x\",_)";
        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            actual = ex.what();
        }
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
        Assert::AreEqual(expected, actual);
    }
    TEST_METHOD(ExtractQueryTreeOneClause) {
        std::string query = "assign a; Select a pattern a(\"a\", _)";
        QueryPreprocessor queryPreprocessor = QueryPreprocessor();
        queryPreprocessor.preprocessQuery(query);
        QueryTree queryTree = queryPreprocessor.getQueryTree();

        std::string actual, expected;

        std::unordered_map<std::string, Symbol> varMap = queryTree.getVarMap();
        for (auto kv : varMap) {
            actual += Constants::SymbolToString(kv.second) + " " + kv.first + "; ";
        }

        std::unordered_map<std::string, Symbol> selectMap = queryTree.getSelect();
        for (auto kv : selectMap) {
            actual += Constants::SymbolToString(kv.second) + " " + kv.first + "; ";
        }

        std::vector<Clause> clauses = queryTree.getClauses("suchThat pattern");
        for (Clause c : clauses) {
            actual += c.getClauseType() + " ";
            for (int i = 0; i < c.getArgCount(); i++) {
                actual += c.getArg()[i] + " ";
            }
        }

        expected = "assign a; assign a; pattern \"a\" _ a ";  // declaration; select a; pattern
        Assert::AreEqual(actual, expected);
    }
    TEST_METHOD(ExtractQueryTreeTwoClauses) {
        std::string query = "assign a, a1; Select a pattern a(\"e\", _) such that Follows(a, a1)";
        QueryPreprocessor queryPreprocessor = QueryPreprocessor();
        queryPreprocessor.preprocessQuery(query);
        QueryTree queryTree = queryPreprocessor.getQueryTree();

        std::string actual, expected;

        std::unordered_map<std::string, Symbol> varMap = queryTree.getVarMap();
        for (auto kv : varMap) {
            actual += Constants::SymbolToString(kv.second) + " " + kv.first + "; ";
        }

        std::unordered_map<std::string, Symbol> selectMap = queryTree.getSelect();
        for (auto kv : selectMap) {
            actual += Constants::SymbolToString(kv.second) + " " + kv.first + "; ";
        }

        std::vector<Clause> clauses = queryTree.getClauses("suchThat pattern");
        for (Clause c : clauses) {
            actual += c.getClauseType() + " ";
            for (int i = 0; i < c.getArgCount(); i++) {
                actual += c.getArg()[i] + " ";
            }
        }

        expected = "assign a; assign a1; assign a; Follows a a1 pattern \"e\" _ a ";  // declaration; select a; Follows
        Assert::AreEqual(actual, expected);
    }
    };
}   // namespace UnitTest

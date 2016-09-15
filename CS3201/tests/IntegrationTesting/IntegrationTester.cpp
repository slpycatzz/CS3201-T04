#include <string>
#include <vector>
#include <stdlib.h>

#include "stdafx.h"
#include "targetver.h"

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "PKB/PKB.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "Constants.h"
#include "QueryProcessor/QueryEvaluator.h"

using std::string;
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SystemTesting {
	TEST_CLASS(SystemTest) {
public:
	void parse(string filePath) {
		FrontendParser frontendParser = FrontendParser();

		try {
			frontendParser.parseProgram(filePath);

		}
		catch (std::exception& ex) {
			Logger::WriteMessage("Parse Error");
			Logger::WriteMessage(ex.what());
			exit(EXIT_FAILURE);
		}
	}
	void getSampleProgram() {
		string fileName = "..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt";
		parse(fileName);
	}
	QueryTree getQueryTree(std::string query) {
		QueryPreprocessor qp;
		qp.preprocessQuery(query);
		QueryTree qt(qp.getQueryTree());
		return qt;
	}

	std::string printTotalCombinationList(TotalCombinationList total) {
		std::string s("<");
		for (auto kv : total) {
			std::string s("<");
			for (CandidateCombination comb : kv.second) {
				s.append(Utils::MapToString(comb));
				s.append(",");
			}
			s.append(">");
			return s;
		}
	}
  TEST_METHOD(Integration_Parser_and_PKB) {

    //since PKB is a static class. CLEAR IT before running test
    PKB::clear();

    string fileName = "..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt";
    parse(fileName);

    Assert::AreEqual(42, int(PKB::GetNumberOfAssign()));
    Assert::AreEqual(0, int(PKB::GetNumberOfCall()));
    Assert::AreEqual(0, int(PKB::GetNumberOfIf()));
    Assert::AreEqual(1, int(PKB::GetNumberOfProcedure()));
    Assert::AreEqual(7, int(PKB::GetNumberOfWhile()));
  }
	TEST_METHOD(Integration_QE_GetCadidatesTest) {

    //since PKB is a static class. CLEAR IT before running test
    PKB::clear();

		getSampleProgram();
		QueryTree qt(getQueryTree("variable v; Select v;"));
		
		QueryEvaluator qe;
		//std::unordered_map<std::string, Symbol> map(qt.getSelect());

		std::vector<std::string> result(qe.selectQueryResults(qt));
		Logger::WriteMessage(Utils::VectorToString(result).c_str());
	}
	TEST_METHOD(TestGetCandidates) {

		getSampleProgram();
		QueryTree qt(getQueryTree("stmt a; Select a such that Modifies(a, \"a\");"));
		QueryEvaluator qe;

		TotalCombinationList total(qe.getTotalCandidateList(qt));
		Logger::WriteMessage(printTotalCombinationList(total).c_str());
		for (unsigned i : PKB::GetSymbolStmtNumbers(STMT)) {
			Logger::WriteMessage(std::to_string(i).c_str());
		}

		/**
		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<1>,<8>,<9>,<10>,<16>,<23>,<29>>");
		**/
		//Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestEvaluateModifies) {

    //since PKB is a static class. CLEAR IT before running test
    PKB::clear();

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; Select a such that Modifies(a, \"a\");"));
		QueryEvaluator qe;

		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<1>,<8>,<9>,<10>,<16>,<23>,<29>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestEvaluateFollows) {
		
		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; while w; Select a such that Follows(a, w);"));
		QueryEvaluator qe;
		
		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<5>,<11>,<29>,<32>,<34>,<37>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestEvaluateFollowsWithConst) {
		
		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; while w; Select w such that Follows(3, a);"));
		QueryEvaluator qe;

		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<6>,<7>,<12>,<30>,<33>,<35>,<38>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestGetModifies) {

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; variable v; Select v such that Modifies(1, v);"));
		QueryEvaluator qe;

		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<a>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestGetUses) {

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; variable v; Select a such that Uses(a, \"a\");"));
		QueryEvaluator qe;

		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<10>,<21>,<22>,<36>,<42>,<44>,<46>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestExactPattern) {

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(\"c\",\"c\");"));
		QueryEvaluator qe;

		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<2>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestExactRHSPattern) {

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_,\"c\");"));
		QueryEvaluator qe;

		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<2>,<16>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestUnderscorePattern) {

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(\"c\",_);"));
		QueryEvaluator qe;

		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<2>,<3>,<5>,<24>,<26>,<41>,<48>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestSubPattern) {

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_, _\"c\"_);"));
		QueryEvaluator qe;

		std::string actual(Utils::VectorToString(qe.selectQueryResults(qt)));
		std::string expected("<<2>,<4>,<10>,<16>,<18>,<37>,<42>,<47>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(TestEvaluateExactPattern) {

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_, \"c\");"));
		QueryEvaluator qe;

		bool f(qe.evaluatePatternClause("2", "c", "\"c\""));
		Assert::IsTrue(f);
	}
	TEST_METHOD(TestEvaluateSubPattern) {

		getSampleProgram();
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_, _\"c\"_);"));
		QueryEvaluator qe;

		Assert::IsTrue(qe.evaluatePatternClause("37", "e", "_\"c\"_"));
	}
	};
}
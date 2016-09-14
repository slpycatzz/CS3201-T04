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
		PKB pkb;
		std::vector<std::string> result(qe.selectQueryResults(pkb, qt));
		Logger::WriteMessage(Utils::VectorToString(result).c_str());
	}
	TEST_METHOD(TestEvaluateSuchThatClause) {

    //since PKB is a static class. CLEAR IT before running test
    PKB::clear();

		getSampleProgram();
		QueryTree qt = getQueryTree("assign a; Select a;");
		QueryEvaluator qe;
		PKB pkb;

		std::vector<std::string> results(qe.selectQueryResults(pkb, qt));
		Logger::WriteMessage(Utils::VectorToString(results).c_str());
	}
	};
}
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
	TEST_METHOD(QE_GetCadidatesTest) {
		getSampleProgram();
		QueryTree qt(getQueryTree("variable v; Select v such that Modifies(1, \"a\");"));

		QueryEvaluator qe;
		PKB pkb;
		std::vector<std::string> result(qe.selectQueryResults(pkb, qt));
		Logger::WriteMessage(Utils::VectorToString(result).c_str());

	}
	TEST_METHOD(TestEvaluateSuchThatClause) {
		getSampleProgram();
		QueryTree qt = getQueryTree("variable v; Select v such that Modifies(1, \"a\");");
		QueryEvaluator qe;
		PKB pkb;

		Clause cl = *qt.getClauses("suchThat pattern").begin();
		Logger::WriteMessage(cl.getClauseType().c_str());
		//Assert::IsTrue(qe.evaluateSuchThatClause(pkb, "Modifies", "1", "a"));
		Assert::AreSame<unsigned>(Utils::StringToInt("1"), 1);
	}
	};
}
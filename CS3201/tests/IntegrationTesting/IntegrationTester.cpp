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
	TEST_METHOD(QE_GetCadidatesTest) {
		string fileName = "..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt";
		parse(fileName);
		
		string query("variable v; Select v such that Modifies(1, \"a\");");
		QueryPreprocessor qp;
		qp.preprocessQuery(query);
		QueryTree qt(qp.getQueryTree());

		QueryEvaluator qe;
		PKB pkb;
		std::vector<std::string> result(qe.selectQueryResults(pkb, qt));
		Logger::WriteMessage(Utils::VectorToString(result).c_str());

	}
	};
}
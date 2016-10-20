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
#include "QueryProcessor/QueryOptimizer.h"
#include "QueryProcessor/QueryProjector.h"

using std::string;
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SystemTesting {
	TEST_CLASS(SystemTest) {
public:
	struct StringCompare {
		bool operator() (const string &str1, const string &str2) {
			if (Utils::IsNonNegativeNumeric(str1) && Utils::IsNonNegativeNumeric(str2)) {
				return (std::stoi(str1) < std::stoi(str2));
			}
			else {
				return (str1 < str2);
			}
		}
	};

	struct CombinationCompare {
		bool operator() (const vector<Candidate> comb1, const vector<Candidate> comb2) {
			StringCompare comp;
			for (unsigned i=0; i < comb1.size(); i++) {
				if (comp(comb1[i], comb2[i])) return true;
			}
			return false;
		}
	};

	vector<vector<Candidate>>& sortResult(vector<vector<Candidate>> &combList) {
		CombinationCompare comp;
		std::sort(combList.begin(), combList.end(), comp);
		return combList;
	}

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

	void getSampleProgram(string fileName) {
        PKB::Clear();
		parse(fileName);
	}

	QueryTree getQueryTree(string query) {
		QueryPreprocessor qp;
		qp.preprocessQuery(query);
		QueryTree qt(qp.getQueryTree());
		QueryOptimizer qo;
		return qo.optimize(qt);
	}

	vector<string> resultToString(vector<vector<Candidate>> &list) {
		vector<string> res;
		for (vector<string> combi : list) {
			res.push_back(Utils::VectorToString(combi));
		}
		return res;
	}

	string format(ResultList &resultList) {
		return Utils::VectorToString(resultToString(sortResult(resultList.second)));
	}

	TEST_METHOD(StringCompareTest) {
		StringCompare comp;
		Assert::IsTrue(comp("8", "29"));
	}

	TEST_METHOD(CombinationCompareTest) {
		CombinationCompare comp;
		Assert::IsTrue(comp({ "8" }, { "29" }));
	}

    TEST_METHOD(Integration_Parser_and_PKB) {
        getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
        Assert::AreEqual(42, int(PKB::GetNumberOfAssign()));
        Assert::AreEqual(0, int(PKB::GetNumberOfCall()));
        Assert::AreEqual(0, int(PKB::GetNumberOfIf()));
        Assert::AreEqual(1, int(PKB::GetNumberOfProcedure()));
        Assert::AreEqual(7, int(PKB::GetNumberOfWhile()));
    }

	TEST_METHOD(Integration_QE_GetCandidates_Generic) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("variable v; Select v;"));
		
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<a>,<b>,<c>,<d>,<e>,<f>,<g>,<h>>");
		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(Integration_QE_GetCandidates_Boolean) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select BOOLEAN such that Modifies(a, v);"));

		QueryEvaluator qe;
		
		//std::unordered_map<string, Symbol> varMap(qt.getVarMap());
		//Assert::IsTrue(varMap.find("BOOLEAN") != varMap.end());
		//string actual(Utils::VectorToString(qt.getResults()));
		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<TRUE>>");
		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(Integration_QE_EvaluateModifies) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; Select a such that Modifies(a, \"a\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<1>,<8>,<9>,<10>,<16>,<23>,<29>>");
		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(Integration_QE_EvaluateFollows) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; while w; Select a such that Follows(a, w)"));
		QueryEvaluator qe;
		
		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<5>,<11>,<29>,<32>,<34>,<37>>");
		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(Integration_QE_EvaluateFollowsWithConst) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; while w; Select w such that Follows(3, 4)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<6>,<7>,<12>,<30>,<33>,<35>,<38>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Intgeration_QE_GetModifies) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select v such that Modifies(1, v)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<a>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_GetUses) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a such that Uses(a, \"a\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<10>,<21>,<22>,<36>,<42>,<44>,<46>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_ExactPattern) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(\"c\",\"c\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<2>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_ExactRHSPattern) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_,\"c\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<2>,<16>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_UnderscorePattern) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(\"c\",_)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<2>,<3>,<5>,<24>,<26>,<41>,<48>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_SubPattern) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_, _\"c\"_)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<2>,<4>,<10>,<16>,<18>,<37>,<42>,<47>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_EvaluateExactPattern) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_, \"c\")"));
		QueryEvaluator qe;

		bool f(qe.evaluatePatternClause("2", "c", "\"c\""));
		Assert::IsTrue(f);
	}
	TEST_METHOD(Integration_QE_EvaluateSubPattern) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_, _\"c\"_)"));
		QueryEvaluator qe;

		Assert::IsTrue(qe.evaluatePatternClause("2", "c", "_\"c\"_"));
	}
    TEST_METHOD(Integration_QE_SubPatternConstant) {
        getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
        QueryTree qt(getQueryTree("assign a; Select a pattern a(_, _\"97\"_)"));
        QueryEvaluator qe;
    
        Assert::IsTrue(qe.evaluatePatternClause("1", "_", "_\"97\"_"));
    }
    TEST_METHOD(Integration_QE_ExactPatternConstant) {
        getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
        QueryTree qt(getQueryTree("assign a; Select a pattern a(_, \"42\")"));
        QueryEvaluator qe;

        Assert::IsTrue(qe.evaluatePatternClause("25", "_", "\"42\""));
    }
    TEST_METHOD(Integration_Optimizer_TestOne) {
        string query, expectedBooleanClauses, actualBooleanClauses;
        string expectedUnselectedClauses, actualUnselectedClauses;
        string expectedSelectedClausesOne, actualSelectedClausesOne;
        string expectedSelectedClausesTwo, actualSelectedClausesTwo;

        getSampleProgram("..\\tests\\IntegrationTesting\\IntegrationTest-OptimizerSource.txt");

        QueryPreprocessor qp;
        QueryOptimizer qo;
        QueryTree qt;

        query = "assign a1,a2,a3; stmt s1,s2,s3,s4; variable v1,v2,v3,v4;";
        query += "Select <s1, s2, v2> such that Follows*(s1, s2) and Parent(s3, s1) and Uses(s2, v1) ";
        query += "and Uses(s3, v1) such that Uses(5, \"y\") such that Follows(3, 4) such that Uses(a3, v4) ";
        query += "such that Modifies(s3, \"x\") and Follows(s2, 3) pattern a1(v2, _\"x+y\"_) and a3(\"z\", _) ";
        query += "such that Uses(s4, v3) and Uses(s4, v4) and Modifies(a3, v3)";

        try {
            qp.preprocessQuery(query);
        }
        catch (std::exception& ex) {
            Logger::WriteMessage(ex.what());
        }

        qt = qo.optimize(qp.getQueryTree());

        for (Clause c : qt.getBooleanClauses()) {
            actualBooleanClauses += c.toString() + " ";
        }

        vector<Clause> unselectedGroup = qt.getUnselectedGroups().at(0).second;
        for (Clause c : unselectedGroup) {
            actualUnselectedClauses += c.toString() + " ";
        }

        //testing for group 1 only, ignore pattern a1(v2,_"x+y"_) in group 2
        vector<Clause> selectedGroupOne = qt.getSelectedGroups().at(0).second;
        for (Clause c : selectedGroupOne) {
            actualSelectedClausesOne += c.toString() + " ";
        }

        vector<Clause> selectedGroupTwo = qt.getSelectedGroups().at(1).second;
        for (Clause c : selectedGroupTwo) {
            actualSelectedClausesTwo += c.toString() + " ";
        }

        expectedBooleanClauses = "Uses(5,\"y\") Follows(3,4) ";
        expectedUnselectedClauses = "pattern a3(\"z\",_) Modifies(a3,v3) Uses(a3,v4) Uses(s4,v4) Uses(s4,v3) ";
        expectedSelectedClausesOne = "Follows(s2,3) Modifies(s3,\"x\") Parent(s3,s1) Uses(s2,v1) Uses(s3,v1) Follows*(s1,s2) ";
        expectedSelectedClausesTwo = "pattern a1(v2,_\"x+y\"_) ";

        Assert::AreEqual(expectedBooleanClauses, actualBooleanClauses);
        Assert::AreEqual(expectedUnselectedClauses, actualUnselectedClauses);
        Assert::AreEqual(expectedSelectedClausesOne, actualSelectedClausesOne);
        Assert::AreEqual(expectedSelectedClausesTwo, actualSelectedClausesTwo);
    }
    TEST_METHOD(Integration_Projector_SelectCallProcedureName) {
        //Tests for Select <a,s,c.procName>
        string fileName = "..\\tests\\IntegrationTesting\\IntegrationTest-QueryProjector.txt";
        PKB::Clear();
        parse(fileName);

        QueryProjector projector;
        string expected, actual;

        std::list<string> results;
        ResultList resultList({ { "a", "s", "c" },{ { "1","2","3" },{ "4","5","6" } } });

        std::unordered_map<string, bool> varAttrMap({ { "c", true } });
        
        projector.projectResult(results, varAttrMap, resultList);

        expected = "1 2 Projector2, 4 5 Projector3";

        for (string s : results) {
            actual += s + ", ";
        }
        actual = actual.substr(0, actual.length() - 2);

        Assert::AreEqual(expected, actual);
    }
	};
}

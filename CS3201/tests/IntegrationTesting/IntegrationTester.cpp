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
		
		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<"+string(SYMBOL_TRUE)+">>");
		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(Integration_QE_Follows) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; while w; Select a such that Follows(a, w)"));
		QueryEvaluator qe;
		
		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<5>,<11>,<29>,<32>,<34>,<37>>");
		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(Integration_QE_FollowsWithConst) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; while w; Select w such that Follows(3, 4)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<6>,<7>,<12>,<30>,<33>,<35>,<38>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Intgeration_QE_ModifiesWithConst_1) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select v such that Modifies(1, v)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<a>>");
		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(Integration_QE_ModifiesWithConst_2) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; Select a such that Modifies(a, \"a\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<1>,<8>,<9>,<10>,<16>,<23>,<29>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Intergation_QE_ModifiesIf) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source2.txt");
		QueryTree qt(getQueryTree("if i; variable v; Select <i,v> such that Modifies(i, v) and Modifies(i, \"a\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<7,a>,<7,b>,<7,d>,<7,e>,<7,g>,<7,h>,<12,a>,<12,b>,<12,d>,<12,e>,<12,h>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_ModifiesWhile) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source2.txt");
		QueryTree qt(getQueryTree("while w; variable v; Select <w,v> such that Modifies(w, v) and Uses(w, v)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<6,a>,<6,b>,<6,c>,<6,d>,<6,e>,<6,g>,<6,h>,<33,c>,<33,e>,<33,g>,<33,h>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_UsesAssign) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a such that Uses(a, \"a\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<10>,<21>,<22>,<36>,<42>,<44>,<46>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_UsesIf) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source2.txt");
		QueryTree qt(getQueryTree("if i; variable v; Select i such that Uses(i, \"a\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<7>,<30>,<35>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_UsesWhile) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("while w; variable v; Select w such that Uses(w, \"a\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<6>,<7>,<30>,<33>,<35>>");
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

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<2>,<16>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_LongPattern_1) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_, \"81 + 45 + 4 + b\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<9>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_LongPattern_2) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select v pattern a(v, _\"g + 91 + 45 + g + 13\"_)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<d>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_LongPattern_3) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select <a,v> pattern a(v, _\"e+f\"_)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<5,c>,<41,c>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_EvaluateSubPattern) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("assign a; variable v; Select a pattern a(_, _\"c\"_)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<2>,<4>,<10>,<16>,<18>,<37>,<42>,<47>>");
		Assert::AreEqual(expected, actual);
	}
    TEST_METHOD(Integration_QE_SubPatternConstant) {
        getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
        QueryTree qt(getQueryTree("assign a; Select a pattern a(_, _\"97\"_)"));
        QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<1>>");
		Assert::AreEqual(expected, actual);
    }
	TEST_METHOD(Integration_QE_IfPattern_1) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source2.txt");
		QueryTree qt(getQueryTree("if i; variable v; Select <i,v> pattern i(v,_,_)"));
		QueryEvaluator qe;

		Clause pattern(qt.getSelectedGroups().at(0).second.at(0));
		string test("<");
		for (string s : pattern.getArg()) {
			test.append(s);
			test.append(",");
		}
		test.append(">");
		Logger::WriteMessage(test.c_str());

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<7,c>,<30,a>,<35,b>,<12,e>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Intergration_QE_IfPattern_2) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source2.txt");
		QueryTree qt(getQueryTree("if i; Select i pattern i(_,_,_)"));
		QueryEvaluator qe;

		Clause pattern(qt.getSelectedGroups().at(0).second.at(0));
		string test("<");
		for (string s : pattern.getArg()) {
			test.append(s);
			test.append(",");
		}
		test.append(">");
		Logger::WriteMessage(test.c_str());

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<7>,<12>,<30>,<35>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_WhilePattern_1) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("while w, w1; variable v; Select <w,w1> pattern w(v,_) and w1(v, _) such that Parent*(w, w1)"));
		QueryEvaluator qe;

		Clause pattern(qt.getSelectedGroups().at(0).second.at(0));
		string test("<");
		for (string s : pattern.getArg()) {
			test.append(s);
			test.append(",");
		}
		test.append(">");
		Logger::WriteMessage(test.c_str());

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<6,7>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_WhilePattern_2) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("while w, w1; Select <w, w1> pattern w(\"a\",_) and w1(\"b\",_)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		Logger::WriteMessage(qe.log.c_str());
		string expected("<<30,35>>");
		Assert::AreEqual(expected, actual);
	}

    TEST_METHOD(Integration_QE_ExactPatternConstant) {
        getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
        QueryTree qt(getQueryTree("assign a; Select a pattern a(_,\"c\")"));
        QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<2>,<16>>");
		Assert::AreEqual(expected, actual);
    }
	TEST_METHOD(Integration_QE_Follows_2var_1) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; Select <s,w> such that Follows(s,w)"));
		QueryEvaluator qe;
		
		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<5,6>,<11,12>,<29,30>,<32,33>,<34,35>,<37,38>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Follows_2var_2) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select <s,v> such that Follows(s, w) and Uses(w, v) and Uses(s, v)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<5,e>,<5,f>,<37,d>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Follows_1var) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select <w,v> such that Follows(29, w) and Uses(w, v)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<30,a>,<30,b>,<30,c>,<30,d>,<30,e>,<30,f>,<30,g>,<30,h>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Follows_1var_Underscore) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select w such that Follows(_, w) and Follows(w, s)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<6>,<12>,<30>,<35>,<38>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Follows_0var) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select BOOLEAN such that Follows(29, 30)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<"+string(SYMBOL_TRUE)+">>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Follows_0var_Underscore) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select BOOLEAN such that Follows(_,_)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<"+string(SYMBOL_TRUE)+">>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Next_2var) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select s such that Next(s, w)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<5>,<6>,<11>,<16>,<22>,<24>,<29>,<32>,<33>,<34>,<37>,<41>,<42>,<43>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Next_1var) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select s such that Next(s, 7)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<6>,<22>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Next_0var) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select BOOLEAN such that Next(6, 7)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<"+string(SYMBOL_TRUE)+">>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Next_1) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source3.txt");
		QueryTree qt(getQueryTree("stmt s1, s2; variable v; Select s1 such that Next(s1, s2)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<1>,<2>,<3>,<6>,<7>,<8>,<9>,<10>,<11>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_NextStar_2var) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source2.txt");
		QueryTree qt(getQueryTree("if s; while w; variable v; Select s such that Next*(s, s)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<7>,<12>,<35>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_NextStar_1var) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select w such that Next*(s, 10)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<6>,<7>,<12>,<30>,<33>,<35>,<38>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_NextStar_0var) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select BOOLEAN such that Next*(11, 10)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<"+string(SYMBOL_TRUE)+">>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_With_2const) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v; Select BOOLEAN with 6=6"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		Logger::WriteMessage(qe.log.c_str());
		string expected("<<"+string(SYMBOL_TRUE)+">>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Mixed_1) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt");
		QueryTree qt(getQueryTree("stmt s; while w; variable v, v1; Select <v,v1> such that Uses(s, v) and Modifies(w, v1) and Follows(s, w)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<c,c>,<c,g>,<c,h>,<d,c>,<d,g>,<d,h>");
		expected.append(",<e,a>,<e,b>,<e,c>,<e,d>,<e,e>,<e,g>,<e,h>,<f,a>,<f,b>,<f,c>,<f,d>,<f,e>,<f,g>,<f,h>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_Mixed_2) {
		getSampleProgram("..\\tests\\SystemTesting\\11-Source.txt");
		QueryTree qt(getQueryTree("assign a; Select <a,a> such that Modifies(a, \"ghost\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<7,7>,<20,20>,<25,25>,<51,51>,<64,64>,<93,93>,<124,124>,<131,131>,<134,134>>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(SystemTest_11_10) {
		getSampleProgram("..\\tests\\SystemTesting\\11-Source.txt");
		QueryTree qt(getQueryTree("variable v1;variable v2;assign a;assign b;stmt s; Select v2 such that Modifies(a,v1) and Uses(b,v2) and Parent*(s,a)"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<<cho>,<doctor>,<ghost>,<good>,<heir>,<man>,<master>,<psy>,<running>,<sun>>");

		Logger::WriteMessage(qe.log.c_str());

		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(Integration_QE_IfStmt) {
		getSampleProgram("..\\tests\\IntegrationTesting\\Integration-Test-Source2.txt");
		QueryTree qt(getQueryTree("call a; if ifstmt; Select <a,ifstmt> such that Modifies(ifstmt, \"a\")"));
		QueryEvaluator qe;

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<>");
		Assert::AreEqual(expected, actual);
	}
	TEST_METHOD(SystemTest_10_21) {
		getSampleProgram("..\\tests\\SystemTesting\\10-Source.txt");
		QueryTree qt(getQueryTree("call c; if ifstmt; Select <c,ifstmt> such that Parent(3, ifstmt)"));
		QueryEvaluator qe;

		//vector<Clause> group = qt.getSelectedGroups().at(0);
		//TotalCombinationList total(qe.)

		string actual(format(qe.selectQueryResults(qt)));
		string expected("<>");

		Assert::AreEqual(expected, actual);
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
        expectedSelectedClausesTwo = "pattern a1(v2,_\"(x+y)\"_) ";

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

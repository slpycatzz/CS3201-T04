#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "PKB/PKB.h"

using std::map;
using std::set;
using std::string;
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(PKBUnitTest) {
     public:
        TEST_METHOD_INITIALIZE(Initialize) {
            PKB::Clear();
        }

        TEST_METHOD(PKB_ConstantTable) {
            set<string> constants;

            constants.insert("1");
            constants.insert("10");
            constants.insert("100");

            PKB::GenerateConstantTable(constants);

            unsigned int index = PKB::GetConstantIndex("1");

            Assert::IsTrue(PKB::HasConstant("10"));
            Assert::IsFalse(PKB::HasConstant("1000"));

            Assert::AreNotEqual(index, static_cast<unsigned int>(0));
            Assert::AreEqual(PKB::GetConstantValue(index), string("1"));
            Assert::AreEqual(PKB::GetConstantIndex("2000"), static_cast<unsigned int>(0));

            vector<string> vec = PKB::GetAllConstantValues();
            set<string> tableConstants(vec.begin(), vec.end());

            Assert::IsTrue(constants == tableConstants);
        }

        TEST_METHOD(PKB_VariableTable) {
            set<string> variables;

            variables.insert("x");
            variables.insert("xyz");
            variables.insert("xyz12345");

            PKB::GenerateVariableTable(variables);

            unsigned int index = PKB::GetVariableIndex("xyz");

            Assert::IsTrue(PKB::HasVariable("x"));
            Assert::IsFalse(PKB::HasVariable("Xyz"));

            Assert::AreNotEqual(index, static_cast<unsigned int>(0));
            Assert::AreEqual(PKB::GetVariableName(index), string("xyz"));
            Assert::AreEqual(PKB::GetVariableIndex("xyz1234"), static_cast<unsigned int>(0));

            vector<string> vec = PKB::GetAllVariableNames();
            set<string> tableVariables(vec.begin(), vec.end());

            Assert::IsTrue(variables == tableVariables);
        }

        TEST_METHOD(PKB_ProcedureTable) {
            set<string> procedures;

            procedures.insert("Apple");
            procedures.insert("Pineapple");
            procedures.insert("Pen");
            procedures.insert("Kangaroo");

            PKB::GenerateProcedureTable(procedures);

            unsigned int index = PKB::GetProcedureIndex("Pineapple");

            Assert::IsTrue(PKB::HasProcedure("Apple"));
            Assert::IsFalse(PKB::HasProcedure("apple"));

            Assert::AreNotEqual(index, static_cast<unsigned int>(0));
            Assert::AreEqual(PKB::GetProcedureName(index), string("Pineapple"));
            Assert::AreEqual(PKB::GetProcedureIndex("angaroo"), static_cast<unsigned int>(0));

            vector<string> vec = PKB::GetAllProcedures();
            set<string> tableProcedures(vec.begin(), vec.end());

            Assert::IsTrue(procedures == tableProcedures);
        }

        TEST_METHOD(PKB_ControlVariableTable) {
            vector<string> controlVariables;
            map<unsigned int, string> controlVariablesMap;

            controlVariables.push_back("zyx");
            controlVariables.push_back("zzzzz");
            controlVariables.push_back("sleepy");

            controlVariablesMap[10] = "zyx";
            controlVariablesMap[11] = "zzzzz";
            controlVariablesMap[12] = "sleepy";

            PKB::GenerateControlVariableTable(controlVariablesMap);

            Assert::IsTrue(PKB::HasControlVariable("zzzzz"));
            Assert::IsFalse(PKB::HasControlVariable("sleEpy"));

            Assert::IsTrue(PKB::HasControlVariableAtStmtNumber(11, "zzzzz"));
            Assert::IsFalse(PKB::HasControlVariableAtStmtNumber(12, "sleep"));
            Assert::IsFalse(PKB::HasControlVariableAtStmtNumber(10, "sleepy"));

            Assert::AreEqual(PKB::GetControlVariable(10), string("zyx"));
            Assert::AreEqual(PKB::GetControlVariable(13), string(""));

            vector<string> vec = PKB::GetAllControlVariables();
            
            std::sort(vec.begin(), vec.end());
            std::sort(controlVariables.begin(), controlVariables.end());

            Assert::IsTrue(vec == controlVariables);
        }

        TEST_METHOD(PKB_CallTable) {
            map<unsigned int, string> callMap;

            callMap[10] = "abc";
            callMap[11] = "def";
            callMap[12] = "abcd";

            PKB::GenerateCallTable(callMap);

            Assert::AreEqual(PKB::GetCallProcedureName(10), string("abc"));
            Assert::AreEqual(PKB::GetCallProcedureName(11), string("def"));
            Assert::AreEqual(PKB::GetCallProcedureName(12), string("abcd"));
            Assert::AreEqual(PKB::GetCallProcedureName(13), string(""));
        }

        TEST_METHOD(PKB_StmtTable) {
            vector<unsigned int> stmtNumbers;
            map<unsigned int, string> stmtsMap;

            stmtNumbers.push_back(11);
            stmtNumbers.push_back(12);

            stmtsMap[10] = SYMBOL_CALL;
            stmtsMap[11] = SYMBOL_ASSIGN;
            stmtsMap[12] = SYMBOL_ASSIGN;
            stmtsMap[13] = SYMBOL_IF;

            PKB::GenerateStmtTable(stmtsMap);

            Assert::AreEqual(PKB::GetStmtSymbol(10), string(SYMBOL_CALL));
            Assert::AreEqual(PKB::GetStmtSymbol(13), string(SYMBOL_IF));

            vector<unsigned int> vec = PKB::GetSymbolStmtNumbers(ASSIGN);

            std::sort(vec.begin(), vec.end());
            std::sort(stmtNumbers.begin(), stmtNumbers.end());

            Assert::IsTrue(vec == stmtNumbers);
            Assert::IsTrue(vector<unsigned int>() == PKB::GetSymbolStmtNumbers(WHILE));
        }

        TEST_METHOD(PKB_StmtlistTable) {
            vector<unsigned int> stmtNumbers;
            map<unsigned int, string> stmtlistsMap;

            stmtNumbers.push_back(10);
            stmtNumbers.push_back(11);
            stmtNumbers.push_back(12);

            stmtlistsMap[10] = SYMBOL_PROCEDURE;
            stmtlistsMap[11] = SYMBOL_WHILE;
            stmtlistsMap[12] = SYMBOL_WHILE;

            PKB::GenerateStmtlistTable(stmtlistsMap);

            vector<unsigned int> vec = PKB::GetAllStmtlistsStmtNumber();

            std::sort(vec.begin(), vec.end());
            std::sort(stmtNumbers.begin(), stmtNumbers.end());

            Assert::IsTrue(vec == stmtNumbers);
        }
  };
}

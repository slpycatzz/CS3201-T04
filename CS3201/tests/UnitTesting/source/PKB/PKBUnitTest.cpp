#include <algorithm>
#include <string>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "PKB/PKB.h"

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
            PKB::InsertConstantValue(1, "1");
            PKB::InsertConstantValue(2, "10");
            PKB::InsertConstantValue(3, "100");

            Assert::AreNotEqual(PKB::GetConstantIndex("1"), static_cast<unsigned int>(0));
            Assert::AreEqual(PKB::GetConstantIndex("10"), static_cast<unsigned int>(2));
            Assert::AreEqual(PKB::GetConstantIndex("2000"), static_cast<unsigned int>(0));

            Assert::AreEqual(PKB::GetConstantValue(1), string("1"));
            Assert::AreEqual(PKB::GetConstantValue(4), string(""));

            vector<string> constantValues = { "1", "10", "100" };
            vector<string> result = PKB::GetAllConstantValues();

            std::sort(constantValues.begin(), constantValues.end());
            std::sort(result.begin(), result.end());

            Assert::IsTrue(constantValues == result);
        }

        TEST_METHOD(PKB_VariableTable) {
            PKB::InsertVariableName(1, "x");
            PKB::InsertVariableName(2, "xyz");
            PKB::InsertVariableName(3, "xyz12345");

            Assert::AreNotEqual(PKB::GetVariableIndex("xyz"), static_cast<unsigned int>(0));
            Assert::AreEqual(PKB::GetVariableIndex("xyz12345"), static_cast<unsigned int>(3));
            Assert::AreEqual(PKB::GetVariableIndex("xyz1234"), static_cast<unsigned int>(0));

            Assert::AreEqual(PKB::GetVariableName(2), string("xyz"));
            Assert::AreEqual(PKB::GetVariableName(10), string(""));

            vector<string> variableNames = { "x", "xyz", "xyz12345" };

            Assert::IsTrue(variableNames == PKB::GetAllVariableNames());
        }

        TEST_METHOD(PKB_ProcedureTable) {
            PKB::InsertProcedureName(1, "Apple");
            PKB::InsertProcedureName(2, "Pineapple");
            PKB::InsertProcedureName(3, "Pen");
            PKB::InsertProcedureName(4, "Kangaroo");

            Assert::AreNotEqual(PKB::GetProcedureIndex("Pineapple"), static_cast<unsigned int>(0));
            Assert::AreEqual(PKB::GetProcedureIndex("Kangaroo"), static_cast<unsigned int>(4));
            Assert::AreEqual(PKB::GetProcedureIndex("PIneapple"), static_cast<unsigned int>(0));

            Assert::AreEqual(PKB::GetProcedureName(3), string("Pen"));
            Assert::AreEqual(PKB::GetProcedureName(5), string(""));

            vector<unsigned int> procedureIndexes = { 1, 2, 3, 4 };
            vector<unsigned int> result = PKB::GetAllProcedureIndexes();

            std::sort(procedureIndexes.begin(), procedureIndexes.end());
            std::sort(result.begin(), result.end());

            Assert::IsTrue(procedureIndexes == result);
        }

        TEST_METHOD(PKB_ControlVariableTable) {
            PKB::InsertControlVariable(10, 1);
            PKB::InsertControlVariable(11, 2);
            PKB::InsertControlVariable(12, 6);

            Assert::IsTrue(PKB::HasControlVariableIndexAtStmtNumber(11, 2));
            Assert::IsFalse(PKB::HasControlVariableIndexAtStmtNumber(12, 4));
            Assert::IsFalse(PKB::HasControlVariableIndexAtStmtNumber(10, 3));

            Assert::AreEqual(PKB::GetControlVariableIndex(11), static_cast<unsigned int>(2));
            Assert::AreEqual(PKB::GetControlVariableIndex(12), static_cast<unsigned int>(6));
            Assert::AreEqual(PKB::GetControlVariableIndex(1), static_cast<unsigned int>(0));

            vector<unsigned int> controlVariableIndexes = { 1, 2, 6 };

            Assert::IsTrue(controlVariableIndexes == PKB::GetAllControlVariableIndexes());
        }

        TEST_METHOD(PKB_CallTable) {
            PKB::InsertCallStmt(10, 100);
            PKB::InsertCallStmt(11, 200);
            PKB::InsertCallStmt(12, 300);

            Assert::AreEqual(PKB::GetCallStmtProcedureIndex(10), static_cast<unsigned int>(100));
            Assert::AreEqual(PKB::GetCallStmtProcedureIndex(11), static_cast<unsigned int>(200));
            Assert::AreEqual(PKB::GetCallStmtProcedureIndex(12), static_cast<unsigned int>(300));
            Assert::AreEqual(PKB::GetCallStmtProcedureIndex(13), static_cast<unsigned int>(0));
        }

        TEST_METHOD(PKB_StmtTable) {
            PKB::InsertStmt(10, CALL);
            PKB::InsertStmt(11, ASSIGN);
            PKB::InsertStmt(12, ASSIGN);
            PKB::InsertStmt(13, IF);

            Assert::AreEqual(Constants::SymbolToString(PKB::GetStmtSymbol(10)).c_str(), SYMBOL_CALL);
            Assert::AreEqual(Constants::SymbolToString(PKB::GetStmtSymbol(15)).c_str(), SYMBOL_INVALID);

            vector<unsigned int> assignStmtNumbers = { 11, 12 };

            Assert::IsTrue(assignStmtNumbers == PKB::GetSymbolStmtNumbers(ASSIGN));
            Assert::IsTrue(vector<unsigned int>() == PKB::GetSymbolStmtNumbers(WHILE));
        }

        TEST_METHOD(PKB_StmtlistTable) {
            PKB::InsertStmtlist(10, PROCEDURE);
            PKB::InsertStmtlist(11, WHILE);
            PKB::InsertStmtlist(12, WHILE);

            vector<unsigned int> stmtlistStmtNumbers = { 10, 11, 12 };

            Assert::IsTrue(stmtlistStmtNumbers == PKB::GetAllStmtlistsStmtNumber());
        }
  };
}

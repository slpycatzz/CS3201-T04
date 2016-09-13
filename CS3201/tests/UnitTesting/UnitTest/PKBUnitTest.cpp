#include <map>
#include <set>
#include <string>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "PKB/PKB.h"
#include "TreeNode.h"

using std::map;
using std::set;
using std::string;
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
  TEST_CLASS(PKBTest) {
    public:
      TEST_METHOD(AST_set_and_get_root) {
        TreeNode * rootNode = PKB::CreateASTNode(PROCEDURE);
        string expected = rootNode->getValue();
        PKB::SetASTRoot(rootNode);
        string actual = PKB::GetASTRoot()->getValue();
        Assert::AreEqual(expected, actual);
      }
      TEST_METHOD(AST_create_node_for_every_symbol) {
        // TODO(pixelducky): Update this test case to cover all symbols
        TreeNode * programNode = PKB::CreateASTNode(PROGRAM);
        TreeNode * procNode = PKB::CreateASTNode(PROCEDURE);
        TreeNode * whileNode = PKB::CreateASTNode(WHILE, 10);
        TreeNode * ifNode = PKB::CreateASTNode(IF, 20);
        TreeNode * stmtListNode = PKB::CreateASTNode(STMTLIST);
        TreeNode * callNode = PKB::CreateASTNode(CALL, 30, "Panda");
        TreeNode * assignNode = PKB::CreateASTNode(ASSIGN, 40);
        TreeNode * plusNode = PKB::CreateASTNode(PLUS);
        TreeNode * minusNode = PKB::CreateASTNode(MINUS);
        TreeNode * multiplyNode = PKB::CreateASTNode(MULTIPLY);
        TreeNode * varNode = PKB::CreateASTNode(VARIABLE, "x");
        TreeNode * constNode = PKB::CreateASTNode(CONSTANT, "1337");

        string symbol_program("program");
        string symbol_procedure("procedure");
        string symbol_while("while");
        string symbol_if("if");
        string symbol_stmtlist("stmtlist");
        string symbol_call("call");
        string symbol_assign("assign");
        string symbol_plus("+");
        string symbol_minus("-");
        string symbol_multiply("*");
        string symbol_variable("variable");
        string symbol_constant("constant");

        Assert::AreEqual(symbol_program, Constants::SymbolToString(programNode->getSymbol()));
        Assert::AreEqual(symbol_procedure, Constants::SymbolToString(procNode->getSymbol()));
        Assert::AreEqual(symbol_while, Constants::SymbolToString(whileNode->getSymbol()));
        Assert::AreEqual(symbol_if, Constants::SymbolToString(ifNode->getSymbol()));
        Assert::AreEqual(symbol_stmtlist, Constants::SymbolToString(stmtListNode->getSymbol()));
        Assert::AreEqual(symbol_call, Constants::SymbolToString(callNode->getSymbol()));
        Assert::AreEqual(symbol_assign, Constants::SymbolToString(assignNode->getSymbol()));
        Assert::AreEqual(symbol_plus, Constants::SymbolToString(plusNode->getSymbol()));
        Assert::AreEqual(symbol_minus, Constants::SymbolToString(minusNode->getSymbol()));
        Assert::AreEqual(symbol_multiply, Constants::SymbolToString(multiplyNode->getSymbol()));
        Assert::AreEqual(symbol_variable, Constants::SymbolToString(varNode->getSymbol()));
        Assert::AreEqual(symbol_constant, Constants::SymbolToString(constNode->getSymbol()));
      }
      TEST_METHOD(AST_create_node_for_every_symbol_with_value) {
        // TODO(pixelducky): Update this test case to cover all symbols
        TreeNode * procNode = PKB::CreateASTNode(PROCEDURE, "Cat");
        TreeNode * whileNode = PKB::CreateASTNode(WHILE, 10);
        TreeNode * ifNode = PKB::CreateASTNode(IF, 20);
        TreeNode * callNode = PKB::CreateASTNode(CALL, 30, "Panda");
        TreeNode * assignNode = PKB::CreateASTNode(ASSIGN, 40);
        TreeNode * varNode = PKB::CreateASTNode(VARIABLE, "x");
        TreeNode * constNode = PKB::CreateASTNode(CONSTANT, "1337");

        Assert::AreEqual(static_cast<unsigned int>(10), whileNode->getStmtNumber());
        Assert::AreEqual(static_cast<unsigned int>(20), ifNode->getStmtNumber());
        Assert::AreEqual(static_cast<unsigned int>(30), callNode->getStmtNumber());
        Assert::AreEqual(static_cast<unsigned int>(40), assignNode->getStmtNumber());

        Assert::AreEqual(string("Cat"), procNode->getValue());
        Assert::AreEqual(string("Panda"), callNode->getValue());
        Assert::AreEqual(string("x"), varNode->getValue());
        Assert::AreEqual(string("1337"), constNode->getValue());
      }
      TEST_METHOD(AST_check_if_tree_is_valid) {
        // TODO(pixelducky): this test case may be redudant.
        TreeNode * programNode = PKB::CreateASTNode(PROGRAM);
        TreeNode * procNode = PKB::CreateASTNode(PROCEDURE, "Cat");
        TreeNode * stmtListNodeProc = PKB::CreateASTNode(STMTLIST);
        TreeNode * stmtListNodeWhile = PKB::CreateASTNode(STMTLIST);
        TreeNode * stmtListNodeIf = PKB::CreateASTNode(STMTLIST);
        TreeNode * stmtListNodeElse = PKB::CreateASTNode(STMTLIST);
        TreeNode * whileNode = PKB::CreateASTNode(WHILE, 10);
        TreeNode * ifNode = PKB::CreateASTNode(IF, 20);
        TreeNode * callNode = PKB::CreateASTNode(CALL, 30, "Panda");
        TreeNode * assignNode1 = PKB::CreateASTNode(ASSIGN, 40);
        TreeNode * assignNode2 = PKB::CreateASTNode(ASSIGN, 41);

        TreeNode * plusNode = PKB::CreateASTNode(PLUS);
        TreeNode * minusNode = PKB::CreateASTNode(MINUS);
        TreeNode * multiplyNode = PKB::CreateASTNode(MULTIPLY);
        TreeNode * varNodeX = PKB::CreateASTNode(VARIABLE, "x");
        TreeNode * varNodeY = PKB::CreateASTNode(VARIABLE, "y");
        TreeNode * constNode1 = PKB::CreateASTNode(CONSTANT, "1337");
        TreeNode * constNode2 = PKB::CreateASTNode(CONSTANT, "2337");
        TreeNode * constNode3 = PKB::CreateASTNode(CONSTANT, "3337");

        programNode->addChild(procNode);
        procNode->addChild(stmtListNodeProc);

        stmtListNodeProc->addChild(whileNode);
        stmtListNodeProc->addChild(ifNode);
        stmtListNodeProc->addChild(callNode);
        stmtListNodeProc->addChild(assignNode1);

        whileNode->addChild(varNodeX);
        whileNode->addChild(stmtListNodeWhile);

        ifNode->addChild(varNodeX);
        ifNode->addChild(stmtListNodeIf);

        assignNode1->addChild(varNodeX);
        assignNode1->addChild(constNode1);

        assignNode2->addChild(varNodeY);
        assignNode2->addChild(plusNode);
        plusNode->addChild(constNode2);
        plusNode->addChild(constNode3);

        PKB::SetASTRoot(programNode);
        PKB::PrintASTTree();
      }
      TEST_METHOD(ProcTable_set_and_get_procedures) {
        // TODO(pixelducky): can be more exhaustive - check if proc names exist
        vector<string> procedureNames;

        procedureNames.push_back(string("Panda"));
        procedureNames.push_back(string("Cat"));
        procedureNames.push_back(string("Giraffe"));
        procedureNames.push_back(string("Tyson"));
        PKB::GenerateProcedureTable(procedureNames);

        Assert::AreEqual(procedureNames.size(), PKB::GetNumberOfProcedure());
      }
      TEST_METHOD(ProcTable_hasProcedure) {
          vector<string> procedureNames;

          procedureNames.push_back(string("Panda"));
          procedureNames.push_back(string("Cat"));
          procedureNames.push_back(string("Giraffe"));
          procedureNames.push_back(string("Tyson"));
          PKB::GenerateProcedureTable(procedureNames);

          Assert::IsTrue(PKB::HasProcedure("Panda"));
          Assert::IsFalse(PKB::HasProcedure("Elephant"));
      }
      TEST_METHOD(ProcTable_getProcName_and_getProcIndex) {
          vector<string> procedureNames;

          procedureNames.push_back(string("Panda"));
          procedureNames.push_back(string("Cat"));
          procedureNames.push_back(string("Giraffe"));
          procedureNames.push_back(string("Tyson"));
          PKB::GenerateProcedureTable(procedureNames);

          Assert::AreEqual(PKB::GetProcedureName(4), string("Tyson"));
          Assert::AreEqual(PKB::GetProcedureIndex(string("Cat")), static_cast<unsigned int>(2));
      }
      TEST_METHOD(ProcTable_GetAllProcedures) {
          vector<string> procedureNames;

          procedureNames.push_back(string("Panda"));
          procedureNames.push_back(string("Cat"));
          procedureNames.push_back(string("Giraffe"));
          procedureNames.push_back(string("Tyson"));
          PKB::GenerateProcedureTable(procedureNames);

          // CollectionAssert::AreEqual(PKB::GetAllProcedures(), procedureNames);
      }
      TEST_METHOD(StmtTable_set_and_get_stmts) {
          map<unsigned int, string> testStmtTable;

          testStmtTable[static_cast<unsigned int>(1)] = string("y = x + 3;");
          testStmtTable[static_cast<unsigned int>(2)] = string("x++;");
          PKB::GenerateStmtTable(testStmtTable);
          Assert::AreEqual(testStmtTable.size(), static_cast<unsigned int>(2));
      }
      TEST_METHOD(StmtTable_getStmtSymbol) {
          map<unsigned int, string> testStmtTable;

          testStmtTable[static_cast<unsigned int>(1)] = string("y = x + 3;");
          testStmtTable[static_cast<unsigned int>(2)] = string("x++;");
          PKB::GenerateStmtTable(testStmtTable);

          Assert::AreEqual(PKB::GetStmtSymbol(static_cast<unsigned int>(1)), string("=+"));
      }
      TEST_METHOD(StmtTable_GetSymbolsStmtNos) {
          map<unsigned int, string> testStmtTable;

          testStmtTable[static_cast<unsigned int>(1)] = string("y = x + 3;");
          testStmtTable[static_cast<unsigned int>(2)] = string("x++;");
          PKB::GenerateStmtTable(testStmtTable);

          vector<unsigned int> stmts;
          stmts.push_back(1);
          stmts.push_back(2);

          // Assert::AreEqual(PKB::GetSymbolStmtNumbers("+"), stmts);
          // Assert::AreNotEqual(PKB::GetSymbolStmtNumbers("*"), stmts);

          // Assert::AreEqual(PKB::GetSymbolStmtNumbers(PLUS), stmts);
          // Assert::AreNotEqual(PKB::GetSymbolStmtNumbers(MINUS), stmts);
      }
      TEST_METHOD(AssignTableTest_setget) {
          map<unsigned int, TreeNode*> testAssgTable;
          TreeNode * ifNode = PKB::CreateASTNode(IF, 20);
          TreeNode * assignNode1 = PKB::CreateASTNode(ASSIGN, 40);
          TreeNode * assignNode2 = PKB::CreateASTNode(ASSIGN, 41);

          testAssgTable[static_cast<unsigned int>(20)] = ifNode;
          testAssgTable[static_cast<unsigned int>(40)] = assignNode1;
          testAssgTable[static_cast<unsigned int>(41)] = assignNode2;

          PKB::GenerateAssignTable(testAssgTable);
          Assert::AreEqual(PKB::GetNumberOfAssign(), static_cast<unsigned int>(2));
      }
      TEST_METHOD(AssignTableTest_getNode) {
          map<unsigned int, TreeNode*> testAssgTable;
          TreeNode * ifNode = PKB::CreateASTNode(IF, 20);
          TreeNode * assignNode1 = PKB::CreateASTNode(ASSIGN, 40);
          TreeNode * assignNode2 = PKB::CreateASTNode(ASSIGN, 41);

          testAssgTable[static_cast<unsigned int>(20)] = ifNode;
          testAssgTable[static_cast<unsigned int>(40)] = assignNode1;
          testAssgTable[static_cast<unsigned int>(41)] = assignNode2;

          PKB::GenerateAssignTable(testAssgTable);
          // Assert::AreEqual(PKB::GetAssignTreeNode(static_cast<unsigned int>(40)), assignNode1);
          // Assert::AreNotEqual(PKB::GetAssignTreeNode(static_cast<unsigned int>(20)), ifNode);
      }
      TEST_METHOD(AssignTableTest_getAllNodes) {
          vector<TreeNode*> testSet;
          map<unsigned int, TreeNode*> testAssgTable;

          TreeNode * ifNode = PKB::CreateASTNode(IF, 20);
          TreeNode * assignNode1 = PKB::CreateASTNode(ASSIGN, 40);
          TreeNode * assignNode2 = PKB::CreateASTNode(ASSIGN, 41);
          testAssgTable[static_cast<unsigned int>(20)] = ifNode;
          testAssgTable[static_cast<unsigned int>(40)] = assignNode1;
          testAssgTable[static_cast<unsigned int>(41)] = assignNode2;

          PKB::GenerateAssignTable(testAssgTable);

          testSet.push_back(assignNode1);
          testSet.push_back(assignNode2);

          // Assert::AreEqual(PKB::GetAllAssignTreeNodes(), testSet);
      }
      TEST_METHOD(ModifiesTableTest_Generate) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // 25 ...
          // }
          map<unsigned int, set<string>> testModTable;
          set<string> testSet, testSet1;
          testSet.insert(string("y"));
          testModTable[static_cast<unsigned int>(23)] = testSet;
          testSet1.insert(string("x"));
          testModTable[static_cast<unsigned int>(24)] = testSet1;

          PKB::GenerateModifiesTable(testModTable);

          Assert::IsTrue(PKB::IsModifies(static_cast<unsigned int>(23), string("y")));
          Assert::IsFalse(PKB::IsModifies(static_cast<unsigned int>(23), string("x")));
          Assert::IsTrue(PKB::IsModifies(static_cast<unsigned int>(24), string("x")));
      }
      TEST_METHOD(ModifiesTableTest_getModVariables) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // 25 ...
          // }
          map<unsigned int, set<string>> testModTable;
          set<string> testSet, testSet1;
          testSet.insert(string("y"));
          testModTable[static_cast<unsigned int>(23)] = testSet;
          testSet1.insert(string("x"));
          testModTable[static_cast<unsigned int>(24)] = testSet1;

          PKB::GenerateModifiesTable(testModTable);
          // Assert::AreEqual(PKB::GetModifiedVariables(static_cast<unsigned int>(23)), testSet);
          // Assert::AreNotEqual(PKB::GetModifiedVariables(static_cast<unsigned int>(24)), testSet);
      }
      TEST_METHOD(ModifiesTableTest_getStmtNo) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // 25 ...
          // }
          map<unsigned int, set<string>> testModTable;
          set<string> testSet, testSet1;
          testSet.insert(string("y"));
          testModTable[static_cast<unsigned int>(23)] = testSet;
          testSet1.insert(string("x"));
          testModTable[static_cast<unsigned int>(24)] = testSet1;
          set<unsigned int> testX;
          testX.insert(24);

          PKB::GenerateModifiesTable(testModTable);

          // Assert::AreEqual(PKB::GetStmtNumberModifying(string("x")), testX);
      }
      TEST_METHOD(ModifiesTableTest_generateModProcTable) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // }
          // 25 TIGER {
          // 26 z = y * 6;
          // 27 y--;

          map<string, set<string>> testModProcTable;
          set<string> testSet, testSet1, testSet2;
          testSet.insert(string("y"));
          testModProcTable[string("PANDA")] = testSet;
          testModProcTable[string("TIGER")] = testSet;
          testSet1.insert(string("x"));
          testModProcTable[string("PANDA")] = testSet1;
          testSet2.insert(string("z"));
          testModProcTable[string("TIGER")] = testSet2;
          PKB::GenerateModifiesProcedureTable(testModProcTable);

          Assert::AreEqual(testModProcTable.size(), static_cast<unsigned int>(4));
      }
      TEST_METHOD(ModifiesTableTest_isModProc) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // }
          // 25 TIGER {
          // 26 z = y * 6;
          // 27 y--;

          map<string, set<string>> testModProcTable;
          set<string> testSet, testSet1, testSet2;
          testSet.insert(string("y"));
          testModProcTable[string("PANDA")] = testSet;
          testModProcTable[string("TIGER")] = testSet;
          testSet1.insert(string("x"));
          testModProcTable[string("PANDA")] = testSet1;
          testSet2.insert(string("z"));
          testModProcTable[string("TIGER")] = testSet2;
          PKB::GenerateModifiesProcedureTable(testModProcTable);

          Assert::IsTrue(PKB::IsModifiesProcedure(string("PANDA"), string("y")));
          Assert::IsFalse(PKB::IsModifiesProcedure(string("PANDA"), string("z")));
          Assert::IsTrue(PKB::IsModifiesProcedure(string("TIGER"), string("y")));
      }
      TEST_METHOD(ModifiesTableTest_get_proc_and_var) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // }
          // 25 TIGER {
          // 26 z = y * 6;
          // 27 y--;

          map<string, set<string>> testModProcTable;
          set<string> testSet, testSet1, testSet2, testSetPanda;
          testSet.insert(string("y"));
          testModProcTable[string("PANDA")] = testSet;
          testModProcTable[string("TIGER")] = testSet;
          testSet1.insert(string("x"));
          testModProcTable[string("PANDA")] = testSet1;
          testSet2.insert(string("z"));
          testModProcTable[string("TIGER")] = testSet2;
          PKB::GenerateModifiesProcedureTable(testModProcTable);

          testSet1.insert(string("y"));
          // Assert::AreEqual(PKB::GetProcedureModifiedVariables(string("PANDA")), testSet1);
          testSetPanda.insert(string("PANDA"));
          // Assert::AreEqual(PKB::GetProceduresNameModifying("x"), testSetPanda);
      }
      TEST_METHOD(UsesTableTest) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // }
          // 25 TIGER {
          // 26 z = (x - y) * 6;
          // 27 y--;
          map<unsigned int, set<string>> testUsesTable;
          set<string> testSet23, testSet26;

          testSet23.insert(string("x"));
          testUsesTable[static_cast<unsigned int>(23)] = testSet23;

          testSet26.insert(string("x"));
          testSet26.insert(string("y"));
          testUsesTable[static_cast<unsigned int>(26)] = testSet26;

          PKB::GenerateUsesTable(testUsesTable);

          Assert::AreEqual(testUsesTable.size(), static_cast<unsigned int>(2));

          Assert::IsTrue(PKB::IsUses(23, "x"));
          Assert::IsFalse(PKB::IsUses(24, "x"));

          // Assert::AreEqual(PKB::GetUsedVariables(26), testSet26);

          set<unsigned int> testSet2326;
          testSet2326.insert(23);
          testSet2326.insert(26);
          // Assert::AreEqual(PKB::GetStmtNumberUsing("x"), testSet2326);
      }
      TEST_METHOD(UsesProcedureTableTest) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // }
          // 25 TIGER {
          // 26 z = (x - y) * 6;
          // 27 y--;
          map<string, set<string>> testUsesProcTable;
          set<string> testSetPanda, testSetTiger;

          testSetPanda.insert("x");
          testSetTiger.insert("x");
          testSetTiger.insert("y");

          testUsesProcTable["PANDA"] = testSetPanda;
          testUsesProcTable["TIGER"] = testSetTiger;
          PKB::GenerateUsesProcedureTable(testUsesProcTable);

          Assert::AreEqual(testUsesProcTable.size(), static_cast<unsigned int>(2));

          Assert::IsTrue(PKB::IsUsesProcedure("PANDA", "x"));
          Assert::IsFalse(PKB::IsUsesProcedure("PANDA", "y"));

          // Assert::AreEqual(PKB::GetProcedureUsedVariables("TIGER"), testSetTiger);

          set<string> testProcSet;
          testProcSet.insert("PANDA");
          testProcSet.insert("TIGER");
          // Assert::AreEqual(PKB::GetProceduresNameUsing("x"), testProcSet);
      }
      TEST_METHOD(ParentTableTest) {
          // sample line
          // 22 PANDA {
          // 23     while(i){
          // 24         y = x - 5;
          // 25         if y then {
          // 26             Call TIGER;
          // 27              x++; }
          // 28         i--;}
          //    }

          map<unsigned int, set<unsigned int>> testParentTable;  // <parent, child>
          set<unsigned int> testSet23, testSet25;
          testSet23.insert(24);
          testSet23.insert(25);
          testSet23.insert(28);
          testSet25.insert(26);
          testSet25.insert(27);

          testParentTable[23] = testSet23;
          testParentTable[25] = testSet25;
          PKB::GenerateParentTable(testParentTable);

          Assert::IsTrue(PKB::IsParent(23, 24));
          Assert::IsFalse(PKB::IsParent(24, 23));
          Assert::IsTrue(PKB::IsParent(25, 26));
          Assert::IsFalse(PKB::IsParent(23, 26));

          Assert::IsTrue(PKB::IsParentTransitive(23, 26));
          Assert::IsFalse(PKB::IsParentTransitive(26, 27));

          Assert::AreEqual(PKB::GetParent(26), static_cast<unsigned int>(25));
          Assert::AreNotEqual(PKB::GetParent(26), static_cast<unsigned int>(23));

          // Assert::AreEqual(PKB::GetParentsTransitive(24), testSet23);
          set<unsigned int> testSet2325, testSetAll;
          testSet2325.insert(23);
          testSet2325.insert(25);
          // Assert::AreEqual(PKB::GetParentsTransitive(27), testSet2325);

          // Assert::AreEqual(PKB::GetChildren(23), testSet23);
          // testSetAll.insert(24, 28);                                           // Line causing indirection problem.
          // Assert::AreEqual(PKB::GetChildrenTransitive(23), testSetAll);
      }
      TEST_METHOD(FollowsTableTest) {
          // sample line
          // 22 PANDA {
          // 23     while(i){
          // 24         y = x - 5;
          // 25         if y then {
          // 26             Call TIGER;
          // 27              x++; }
          // 28         i--;
          // 29         y--;}
          //    }
          map<unsigned int, unsigned int> testFollowsTable;  // <before, after>
          testFollowsTable[24] = 25;
          testFollowsTable[26] = 27;
          testFollowsTable[25] = 28;
          testFollowsTable[28] = 29;

          PKB::GenerateFollowsTable(testFollowsTable);

          Assert::AreEqual(testFollowsTable.size(), static_cast<unsigned int>(3));

          Assert::IsTrue(PKB::IsFollows(24, 25));   // follows immediately
          Assert::IsTrue(PKB::IsFollows(25, 28));   // same nesting level, and same stmtList
          Assert::IsFalse(PKB::IsFollows(25, 24));  // not the reverse
          Assert::IsFalse(PKB::IsFollows(24, 26));  // different nesting level
          Assert::IsFalse(PKB::IsFollows(24, 28));  // not immediately after

          Assert::IsTrue(PKB::IsFollowsTransitive(24, 28));
          Assert::IsTrue(PKB::IsFollowsTransitive(28, 29));

          Assert::AreEqual(PKB::GetFollows(24), static_cast<unsigned int>(25));
          Assert::AreEqual(PKB::GetFollowing(29), static_cast<unsigned int>(28));

          set<unsigned int> testSetFollows24, testSetFollowing27;
          testSetFollows24.insert(25);
          testSetFollows24.insert(28);
          testSetFollows24.insert(29);
          testSetFollowing27.insert(26);

          // Assert::AreEqual(PKB::GetFollowsTransitive(24), testSetFollows24);
          // Assert::AreEqual(PKB::GetFollowingTransitive(26), testSetFollowing27);
      }
  };
}  // namespace UnitTest

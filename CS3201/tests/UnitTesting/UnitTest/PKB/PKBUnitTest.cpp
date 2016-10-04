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
        PKB::Clear();
        TreeNode * rootNode = PKB::CreateASTNode(PROCEDURE);
        string expected = rootNode->getValue();
        PKB::SetASTRoot(rootNode);
        string actual = PKB::GetASTRoot()->getValue();
        Assert::AreEqual(expected, actual);
      }
      TEST_METHOD(AST_create_node_for_every_symbol) {
        // TODO(pixelducky): Update this test case to cover all symbols
        PKB::Clear();
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
        PKB::Clear();
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
        PKB::Clear();
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
        PKB::Clear();

        vector<string> procedureNames;

        procedureNames.push_back(string("Panda"));
        procedureNames.push_back(string("Cat"));
        procedureNames.push_back(string("Giraffe"));
        procedureNames.push_back(string("Tyson"));
        PKB::GenerateProcedureTable(procedureNames);

        Assert::IsTrue(PKB::HasProcedure("Panda"));
        Assert::IsTrue(PKB::HasProcedure("Cat"));
        Assert::IsTrue(PKB::HasProcedure("Giraffe"));
        Assert::IsTrue(PKB::HasProcedure("Tyson"));
        Assert::IsFalse(PKB::HasProcedure("Bevin"));
        Assert::AreEqual(string("Tyson"), PKB::GetProcedureName(4));
        Assert::AreEqual(unsigned int(2), PKB::GetProcedureIndex(string("Cat")));
      }
      TEST_METHOD(StmtTable_set_and_get_stmtSymbol) {
          PKB::Clear();
          map<unsigned int, string> stmts;
          stmts.emplace(10, "call");
          stmts.emplace(11, "assign");
          stmts.emplace(12, "assign");
          stmts.emplace(13, "if");
          stmts.emplace(14, "while");

          PKB::GenerateStmtTable(stmts);
          Assert::AreEqual(string("call"), PKB::GetStmtSymbol(10));
          Assert::AreEqual(string("assign"), PKB::GetStmtSymbol(11));
          Assert::AreEqual(string("assign"), PKB::GetStmtSymbol(12));
          Assert::AreEqual(string("if"), PKB::GetStmtSymbol(13));
          Assert::AreEqual(string("while"), PKB::GetStmtSymbol(14));
      }
      TEST_METHOD(StmtTable_GetSymbolsStmtNos) {
          PKB::Clear();
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
      TEST_METHOD(AssignTableTest_set_and_get) {
          PKB::Clear();
          map<unsigned int, TreeNode*> assignments;
          TreeNode * assignNode1 = PKB::CreateASTNode(ASSIGN, 40);
          TreeNode * assignNode2 = PKB::CreateASTNode(ASSIGN, 41);
          assignments.insert(std::make_pair(assignNode1->getStmtNumber(), assignNode1));
          assignments.insert(std::make_pair(assignNode2->getStmtNumber(), assignNode2));

          PKB::GenerateAssignTable(assignments);
          Assert::AreEqual(assignNode1->getStmtNumber(), PKB::GetAssignTreeNode(40)->getStmtNumber());
          Assert::AreEqual(assignNode2->getStmtNumber(), PKB::GetAssignTreeNode(41)->getStmtNumber());
      }
      TEST_METHOD(ModifiesTable_set_and_get) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // 25 ...
          // }
          PKB::Clear();
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
      TEST_METHOD(ModifiesProcedureTable_set_and_get) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // }
          // 25 TIGER {
          // 26 z = y * 6;
          // 27 y--;
          PKB::Clear();
          map<string, set<string>> testModProcTable;
          set<string> testSet, testSet1, testSet2;
          testSet.insert(string("y"));
          testSet1.insert(string("x"));
          testModProcTable[string("PANDA")] = testSet;
          testModProcTable[string("TIGER")] = testSet1;

          PKB::GenerateModifiesProcedureTable(testModProcTable);

          Assert::IsTrue(PKB::IsModifiesProcedure(string("PANDA"), string("y")));
          Assert::IsTrue(PKB::IsModifiesProcedure(string("TIGER"), string("x")));
          Assert::IsFalse(PKB::IsModifiesProcedure(string("TIGER"), string("z")));
          Assert::AreEqual(1, int(PKB::GetProceduresNameModifying("y").size()));
      }
      TEST_METHOD(UsesTable_set_and_get) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // }
          // 25 TIGER {
          // 26 z = (x - y) * 6;
          // 27 y--;
          PKB::Clear();
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
      TEST_METHOD(UsesProcedureTable_set_and_get) {
          // sample line
          // 22 PANDA {
          // 23 y = x - 5;
          // 24 x++;
          // }
          // 25 TIGER {
          // 26 z = (x - y) * 6;
          // 27 y--;
          PKB::Clear();
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
      TEST_METHOD(ParentTable_set_and_get) {
          // sample line
          // 22 PANDA {
          // 23     while(i){
          // 24         y = x - 5;
          // 25         if y then {
          // 26             Call TIGER;
          // 27              x++; }
          // 28         i--;}
          //    }
          PKB::Clear();
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
          Assert::IsFalse(PKB::IsParent(25, 25)); 

          Assert::IsTrue(PKB::IsParentTransitive(23, 25));
      }
      TEST_METHOD(FollowsTable_set_and_get) {
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
          PKB::Clear();
          map<unsigned int, unsigned int> testFollowsTable;  // <before, after>
          testFollowsTable[24] = 25;
          testFollowsTable[26] = 27;
          testFollowsTable[25] = 28;
          testFollowsTable[28] = 29;

          PKB::GenerateFollowsTable(testFollowsTable);
          Assert::IsTrue(PKB::IsFollows(24, 25));   // follows immediately
          Assert::IsTrue(PKB::IsFollows(25, 28));   // same nesting level, and same stmtList
          Assert::IsFalse(PKB::IsFollows(25, 24));  // not the reverse
          //new test case for Follows (s,s), should assert false
          Assert::IsFalse(PKB::IsFollows(25, 25));
          Assert::IsFalse(PKB::IsFollows(24, 26));  // different nesting level
          Assert::IsFalse(PKB::IsFollows(24, 28));  // not immediately after
          Assert::IsTrue(PKB::IsFollowsTransitive(24, 28));
          Assert::IsTrue(PKB::IsFollowsTransitive(28, 29));
          Assert::AreEqual(unsigned int(25), PKB::GetFollowing(24));
          Assert::AreEqual(unsigned int(0), PKB::GetFollowing(29));

          Assert::AreEqual(unsigned int(0), PKB::GetFollows(10));
      }
  };
}  // namespace UnitTest

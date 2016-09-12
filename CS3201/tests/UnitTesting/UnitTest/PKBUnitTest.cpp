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
        //TODO(pixelducky): Update this test case to cover all symbols
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
        //TODO(pixelducky): Update this test case to cover all symbols
        TreeNode * procNode = PKB::CreateASTNode(PROCEDURE, "Cat");
        TreeNode * whileNode = PKB::CreateASTNode(WHILE, 10);
        TreeNode * ifNode = PKB::CreateASTNode(IF, 20);
        TreeNode * callNode = PKB::CreateASTNode(CALL, 30, "Panda");
        TreeNode * assignNode = PKB::CreateASTNode(ASSIGN, 40);
        TreeNode * varNode = PKB::CreateASTNode(VARIABLE, "x");
        TreeNode * constNode = PKB::CreateASTNode(CONSTANT, "1337");


        Assert::AreEqual(unsigned int(10), whileNode->getStmtNumber());
        Assert::AreEqual(unsigned int(20), ifNode->getStmtNumber());
        Assert::AreEqual(unsigned int(30), callNode->getStmtNumber());
        Assert::AreEqual(unsigned int(40), assignNode->getStmtNumber());
        
        Assert::AreEqual(string("Cat"), procNode->getValue());
        Assert::AreEqual(string("Panda"), callNode->getValue());
        Assert::AreEqual(string("x"), varNode->getValue());
        Assert::AreEqual(string("1337"), constNode->getValue());
      }
      TEST_METHOD(AST_check_if_tree_is_valid) {
        //TODO(pixelducky): this test case may be redudant.
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
        //TODO(pixelducky): can be more exhaustive - check if proc names exist
        vector<string> procedureNames;
        
        procedureNames.push_back(string("Panda"));
        procedureNames.push_back(string("Cat"));
        procedureNames.push_back(string("Giraffe"));
        procedureNames.push_back(string("Tyson"));
        
        PKB::GenerateProcedureTable(procedureNames);
        Assert::AreEqual(procedureNames.size(), PKB::GetNumberOfProcedure());
      }
      TEST_METHOD(StmtTableTest) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(AssignTableTest) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(ModifiesTableTest) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(UsesTableTest) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(ParentTableTest) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(FollowsTableTest) {
        Assert::AreEqual(true, false);
      }
  };
}

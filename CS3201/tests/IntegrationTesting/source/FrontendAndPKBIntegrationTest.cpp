#include <string>

#include "stdafx.h"
#include "targetver.h"

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "PKB/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::string;

namespace IntegrationTesting {
    TEST_CLASS(FrontendAndPKBIntegrationTest) {
        FrontendParser frontendParser;
        string testcasesDirectory = "..\\tests\\IntegrationTesting\\testcases\\FrontendAndPKB\\";

        TEST_METHOD_INITIALIZE(Initialize) {
            frontendParser = FrontendParser();
            PKB::Clear();
        }
		TEST_METHOD(Integration_Parser_PKB_Is_Parent_Valid) {
			string fileName = "Parent.txt";
			frontendParser.parseProgram(testcasesDirectory + fileName);

			//EP: invalid line number
			Assert::IsFalse(PKB::IsParent(0, 1));
			Assert::IsFalse(PKB::IsParent(1, -1));
			Assert::IsFalse(PKB::IsParent(1, 999));

			//EP: different nesting level
			Assert::IsFalse(PKB::IsParent(1, 7));
			Assert::IsFalse(PKB::IsParent(26, 39)); //39 is not inside 26

			//EP: 9 is not a container
			Assert::IsFalse(PKB::IsParent(9, 15));

			//EP: nested stmtLst
			Assert::IsFalse(PKB::IsParent(1, 5)); //3 level nesting
			Assert::IsFalse(PKB::IsParent(10, 15)); //3 level nesting
			Assert::IsFalse(PKB::IsParent(10, 16)); //3 level nesting
			Assert::IsFalse(PKB::IsParent(10, 21)); //2 level nesting

			//EP: invalid cases
			Assert::IsFalse(PKB::IsParent(7, 1));

			//EP: valid cases
			Assert::IsTrue(PKB::IsParent(1, 6));
			Assert::IsTrue(PKB::IsParent(10, 12)); //if else
			Assert::IsTrue(PKB::IsParent(10, 19)); //if else
			Assert::IsTrue(PKB::IsParent(26, 29));
		}
		TEST_METHOD(Integration_Parser_PKB_Is_Parent_Transitive_Valid) {
			string fileName = "Parent.txt";
			frontendParser.parseProgram(testcasesDirectory + fileName);

			//EP: invalid line number
			Assert::IsFalse(PKB::IsParentTransitive(0, 1));
			Assert::IsFalse(PKB::IsParentTransitive(1, -1));
			Assert::IsFalse(PKB::IsParentTransitive(1, 999));

			//EP: different nesting level
			Assert::IsFalse(PKB::IsParentTransitive(1, 7));
			Assert::IsFalse(PKB::IsParentTransitive(26, 39));

			//EP: 9 is not a container
			Assert::IsFalse(PKB::IsParentTransitive(9, 15));

			//EP: invalid cases
			Assert::IsFalse(PKB::IsParentTransitive(7, 1));

			//EP: valid cases
			Assert::IsTrue(PKB::IsParentTransitive(1, 6));
			Assert::IsTrue(PKB::IsParentTransitive(10, 12)); //if else
			Assert::IsTrue(PKB::IsParentTransitive(10, 19)); //if else
			Assert::IsTrue(PKB::IsParentTransitive(26, 29));

			//EP: nested stmtLst
			Assert::IsTrue(PKB::IsParentTransitive(1, 5)); //3 level nesting
			Assert::IsTrue(PKB::IsParentTransitive(10, 15)); //3 level nesting
			Assert::IsTrue(PKB::IsParentTransitive(10, 16)); //3 level nesting
			Assert::IsTrue(PKB::IsParentTransitive(10, 21)); //2 level nesting
		}
		TEST_METHOD(Integration_Parser_PKB_Is_Follows_Valid) {
			string fileName = "Follows.txt";
			frontendParser.parseProgram(testcasesDirectory + fileName);

			//EP: invalid line number
			Assert::IsFalse(PKB::IsFollows(0, 1));
			Assert::IsFalse(PKB::IsFollows(1, -1));
			Assert::IsFalse(PKB::IsFollows(1, 999));

			//EP: different nesting level
			Assert::IsFalse(PKB::IsFollows(1, 5));
			Assert::IsFalse(PKB::IsFollows(9, 15));
			Assert::IsFalse(PKB::IsFollows(25, 38));

			//EP: invalid cases. indirect and same stmtLst
			Assert::IsFalse(PKB::IsFollows(25, 39)); 
			Assert::IsFalse(PKB::IsFollows(9, 24));

			//EP: valid cases
			Assert::IsTrue(PKB::IsFollows(1, 7));
			Assert::IsTrue(PKB::IsFollows(2, 6));
			Assert::IsTrue(PKB::IsFollows(25, 26));
		}
		TEST_METHOD(Integration_Parser_PKB_Is_Follows_Transitive_Valid) {
			string fileName = "Follows.txt";
			frontendParser.parseProgram(testcasesDirectory + fileName);

			//EP: invalid line number
			Assert::IsFalse(PKB::IsFollowsTransitive(0, 1));
			Assert::IsFalse(PKB::IsFollowsTransitive(1, -1));
			Assert::IsFalse(PKB::IsFollowsTransitive(1, 999));

			//EP: different nesting level
			Assert::IsFalse(PKB::IsFollowsTransitive(1, 5));
			Assert::IsFalse(PKB::IsFollowsTransitive(9, 15));
			Assert::IsFalse(PKB::IsFollowsTransitive(25, 38));

			//EP: valid cases
			Assert::IsTrue(PKB::IsFollowsTransitive(1, 7));
			Assert::IsTrue(PKB::IsFollowsTransitive(1, 8)); //indirect and same stmtLst
			Assert::IsTrue(PKB::IsFollowsTransitive(2, 6));
			Assert::IsTrue(PKB::IsFollowsTransitive(9, 24));
			Assert::IsTrue(PKB::IsFollowsTransitive(25, 26));
			Assert::IsTrue(PKB::IsFollowsTransitive(25, 39)); //indirect and same stmtLst
		}
		TEST_METHOD(Integration_Parser_PKB_Is_Next_Valid) {
			string fileName = "Next.txt";
			frontendParser.parseProgram(testcasesDirectory + fileName);

			//EP: invalid line number
			Assert::IsFalse(PKB::IsNext(0, 1));
			Assert::IsFalse(PKB::IsNext(1, -1));
			Assert::IsFalse(PKB::IsNext(1, 999));

			//EP: lines in other procedures
			Assert::IsFalse(PKB::IsNext(1, 5));
			Assert::IsFalse(PKB::IsNext(23, 24));
			Assert::IsFalse(PKB::IsNext(42, 43));

			//EP: flow will never exit a while loop container without looping back first
			Assert::IsFalse(PKB::IsNext(22, 23));

			//EP: valid cases
			Assert::IsTrue(PKB::IsNext(1, 2));
			Assert::IsTrue(PKB::IsNext(2, 3));
			Assert::IsTrue(PKB::IsNext(3, 4));

			//EP: loopback in while container
			Assert::IsTrue(PKB::IsNext(4, 3));
			Assert::IsTrue(PKB::IsNext(3, 2));
			Assert::IsTrue(PKB::IsNext(2, 1));

			//EP: control flow in if container
			Assert::IsTrue(PKB::IsNext(26, 27));
			Assert::IsTrue(PKB::IsNext(26, 39));
		}
		TEST_METHOD(Integration_Parser_PKB_Is_Next_Transitive_Valid) {
			string fileName = "Next.txt";
			frontendParser.parseProgram(testcasesDirectory + fileName);

			//EP: invalid line number
			Assert::IsFalse(PKB::IsNextTransitive(0, 1));
			Assert::IsFalse(PKB::IsNextTransitive(1, -1));
			Assert::IsFalse(PKB::IsNextTransitive(1, 999));

			//EP: lines in other procedures
			Assert::IsFalse(PKB::IsNextTransitive(1, 5));
			Assert::IsFalse(PKB::IsNextTransitive(23, 24));
			Assert::IsFalse(PKB::IsNextTransitive(42, 43));

			//EP: 22->21->13->11->7->23
			Assert::IsTrue(PKB::IsNextTransitive(22, 23));

			//EP: valid for normal cases of Next
			Assert::IsTrue(PKB::IsNextTransitive(1, 2));
			Assert::IsTrue(PKB::IsNextTransitive(2, 3));
			Assert::IsTrue(PKB::IsNextTransitive(3, 4));

			//EP: loopback in while container
			Assert::IsTrue(PKB::IsNextTransitive(4, 3));
			Assert::IsTrue(PKB::IsNextTransitive(3, 2));
			Assert::IsTrue(PKB::IsNextTransitive(2, 1));

			//EP: transitive loopback in while container
			Assert::IsTrue(PKB::IsNextTransitive(22, 22));

			//EP: control flow in if container
			Assert::IsTrue(PKB::IsNextTransitive(26, 27));
			Assert::IsTrue(PKB::IsNextTransitive(26, 39));
			Assert::IsTrue(PKB::IsNextTransitive(26, 32));
			Assert::IsTrue(PKB::IsNextTransitive(26, 33));
		}
    };
}
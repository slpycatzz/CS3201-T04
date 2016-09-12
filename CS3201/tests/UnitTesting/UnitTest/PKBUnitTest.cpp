#include <string>
#include <vector>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
  TEST_CLASS(PKBTest) {
    public:
      TEST_METHOD(ASTTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(ConstantTableTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(VarTableTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(ProcTableTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(StmtTableTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(AssignTableTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(ModifiesTableTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(UsesTableTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(ParentTableTestMethod) {
        Assert::AreEqual(true, false);
      }
      TEST_METHOD(FollowsTableTestMethod) {
        Assert::AreEqual(true, false);
      }
  };
}   // namespace UnitTest

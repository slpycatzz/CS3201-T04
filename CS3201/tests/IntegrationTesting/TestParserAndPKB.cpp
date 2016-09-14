#include <string>
#include <vector>
#include <stdlib.h>

#include "stdafx.h"
#include "targetver.h"

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "PKB/PKB.h"

using std::string;
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
  TEST_CLASS(IntegrationTest) {
    public:
      TEST_METHOD(Parser_and_PKB) {
        string fileName = "..\\tests\\IntegrationTesting\\Integration-Test-Source1.txt";
        parse(fileName);
        
        Assert::AreEqual(42, int(PKB::GetNumberOfAssign()));
        Assert::AreEqual(0, int(PKB::GetNumberOfCall()));   
        Assert::AreEqual(0, int(PKB::GetNumberOfIf()));
        Assert::AreEqual(1, int(PKB::GetNumberOfProcedure()));
        Assert::AreEqual(7, int(PKB::GetNumberOfWhile()));
      }

      void parse(string filePath) {
        FrontendParser frontendParser = FrontendParser();
        
        try {
          frontendParser.parseProgram(filePath);

        } catch(std::exception& ex) {
          Logger::WriteMessage("Parse Error");
          Logger::WriteMessage(ex.what());
          exit(EXIT_FAILURE);
        }
      }
  };
}  // namespace IntegrationTesting

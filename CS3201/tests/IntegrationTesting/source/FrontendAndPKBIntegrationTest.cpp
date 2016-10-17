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



    };
}
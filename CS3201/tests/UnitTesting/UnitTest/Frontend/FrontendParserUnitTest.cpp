#include "stdafx.h"
#include "targetver.h"

#include "Frontend/FrontendParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
    TEST_CLASS(FrontendParserTest) {
public:
    TEST_METHOD(FrontendParser_EmptyStmtList) {
        FrontendParser frontendParser = FrontendParser();
        std::string filePath = "..\\tests\\UnitTesting\\UnitTest\\testcases\\Frontend\\test-empty-stmtlist.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), "Frontend parser encountered a logic error in the program.");
        }
    }

    TEST_METHOD(FrontendParser_ExtraBrackets) {
        FrontendParser frontendParser = FrontendParser();
        std::string filePath = "..\\tests\\UnitTesting\\UnitTest\\testcases\\Frontend\\test-extra-brackets.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), "Frontend parser encountered a syntax error in the program.");
        }
    }

    TEST_METHOD(FrontendParser_InvalidProcedureName) {
        FrontendParser frontendParser = FrontendParser();
        std::string filePath = "..\\tests\\UnitTesting\\UnitTest\\testcases\\Frontend\\test-invalid-procedure-name.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), "Frontend parser encountered a syntax error in the program.");
        }
    }

    TEST_METHOD(FrontendParser_InvalidVarName) {
        FrontendParser frontendParser = FrontendParser();
        std::string filePath = "..\\tests\\UnitTesting\\UnitTest\\testcases\\Frontend\\test-invalid-variable-name.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), "Frontend parser encountered a syntax error in the program.");
        }
    }

    TEST_METHOD(FrontendParser_InvalidVarName2) {
        FrontendParser frontendParser = FrontendParser();
        std::string filePath = "..\\tests\\UnitTesting\\UnitTest\\testcases\\Frontend\\test-invalid-variable-name-2.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), "Frontend parser encountered a syntax error in the program.");
        }
    }

    TEST_METHOD(FrontendParser_MissingBrackets) {
        FrontendParser frontendParser = FrontendParser();
        std::string filePath = "..\\tests\\UnitTesting\\UnitTest\\testcases\\Frontend\\test-missing-brackets.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), "Frontend parser encountered a syntax error in the program.");
        }
    }

    TEST_METHOD(FrontendParser_ValidProgram) {
        FrontendParser frontendParser = FrontendParser();
        std::string filePath = "..\\tests\\UnitTesting\\UnitTest\\testcases\\Frontend\\test-valid-program.txt";

        bool success;
        try {
            frontendParser.parseProgram(filePath);
            success = true;
        }
        catch (std::exception& ex) {
            success = false;
        }
        Assert::AreEqual(success, true);
    }

    };
}   // namespace UnitTest

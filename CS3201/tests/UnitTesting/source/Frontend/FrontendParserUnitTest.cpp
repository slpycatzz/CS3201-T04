#include "stdafx.h"
#include "targetver.h"

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;

namespace UnitTesting {
    TEST_CLASS(FrontendParserTest) {
public:
    TEST_METHOD(FrontendParser_EmptyStmtList) {
        FrontendParser frontendParser = FrontendParser();
        string filePath = "..\\tests\\UnitTesting\\testcases\\Frontend\\test-empty-stmtlist.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            string errorMessage = GENERIC_MESSAGE_PROGRAMLOGICERROR;
            errorMessage += " : ";
            errorMessage += MESSAGE_EMPTY_STMTLIST;

            Assert::AreEqual(ex.what(), errorMessage.c_str());
        }
    }

    TEST_METHOD(FrontendParser_ExtraBrackets) {
        FrontendParser frontendParser = FrontendParser();
        string filePath = "..\\tests\\UnitTesting\\testcases\\Frontend\\test-extra-brackets.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), GENERIC_MESSAGE_PROGRAMSYNTAXERROR);
        }
    }

    TEST_METHOD(FrontendParser_InvalidProcedureName) {
        FrontendParser frontendParser = FrontendParser();
        string filePath = "..\\tests\\UnitTesting\\testcases\\Frontend\\test-invalid-procedure-name.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            string errorMessage = GENERIC_MESSAGE_PROGRAMSYNTAXERROR;
            errorMessage += " : ";
            errorMessage += MESSAGE_PROCEDURENAME_INVALID;

            Assert::AreEqual(ex.what(), errorMessage.c_str());
        }
    }

    TEST_METHOD(FrontendParser_InvalidVarName) {
        FrontendParser frontendParser = FrontendParser();
        string filePath = "..\\tests\\UnitTesting\\testcases\\Frontend\\test-invalid-variable-name.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            string errorMessage = GENERIC_MESSAGE_PROGRAMSYNTAXERROR;
            errorMessage += " : ";
            errorMessage += MESSAGE_VARIABLENAME_INVALID;

            Assert::AreEqual(ex.what(), errorMessage.c_str());
        }
    }

    TEST_METHOD(FrontendParser_InvalidVarName2) {
        FrontendParser frontendParser = FrontendParser();
        string filePath = "..\\tests\\UnitTesting\\testcases\\Frontend\\test-invalid-variable-name-2.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            string errorMessage = GENERIC_MESSAGE_PROGRAMSYNTAXERROR;
            errorMessage += " : ";
            errorMessage += MESSAGE_VARIABLENAME_INVALID;

            Assert::AreEqual(ex.what(), errorMessage.c_str());
        }
    }

    TEST_METHOD(FrontendParser_MissingBrackets) {
        FrontendParser frontendParser = FrontendParser();
        string filePath = "..\\tests\\UnitTesting\\testcases\\Frontend\\test-missing-brackets.txt";

        try {
            frontendParser.parseProgram(filePath);
        }
        catch (std::exception& ex) {
            Assert::AreEqual(ex.what(), GENERIC_MESSAGE_PROGRAMSYNTAXERROR);
        }
    }

    TEST_METHOD(FrontendParser_ValidProgram) {
        FrontendParser frontendParser = FrontendParser();
        string filePath = "..\\tests\\UnitTesting\\testcases\\Frontend\\test-valid-program.txt";

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

#include <string>

#include "stdafx.h"

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::string;

namespace UnitTesting {
    TEST_CLASS(FrontendParserUnitTest) {
        FrontendParser frontendParser;
        string frontendTestcasesDirectory = "..\\tests\\UnitTesting\\testcases\\Frontend\\";
        string genericMessageProgramLogicError = GENERIC_MESSAGE_PROGRAMLOGICERROR;
        string genericMessageProgramSyntaxError = GENERIC_MESSAGE_PROGRAMSYNTAXERROR;

     public:
        TEST_METHOD_INITIALIZE(Initialize) {
            frontendParser = FrontendParser();
        }

        TEST_METHOD(FrontendParser_EmptyStmtListProcedure) {
            string filePath = frontendTestcasesDirectory + "EmptyStmtListProcedure.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramLogicError + " : ";
                errorMessage += MESSAGE_EMPTY_STMTLIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_EmptyStmtListWhile) {
            string filePath = frontendTestcasesDirectory + "EmptyStmtListWhile.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramLogicError + " : ";
                errorMessage += MESSAGE_EMPTY_STMTLIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_EmptyStmtListIf) {
            string filePath = frontendTestcasesDirectory + "EmptyStmtListIf.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramLogicError + " : ";
                errorMessage += MESSAGE_EMPTY_STMTLIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_EmptyStmtListElse) {
            string filePath = frontendTestcasesDirectory + "EmptyStmtListElse.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramLogicError + " : ";
                errorMessage += MESSAGE_EMPTY_STMTLIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_ExtraBrackets) {
            string filePath = frontendTestcasesDirectory + "ExtraBrackets.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramSyntaxError + " : ";
                errorMessage += MESSAGE_TOKEN_INVALID;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_MissingBrackets) {
            string filePath = frontendTestcasesDirectory + "MissingBrackets.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramSyntaxError + " : ";
                errorMessage += MESSAGE_STMT_INVALID;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_DuplicateProcedureName) {
            string filePath = frontendTestcasesDirectory + "DuplicateProcedureName.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramSyntaxError + " : ";
                errorMessage += MESSAGE_PROCEDURENAME_EXIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_InvalidProcedureName) {
            string filePath = frontendTestcasesDirectory + "InvalidProcedureName.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramSyntaxError + " : ";
                errorMessage += MESSAGE_PROCEDURENAME_INVALID;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_InvalidVariableName) {
            string filePath = frontendTestcasesDirectory + "InvalidVariableName.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramSyntaxError + " : ";
                errorMessage += MESSAGE_VARIABLENAME_INVALID;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_InvalidVariableNameTwo) {
            string filePath = frontendTestcasesDirectory + "InvalidVariableNameTwo.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramSyntaxError + " : ";
                errorMessage += MESSAGE_VARIABLENAME_INVALID;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_InvalidCall) {
            string filePath = frontendTestcasesDirectory + "InvalidCall.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramSyntaxError + " : ";
                errorMessage += MESSAGE_NONEXISTENTCALL_EXIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_RecursiveCall) {
            string filePath = frontendTestcasesDirectory + "RecursiveCall.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramLogicError + " : ";
                errorMessage += MESSAGE_RECURSIVECALL_EXIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_RecursiveCallOne) {
            string filePath = frontendTestcasesDirectory + "RecursiveCallOne.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramLogicError + " : ";
                errorMessage += MESSAGE_RECURSIVECALL_EXIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_RecursiveCallTwo) {
            string filePath = frontendTestcasesDirectory + "RecursiveCallTwo.txt";

            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                string errorMessage = genericMessageProgramLogicError + " : ";
                errorMessage += MESSAGE_RECURSIVECALL_EXIST;

                Assert::AreEqual(ex.what(), errorMessage.c_str());
            }
        }

        TEST_METHOD(FrontendParser_ValidProgramSingleProcedure) {
            string filePath = frontendTestcasesDirectory + "ValidProgramSingleProcedure.txt";

            bool isSuccessful = true;
            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                isSuccessful = false;
            }

            Assert::AreEqual(isSuccessful, true);
        }

        TEST_METHOD(FrontendParser_ValidProgramMultipleProcedure) {
            string filePath = frontendTestcasesDirectory + "ValidProgramMultipleProcedure.txt";

            bool isSuccessful = true;
            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                isSuccessful = false;
            }

            Assert::AreEqual(isSuccessful, true);
        }

        TEST_METHOD(FrontendParser_ValidProgramMixTabsAndSpaces) {
            string filePath = frontendTestcasesDirectory + "ValidProgramMixTabsAndSpaces.txt";

            bool isSuccessful = true;
            try {
                frontendParser.parseProgram(filePath);
            } catch (std::exception &ex) {
                isSuccessful = false;
            }

            Assert::AreEqual(isSuccessful, true);
        }
    };
}

#pragma once

#include <exception>
#include <string>

const char GENERIC_MESSAGE_DEFAULT[]            = "An unknown exception has occurred";
const char GENERIC_MESSAGE_FILENOTFOUND[]       = "A file with the specified filepath does not exist";
const char GENERIC_MESSAGE_PROGRAMSYNTAXERROR[] = "Frontend parser encountered a syntax error in the program";
const char GENERIC_MESSAGE_PROGRAMLOGICERROR[]  = "Frontend parser encountered a logic error in the program";
const char GENERIC_MESSAGE_QUERYSYNTAXERROR[]   = "Query parser encountered a syntax error in the query";
const char GENERIC_MESSAGE_QUERYLOGICERROR[]    = "Query parser encountered a logic error in the query";

const char MESSAGE_TOKEN_INVALID[]         = "Invalid token : ";
const char MESSAGE_STMT_INVALID[]          = "Invalid statement";
const char MESSAGE_ASSIGN_STMT_INVALID[]   = "Invalid assign statement";
const char MESSAGE_VARIABLENAME_INVALID[]  = "Invalid variable name";
const char MESSAGE_PROCEDURENAME_INVALID[] = "Invalid procedure name";
const char MESSAGE_PROCEDURENAME_EXIST[]   = "Procedure name already exist";
const char MESSAGE_EMPTY_STMTLIST[]        = "Statement list is empty";
const char MESSAGE_RECURSIVECALL_EXIST[]   = "Recursive call detected";
const char MESSAGE_NONEXISTENTCALL_EXIST[] = "Procedure name does not exist";

class Exception : public std::exception {
 public:
    virtual const char* what() const throw() {
        return message_.c_str();
    }

    Exception() {
        message_ = GENERIC_MESSAGE_DEFAULT;
    }

    void setMessage(const char *message) {
        message_ = message;
    }

    void setMessageWithReason(const char *message, std::string reason) {
        message_ = message;
        message_.append(" : " + reason);
    }

 private:
    std::string message_;
};

class FileNotFoundException : public Exception {
 public:
    FileNotFoundException() {
        setMessage(GENERIC_MESSAGE_FILENOTFOUND);
    }

    FileNotFoundException(std::string reason) {
        setMessageWithReason(GENERIC_MESSAGE_FILENOTFOUND, reason);
    }
};

class ProgramSyntaxErrorException : public Exception {
 public:
    ProgramSyntaxErrorException() {
        setMessage(GENERIC_MESSAGE_PROGRAMSYNTAXERROR);
    }

    ProgramSyntaxErrorException(std::string reason) {
        setMessageWithReason(GENERIC_MESSAGE_PROGRAMSYNTAXERROR, reason);
    }
};

class ProgramLogicErrorException : public Exception {
 public:
    ProgramLogicErrorException() {
        setMessage(GENERIC_MESSAGE_PROGRAMLOGICERROR);
    }

    ProgramLogicErrorException(std::string reason) {
        setMessageWithReason(GENERIC_MESSAGE_PROGRAMLOGICERROR, reason);
    }
};

class QuerySyntaxErrorException : public Exception {
 public:
    QuerySyntaxErrorException() {
        setMessage(GENERIC_MESSAGE_QUERYSYNTAXERROR);
    }

    QuerySyntaxErrorException(std::string reason) {
        setMessageWithReason(GENERIC_MESSAGE_QUERYSYNTAXERROR, reason);
    }
};

class QueryLogicErrorException : public Exception {
 public:
    QueryLogicErrorException() {
        setMessage(GENERIC_MESSAGE_QUERYLOGICERROR);
    }

    QueryLogicErrorException(std::string reason) {
        setMessageWithReason(GENERIC_MESSAGE_QUERYLOGICERROR, reason);
    }
};

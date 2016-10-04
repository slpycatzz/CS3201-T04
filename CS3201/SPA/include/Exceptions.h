#pragma once

#include <exception>
#include <string>

const char MESSAGE_DEFAULT[]            = "An unknown exception has occurred.";
const char MESSAGE_FILENOTFOUND[]       = "A file with the specified filepath does not exist.";
const char MESSAGE_PROGRAMSYNTAXERROR[] = "Frontend parser encountered a syntax error in the program.";
const char MESSAGE_PROGRAMLOGICERROR[]  = "Frontend parser encountered a logic error in the program.";
const char MESSAGE_QUERYSYNTAXERROR[]   = "Query parser encountered a syntax error in the query.";
const char MESSAGE_QUERYLOGICERROR[]    = "Query parser encountered a logic error in the query.";

class Exception : public std::exception {
 public:
    virtual const char* what() const throw() {
        return message_.c_str();
    }

    Exception() {
        message_ = MESSAGE_DEFAULT;
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
        setMessage(MESSAGE_FILENOTFOUND);
    }

    FileNotFoundException(std::string reason) {
        setMessageWithReason(MESSAGE_FILENOTFOUND, reason);
    }
};

class ProgramSyntaxErrorException : public Exception {
 public:
    ProgramSyntaxErrorException() {
        setMessage(MESSAGE_PROGRAMSYNTAXERROR);
    }

    ProgramSyntaxErrorException(std::string reason) {
        setMessageWithReason(MESSAGE_PROGRAMSYNTAXERROR, reason);
    }
};

class ProgramLogicErrorException : public Exception {
 public:
    ProgramLogicErrorException() {
        setMessage(MESSAGE_PROGRAMLOGICERROR);
    }

    ProgramLogicErrorException(std::string reason) {
        setMessageWithReason(MESSAGE_PROGRAMLOGICERROR, reason);
    }
};

class QuerySyntaxErrorException : public Exception {
 public:
    QuerySyntaxErrorException() {
        setMessage(MESSAGE_QUERYSYNTAXERROR);
    }

    QuerySyntaxErrorException(std::string reason) {
        setMessageWithReason(MESSAGE_QUERYSYNTAXERROR, reason);
    }
};

class QueryLogicErrorException : public Exception {
 public:
    QueryLogicErrorException() {
        setMessage(MESSAGE_QUERYLOGICERROR);
    }

    QueryLogicErrorException(std::string reason) {
        setMessageWithReason(MESSAGE_QUERYLOGICERROR, reason);
    }
};

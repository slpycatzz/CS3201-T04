#pragma once

#include <exception>
#include <string>

const char MESSAGE_DEFAULT[]            = "An unknown exception has occurred.";
const char MESSAGE_FILENOTFOUND[]       = "A file with the specified filepath does not exist.";
const char MESSAGE_PROGRAMSYNTAXERROR[] = "Frontend parser encountered a syntax error in the program.";
const char MESSAGE_PROGRAMLOGICERROR[]  = "Frontend parser encounted a logic error in the program.";
const char MESSAGE_QUERYSYNTAXERROR[]   = "Query parser encountered a syntax error in the query.";

class Exception : public std::exception {
 public:
    virtual const char* what() const throw() {
        return message.c_str();
    }

    Exception() {
        message = MESSAGE_DEFAULT;
    }

    std::string message;
};

class FileNotFoundException : public Exception {
 public:
    FileNotFoundException() {
        message = MESSAGE_FILENOTFOUND;
    }
};

class ProgramSyntaxErrorException : public Exception {
 public:
    ProgramSyntaxErrorException() {
        message = MESSAGE_PROGRAMSYNTAXERROR;
    }
};

class ProgramLogicErrorException : public Exception {
 public:
    ProgramLogicErrorException() {
        message = MESSAGE_PROGRAMLOGICERROR;
    }
};

class QuerySyntaxErrorException : public Exception {
 public:
    QuerySyntaxErrorException() {
        message = MESSAGE_QUERYSYNTAXERROR;
    }
};

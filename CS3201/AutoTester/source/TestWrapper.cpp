#include <exception>
#include <iostream>
#include <string>

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}

// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

/*
 * Function for parsing the SIMPLE source program.
 */
void TestWrapper::parse(std::string filePath) {
    FrontendParser frontendParser = FrontendParser();
    
    try {
        frontendParser.parseProgram(filePath);
    
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}

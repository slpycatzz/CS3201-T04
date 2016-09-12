#include <exception>
#include <iostream>
#include <list>
#include <string>

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/QueryProjector.h"
#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}

// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

TestWrapper::TestWrapper() {}

/*
 * Function for parsing a SIMPLE source program.
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

/*
 * Function for evaluating a list of SIMPLE queries.
 */
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
    QueryPreprocessor queryPreprocessor = QueryPreprocessor();
    QueryProjector queryProjector = QueryProjector();

    try {
        queryPreprocessor.preprocessQuery(query);

        // Evaluator here.
        std::vector<std::string> queryResult { "" }; // store query result here

        /* Projector here. Store answer into results. */
        results = queryProjector.formatOutput(queryResult);

        /* Check if AutoTester sent stop signal. */
        if (AbstractWrapper::GlobalStop) {
            return;
        }
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

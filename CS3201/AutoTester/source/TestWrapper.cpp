#include <exception>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <vector>

#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryOptimizer.h"
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
    /* Check if AutoTester sent stop signal. */
    if (AbstractWrapper::GlobalStop) {
        return;
    }

    QueryPreprocessor queryPreprocessor = QueryPreprocessor();
    QueryEvaluator queryEvaluator = QueryEvaluator();
    QueryProjector queryProjector = QueryProjector();
    QueryOptimizer optimizer = QueryOptimizer();

    try {
        // Iteration 1
        queryPreprocessor.preprocessQuery(query);
        QueryTree queryTree = queryPreprocessor.getQueryTree();
        
        //Evaluator here.
        std::vector<std::string> queryResult = queryEvaluator.selectQueryResults(queryTree);

        // Projector here. Store answer into results. 
        results = queryProjector.formatResult(queryResult);
        

        /* OPTIMIZER TESTS
        queryPreprocessor.processDeclaration("assign a1,a2,a3");
        queryPreprocessor.processDeclaration("stmt s1,s2,s3");
        queryPreprocessor.processDeclaration("variable v1,v2,v3");
        queryPreprocessor.parseSelect(Utils::Split(Utils::TrimLeadingSpaces("Select s1,s2,v2"), ' '));
        queryPreprocessor.parseSuchThat(Utils::Split(Utils::TrimLeadingSpaces("such that Uses(s3,v1)"), ' '));
        queryPreprocessor.parseSuchThat(Utils::Split(Utils::TrimLeadingSpaces("such that Modifies(s3,\"x\")"), ' '));
        queryPreprocessor.parseSuchThat(Utils::Split(Utils::TrimLeadingSpaces("such that Follows(s1,s2)"), ' '));
        queryPreprocessor.parseSuchThat(Utils::Split(Utils::TrimLeadingSpaces("such that Parent(s3,s1)"), ' '));
        queryPreprocessor.parseSuchThat(Utils::Split(Utils::TrimLeadingSpaces("such that Uses(s2,v1)"), ' '));

        queryPreprocessor.parseSuchThat(Utils::Split(Utils::TrimLeadingSpaces("such that Uses(5,\"y\")"), ' '));
        queryPreprocessor.parseSuchThat(Utils::Split(Utils::TrimLeadingSpaces("such that Follows(3,4)"), ' '));
        queryPreprocessor.parsePattern(Utils::Split(Utils::TrimLeadingSpaces("pattern a1(v2,_\"x+y\"_)"), ' '));
        queryPreprocessor.parseSuchThat(Utils::Split(Utils::TrimLeadingSpaces("such that Modifies(a3,v3)"), ' '));
        queryPreprocessor.parsePattern(Utils::Split(Utils::TrimLeadingSpaces("pattern a3(\"z\",_)"), ' '));
        QueryTree queryTree = queryPreprocessor.getQueryTree();
        QueryTree optimizedTree = optimizer.optimize(queryTree);
        optimizedTree.printGroups();
        */
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return;
    }
    
}
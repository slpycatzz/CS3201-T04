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
#include "QueryProcessor/QueryProjector.h"
#include "TestWrapper.h"

std::ofstream out;

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}

// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

TestWrapper::TestWrapper() {
    out.clear();
    out.open("tests\\output.txt");
}

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

    try {
        QueryTree queryTree;
        queryPreprocessor.preprocessQuery(query);

        //Evaluator here.
        queryTree = queryPreprocessor.getQueryTree();
        //std::vector<std::string> queryResult = queryEvaluator.selectQueryResults(PKB(), queryTree);
        
        //test select
        std::unordered_map<std::string, Symbol> selectMap = queryTree.getSelect();
        for (auto kv : selectMap) {
            out << kv.second << " " << kv.first << "; ";
        }
        
        //test clauses
        std::vector<Clause> clauses = queryTree.getClauses("suchThat pattern");
        
        for (Clause c : clauses) {
            out << c.getClauseType() << " ";
            for (int i = 0; i < c.getArgCount(); i++) {
                out << c.getArg()[i] << " ";
            }
            out << std::endl;
        }
        
        //out.close();
        

        /* Projector here. Store answer into results. */
        //results = queryProjector.formatResult(queryResult);
    } catch (std::exception& ex) {
        //std::cout << ex.what() << std::endl;
        out << std::endl << ex.what() << std::endl;
        return;
    }
}

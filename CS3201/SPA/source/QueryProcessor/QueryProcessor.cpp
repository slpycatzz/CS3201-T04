#include "Exceptions.h"
#include "Frontend/FrontendParser.h"
#include "QueryProcessor/QueryProcessor.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/QueryEvaluator.h"
#include "QueryProcessor/QueryOptimizer.h"
#include "QueryProcessor/QueryProjector.h"

QueryProcessor::QueryProcessor() {}

QueryProcessor::~QueryProcessor() {}

void QueryProcessor::process(std::string query, std::list<std::string>& results) {
    QueryPreprocessor queryPreprocessor = QueryPreprocessor();
    QueryEvaluator queryEvaluator = QueryEvaluator();
    QueryProjector queryProjector = QueryProjector();
    QueryOptimizer queryOptimizer = QueryOptimizer();
    
    queryPreprocessor.preprocessQuery(query);
    QueryTree queryTree = queryOptimizer.optimize(queryPreprocessor.getQueryTree());
    ResultList queryResult = queryEvaluator.selectQueryResults(queryTree);
    queryProjector.projectResult(results, queryTree.getResultsInfo(), queryResult);
    
    PKB::ClearComputeOnDemands();
    // For testing
    //queryTree.printGroups();
}
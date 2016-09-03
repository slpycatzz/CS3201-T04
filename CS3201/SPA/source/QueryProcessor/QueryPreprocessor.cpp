#include <fstream>
#include <string>
#include <vector>

#include "Exceptions.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "Utils.h"

QueryPreprocessor::QueryPreprocessor() {}

QueryPreprocessor::~QueryPreprocessor() {}

void QueryPreprocessor::preprocess(std::string filePath) {
    std::ifstream fileStream(filePath);

    /* Validate if file exists. */
    if (!fileStream.is_open()) {
        throw FileNotFoundException();
    }

    preprocessFile(fileStream);
}

void QueryPreprocessor::preprocessFile(std::ifstream& fileStream) {
    int counter = 0;
    std::string currentLine;
    std::vector<std::string> query;

    /* Assumes the query is in the following format:
       First line:  Comment
       Second line: Declaration
       Third line:  Query
       Fourth line: Expected Result
       Fifth line:  Time limit
    */
    /* Comment(YH): Do not restrict yourself to my implementation. You can create a Query class if you want to. */
    while (std::getline(fileStream, currentLine)) {
        currentLine = Utils::TrimLeadingSpaces(currentLine);
        currentLine = Utils::TrimTrailingSpaces(currentLine);

        /* Ignore empty lines. */
        if (currentLine.empty()) {
            continue;
        }

        switch (counter) {
            default:
            /* Comment line of query. */
            case 0:
                break;

            /* Query data. */
            case 1:
            case 2:
            case 3:
                query.push_back(currentLine);
                break;

            /* Time limit line of query. Used it to delimit end of query. */
            case 4:
                queries.push_back(query);
                query.clear();
                break;
        }

        counter = (counter + 1) % QUERY_NUM_OF_LINES;
    }

    fileStream.close();
}

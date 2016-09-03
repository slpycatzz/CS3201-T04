#include <fstream>
#include <string>
#include <vector>

#include "Exceptions.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/RelationTable.h"
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
    while (std::getline(fileStream, currentLine)) {
        currentLine = Utils::TrimSpaces(currentLine);

        /* Ignore empty lines. */
        if (currentLine.empty()) {
            continue;
        }

        switch (counter) {
            default:
            /* Comment line of query. */
            case 0:
                break;

            /* Query data - declaration stmt. */
            case 1:

            /* Query data - query stmt. */
            case 2:

            /* Query data - expected output of query. */
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

// priority todo: 1. split tokens correctly and test 2. implement incomplete methods
bool QueryPreprocessor::processDeclaration(std::string declaration) {
    // w todo: multiple declarations - assign a;assign b;
    std::vector<std::string> declarationList = Utils::Split(declaration, ' ');

    // w todo: isValidVarType not tested
    if (declarationList.size() != 2 || !isValidVarType(declarationList[0])) {
        return false;
    }

    std::vector<std::string> synonyms = Utils::Split(declarationList[1], ',');

    if (synonyms.size() < 1) {
        return false;
    }

    for (unsigned int i = 0; i < synonyms.size(); i++) {
        if (!isValidVarName(synonyms[i])) {
            return false;
        } else {
            declareVar.push_back(synonyms[i]);
            declareVarType.push_back(declarationList[0]);
            // insertVariable(synonyms.at(i), declarationList.at(0));
        }
    }

    return true;
}

bool QueryPreprocessor::processQuery(std::string query) {
    std::vector<std::string> queryList = Utils::Split(query, ' ');

    // First token should be Select [arg]
    if (queryList[0].compare("select") != 0 || queryList.size() != 2) {
        return false;
    }

    queryList[0] = queryList[0].substr(1, queryList[0].size() - 1);

    std::vector<std::string> selectList = Utils::Split(queryList[0], ',');
    std::vector<std::string> var(selectList.size()), varType(selectList.size());

    for (unsigned int i = 0; i < selectList.size(); i++) {
        queryList = Utils::Split(queryList[1], ' ');

        if (queryList.at(0).compare("boolean") != 0) {
            // todo: create boolean var type for querytree
            // insertSelect("", "boolean");
            selectVar.push_back("boolean");
        } else if (isVarExist(queryList[0])) {
            // todo: insert var into querytree
            // insertSelect(queryList[0], getVarType(queryList[0]));
            selectVar.push_back(queryList[0]);
        } else {
            return false;
        }
    }

    if (queryList.size() == 1) {
        return true;
    }

    bool isSuccess = false;
    std::string prevClause;

    while (true) {
        if ((queryList[1].size() > 0 && queryList[1][0] == NULL) || queryList[1].size() == 0) {
            break;
        }

        queryList = Utils::Split(queryList.at(1), ' ');

        if (queryList.size() != 2) {
            return false;
        }

        if (queryList.at(0).compare("such that") == 0) {
            queryList = Utils::Split(queryList.at(1), ' ');

            isSuccess = parseSuchThat(queryList.at(1));
            prevClause = "such that";
        } else if (queryList.at(0).compare("pattern") == 0) {
            isSuccess = parsePattern(queryList.at(1));
            prevClause = "pattern";

        } else {
            break;
        }

        if (isSuccess == false) {
            return false;
        }
    }

    return true;
}

bool QueryPreprocessor::parseSuchThat(std::string suchThat) {
    std::vector<std::string> suchThatList = Utils::Split(suchThat, '(');

    // use RelationTable to process and verify Uses, Modifies rel is correct etc...

    return true;
}

bool QueryPreprocessor::parseSuchThatRelation(std::string relType, std::vector<std::string> &varList, std::vector<std::string> &varTypes) {
    for (unsigned int i = 0; i < varList.size(); i++) {
        if (isVarExist(varList[i])) {
            if (!r.isArgValid(relType, getVarType(varList[i]), "")) {
                return false;
            } else {
                varTypes.at(i) = getVarType(varList.at(i));
                /*}
                    else if (isConstantVar(varList[i])) {
                }
                else if (isConstantInteger(varList[i])) {
                }
                else if (varList.at(i).compare("_") == 0) {
                */
            }
        } else {
            return false;
        }
    }
        return true;
}

bool QueryPreprocessor::parsePattern(std::string pattern) {
    return false;
}

bool QueryPreprocessor::isVarExist(std::string var) {
    if (var.compare("BOOLEAN")) {
        return true;
    }

    for (unsigned int i = 0; i < declareVar.size(); i++) {
        if (declareVar[i].compare(var) == 0)
            return true;
    }
    return false;
}

bool QueryPreprocessor::isValidVarName(std::string varName) {
    return true;
}

bool QueryPreprocessor::isValidVarType(std::string varName) {
    return true;
}

// todo: return symbol enum type
std::string QueryPreprocessor::getVarType(std::string var) {
    return "";
}

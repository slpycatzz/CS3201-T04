#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "Exceptions.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/RelationTable.h"
#include "Utils.h"
#include "Constants.h"

QueryPreprocessor::QueryPreprocessor() {
    relMap = {
        { "follows", "f_symbol" }, { "follows*", "f_s_symbol" },
        { "parent", "p_symbol" }, { "parent*", "p_s_symbol" },
        { "modifies", "m_symbol" }, { "uses", "uses_symbol" }
    };
    entMap = {
        { "stmt", "0" }, { "assign", "1" }, { "while", "2" }, { "variable", "3" }, { "constant", "4" }, { "prog_line", "5" }
    };
}

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

bool QueryPreprocessor::processDeclaration(std::string declaration) {
    std::vector<std::string> declarationList = Utils::Split(declaration, CHAR_SYMBOL_SEMICOLON);
    for (unsigned int line_num = 0; line_num < declarationList.size(); line_num++) {
        std::vector<std::string> declarationType = Utils::Split(declarationList[line_num], ' ');

        if (!isValidVarType(declarationType[0])) {
            // std::cout << "invalid declaration type!";
            return false;
        }
        std::string s;
        s = declarationList[line_num];
        s = s.substr(s.find_first_of(" \t") + 1);

        std::vector<std::string> synonyms = Utils::Split(s, CHAR_SYMBOL_COMMA);

        if (synonyms.size() < 1) {
            // std::cout<< "invalid declaration!\n usage: assign [varName];";
            return false;
        }

        for (unsigned int i = 0; i < synonyms.size(); i++) {
            // strip unnecessary whitespaces
            synonyms[i] = std::regex_replace(synonyms[i], std::regex("\\s+"), "");
            if (!isValidVarName(synonyms[i])) {
                return false;
            } else {
                // wm todo: 1. assign to correct varType container 2. assign to queryTree

                declareVar.push_back(synonyms[i]);
                declareVarType.push_back(declarationType[0]);
                // insertVariable(synonyms.at(i), declarationList.at(0));
            }
        }
    }
    return true;
}

bool QueryPreprocessor::processQuery(std::string query) {
    std::vector<std::string> queryList = Utils::Split(query, ' ');

    // Select [arg] such that ... pattern...
    if (queryList[0].compare("select") == 0) {
        // std::cout << "no select found";
        return false;
    }

    parseSelect(queryList);
    queryList = getNextToken(queryList);

    if (queryList.size() == 2) {
        return true;
    }

    bool isSuccess = false;
    std::string prevClause;

    while (true) {
        // wm todo: check if EOL reached or no items left works
        if ((queryList[1].size() > 0 && queryList[1][0] == NULL) || queryList[1].size() == 0) {
            break;
        }

        if (queryList[0].compare("such") == 0 && queryList[1].compare("that")) {
            isSuccess = parseSuchThat(queryList);
            prevClause = "such that";

        } else if (queryList[0].compare("pattern") == 0) {
            isSuccess = parsePattern(queryList);
            prevClause = "pattern";

        } else {
            break;
        }

        if (isSuccess == false) {
            return false;
        } else {
            queryList = getNextToken(queryList);
        }
    }

    return true;
}

bool QueryPreprocessor::parseSelect(std::vector<std::string> queryList) {
    std::vector<std::string> selectList;
    std::vector<std::string> var, varType;

    std::vector<std::string> temp = getNextToken(queryList);
    unsigned int numberOfVarsInSelect = (queryList.size() - temp.size());
    std::string selectVars = "";

    for (unsigned int i = 1; i < numberOfVarsInSelect; i++) {
        selectVars += queryList[i];
    }

    selectList = Utils::Split(selectVars, CHAR_SYMBOL_COMMA);

    // wm todo: move this loop into another function
    for (unsigned int i = 0; i < selectList.size(); i++) {
        if (selectList[0].compare("boolean") != 0) {
            // wm todo: create boolean var type for querytree
            // insertSelect("", "boolean");
            selectVar.push_back("boolean");
        } else if (isVarExist(selectList[0])) {
            // wm todo: insert var into querytree
            // insertSelect(queryList[0], getVarType(queryList[0]));
            selectVar.push_back(selectList[0]);
        } else {
            return false;
        }
    }

    return true;
}

bool QueryPreprocessor::parseSuchThat(std::vector<std::string> suchThat) {
    // use RelationTable to process and verify Uses, Modifies rel is correct etc...
    std::vector<std::string> suchThatList;

    std::vector<std::string> temp = getNextToken(suchThat);
    unsigned int numberOfItemsInSuchThat = (suchThat.size() - temp.size());
    std::string suchThatStr = "";

    for (unsigned int i = 2; i < numberOfItemsInSuchThat; i++) {
        suchThatStr += suchThat[i];
    }

    std::string relation = suchThatStr.substr(0, suchThatStr.find_first_of(CHAR_SYMBOL_OPENBRACKET));
    std::string argStr = suchThatStr.substr(
        suchThatStr.find_first_of(CHAR_SYMBOL_OPENBRACKET)+1, suchThatStr.find_first_of(CHAR_SYMBOL_CLOSEBRACKET));
    std::vector<std::string> argList = Utils::Split(argStr, CHAR_SYMBOL_COMMA);

    bool isSuccess = parseSuchThatRelation(relation, argList);

    return isSuccess;
}

bool QueryPreprocessor::parseSuchThatRelation(std::string relType, std::vector<std::string>& varList) {
    for (unsigned int i = 0; i < varList.size(); i++) {
        if (isVarExist(varList[i])) {
            if (!r.isArgValid(relType, getVarType(varList[i]), "")) {
                return false;
            } else {
                // wm todo: assign accepted s.t. relations into querytree
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

// wm todo: consider combining with parse s.t.Relation
bool QueryPreprocessor::parsePatternRelation(std::string relType, std::vector<std::string>& varList) {
    for (unsigned int i = 0; i < varList.size(); i++) {
        if (isVarExist(varList[i])) {
            if (!r.isArgValid(relType, getVarType(varList[i]), "")) {
                return false;
            } else {
                // wm todo: assign accepted s.t. relations into querytree
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

bool QueryPreprocessor::parsePattern(std::vector<std::string> pattern) {
    std::vector<std::string> patternList;

    std::vector<std::string> temp = getNextToken(pattern);
    unsigned int numberOfItemsInPattern = (pattern.size() - temp.size());
    std::string patternStr = "";

    for (unsigned int i = 1; i < numberOfItemsInPattern; i++) {
        patternStr += pattern[i];
    }

    std::string var = patternStr.substr(0, patternStr.find_first_of(CHAR_SYMBOL_OPENBRACKET));
    if (!isVarExist(var)) {
        // std::cout << "invalid var: " + var;
        return false;
    }

    std::string argStr = patternStr.substr(
        patternStr.find_first_of(CHAR_SYMBOL_OPENBRACKET) + 1, patternStr.find_first_of(CHAR_SYMBOL_CLOSEBRACKET));
    std::vector<std::string> argList = Utils::Split(argStr, CHAR_SYMBOL_COMMA);

    bool isSuccess = parsePatternRelation(var, argList);

    return isSuccess;
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
    if (varName.length() == 0) {
        return false;
    }
    const std::regex regex_pattern("^[a-zA-Z][a-zA-Z0-9_]*$");
    return std::regex_match(varName, regex_pattern);
}

bool QueryPreprocessor::isValidVarType(std::string varName) {
    if (varName.length() == 0) {
        return false;
    }
    auto t = entMap.find(varName);
    if (t == entMap.end()) {
        return false;
    }
    return true;
}

// find next token in the list and return list from next token onwards
std::vector<std::string> QueryPreprocessor::getNextToken(std::vector<std::string> queryList) {
    std::vector<std::string> resultList;

    return resultList;
}

// todo: return symbol enum type
std::string QueryPreprocessor::getVarType(std::string var) {
    return "";
}

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>

#include "Exceptions.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/RelationTable.h"
#include "Utils.h"

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
    std::vector<std::string> declarationList = split(declaration, ';');
    for (int line_num = 0; line_num < declarationList.size(); line_num++) {
        std::vector<std::string> declarationType = split(declarationList[line_num], ' ');

        if (!isValidVarType(declarationType[0])) {
            // std::cout << "invalid declaration type!";
            return false;
        }
        std::string s;
        s = declarationList[line_num];
        s = s.substr(s.find_first_of(" \t") + 1);

        std::vector<std::string> synonyms = split(s, ',');

        if (synonyms.size() < 1) {
            // std::cout<< "invalid declaration!\n usage: assign [varName];";
            return false;
        }

        for (int i = 0; i < synonyms.size(); i++) {
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
    std::vector<std::string> queryList = split(query, ' ');

    // Select [arg] such that ... pattern...
    if (queryList[0].compare("select") == 0) {
        // std::cout << "no select found";
        return false;
    }

    parseSelect(queryList);

    if (queryList.size() == 2) {
        return true;
    }

    bool isSuccess = false;
    std::string prevClause;

    // wm todo: in progress...
    while (true) {
        if ((queryList[1].size() > 0 && queryList[1][0] == NULL) || queryList[1].size() == 0) {
            break;
        }

        queryList = split(queryList.at(1), ' ');

        if (queryList.size() != 2) {
            return false;
        }

        if (queryList.at(0).compare("such that") == 0) {
            queryList = split(queryList.at(1), ' ');

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

// wm todo: incomplete, last stopped here
bool QueryPreprocessor::parseSelect(std::vector<std::string> queryList) {
    std::vector<std::string> selectList = split(queryList[1], ',');
    std::vector<std::string> var(selectList.size()), varType(selectList.size());

    for (int i = 0; i < selectList.size(); i++) {
        // possible clause-arg: s,a1  , a2,a3 | boolean
        queryList = split(queryList[1], ' ');

        if (queryList.at[0].compare("boolean") != 0) {
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
}

bool QueryPreprocessor::parseSuchThat(std::string suchThat) {
    std::vector<std::string> suchThatList = split(suchThat, '(');

    // use RelationTable to process and verify Uses, Modifies rel is correct etc...

    return true;
}

bool QueryPreprocessor::parseSuchThatRelation(std::string relType, std::vector<std::string>& varList, std::vector<std::string>& varTypes) {
    for (int i = 0; i < varList.size(); i++) {
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

    for (int i = 0; i < declareVar.size; i++) {
        if (declareVar[i].compare(var) == 0)
            return true;
    }
    return false;
}

bool QueryPreprocessor::isValidVarName(std::string varName) {
    if (varName.length == 0) {
        return false;
    }
    const std::regex regex_pattern("^[a-zA-Z][a-zA-Z0-9_]*$");
    return std::regex_match(varName, regex_pattern);
}

bool QueryPreprocessor::isValidVarType(std::string varName) {
    if (varName.length == 0) {
        return false;
    }
    auto t = entMap.find(varName);
    if (t == entMap.end()) {
        return false;
    }
    return true;
}

// todo: return symbol enum type
std::string QueryPreprocessor::getVarType(std::string var) {
}

std::vector<std::string> QueryPreprocessor::split(std::string str, char delimiter) {
    std::vector<std::string> result;
    const char *cur = str.c_str();
    int i = 1;

    while (*cur != 0) {
        const char *begin = cur;

        while (*cur != delimiter && *cur) {
            cur++;
        }
        result.push_back(trim(std::string(begin, cur)));
        cur++;
    }
    return result;
}

std::string trim(std::string s) {
    // w TODO: remove extra whitespaces, remove newline, tabs
    return s;
}

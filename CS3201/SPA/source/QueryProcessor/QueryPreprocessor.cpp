#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "Constants.h"
#include "Exceptions.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/RelationTable.h"
#include "Utils.h"

using std::string;
using std::vector;

QueryPreprocessor::QueryPreprocessor() {
    // wm todo: remove these if not used
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

void QueryPreprocessor::preprocessQuery(string query) {
    /* In the format, queries have size n, [first n - 1 will be declaration, n will be all the clauses] */
    queries = Utils::Split(query, CHAR_SYMBOL_SEMICOLON);

    for (unsigned int i = 0; i < queries.size() - 1; i++) {
        processDeclaration(queries[i]);
    }

    processQuery(queries[queries.size() - 1]);
}

bool QueryPreprocessor::processDeclaration(string declaration) {
    declaration = Utils::TrimSpaces(declaration);

    string declarationType = declaration.substr(0, declaration.find_first_of(" \t"));
    string variablesStr = declaration.substr(declaration.find_first_of(" \t"));
    
    declarationType = Utils::TrimSpaces(declarationType);
    variablesStr = Utils::TrimSpaces(variablesStr);

    /* Validate if variable type exists. */
    if (!isValidVarType(declarationType)) {
        throw QuerySyntaxErrorException();
    }

    vector<string> variableNames = Utils::Split(variablesStr, CHAR_SYMBOL_COMMA);

    if (variableNames.size() == 0) {
        throw QuerySyntaxErrorException();
    }

    for (unsigned int i = 0; i < variableNames.size(); i++) {
        variableNames[i] = Utils::TrimSpaces(variableNames[i]);

        /* Validate if variable name follows naming convention. */
        if (!Utils::IsValidNamingConvention(variableNames[i])) {
            throw QuerySyntaxErrorException();
        }

        // note(to self): currently no need to insert to qt, only Select vars are required
        varMap[variableNames[i]] = declarationType;
    }

    return true;
}

// wm TODO: case insensitive str_cmp...>.<, throw err...
bool QueryPreprocessor::processQuery(string query) {
    vector<string> queryList = Utils::Split(query, ' ');

    // Select [arg] such that ... pattern...
    if (queryList[0] != SYMBOL_SELECT) {
        // std::cout << "no select found";
        throw QuerySyntaxErrorException();
    }

    parseSelect(queryList);

    queryList = getNextToken(queryList);

    if (queryList.size() == 0) {
        return true;
    }

    bool isSuccess = false;
    string prevClause;

    while (queryList.size() > 0) {
        // wm todo: check if EOL reached or no items left works
        if ((queryList[1].size() > 0 && queryList[1][0] == NULL) || queryList[1].size() == 0) {
            break;
        }
        if (queryList[0].compare("such") == 0 && queryList[1].compare("that") == 0) {
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

bool QueryPreprocessor::parseSelect(vector<string> queryList) {
    vector<string> selectList;
    vector<string> var, varType;

    vector<string> temp = getNextToken(queryList);

    unsigned int numberOfVarsInSelect = (queryList.size() - temp.size());
    string selectVars = "";

    for (unsigned int i = 1; i < numberOfVarsInSelect; i++) {
        selectVars += queryList[i];
    }
    selectList = Utils::Split(selectVars, CHAR_SYMBOL_COMMA);

    // wm todo: move this loop into another function
    for (unsigned int i = 0; i < selectList.size(); i++) {
        if (selectList[0].compare("boolean") == 0) {
            // wm todo: create boolean var type for querytree
            qt.insertSelect("boolean", "boolean");
        } else if (isVarExist(selectList[i])) {
            qt.insertSelect(selectList[i], getVarType(selectList[i]));
        } else {
            // std::cout << "invalid variable entered!";
            throw QuerySyntaxErrorException();
        }
    }

    return true;
}

bool QueryPreprocessor::parseSuchThat(vector<string> suchThat) {
    // use RelationTable to process and verify Uses, Modifies rel is correct etc...
    vector<string> suchThatList;

    vector<string> temp = getNextToken(suchThat);
    unsigned int numberOfItemsInSuchThat = (suchThat.size() - temp.size());
    string suchThatStr = "";

    for (unsigned int i = 2; i < numberOfItemsInSuchThat; i++) {
        suchThatStr += suchThat[i];
    }

    string relation = suchThatStr.substr(0, suchThatStr.find_first_of(CHAR_SYMBOL_OPENBRACKET));
    string argStr = suchThatStr.substr(
        suchThatStr.find_first_of(CHAR_SYMBOL_OPENBRACKET)+1,
        suchThatStr.find_first_of(CHAR_SYMBOL_CLOSEBRACKET)
        - suchThatStr.find_first_of(CHAR_SYMBOL_OPENBRACKET) - 1);

    vector<string> argList = Utils::Split(argStr, CHAR_SYMBOL_COMMA);

    bool isSuccess = parseRelation("such that", relation, argList);

    return isSuccess;
}

bool QueryPreprocessor::parseRelation(string clauseType, string relType, vector<string>& varList) {
    vector<string> varTypeList;
    string relTypeArg = "";
    if (clauseType == "pattern") {
        relTypeArg = relType;
        relType = getVarType(relType);
        if (relType.compare("assign") == 0) {
            relType = "patternAssign";
        } else {
            relType = "pattern";
        }
    }
    for (unsigned int i = 0; i < varList.size(); i++) {
        if (isVarExist(varList[i])) {
            // isArgValid(relationType, argType, argNumber)
            // argNumber e.g. 1st arg, 2nd arg...
            if (!r.isArgValid(relType, getVarType(varList[i]), i)) {
                return false;
            }
            varTypeList.push_back(getVarType(varList[i]));
        } else if (isConstantVar(varList[i])) {
        // constant var, e.g. ("x","v",_, _"a"_), pattern string belongs here
            // wm todo: add pattern string type
            if (!r.isArgValid(relType, "variable", i)) {
                return false;
            }
            varTypeList.push_back("variable");
        } else if (Utils::IsNonNegativeNumeric(varList[i])) {
        // constant int, e.g. (1,2)
            if (!r.isArgValid(relType, "constant", i)) {
                return false;
            }
            varTypeList.push_back("constant");
        } else {
            // std::cout << "invalid relation entered!";
            throw QuerySyntaxErrorException();
        }
    }

    if (clauseType == "such that") {
        qt.insertSuchThat(relType, varList);
    } else if (clauseType == "pattern") {
        // clauseType: "pattern", arg: arg1, arg2, arg3/patternType(a,ifstmt,while...)
        varList.push_back(relTypeArg);
        qt.insertPattern("pattern", varList);
    }
    return true;
}

bool QueryPreprocessor::parsePattern(vector<string> pattern) {
    vector<string> patternList;
    vector<string> temp = getNextToken(pattern);
    unsigned int numberOfItemsInPattern = (pattern.size() - temp.size());
    string patternStr = "";

    for (unsigned int i = 1; i < numberOfItemsInPattern; i++) {
        patternStr += pattern[i];
    }

    string var = patternStr.substr(0, patternStr.find_first_of(CHAR_SYMBOL_OPENBRACKET));

    if (!isVarExist(var)) {
        // std::cout << "invalid var: " + var;
        throw QuerySyntaxErrorException();
     }

    string argStr = patternStr.substr(
        patternStr.find_first_of(CHAR_SYMBOL_OPENBRACKET) + 1,
        patternStr.find_first_of(CHAR_SYMBOL_CLOSEBRACKET)
        - patternStr.find_first_of(CHAR_SYMBOL_OPENBRACKET) - 1);
    vector<string> argList = Utils::Split(argStr, CHAR_SYMBOL_COMMA);

    bool isSuccess = parseRelation("pattern", var, argList);

    return isSuccess;
}

bool QueryPreprocessor::isConstantVar(string var) {
    bool isSurroundWithDblQuotes = (var[0] == '"') && (var[var.length() - 1] == '"');
    bool isUnderscoreExist = (var[0] == '_');
    bool isSecondUnderscoreExist = (var[var.length() - 1] == '_');
    bool isSurroundWithInnerDblQuotes;

    if (var.length() == 1) {
        return isUnderscoreExist;
    }
    if (var.length() >= 5) {
        isSurroundWithInnerDblQuotes = (var[1] == '"') && (var[var.length() - 2] == '"');
        bool isDblWildcard = isSurroundWithInnerDblQuotes && (isUnderscoreExist && isSecondUnderscoreExist);
        return isDblWildcard;
    }

    // wm todo: isValid pattern expr: "+5", "5-4x"
    return isSurroundWithDblQuotes;
}

bool QueryPreprocessor::isVarExist(string var) {
    if (var.compare("boolean") == 0) {
        return true;
    }
    if (varMap.find(var) != varMap.end()) {
        return true;
    }
    return false;
}

bool QueryPreprocessor::isValidVarName(string varName) {
    if (varName.length() == 0) {
        return false;
    }
    const std::regex regex_pattern("^[a-zA-Z][a-zA-Z0-9_]*$");
    return std::regex_match(varName, regex_pattern);
}

bool QueryPreprocessor::isValidVarType(string varName) {
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
vector<string> QueryPreprocessor::getNextToken(vector<string> queryList) {
    unsigned int end = 0;
    vector<string> result;

    for (unsigned int i = 0; i < queryList.size(); i++) {
        if (queryList[i].compare("such") == 0 || queryList[i].compare("pattern") == 0) {
            end = i;
            break;
        }
    }
    if (end == 0) {
        queryList.clear();
    } else {
        queryList.erase(queryList.begin(), queryList.begin() + end);
    }
    return queryList;
}

// todo: return symbol enum type
string QueryPreprocessor::getVarType(string var) {
    string varType = varMap.find(var)->second;
    return varType;
}

QueryTree QueryPreprocessor::getQueryTree() {
    return qt;
}

// for debugging using unit test, to be removed after testing is complete
string QueryPreprocessor::testMethodOut() {
    return out;
}


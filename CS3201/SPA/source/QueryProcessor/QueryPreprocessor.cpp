#include <algorithm>
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

QueryPreprocessor::QueryPreprocessor() {}

QueryPreprocessor::~QueryPreprocessor() {}

/* START: methods kept temporarily for optimizer testing [not in use] */

/*  methods affected:
 *  bool parseSuchThat(std::vector<std::string> suchThat);
 *  bool parsePattern(std::vector<std::string> pattern);
 *  
 *  bool parseSelect(std::vector<std::string> queryList);
 *  bool parseRelation(std::string, std::string, std::vector<std::string>);
 */
/*
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

    // Iteration 1 only
    //if (selectList.size() >= 2) {
    //    throw QuerySyntaxErrorException("01");
    //}
    

    // wm todo: move this loop into another function
    for (unsigned int i = 0; i < selectList.size(); i++) {
        if (toLower(selectList[0]).compare("boolean") == 0) {
            // wm todo: create boolean var type for querytree
            qt.insertSelect("boolean", "boolean");
        } else if (isVarExist(selectList[i])) {
            qt.insertSelect(selectList[i], getVarType(selectList[i]));
        } else {
            // std::cout << "invalid variable entered!";
            throw QuerySyntaxErrorException("02");
        }
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
        throw QuerySyntaxErrorException("03");
     }

    string argStr = patternStr.substr(
        patternStr.find_first_of(CHAR_SYMBOL_OPENBRACKET) + 1,
        patternStr.find_first_of(CHAR_SYMBOL_CLOSEBRACKET)
        - patternStr.find_first_of(CHAR_SYMBOL_OPENBRACKET) - 1);
    vector<string> argList = Utils::Split(argStr, CHAR_SYMBOL_COMMA);

    bool isSuccess = parseRelation("pattern", var, argList);

    return isSuccess;
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
        if (toLower(relType).compare("assign") == 0) {
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
                throw QuerySyntaxErrorException("04");
            }
            varTypeList.push_back(getVarType(varList[i]));
        } else if (isConstantVar(varList[i])) {
            // constant var, e.g. ("x","v",_, _"a"_), pattern string belongs here
            // wm todo: add pattern string type
            if (!r.isArgValid(relType, "variable", i)) {
                throw QuerySyntaxErrorException("05");
            }
            varTypeList.push_back("variable");
        } else if (Utils::IsNonNegativeNumeric(varList[i])) {
            // constant int, e.g. (1,2)
            if (!r.isArgValid(relType, "constant", i)) {
                throw QuerySyntaxErrorException("06");
            }
            varTypeList.push_back("constant");
        } else {
            // std::cout << "invalid relation entered!";
            std::string message = "vars: ";
            for (std::string var : varList) {
                message += var + " ";
            }

            throw QuerySyntaxErrorException("10. " + message);
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
*/
/* END: methods kept temporarily for optimizer testing [not in use] */

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
        throw QuerySyntaxErrorException("1");
    }

    vector<string> variableNames = Utils::Split(variablesStr, CHAR_SYMBOL_COMMA);

    if (variableNames.size() == 0) {
        throw QuerySyntaxErrorException("2");
    }

    for (unsigned int i = 0; i < variableNames.size(); i++) {
        variableNames[i] = Utils::TrimSpaces(variableNames[i]);

        /* Validate if variable name follows naming convention. */
        if (!Utils::IsValidNamingConvention(variableNames[i])) {
            throw QuerySyntaxErrorException("3");
        }

        /* contains declared vars, pass to queryTree */
        varSymbolMap[variableNames[i]] = Constants::StringToSymbol(declarationType);
    }

    qt.insertDeclaration(varSymbolMap);

    return true;
}

// wm TODO: case insensitive str_cmp
bool QueryPreprocessor::processQuery(string query) {
    queryList = Utils::Split(Utils::TrimLeadingSpaces(query), ' ');
    cur = 0;
    /* Expecting first token to be Select (case-sensitive) */
    if (queryList[cur] != SYMBOL_SELECT) {
        // std::cout << "no select found";
        throw QuerySyntaxErrorException("4");
    }

    /* parse [Select] ... */
    parseSelect();

    if (cur >= queryList.size()) {
        return true;
    }

    /* remaining string: ...[such that]...[pattern]...[with]...*/
    Symbol prevClause = INVALID;

    /* continue parsing [such that... pattern...] clauses until end of query */
    while (cur+1 < queryList.size()) {
        // wm todo: check if EOL reached or no items left works
        if ((queryList[cur+1].size() > 0 && queryList[cur+1][0] == NULL) || queryList[cur+1].size() == 0) {
            break;
        }
        if (peek().compare("such") == 0) {
            parseSuchThat();
            prevClause = SUCH_THAT;
        } else if (queryList[cur].compare("pattern") == 0) {
            parsePattern();
            prevClause = PATTERN;
        } else if (queryList[cur].compare("and") == 0) {
            parseAnd(prevClause);
        } else if (queryList[cur].compare("with") == 0) {
            parseWith();
        } else {
            break;
        }

        /* stop parsing if [such that...pattern...] query fails, trim processed part of query */
        }
    return true;
}

void QueryPreprocessor::parseSelect() {
    string temp;
    vector<string> var;
    /* case1: Select <a,a1 > pattern... */
    /* case2: Select a such that... */
    expect("Select");
    if (accept('<')) {
        while (accept('>') == 0) {
            if (accept(VARIABLE)) {
                var.push_back(getVar());  // wm todo getVariable after validation for saving
                temp = peek();
                queryList[cur] = temp.substr(getVar().size());
            } else if (accept(BOOLEAN)) {
                var.push_back(getVar());  // wm todo getVariable after validation for saving
                temp = peek();
                queryList[cur] = temp.substr(getVar().size());
            } else {
                throw QuerySyntaxErrorException("5");
            }
            accept(',');
        }
    } else {
        if (isValidVarName(peek())) {
            var.push_back(peek());
            temp = peek();
            queryList[cur] = temp.substr(getVar().size());
        } else if (accept(BOOLEAN)) {
            var.push_back("BOOLEAN");
            temp = peek();
            queryList[cur] = temp.substr(getVar().size());
            varSymbolMap["BOOLEAN"] = BOOLEAN;
        } else {
            throw QuerySyntaxErrorException("6");
        }
    }
    for (string v : var) {
        qt.insertSelect(v, Constants::SymbolToString(varSymbolMap[v]));
    }
}

void QueryPreprocessor::parseSuchThat() {
    vector<string> argList;
    string relation;
    expect("such");
    expect("that");
    relation = peek();
    relation = relation.substr(0, relation.find_first_of('('));
    queryList[cur] = peek().substr(relation.size());
    // wm todo: isRelationValid
    /* case: uses(a1,1)*/
    expect('(');
    int i = 0;
    while (i < 2) {
        if (accept(VARIABLE)) {
            string varType;
            varType = Constants::SymbolToString(varSymbolMap[getVar()]);
            if (varType == "Invalid") {
                varType = "variable";
            }

            if (r.isArgValid(relation, varType, i)) {
                argList.push_back(getVar());  // wm todo getVariable after validation for saving
            } else {
                throw QuerySyntaxErrorException("7");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(CONSTANT)) {
            if (r.isArgValid(relation, Constants::SymbolToString(CONSTANT), i)) {
                argList.push_back(getVar());  // wm todo getVariable after validation for saving
            } else {
                throw QuerySyntaxErrorException("8");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else {
            throw QuerySyntaxErrorException("9");
        }
        accept(',');
        i++;
    }
    expect(')');
    qt.insertSuchThat(relation, argList);
}

/* expects the following to be set:
    Symbol prevClause;
    Symbol prevRelation;
    int cur; // index of parsed queryList
*/
void QueryPreprocessor::parsePattern() {
    string relation, rRelation;
    vector<string> argList;

    expect("pattern");
    rRelation = "pattern";
    relation = peek();
    relation = relation.substr(0, relation.find_first_of('('));

    queryList[cur] = peek().substr(relation.size());
    // wm todo: isRelationValid
    if (isValidVarName(relation)) {
        Symbol varType = varSymbolMap[relation];
        if (varType == ASSIGN) {
            rRelation = "patternAssign";
        } else {
            throw QuerySyntaxErrorException("10");
        }
    } else {
        throw QuerySyntaxErrorException("11");
    }
    expect('(');
    int i = 0;
    do {
        // wm todo: constant variable type
        if (accept(VARIABLE)) {
            string varType = Constants::SymbolToString(varSymbolMap[getVar()]);
            if (varType == "Invalid") {
                varType = "variable";
            }
            if (r.isArgValid(rRelation, varType, i)) {
                argList.push_back(getVar());
            } else {
                throw QuerySyntaxErrorException("12");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(CONSTANT)) {
            if (r.isArgValid(relation, Constants::SymbolToString(CONSTANT), i)) {
                argList.push_back(getVar());
            } else {
                throw QuerySyntaxErrorException("13");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(UNDERSCORE)) {
            if (r.isArgValid(rRelation, "underscore", i)) {
                argList.push_back("underscore");
            } else {
                throw QuerySyntaxErrorException("14");
            }

            queryList[cur] = peek().substr(getVar().size());
        } else {
            throw QuerySyntaxErrorException("15");
        }
        i++;
    } while (accept(',') == 1 && i < 3);
    expect(')');

    argList.push_back(relation);
    qt.insertPattern("pattern", argList);
}

void QueryPreprocessor::parseAnd(Symbol prevClause) {
    expect("and");
    switch (prevClause) {
    case SUCH_THAT:
        queryList[--cur] = "that";
        queryList[--cur] = "such";
        parseSuchThat();
    case PATTERN:
        queryList[--cur] = "such";
        parsePattern();
    case WITH:
        throw QuerySyntaxErrorException("TODO: work in progress...");
    }
}
void QueryPreprocessor::parseWith() {
    string var, varAttribute;
    expect("with");
    var = getVar();
    queryList[cur] = peek().substr(var.size());
    expect('.');
    varAttribute = peek();  // wm todo: need to validate?
    cur++;
    expect('=');
    if (accept(varAttributeToSymbol(varAttribute))) {
        // wm todoL insert to qt: [with] var.varAttr = value
    }
}

// wm todo: incomplete
Symbol QueryPreprocessor::varAttributeToSymbol(string varAttribute) {
    if (varAttribute == "procName")
        return PROCEDURE;
    if (varAttribute == "stmtNumber")
        return CONSTANT;
}
string QueryPreprocessor::getVar() {
    string word = queryList[cur];
    // delimiters: ,>;)
    int pos = 0;
    for (char c : word) {
        if (c == ',' || c == '>' || c == ';' || c == ')' || c == '.') {
            break;
        }
        pos++;
    }
    return word.substr(0, pos);
}

bool QueryPreprocessor::isConstantVar(string var) {
    bool isSurroundWithDblQuotes = (var[0] == '"') && (var[var.length() - 1] == '"');
    bool isUnderscoreExist = (var[0] == '_');
    bool isSecondUnderscoreExist = (var[var.length() - 1] == '_');
    bool isSurroundWithInnerDblQuotes;

    if (var.length() == 1) {
        return isUnderscoreExist;
    }
    if (var.length() > 1) {
        isSurroundWithInnerDblQuotes = (var[1] == '"') && (var[var.length() - 2] == '"');
        bool isDblWildcard = isSurroundWithInnerDblQuotes && (isUnderscoreExist && isSecondUnderscoreExist);
        if (isDblWildcard) {
            if (isdigit(var[2])) {
                std::string removedWildcard = var.substr(2, var.length() - 4);
                if (Utils::IsNonNegativeNumeric(removedWildcard)) {
                    return isDblWildcard;
                } else {
                    throw QuerySyntaxErrorException("16");
                }
            } else {
                return isDblWildcard;
            }
        } else {
            if (isSurroundWithDblQuotes && isdigit(var[1])) {
                std::string removedQuotes = var.substr(1, var.length() - 3);

                if (Utils::IsNonNegativeNumeric(removedQuotes)) {
                    return isSurroundWithDblQuotes;
                } else {
                    throw QuerySyntaxErrorException("17");
                }
            }
            return isSurroundWithDblQuotes;
        }
    }

    // wm todo: isValid pattern expr: "+5", "5-4x"
}

bool QueryPreprocessor::isVarExist(string var) {
    if (toLower(var).compare("boolean") == 0) {
        return true;
    }
    if (varSymbolMap.find(var) != varSymbolMap.end()) {
        return true;
    }
    return false;
}

bool QueryPreprocessor::isValidVarName(string varName) {
    if (varName.length() == 0) {
        return false;
    }

    /* varName cannot be a token */
    if (Constants::StringToSymbol(varName) != INVALID) {
        return false;
    }

    const std::regex regex_pattern("^[a-zA-Z][a-zA-Z0-9_]*$");
    return std::regex_match(varName, regex_pattern);
}

bool QueryPreprocessor::isValidVarType(string varName) {
    if (varName.length() == 0) {
        return false;
    }

    Symbol varTypeSymbol = Constants::StringToSymbol(varName);

    switch (varTypeSymbol) {
        case STMT:
        case ASSIGN:
        case WHILE:
        case VARIABLE:
        case CONSTANT:
        case PROGRAM_LINE:
            return true;
        default:
            return false;
    }

    return false;
}



// find next token in the list and return list from next token onwards
vector<string> QueryPreprocessor::getNextToken(vector<string> queryList) {
    unsigned int end = 0;
    vector<string> result;

    for (unsigned int i = 0; i < queryList.size(); i++) {
        if (queryList[i].compare("such") == 0 || queryList[i].compare("pattern") == 0
            || queryList[i].compare("and") == 0) {
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
    Symbol varType = varSymbolMap.find(var)->second;
    return Constants::SymbolToString(varType);
}

QueryTree QueryPreprocessor::getQueryTree() {
    return qt;
}

// for debugging using unit test, to be removed after testing is complete
string QueryPreprocessor::testMethodOut() {
    return out;
}

string QueryPreprocessor::toLower(string data) {
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
}

/* accept if string found, advance index */
int QueryPreprocessor::accept(string token) {
    string var = getVar();
    if (toLower(var).compare(token) == 0) {
        cur++;
        return 1;
    }
    return 0;
}

/* accept if char found, advance index */
int QueryPreprocessor::accept(char token) {
    if (peek()[0] == token) {
        queryList[cur] = peek().substr(1);
        peek();
        return 1;
    }
    return 0;
}

/* accept if token found, do nothing after */
int QueryPreprocessor::accept(Symbol token) {
    string var = getVar();
    if (var.size() < 1) {
        return 0;
    }

    switch (token) {
    default:
        return 0;
    case VARIABLE:
        /* validate for variable a1,"a+1+a", _"a"_ */
        if (isVarExist(var)) {
            return 1;
        } else if (isConstantVar(var)) {
                    return 1;
        }
        break;
    case CONSTANT:
        /* validate for constant number 1,2,123 */
        return 1;
    case BOOLEAN:
        /* validate for _ wildcard */
        if (var == "_") {
            return 1;
        }
    }
    return 0;
}
void QueryPreprocessor::expect(string token) {
    if (toLower(queryList[cur]).compare(toLower(token)) == 0) {
        cur++;
    } else {
        // wm todo: throw correct exception
        throw QuerySyntaxErrorException("18 (str)token is");
    }
}

void QueryPreprocessor::expect(char token) {
    /* peek to remove empty vector */
    peek();
    // wm todo: reached end of query??
    if (cur == queryList.size()) throw QuerySyntaxErrorException("19 expect(char)token: cur==size");

    if (queryList[cur][0] == token) {
        queryList[cur] = peek().substr(1);
        peek();
    } else {
        // wm todo: throw correct exception
        throw QuerySyntaxErrorException("20");
    }
}

string QueryPreprocessor::peek() {
    int len = queryList.size();
    string emptyString;

    if (cur >= len) {
        return emptyString;
    }

    if (cur != len && queryList[cur].size() == 0) {
        cur++;
    }

    if (cur < queryList.size()) {
        return queryList[cur];
    }

    return emptyString;
}

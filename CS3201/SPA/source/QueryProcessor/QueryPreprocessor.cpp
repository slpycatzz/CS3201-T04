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

// wm todo: case-insensitive compare
bool QueryPreprocessor::processQuery(string query) {
    queryList = Utils::Split(Utils::TrimLeadingSpaces(query), ' ');
    cur = 0;
    /* Expecting first token to be Select (case-sensitive) */
    if (queryList[cur] != SYMBOL_SELECT) {
        throw QuerySyntaxErrorException("4");
    }

    /* parse [Select] ... */
    parseSelect();

    if (cur >= queryList.size()) {
        return true;
    }

    /* remaining string: ...[such that]...and...[pattern]...and...[with]...and...*/
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
            prevClause = WITH;
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
                string var1;
                temp = getVar();
                var.push_back(temp);
                varAttrMap[temp] = false;
                queryList[cur] = peek().substr(getVar().size());

                if (accept('.')) {
                    string varAttribute1;
                    varAttribute1 = getVar();
                    queryList[cur] = peek().substr(getVar().size());
                    if (isAttributeValid(temp, varAttribute1, false)) {
                        // only call needs to return true
                        if (getVarType(temp) == CALL) {
                            varAttrMap[temp] = true;
                        }
                    } else {
                        throw QuerySyntaxErrorException("21a"+varAttribute1);
                    }
                }
            } else {
                throw QuerySyntaxErrorException("5");
            }
            accept(',');
        }
    } else {
        if (isVarExist(peek())) {
            var.push_back(peek());
            temp = peek();
            queryList[cur] = temp.substr(getVar().size());

            varAttrMap[temp] = false;

            // case: <var1.varAttr, var2>
            if (accept('.')) {
                string varAttribute1;
                varAttribute1 = getVar();
                queryList[cur] = peek().substr(getVar().size());
                if (isAttributeValid(temp, varAttribute1, false)) {
                    // only call needs to return true
                    if (getVarType(temp) == CALL) {
                        varAttrMap[temp] = true;
                    }
                } else {
                    throw QuerySyntaxErrorException("21b");
                }
            }
        } else if (accept(SYMBOL_BOOLEAN)) {
            var.push_back(SYMBOL_BOOLEAN);
            varSymbolMap[SYMBOL_BOOLEAN] = BOOLEAN;
            varAttrMap[SYMBOL_BOOLEAN] = false;
        } else {
            throw QuerySyntaxErrorException("6");
        }
    }
    if (var.size() < 1) {
        // No QueryResult found
        throw QuerySyntaxErrorException("7");
    }

    qt.insert(QUERY_RESULT, "placeholder", var);
    qt.insert(QUERY_RESULT, "placeholder", varAttrMap);
}

void QueryPreprocessor::parseSuchThat() {
    vector<string> argList;
    Symbol relation;
    string relationString;
    expect("such");
    expect("that");
    relationString = peek().substr(0, peek().find_first_of('('));
    relation = Constants::StringToSymbol(relationString);
    if (relation == INVALID) {
        throw QuerySyntaxErrorException("8");
    }

    queryList[cur] = peek().substr(relationString.size());
    if (!r.isRelationValid(relation, SUCH_THAT)) {
        throw QuerySyntaxErrorException("9");
    }
    /* case: uses(a1,1)*/
    expect('(');
    int i = 0;
    while (i < 2) {
        if (accept(VARIABLE)) {
            Symbol varType;
            varType = getVarType(getVar());
            if (varType == INVALID) {
                // case: var = "x" is not mapped to a varType
                varType = VARIABLE;
            }
            if (r.isArgValid(relation, Constants::SymbolToString(varType), i)) {
                argList.push_back(getVar());
            } else {
                throw QuerySyntaxErrorException("10");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(CONSTANT)) {
            if (r.isArgValid(relation, Constants::SymbolToString(CONSTANT), i)) {
                argList.push_back(getVar());  // wm todo getVariable after validation for saving
            } else {
                throw QuerySyntaxErrorException("11");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(UNDERSCORE)) {
            if (r.isArgValid(relation, "_", i)) {
                argList.push_back("_");
            } else {
                throw QuerySyntaxErrorException("12");
            }

            queryList[cur] = peek().substr(getVar().size());

        } else {
            throw QuerySyntaxErrorException("13");
        }
        accept(',');
        i++;
    }
    expect(')');
    qt.insert(SUCH_THAT, relationString, argList);
}

/* expects the following to be set:
    Symbol prevClause;
    Symbol prevRelation;
    int cur; // index of parsed queryList
*/
void QueryPreprocessor::parsePattern() {
    string relation;
    vector<string> argList;

    expect("pattern");
    relation = peek();
    relation = relation.substr(0, relation.find_first_of('('));

    queryList[cur] = peek().substr(relation.size());

    if (isValidVarName(relation)) {
        if (!r.isRelationValid(varSymbolMap[relation], PATTERN)) {
            throw QuerySyntaxErrorException("14");
        }
    } else {
        throw QuerySyntaxErrorException("15");
    }
    mergeSeparatedClauses();
    expect('(');
    int i = 0;
    do {
        // wm todo: constant variable type
        if (accept(VARIABLE)) {
            Symbol varType = varSymbolMap[getVar()];
            if (varType == INVALID) {
                // case: var = "a" is not mapped to a varType
                varType = VARIABLE;
            }
            if (r.isArgValid(varSymbolMap[relation], Constants::SymbolToString(varType), i)) {
                argList.push_back(getVar());
            } else {
                throw QuerySyntaxErrorException("16");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(CONSTANT)) {
            if (r.isArgValid(varSymbolMap[relation], Constants::SymbolToString(CONSTANT), i)) {
                argList.push_back(getVar());
            } else {
                throw QuerySyntaxErrorException("17");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(UNDERSCORE)) {
            if (r.isArgValid(varSymbolMap[relation], "_", i)) {
                argList.push_back("_");
            } else {
                throw QuerySyntaxErrorException("18");
            }

            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(PATTERN)) {
            // case: pattern expression string e.g. _"a+1"_, "a+1"
            if (r.isArgValid(varSymbolMap[relation], "pattern", i)) {
                argList.push_back(getVar());
            } else {
                throw QuerySyntaxErrorException("19");
            }

            queryList[cur] = peek().substr(getVar().size());
        } else {
            throw QuerySyntaxErrorException("20");
        }
        i++;
    } while (accept(',') == 1 && i < 3);
    expect(')');

    argList.insert(argList.begin(), relation);
    qt.insert(PATTERN, "pattern", argList);
}

void QueryPreprocessor::parseAnd(Symbol prevClause) {
    expect("and");
    switch (prevClause) {
    case SUCH_THAT:
        queryList[--cur] = "that";
        queryList[--cur] = "such";
        parseSuchThat();
        break;
    case PATTERN:
        out += queryList[cur-1];
        queryList[--cur] = "pattern";
        parsePattern();
        break;
    case WITH:
        queryList[--cur] = "with";
        parseWith();
    }
}

void QueryPreprocessor::parseWith() {
    string var, varAttribute, varValue;
    expect("with");
    var = getVar();
    queryList[cur] = peek().substr(getVar().size());
    expect('.');
    varAttribute = getVar();
    queryList[cur] = peek().substr(getVar().size());
    peek();
    expect('=');
    varValue = peek();
    if (varValue.find('.') != std::string::npos) {
        // case: var1.varAttr = var2.varAttr
        string var2, varAttribute2;
        var2 = getVar();
        queryList[cur] = peek().substr(var2.size());
        expect('.');
        varAttribute2 = peek();
        if (isAttributeValid(var, varAttribute, var2, varAttribute2)) {
            vector<string> varList = { var, var2 };
            qt.insert(WITH, "with", varList);
        } else {
            throw QuerySyntaxErrorException("21");
        }
    } else {
        // case: var1.varAttr = "varValue"
        if (isAttributeValid(var, varAttribute, true)) {
            vector<string> varList = { var, varValue };
            qt.insert(WITH, "with", varList);
        } else {
            throw QuerySyntaxErrorException("22");
        }
    }
    cur++;
}

// return only CONSTANT | VARIABLE | INVALID
Symbol QueryPreprocessor::getAttributeType(string var) {
    Symbol varType = getVarType(var);
    switch (varType) {
    case PROCEDURE:
    case CALL:
        return VARIABLE;
    case VARIABLE:
        return VARIABLE;
    case CONSTANT:
        return CONSTANT;
    case STMT:
    case ASSIGN:
    case WHILE:
    case IF:
        return CONSTANT;
    default:
        return INVALID;
    }

    return INVALID;
}
// case: var1.varAttr = var2.varAttr
bool QueryPreprocessor::isAttributeValid(string var, string varAttr, string var2, string varAttr2) {
    Symbol attrType1 = getAttributeType(var);
    Symbol attrType2 = getAttributeType(var);
    bool isValid;
    // check same attrType e.g. CONSTANT == CONSTANT | VARIABLE == VARIABLE
    isValid = isAttributeValid(var, varAttr, false) && isAttributeValid(var2, varAttr2, false);
    // check var and attrType is valid e.g. proc == "procName"
    isValid = (attrType1 == attrType2) && isValid;
    return isValid;
}

// case 1: var1.varAttr = "varValue"
// case 2: var1.varAttr = var2.varAttr
// isVarValue = false [for-case2], compare varAttr valid for var1 only
bool QueryPreprocessor::isAttributeValid(string var, string varAttribute, bool isVarValue) {
    Symbol varType = getVarType(var);
    bool isValid = false;

    if (isVarValue) {
        // check varValue type matches varAttribute
        Symbol attrType = getAttributeType(var);
        switch (attrType) {
        case VARIABLE:
            isValid = accept(VARIABLE);
            break;
        case CONSTANT:
            isValid = accept(CONSTANT);
            break;
        default:
            isValid = false;
        }
    } else {
        isValid = true;
    }

    switch (varType) {
    case PROCEDURE:
    case CALL:
        if (varAttribute == "procName") {
            return isValid;
        } else {
            throw QuerySyntaxErrorException("23");
        }
    case VARIABLE:
        if (varAttribute == "varName") {
            return isValid;
        } else {
            throw QuerySyntaxErrorException("24");
        }
    case CONSTANT:
        if (varAttribute == "value") {
            return isValid;
        } else {
            throw QuerySyntaxErrorException("25");
        }
    case STMT:
    case ASSIGN:
    case WHILE:
    case IF:
        if (varAttribute == "stmt#") {
            return isValid;
        } else {
            throw QuerySyntaxErrorException("26");
        }
    default:
        return false;
    }
    return false;
}
string QueryPreprocessor::getVar() {
    string word = queryList[cur];
    bool isFound = false;

    // delimiters: ,>;.=)
    int pos = 0;
    for (char c : word) {
        if (c == ',' || c == '>' || c == ';' || c == ')' || c == '.' || c == '=') {
            break;
        }
        pos++;
    }
    return word.substr(0, pos);
}

// for pattern expression only e.g. "axe12+1"
string QueryPreprocessor::getVar(string word) {
    bool isFound = false;

    // delimiters: ,>;.=)
    int pos = 0;
    for (char c : word) {
        if (c == '+' || c == '-' || c == '*' || c == '"') {
            break;
        }
        pos++;
    }
    return word.substr(0, pos);
}

bool QueryPreprocessor::isConstantVar(string var) {
    // [accepted] case: "x+1", _"x+1"_
    // [rejected] case: "x+1, _"x+"_
    string varCopy = var;
    bool isValid = false;
    if (accept(var, '"')) {
        while (accept(var, VARIABLE) || accept(var, CONSTANT)) {
            if (accept(var, '+')) {
                isValid = false;
            } else if (accept(var, '-')) {
                isValid = false;
            } else if (accept(var, '*')) {
                isValid = false;
            } else {
                isValid = true;
            }
        }
        if (isValid == false) {
            throw QuerySyntaxErrorException("Invalid pattern expression 1: "+ varCopy);
        }
        return expect(var, '"');
    } else if (accept(var, '_')) {
        expect(var, '"');

        while (accept(var, VARIABLE) || accept(var, CONSTANT)) {
            if (accept(var, '+')) {
                isValid = false;
            } else if (accept(var, '-')) {
                isValid = false;
            } else if (accept(var, '*')) {
                isValid = false;
            } else {
                isValid = true;
            }
        }

        if (isValid == false) {
            throw QuerySyntaxErrorException("Invalid pattern expression 2: "+varCopy);
        }

        expect(var, '"');
        return expect(var, '_');
    }
}

bool QueryPreprocessor::accept(string &var, char token) {
    if (var[0] == token) {
        var = var.substr(1);
        return true;
    }
    return false;
}

bool QueryPreprocessor::accept(string &var, Symbol token) {
    string var1 = getVar(var);
    if (var1.size() == 0) {
        return false;
    }
    switch (token) {
    case VARIABLE:
        if (isValidVarName(var1)) {
            var = var.substr(var1.size());
            return true;
        }
        return false;
    case CONSTANT:
        if (Utils::IsNonNegativeNumeric(var1)) {
            var = var.substr(var1.size());
            return true;
        }
        return false;
    default:
        return false;
    }
}

bool QueryPreprocessor::expect(string &var, char token) {
    if (var[0] == token) {
        var = var.substr(1);
        return true;
    } else {
        throw QuerySyntaxErrorException("invalid pattern expression");
    }
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
        case STMTLIST:
        case STMT:
        case ASSIGN:
        case WHILE:
        case VARIABLE:
        case CONSTANT:
        case PROGRAM_LINE:
        case PROCEDURE:
        case CALL:
        case IF:
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
Symbol QueryPreprocessor::getVarType(string var) {
    Symbol varType = varSymbolMap[var];
    return varType;
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
    if (var == token) {
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

// if(found) { cur.substr + remove Null; return 1 } else { return 0 }
int QueryPreprocessor::accept(Symbol token) {
    // token = VARIABLE/UNDERSCORE

    string var = getVar();
    if (var.size() < 1) {
        return 0;
    }

    switch (token) {
    default:
        return 0;
    case VARIABLE:
        /* validate for variable a1,"a1" */
        if (isVarExist(var)) {
            return 1;
        } else {
            string varName = var.substr(1, var.size()-2);
            if (isValidVarName(varName)) {
                return 1;
            }
        }
        break;
    case CONSTANT:
        /* validate for constant number 1,2,123 */
        if (Utils::IsNonNegativeNumeric(var)) {
            return 1;
        }
        break;
    case UNDERSCORE:
        /* validate for _ wildcard */
        if (var == "_") {
            return 1;
        }
        break;
    case PATTERN:
        /* validate for pattern expr "a+1", _"a"_ */
        if (isConstantVar(var)) {
            return 1;
        }
        break;
    }
    return 0;
}

// do cur++ if { found } else { throw ExpectedTokenNotFound }
void QueryPreprocessor::expect(string token) {
    if (toLower(queryList[cur]).compare(toLower(token)) == 0) {
        cur++;
    } else {
        throw QuerySyntaxErrorException("29 (str)token is "+ queryList[cur] + token);
    }
}

// do {cur.substr + remove Null}; if(found) return { null } else { throw ExpectedTokenNotFound }
void QueryPreprocessor::expect(char token) {
    // peek to remove empty vector
    peek();
    // wm todo: check if end of query is valid
    if (cur == queryList.size()) throw QuerySyntaxErrorException("30 expect(char)token: cur==size");

    if (queryList[cur][0] == token) {
        queryList[cur] = peek().substr(1);
        peek();
    } else {
        throw QuerySyntaxErrorException("31");
    }
}

// do removeNull; if(Null) return { emptyString } else { return queryList[cur] }
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

string QueryPreprocessor::removeWhitespaces(string str) {
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
    str.erase(end_pos, str.end());
    end_pos = std::remove(str.begin(), str.end(), '\t');
    str.erase(end_pos, str.end());
    return str;
}

void QueryPreprocessor::mergeSeparatedClauses() {
    bool isFound = false;
    while (cur < queryList.size() && !isFound) {
        isFound = queryList[cur].find('(') != std::string::npos;
        isFound = isFound && (queryList[cur].find(')') != std::string::npos);

        if (isFound == false) {
            queryList[cur + 1] = queryList[cur] + queryList[cur + 1];
            cur++;
        }
    }
    queryList[cur] = removeWhitespaces(peek());
}

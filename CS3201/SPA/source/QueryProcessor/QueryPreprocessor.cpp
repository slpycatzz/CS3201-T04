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
        if (!isValidVarName(variableNames[i])) {
            throw QuerySyntaxErrorException("3"+ variableNames[i]);
        }

        /* Validate if variable name has already been declared. */
        if (isVarExist(variableNames[i])) {
            throw QuerySyntaxErrorException("4");
        }

        /* contains declared vars, pass to queryTree */
        varSymbolMap[variableNames[i]] = Constants::StringToSymbol(declarationType);
    }

    return true;
}

// wm todo: case-insensitive compare
bool QueryPreprocessor::processQuery(string query) {
    queryList = Utils::Split(Utils::TrimLeadingSpaces(query), ' ');
    cur = 0;
    /* Expecting first token to be Select (case-sensitive) */
    if (queryList[cur] != SYMBOL_SELECT) {
        throw QuerySyntaxErrorException("5");
    }

    /* parse [Select] ... */
    parseSelect();
    
    if (cur >= queryList.size()) {
        return true;
    }

    /* remaining string: ...[such that]...and...[pattern]...and...[with]...and...*/
    Symbol prevClause = INVALID;



    /* continue parsing [such that... pattern...] clauses until end of query */
    while (cur + 1 < queryList.size()) {
        // wm todo: check if EOL reached or no items left works
        if ((queryList[cur + 1].size() > 0 && queryList[cur + 1][0] == NULL) || queryList[cur + 1].size() == 0) {
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
            throw QuerySyntaxErrorException("abc");
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
            if (accept(SYMBOL_BOOLEAN)) {
                throw QuerySyntaxErrorException("5");
            } else if (accept(VARIABLE)) {
                string var1;
                temp = getVar();
                // Select <"a",noSuchVarDecl> are invalid
                if (getVarType(temp) == INVALID) {
                    throw QuerySyntaxErrorException("6");
                }
                var.push_back(temp);
                varAttrMap[temp] = false;
                queryList[cur] = peek().substr(getVar().size());
                
                if (accept('.')) {
                    string varAttribute1;
                    varAttribute1 = getVar();
                    queryList[cur] = peek().substr(getVar().size());
                    if (isAttributeValid(temp, varAttribute1)) {
                        // only call needs to return true
                        if (getVarType(temp) == CALL && varAttribute1 == "procName") {
                            varAttrMap[temp] = true;
                        }
                    } else {
                        throw QuerySyntaxErrorException("7" + varAttribute1);
                    }
                }
            } else {
                throw QuerySyntaxErrorException("8");
            }
            accept(',');
        }
    } else {
        if (peek() == SYMBOL_BOOLEAN) {
            queryList[cur] = peek().substr(7);
            var.push_back(SYMBOL_BOOLEAN);
            varSymbolMap[SYMBOL_BOOLEAN] = BOOLEAN;
            // qt.insertBooleanDeclaration();
            varAttrMap[SYMBOL_BOOLEAN] = false;
        } else if (accept(VARIABLE)) {
            temp = getVar();
            // Select <"a",noSuchVarDecl> are invalid
            if (getVarType(temp) == INVALID) {
                throw QuerySyntaxErrorException("9");
            }
            var.push_back(temp);
            varAttrMap[temp] = false;
            queryList[cur] = peek().substr(getVar().size());

            // case: <var1.varAttr, var2>
            if (accept('.')) {
                string varAttribute1;
                varAttribute1 = getVar();
                queryList[cur] = peek().substr(getVar().size());
                if (isAttributeValid(temp, varAttribute1)) {
                    // only call needs to return true
                    if (getVarType(temp) == CALL  && varAttribute1 == "procName") {
                        varAttrMap[temp] = true;
                    }
                } else {
                    throw QuerySyntaxErrorException("10");
                }
            }
        } else {
            throw QuerySyntaxErrorException("11" + peek());
        }
    }
    if (var.size() < 1) {
        // No QueryResult found
        throw QuerySyntaxErrorException("12");
    }


    qt.insertDeclaration(varSymbolMap);
    qt.insert(QUERY_RESULT, "placeholder", var);
    qt.insert(QUERY_RESULT, "placeholder", varAttrMap);
}

void QueryPreprocessor::parseSuchThat() {
    vector<string> argList;
    Symbol relation;
    string relationString;
    expect(SYMBOL_SUCH);
    expect(SYMBOL_THAT);
    mergeSeparatedClauses();
    relationString = peek().substr(0, peek().find_first_of('('));
    relation = Constants::StringToSymbol(relationString);
    if (relation == INVALID) {
        throw QuerySyntaxErrorException("13");
    }

    queryList[cur] = peek().substr(relationString.size());
    if (!r.isRelationValid(relation, SUCH_THAT)) {
        throw QuerySyntaxErrorException("14");
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
            bool isRelValid;
            isRelValid = r.isArgValid(relation, Constants::SymbolToString(varType), i);
            // wm todo: special case for Calls("procName",_) where procName is not in PQL side
            if (relation == CALLS || relation == CALLS_TRANSITIVE) {
                if (Utils::IsValidNamingConvention(getVar().substr(1, getVar().size() - 1))) {
                    isRelValid = r.isArgValid(relation, SYMBOL_VARIABLE, i);
                }
            }
            if (isRelValid) {
                argList.push_back(getVar());
            } else {
                throw QuerySyntaxErrorException("15");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(CONSTANT)) {
            if (r.isArgValid(relation, Constants::SymbolToString(CONSTANT), i)) {
                argList.push_back(getVar());  // wm todo getVariable after validation for saving
            } else {
                throw QuerySyntaxErrorException("16");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(UNDERSCORE)) {
            if (r.isArgValid(relation, string(1, CHAR_SYMBOL_UNDERSCORE), i)) {
                argList.push_back(string(1, CHAR_SYMBOL_UNDERSCORE));
            } else {
                throw QuerySyntaxErrorException("17");
            }

            queryList[cur] = peek().substr(getVar().size());

        } else {
            throw QuerySyntaxErrorException("18");
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

    expect(SYMBOL_PATTERN);
    relation = peek();
    relation = relation.substr(0, relation.find_first_of('('));

    queryList[cur] = peek().substr(relation.size());

    if (isValidVarName(relation)) {
        if (!r.isRelationValid(varSymbolMap[relation], PATTERN)) {
            throw QuerySyntaxErrorException("19");
        }
    } else {
        throw QuerySyntaxErrorException("20" + queryList[cur] + "|" + queryList[cur + 1]);
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
                throw QuerySyntaxErrorException("21");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(CONSTANT)) {
            if (r.isArgValid(varSymbolMap[relation], Constants::SymbolToString(CONSTANT), i)) {
                argList.push_back(getVar());
            } else {
                throw QuerySyntaxErrorException("22");
            }
            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(UNDERSCORE)) {
            if (r.isArgValid(varSymbolMap[relation], string(1, CHAR_SYMBOL_UNDERSCORE), i)) {
                argList.push_back(string(1, CHAR_SYMBOL_UNDERSCORE));
            } else {
                throw QuerySyntaxErrorException("23");
            }

            queryList[cur] = peek().substr(getVar().size());
        } else if (accept(PATTERN)) {
            string expressionFront;
            // case: pattern expression string e.g. _"a+1"_, "a+1"
            if (r.isArgValid(varSymbolMap[relation], SYMBOL_PATTERN, i)) {
                string expressionWithBrackets;
                expressionFront = patternList[patternList.size() - 1];
                string expressionBack = "\"";
                if (expressionFront.size() == 2) {
                    expressionBack += "_";
                }
                patternList.pop_back();
                string patternBack = patternList[patternList.size() - 1];
                if (patternBack == "+" || patternBack == "-" || patternBack == "*") {
                    throw QuerySyntaxErrorException("24 invalid pattern found");
                }
                expressionWithBrackets = Utils::GetExactExpressionWithBrackets(Utils::GetPostfixExpression(patternList));
                argList.push_back(expressionFront + expressionWithBrackets + expressionBack);
                patternList.clear();
            } else {
                throw QuerySyntaxErrorException("25");
            }

            queryList[cur] = peek().substr(getPatternExpression().size() + expressionFront.size() - 1);
        } else {
            throw QuerySyntaxErrorException("26");
        }
        i++;
    } while (accept(',') == 1 && i < 3);
    expect(')');

    argList.insert(argList.begin(), relation);
    qt.insert(PATTERN, SYMBOL_PATTERN, argList);
}

void QueryPreprocessor::parseAnd(Symbol prevClause) {
    expect(SYMBOL_AND);
    switch (prevClause) {
    case SUCH_THAT:
        queryList[--cur] = SYMBOL_THAT;
        queryList[--cur] = SYMBOL_SUCH;
        parseSuchThat();
        break;
    case PATTERN:
        out += queryList[cur - 1];
        queryList[--cur] = SYMBOL_PATTERN;
        parsePattern();
        break;
    case WITH:
        queryList[--cur] = SYMBOL_WITH;
        parseWith();
    }
}

void QueryPreprocessor::parseWith() {
    string var, varAttribute;
    string var2, varAttribute2;
    expect(SYMBOL_WITH);
    mergeSeparatedClauses();
    var = getVar();
    queryList[cur] = peek().substr(getVar().size());
    if (accept('.')) {
        // case: var1.varAttr = ...
        varAttribute = getVar();
        queryList[cur] = peek().substr(getVar().size());
        peek();
        if (!isAttributeValid(var, varAttribute)) {
            throw QuerySyntaxErrorException("27"+var + " does not have attribute " + varAttribute);
        }
    } else {
        varAttribute = "";
    }

    expect('=');
    var2 = getVar();
    queryList[cur] = peek().substr(var2.size());
    if (accept('.')) {
        // case: ... = var2.varAttr
        varAttribute2 = getVar();
        queryList[cur] = peek().substr(varAttribute2.size());
    } else {
        // case: ... = var2
        varAttribute2 = "";
    }
    if (isAttributeValid(var, varAttribute, var2, varAttribute2)) {
        vector<string> varList = { var, var2 };
        Symbol varAttrType = getAttributeType(var, varAttribute);

        qt.insert(WITH, Constants::SymbolToString(varAttrType), varList);
    } else {
        throw QuerySyntaxErrorException("22");
    }
    peek();
}

// return only CONSTANT | VARIABLE | INVALID
Symbol QueryPreprocessor::getAttributeType(string var, string varAttr) {
    Symbol varType = getVarType(var);
    if (varType == INVALID) {
        // case: CONSTANT or "var"
        if (var[0] == '\"') {
            string varName = var.substr(1, var.size() - 2);
            if (!isValidVarName(varName)) {
                throw QuerySyntaxErrorException("91");
            }
            return VARIABLE;
        } else {
            if (!Utils::IsNonNegativeNumeric(var)) {
                throw QuerySyntaxErrorException("92");
            }
            return CONSTANT;
        }
    }
    switch (varType) {
    case PROCEDURE:
    case VARIABLE:
        return VARIABLE;
    case CONSTANT:
    case PROGRAM_LINE:
    case STMT:
    case ASSIGN:
    case WHILE:
    case IF:
        return CONSTANT;
        // special case for call
    case CALL:
        if (varAttr == "" || varAttr == "stmt#") {
            return CONSTANT;
        }
        return VARIABLE;
    default:
        return INVALID;
    }

    return INVALID;
}
// case: var1.varAttr = var2.varAttr
bool QueryPreprocessor::isAttributeValid(string var, string varAttr, string var2, string varAttr2) {
    Symbol attrType1 = getAttributeType(var, varAttr);
    Symbol attrType2 = getAttributeType(var2, varAttr2);
    bool isValid;
    // check same attrType e.g. CONSTANT == CONSTANT | VARIABLE == VARIABLE
    isValid = isAttributeValid(var, varAttr) && isAttributeValid(var2, varAttr2);
    // check var and attrType is valid e.g. proc == "procName"

    isValid = (attrType1 == attrType2) && isValid;
    return isValid;
}

// test for var.varAttr: getAttr(var) == varAttr
bool QueryPreprocessor::isAttributeValid(string var, string varAttribute) {
    Symbol attrType = INVALID;
    bool isConstantOrVariable = false;

    if (getVarType(var) == INVALID) {
        isConstantOrVariable = true;
    }

    attrType = getAttributeType(var, varAttribute);

    if (varAttribute == "") {
    } else if (!isConstantOrVariable) {
        switch (getVarType(var)) {
        case PROCEDURE:
            if (varAttribute == "procName") {
                break;
            } else {
                throw QuerySyntaxErrorException("100");
            }
        case CALL:
            if (varAttribute == "procName" || varAttribute == "stmt#") {
                break;
            } else {
                throw QuerySyntaxErrorException("1" + var + "and" + varAttribute + "is an invalid attribute type");
            }
        case VARIABLE:
            if (varAttribute == "varName") {
                break;
            } else {
                throw QuerySyntaxErrorException(varAttribute + "2is an invalid attribute type");
            }
        case CONSTANT:
            if (varAttribute == "value") {
                break;
            } else {
                throw QuerySyntaxErrorException(var + varAttribute + "3is an invalid attribute type");
            }
        case STMT:
        case ASSIGN:
        case WHILE:
        case IF:
        case PROGRAM_LINE:
            if (varAttribute == "stmt#") {
                break;
            } else {
                throw QuerySyntaxErrorException(varAttribute + "4is an invalid attribute type");
            }
        default:
            throw QuerySyntaxErrorException("21");
        }
    } else {
        throw QuerySyntaxErrorException("1CONSTANT/VARIABLE should not have a varAttr");
    }
    if (attrType == INVALID) {
        return false;
    }
    return true;
}

string QueryPreprocessor::getVar() {
    string word = queryList[cur];
    bool isFound = false;

    // delimiters: ,>;.=)
    int pos = 0;
    for (char c : word) {
        if (c == ',' || c == '>' || c == ';' || c == ')' || c == '.' || c == '=') {
            isFound = true;
            break;
        }
        pos++;
    }
    return word.substr(0, pos);
}

string QueryPreprocessor::getPatternExpression() {
    string word = queryList[cur];

    return word.substr(0, word.find_last_of('"') + 1);
}

// for pattern expression only e.g. "axe12+1"
string QueryPreprocessor::getVar(string word) {
    bool isFound = false;

    // delimiters: ,>;.=)
    int pos = 0;
    for (char c : word) {
        if (c == '+' || c == '-' || c == '*' || c == '"' || c == ')' || c == '(') {
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
    if (accept(var, '"')) {
        isConstantVarTerm(var);
        patternList.push_back("\"");
        return expect(var, '"');
    } else if (accept(var, '_')) {
        expect(var, '"');
        isConstantVarTerm(var);
        expect(var, '"');
        patternList.push_back("_\"");
        return expect(var, '_');
    }
}


void QueryPreprocessor::callFactorRecognizer(string &var) {
    string name = getVar(var);

    /* Higher precedence for expressions in (). */
    if (accept(var, CHAR_SYMBOL_OPENBRACKET)) {
        patternList.push_back(string(1, CHAR_SYMBOL_OPENBRACKET));

        callExpressionRecognizer(var);

        expect(var, CHAR_SYMBOL_CLOSEBRACKET);

        patternList.push_back(string(1, CHAR_SYMBOL_CLOSEBRACKET));

        /* Variable. */
    } else if (accept(var, VARIABLE)) {
        patternList.push_back(name);
        /* Constant. */
    } else if (accept(var, CONSTANT)) {
        patternList.push_back(name);
    } else if (var[0] == '\"') {
    } else if (var[0] == ')') {
    } else {
        var += "end";
        throw QuerySyntaxErrorException("21invalid var or const" + var);
    }
}

void QueryPreprocessor::callExpressionRecognizer(string &var) {
    callFactorRecognizer(var);
    int limit = 0;
    while (true || limit++ > 999) {
        if (accept(var, CHAR_SYMBOL_PLUS)) {
            patternList.push_back(string(1, CHAR_SYMBOL_PLUS));
        } else if (accept(var, CHAR_SYMBOL_MINUS)) {
            patternList.push_back(string(1, CHAR_SYMBOL_MINUS));
        } else if (accept(var, CHAR_SYMBOL_MULTIPLY)) {
            patternList.push_back(string(1, CHAR_SYMBOL_MULTIPLY));
        } else {
            break;
        }

        callFactorRecognizer(var);
    }

    if (limit > 999) {
        throw QuerySyntaxErrorException("111Error occurred somewhere");
    }
}

bool QueryPreprocessor::isConstantVarTerm(string &var) {
    bool isValid = false;
    callExpressionRecognizer(var);
    return true;
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
    if (var.size() == 0) {
        var = peek();
    }
    if (var[0] == token) {
        var = var.substr(1);
        return true;
    } else {
        var += "end";
        throw QuerySyntaxErrorException("123invalid pattern expression" + var);
    }
}

bool QueryPreprocessor::isVarExist(string var) {
    /*
    if (toLower(var).compare("boolean") == 0) {
    return true;
    }
    */
    if (varSymbolMap.find(var) != varSymbolMap.end()) {
        return true;
    }
    return false;
}

bool QueryPreprocessor::isValidVarName(string varName) {
    if (varName.length() == 0) {
        return false;
    }

    // varName may be token(e.g. assign assign; if if;) and BOOLEAN
    // if (Constants::StringToSymbol(varName) != INVALID) {
    //    return false;
    //}

    const std::regex regex_pattern("^[a-zA-Z][a-zA-Z0-9_#]*$");
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
    /*
    // Bonus feature
    case PLUS:
    case MINUS:
    case TIMES:
    */
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
            string varName = var.substr(1, var.size() - 2);
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
        if (var == string(1, CHAR_SYMBOL_UNDERSCORE)) {
            return 1;
        }
        break;
    case PATTERN:
        /* validate for pattern expr "a+1", _"a"_ */
        var = getPatternExpression();
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
        throw QuerySyntaxErrorException("29 (str)token is " + queryList[cur] + token);
    }
}

// do {cur.substr + remove Null}; if(found) return { null } else { throw ExpectedTokenNotFound }
void QueryPreprocessor::expect(char token) {
    // peek to remove empty vector
    peek();
    // wm todo: check if end of query is valid
    if (cur == queryList.size()) throw QuerySyntaxErrorException("30");

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
    while (cur < queryList.size() - 1 && !isFound) {
        isFound = ((queryList[cur + 1].find("such") != std::string::npos)
            || (queryList[cur + 1].find("pattern") != std::string::npos)
            || (queryList[cur + 1].find("with") != std::string::npos)
            || (queryList[cur + 1].find("and") != std::string::npos));

        if (isFound == false) {
            queryList[cur + 1] = queryList[cur] + queryList[cur + 1];
            cur++;
        }
    }
    queryList[cur] = removeWhitespaces(peek());
}

vector<string> QueryPreprocessor::unitTestStubGetParams(string filePath, bool isInput) {
    string currentLine, query;
    vector<string> queryLines;
    std::ifstream fileStream(filePath);
    int lineNumber = 0;

    if (isInput) {
        while (std::getline(fileStream, currentLine)) {
            if (lineNumber % 5 == 1) {
                query = currentLine;
            }
            if (lineNumber % 5 == 2) {
                queryLines.push_back(query + currentLine);
                query = "";
            }
            lineNumber++;
        }
        return queryLines;
    } else {
        while (std::getline(fileStream, currentLine)) {
            if (lineNumber % 2 == 0) {
            }
            if (lineNumber % 2 == 1) {
                queryLines.push_back(currentLine);
            }
            lineNumber++;
        }
        return queryLines;
    }
}

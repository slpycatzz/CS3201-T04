#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Constants.h"
#include "PKB/AST.h"
#include "PKB/PKB.h"
#include "PKB/Table.h"
#include "PKB/TransitiveTable.h"
#include "TreeNode.h"

using std::map;
using std::set;
using std::string;
using std::vector;

unsigned int PKB::numberOfProcedure_ = 0;
unsigned int PKB::numberOfAssign_    = 0;
unsigned int PKB::numberOfWhile_     = 0;
unsigned int PKB::numberOfIf_        = 0;
unsigned int PKB::numberOfCall_      = 0;

Table<unsigned int, string> PKB::constantTable_;
Table<unsigned int, string> PKB::variableTable_;
Table<unsigned int, string> PKB::procedureTable_;
Table<unsigned int, string> PKB::controlVariableTable_;
Table<unsigned int, string> PKB::stmtTable_;

Table<unsigned int, Symbol> PKB::priorityTable_;

Table<unsigned int, string> PKB::expressionTable_;
Table<unsigned int, string> PKB::subExpressionTable_;

/* Deprecated */
Table<unsigned int, TreeNode*> PKB::assignTable_;

Table<string, string> PKB::callsTable_;
TransitiveTable<string, string> PKB::callsTransitiveTable_;

Table<unsigned int, string> PKB::modifiesTable_;
Table<string, string> PKB::modifiesProcedureTable_;

Table<unsigned int, string> PKB::usesTable_;
Table<string, string> PKB::usesProcedureTable_;

Table<unsigned int, unsigned int> PKB::parentTable_;
TransitiveTable<unsigned int, unsigned int> PKB::parentTransitiveTable_;

Table<unsigned int, unsigned int> PKB::followsTable_;
TransitiveTable<unsigned int, unsigned int> PKB::followsTransitiveTable_;

/* START - AST functions */

void PKB::SetASTRoot(TreeNode *root) {
    AST::getInstance()->setRoot(root);
}

TreeNode* PKB::GetASTRoot() {
    return AST::getInstance()->getRoot();
}

TreeNode* PKB::CreateASTNode(Symbol symbol) {
    return AST::getInstance()->createNode(symbol);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, string value) {
    return AST::getInstance()->createNode(symbol, value);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, unsigned int stmtNumber) {
    return AST::getInstance()->createNode(symbol, stmtNumber);
}

TreeNode* PKB::CreateASTNode(Symbol symbol, unsigned int stmtNumber, string value) {
    return AST::getInstance()->createNode(symbol, stmtNumber, value);
}

void PKB::PrintASTTree() {
    AST::getInstance()->printTree();
}

/* END   - AST functions */
/* START - Constant table functions */

void PKB::GenerateConstantTable(set<string> constants) {
    unsigned int i = 1;
    for (auto &constant : constants) {
        constantTable_.insert(i++, constant);
    }
}

bool PKB::HasConstant(string constantValue) {
    return constantTable_.hasValue(constantValue);
}

string PKB::GetConstantValue(unsigned int index) {
    return (constantTable_.hasKey(index)) ? constantTable_.getValue(index) : "";
}

unsigned int PKB::GetConstantIndex(string constantValue) {
    return (constantTable_.hasValue(constantValue)) ? constantTable_.getKey(constantValue) : 0;
}

vector<string> PKB::GetAllConstantValues() {
    set<string> result = constantTable_.getValues();

    vector<string> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintConstantTable() {
    constantTable_.printTable();
}

/* END   - Constant table functions */
/* START - Variable table functions */

void PKB::GenerateVariableTable(set<string> variableNames) {
    unsigned int i = 1;
    for (auto &variableName : variableNames) {
        variableTable_.insert(i++, variableName);
    }
}

bool PKB::HasVariable(string variableName) {
    return variableTable_.hasValue(variableName);
}

string PKB::GetVariableName(unsigned int index) {
    return (variableTable_.hasKey(index)) ? variableTable_.getValue(index) : "";
}

unsigned int PKB::GetVariableIndex(string variableName) {
    return (variableTable_.hasValue(variableName)) ? variableTable_.getKey(variableName) : 0;
}

vector<string> PKB::GetAllVariableNames() {
    set<string> result = variableTable_.getValues();

    vector<string> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintVariableTable() {
    variableTable_.printTable();
}

/* END   - Variable table functions */
/* START - Procedure table functions */

void PKB::GenerateProcedureTable(set<string> procedureNames) {
    unsigned int i = 1;
    for (auto &procedureName : procedureNames) {
        numberOfProcedure_++;
        procedureTable_.insert(i++, procedureName);
    }
}

bool PKB::HasProcedure(string procedureName) {
    return procedureTable_.hasValue(procedureName);
}

string PKB::GetProcedureName(unsigned int index) {
    return (procedureTable_.hasKey(index)) ? procedureTable_.getValue(index) : "";
}

unsigned int PKB::GetProcedureIndex(string procedureName) {
    return (procedureTable_.hasValue(procedureName)) ? procedureTable_.getKey(procedureName) : 0;
}

vector<string> PKB::GetAllProcedures() {
    set<string> result = procedureTable_.getValues();

    vector<string> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintProcedureTable() {
    procedureTable_.printTable();
}

/* END   - Procedure table functions */
/* START - Control variable table functions */

void PKB::GenerateControlVariableTable(map<unsigned int, string> controlVariables) {
    for (auto &controlVariable : controlVariables) {
        controlVariableTable_.insert(controlVariable.first, controlVariable.second);
    }
}

bool PKB::HasControlVariable(std::string controlVariable) {
    return controlVariableTable_.hasValue(controlVariable);
}

bool PKB::HasControlVariableAtStmtNumber(unsigned int stmtNumber, string controlVariable) {
    return controlVariableTable_.hasKeyToValue(stmtNumber, controlVariable);
}

string PKB::GetControlVariable(unsigned int stmtNumber) {
    return (controlVariableTable_.hasKey(stmtNumber)) ? controlVariableTable_.getValue(stmtNumber) : "";
}

vector<string> PKB::GetAllControlVariables() {
    set<string> result = controlVariableTable_.getValues();

    vector<string> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

void PKB::PrintControlVariableTable() {
    controlVariableTable_.printTable();
}

/* END - Control variable table functions */
/* START - Stmt table functions */

void PKB::GenerateStmtTable(map<unsigned int, string> stmts) {
    for (auto &stmt : stmts) {
        switch (Constants::StringToSymbol(stmt.second)) {
            default:
                break;

            case ASSIGN:
                numberOfAssign_++;
                break;

            case WHILE:
                numberOfWhile_++;
                break;

            case IF:
                numberOfIf_++;
                break;

            case CALL:
                numberOfCall_++;
                break;
        }

        stmtTable_.insert(stmt.first, stmt.second);
    }
}

string PKB::GetStmtSymbol(unsigned int stmtNumber) {
    return (stmtTable_.hasKey(stmtNumber)) ? stmtTable_.getValue(stmtNumber) : "";
}

vector<unsigned int> PKB::GetSymbolStmtNumbers(string symbol) {
    set<unsigned int> result = (symbol == SYMBOL_STMT) ? stmtTable_.getKeys() : stmtTable_.getKeys(symbol);

    vector<unsigned int> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

vector<unsigned int> PKB::GetSymbolStmtNumbers(Symbol symbol) {
    return GetSymbolStmtNumbers(Constants::SymbolToString(symbol));
}

void PKB::PrintStmtTable() {
    stmtTable_.printTable();
}

/* END   - Stmt table functions */
/* START - Priority table functions */

void PKB::GeneratePriorityTable() {
    vector<std::pair<unsigned int, Symbol>> tablesSize;

    tablesSize.push_back(std::make_pair(callsTable_.getNumberOfValues(),   CALLS));
    tablesSize.push_back(std::make_pair(followsTable_.getNumberOfValues(), FOLLOWS));
    tablesSize.push_back(std::make_pair(parentTable_.getNumberOfValues(),  PARENT));

    tablesSize.push_back(std::make_pair(callsTransitiveTable_.getNumberOfValues(),   CALLS_TRANSITIVE));
    tablesSize.push_back(std::make_pair(followsTransitiveTable_.getNumberOfValues(), FOLLOWS_TRANSITIVE));
    tablesSize.push_back(std::make_pair(parentTransitiveTable_.getNumberOfValues(),  PARENT_TRANSITIVE));

    tablesSize.push_back(std::make_pair(modifiesTable_.getNumberOfValues(),          MODIFIES));
    tablesSize.push_back(std::make_pair(modifiesProcedureTable_.getNumberOfValues(), MODIFIES_PROCEDURE));
    tablesSize.push_back(std::make_pair(usesTable_.getNumberOfValues(),              USES));
    tablesSize.push_back(std::make_pair(usesProcedureTable_.getNumberOfValues(),     USES_PROCEDURE));

    /* Sort the size in ascending order to determine the priority. */
    std::sort(tablesSize.begin(), tablesSize.end(), ComparePairAscending);

    for (unsigned int i = 0; i < tablesSize.size(); i++) {
        priorityTable_.insert(i + 1, tablesSize[i].second);
    }
}

unsigned int PKB::GetPriority(Symbol symbol) {
    return (priorityTable_.hasValue(symbol)) ? priorityTable_.getKey(symbol) : 0;
}

void PKB::PrintPriorityTable() {
    for (const auto &pair : priorityTable_.getKeyToValuesMap()) {
        std::cout << pair.first << " -> { ";

        for (const auto &value : pair.second) {
            std::cout << Constants::SymbolToString(value) << " ";
        }

        std::cout << "}" << std::endl;
    }

    std::cout << "=====================" << std::endl;

    for (const auto &pair : priorityTable_.getValueToKeysMap()) {
        std::cout << Constants::SymbolToString(pair.first) << " -> { ";

        for (const auto &key : pair.second) {
            std::cout << key << " ";
        }

        std::cout << "}" << std::endl;
    }
}

/* END - Priority table functions */
/* START - Expression table functions */

void PKB::GenerateExpressionTable(map<unsigned int, string> expressions) {
    for (auto &pair : expressions) {
        expressionTable_.insert(pair.first, pair.second);
    }
}

void PKB::GenerateSubExpressionTable(map<unsigned int, set<string>> subExpressions) {
    for (auto &pair : subExpressions) {
        subExpressionTable_.insert(pair.first, pair.second);
    }
}

bool PKB::IsExactPattern(unsigned int stmtNumber, string controlVariable, string expression) {
    string stmtSymbol = GetStmtSymbol(stmtNumber);

    if (stmtSymbol == SYMBOL_WHILE || stmtSymbol == SYMBOL_IF) {
        return HasControlVariableAtStmtNumber(stmtNumber, controlVariable);

    } else if (stmtSymbol == SYMBOL_ASSIGN) {
        if (HasControlVariableAtStmtNumber(stmtNumber, controlVariable)) {
            return IsExactExpression(stmtNumber, expression);
        }
    }

    return false;
}

bool PKB::IsSubPattern(unsigned int stmtNumber, string controlVariable, string subExpression) {
    string stmtSymbol = GetStmtSymbol(stmtNumber);

    if (stmtSymbol == SYMBOL_WHILE || stmtSymbol == SYMBOL_IF) {
        return HasControlVariableAtStmtNumber(stmtNumber, controlVariable);

    } else if (stmtSymbol == SYMBOL_ASSIGN) {
        if (HasControlVariableAtStmtNumber(stmtNumber, controlVariable)) {
            return IsSubExpression(stmtNumber, subExpression);
        }
    }

    return false;
}

bool PKB::HasExactExpression(string expression) {
    return expressionTable_.hasValue(expression);
}

bool PKB::HasSubExpression(string subExpression) {
    return subExpressionTable_.hasValue(subExpression);
}

bool PKB::IsExactExpression(unsigned int stmtNumber, string expression) {
    return expressionTable_.hasKeyToValue(stmtNumber, expression);
}

bool PKB::IsSubExpression(unsigned int stmtNumber, string subExpression) {
    return subExpressionTable_.hasKeyToValue(stmtNumber, subExpression);
}

void PKB::PrintExactExpressionTable() {
    expressionTable_.printTable();
}

void PKB::PrintSubExpressionTable() {
    subExpressionTable_.printTable();
}

/* END   - Expression table functions */
/* START - Calls table functions */

void PKB::GenerateCallsTable(map<string, set<string>> calls) {
    for (auto &pair : calls) {
        callsTable_.insert(pair.first, pair.second);
    }

    callsTransitiveTable_.generateKeyToValueTransitiveMap(callsTable_);
    callsTransitiveTable_.generateValueToKeyTransitiveMap(callsTable_);
}

bool PKB::IsCalls(string calling, string called) {
    return callsTable_.hasKeyToValue(calling, called);
}

bool PKB::IsCalls(string calling, set<string> called) {
    return callsTable_.hasKeyToValues(calling, called);
}

bool PKB::IsCallsTransitive(string calling, string called) {
    return callsTransitiveTable_.hasKeyToValue(calling, called);
}

bool PKB::IsCallsTransitive(string calling, set<string> called) {
    return callsTransitiveTable_.hasKeyToValues(calling, called);
}

string PKB::GetCalling(string called) {
    return (callsTable_.hasValue(called)) ? callsTable_.getKey(called) : "";
}

set<string> PKB::GetCalled(string calling) {
    return callsTable_.getValues(calling);
}

set<string> PKB::GetCallingTransitive(string called) {
    return callsTransitiveTable_.getKeys(called);
}

set<string> PKB::GetCalledTransitive(string calling) {
    return callsTransitiveTable_.getValues(calling);
}

void PKB::PrintCallsTable() {
    callsTable_.printTable();
}

void PKB::PrintCallsTransitiveTable() {
    callsTransitiveTable_.printTable();
}

/* END   - Calls table functions */
/* START - Modifies table functions */

void PKB::GenerateModifiesTable(map<unsigned int, set<string>> modifies) {
    for (auto &pair : modifies) {
        modifiesTable_.insert(pair.first, pair.second);
    }
}

void PKB::GenerateModifiesProcedureTable(map<string, set<string>> modifiesProcedure) {
    for (auto &pair : modifiesProcedure) {
        modifiesProcedureTable_.insert(pair.first, pair.second);
    }
}

bool PKB::IsModifies(unsigned int stmtNumber, string variableName) {
    return modifiesTable_.hasKeyToValue(stmtNumber, variableName);
}

bool PKB::IsModifies(unsigned int stmtNumber, set<string> variableNames) {
    return modifiesTable_.hasKeyToValues(stmtNumber, variableNames);
}

bool PKB::IsModifiesProcedure(string procedureName, string variableName) {
    return modifiesProcedureTable_.hasKeyToValue(procedureName, variableName);
}

bool PKB::IsModifiesProcedure(string procedureName, set<string> variableNames) {
    return modifiesProcedureTable_.hasKeyToValues(procedureName, variableNames);
}

set<string> PKB::GetModifiedVariables(unsigned int stmtNumber) {
    return modifiesTable_.getValues(stmtNumber);
}

set<unsigned int> PKB::GetStmtNumberModifying(string variableName) {
    return modifiesTable_.getKeys(variableName);
}

set<string> PKB::GetProcedureModifiedVariables(string procedureName) {
    return modifiesProcedureTable_.getValues(procedureName);
}

set<string> PKB::GetProceduresNameModifying(string variableName) {
    return modifiesProcedureTable_.getKeys(variableName);
}

void PKB::PrintModifiesTable() {
    modifiesTable_.printTable();
}

void PKB::PrintModifiesProcedureTable() {
    modifiesProcedureTable_.printTable();
}

/* END   - Modifies table functions */
/* START - Uses table functions */

void PKB::GenerateUsesTable(map<unsigned int, set<string>> uses) {
    for (auto &pair : uses) {
        usesTable_.insert(pair.first, pair.second);
    }
}

void PKB::GenerateUsesProcedureTable(map<string, set<string>> usesProcedure) {
    for (auto &pair : usesProcedure) {
        usesProcedureTable_.insert(pair.first, pair.second);
    }
}

bool PKB::IsUses(unsigned int stmtNumber, string variableName) {
    return usesTable_.hasKeyToValue(stmtNumber, variableName);
}

bool PKB::IsUses(unsigned int stmtNumber, set<string> variableNames) {
    return usesTable_.hasKeyToValues(stmtNumber, variableNames);
}

bool PKB::IsUsesProcedure(string procedureName, string variableName) {
    return usesProcedureTable_.hasKeyToValue(procedureName, variableName);
}

bool PKB::IsUsesProcedure(string procedureName, set<string> variableNames) {
    return usesProcedureTable_.hasKeyToValues(procedureName, variableNames);
}

set<string> PKB::GetUsedVariables(unsigned int stmtNumber) {
    return usesTable_.getValues(stmtNumber);
}

set<unsigned int> PKB::GetStmtNumberUsing(string variableName) {
    return usesTable_.getKeys(variableName);
}

set<string> PKB::GetProcedureUsedVariables(string procedureName) {
    return usesProcedureTable_.getValues(procedureName);
}

set<string> PKB::GetProceduresNameUsing(string variableName) {
    return usesProcedureTable_.getKeys(variableName);
}

void PKB::PrintUsesTable() {
    usesTable_.printTable();
}

void PKB::PrintUsesProcedureTable() {
    usesProcedureTable_.printTable();
}

/* END   - Uses table functions */
/* START - Parent table functions */

void PKB::GenerateParentTable(map<unsigned int, set<unsigned int>> parent) {
    for (auto &pair : parent) {
        parentTable_.insert(pair.first, pair.second);
    }

    parentTransitiveTable_.generateKeyToValueTransitiveMap(parentTable_);
    parentTransitiveTable_.generateValueToKeyTransitiveMap(parentTable_);
}

bool PKB::IsParent(unsigned int parent, unsigned int child) {
    return parentTable_.hasKeyToValue(parent, child);
}

bool PKB::IsParent(unsigned int parent, set<unsigned int> children) {
    return parentTable_.hasKeyToValues(parent, children);
}

bool PKB::IsParentTransitive(unsigned int parent, unsigned int child) {
    return parentTransitiveTable_.hasKeyToValue(parent, child);
}

bool PKB::IsParentTransitive(unsigned int parent, set<unsigned int> children) {
    return parentTransitiveTable_.hasKeyToValues(parent, children);
}

unsigned int PKB::GetParent(unsigned int child) {
    return (parentTable_.hasValue(child)) ? parentTable_.getKey(child) : 0;
}

set<unsigned int> PKB::GetChildren(unsigned int parent) {
    return parentTable_.getValues(parent);
}

set<unsigned int> PKB::GetParentsTransitive(unsigned int child) {
    return parentTransitiveTable_.getKeys(child);
}

set<unsigned int> PKB::GetChildrenTransitive(unsigned int parent) {
    return parentTransitiveTable_.getValues(parent);
}

void PKB::PrintParentTable() {
    parentTable_.printTable();
}

void PKB::PrintParentTransitiveTable() {
    parentTransitiveTable_.printTable();
}

/* END   - Parent table functions */
/* START - Follows table functions */

void PKB::GenerateFollowsTable(map<unsigned int, unsigned int> follows) {
    for (auto &pair : follows) {
        followsTable_.insert(pair.first, pair.second);
    }

    followsTransitiveTable_.generateKeyToValueTransitiveMap(followsTable_);
    followsTransitiveTable_.generateValueToKeyTransitiveMap(followsTable_);
}

bool PKB::IsFollows(unsigned int follows, unsigned int following) {
    return followsTable_.hasKeyToValue(follows, following);
}

bool PKB::IsFollows(unsigned int follows, set<unsigned int> followings) {
    return followsTable_.hasKeyToValues(follows, followings);
}

bool PKB::IsFollowsTransitive(unsigned int follows, unsigned int following) {
    return followsTransitiveTable_.hasKeyToValue(follows, following);
}

bool PKB::IsFollowsTransitive(unsigned int follows, set<unsigned int> followings) {
    return followsTransitiveTable_.hasKeyToValues(follows, followings);
}

unsigned int PKB::GetFollows(unsigned int following) {
    return (followsTable_.hasValue(following)) ? followsTable_.getKey(following) : 0;
}

unsigned int PKB::GetFollowing(unsigned int follows) {
    return (followsTable_.hasKey(follows)) ? followsTable_.getValue(follows) : 0;
}

set<unsigned int> PKB::GetFollowsTransitive(unsigned int following) {
    return followsTransitiveTable_.getKeys(following);
}

set<unsigned int> PKB::GetFollowingTransitive(unsigned int follows) {
    return followsTransitiveTable_.getValues(follows);
}

void PKB::PrintFollowsTable() {
    followsTable_.printTable();
}

void PKB::PrintFollowsTransitiveTable() {
    followsTransitiveTable_.printTable();
}

/* END   - Follows table functions */
/* START - Miscellaneous functions */

unsigned int PKB::GetNumberOfProcedure() {
    return numberOfProcedure_;
}

unsigned int PKB::GetNumberOfAssign() {
    return numberOfAssign_;
}

unsigned int PKB::GetNumberOfWhile() {
    return numberOfWhile_;
}

unsigned int PKB::GetNumberOfIf() {
    return numberOfIf_;
}

unsigned int PKB::GetNumberOfCall() {
    return numberOfCall_;
}

unsigned int PKB::GetNumberOfContainerStmt() {
    return (numberOfWhile_ + numberOfIf_);
}

void PKB::Clear() {
    numberOfProcedure_ = 0;
    numberOfAssign_ = 0;
    numberOfWhile_ = 0;
    numberOfIf_ = 0;
    numberOfCall_ = 0;

    constantTable_.clear();
    variableTable_.clear();
    procedureTable_.clear();
    stmtTable_.clear();
    assignTable_.clear();

    callsTable_.clear();
    callsTransitiveTable_.clear();

    modifiesTable_.clear();
    modifiesProcedureTable_.clear();

    usesTable_.clear();
    usesProcedureTable_.clear();

    parentTable_.clear();
    parentTransitiveTable_.clear();

    followsTable_.clear();
    followsTransitiveTable_.clear();
}

bool PKB::ComparePairAscending(const std::pair<unsigned int, Symbol> &pairOne, const std::pair<unsigned int, Symbol> &pairTwo) {
    return pairOne.first < pairTwo.first;
}

/* START - Miscellaneous functions */
/* START - Deprecated */

void PKB::GenerateAssignTable(map<unsigned int, TreeNode*> assigns) {
    for (auto &assign : assigns) {
        assignTable_.insert(assign.first, assign.second);
    }
}

TreeNode* PKB::GetAssignTreeNode(unsigned int stmtNumber) {
    return (assignTable_.hasKey(stmtNumber)) ? assignTable_.getValue(stmtNumber) : nullptr;
}

vector<TreeNode*> PKB::GetAllAssignTreeNodes() {
    set<TreeNode*> result = assignTable_.getValues();

    vector<TreeNode*> vec(result.size());
    std::copy(result.begin(), result.end(), vec.begin());

    return vec;
}

bool PKB::IsExactPattern(unsigned int stmtNo, string varName, TreeNode* exprTree) {
    TreeNode* RHS(PKB::GetAssignTreeNode(stmtNo)->getChildren()[1]);

    bool matchLHS(PKB::IsModifies(stmtNo, varName));
    bool matchRHS(Utils::IsSameTree(*RHS, *exprTree));

    return (matchLHS && matchRHS);
}

bool PKB::IsSubPattern(unsigned int stmtNo, string varName, TreeNode* exprTree) {
    TreeNode* RHS(PKB::GetAssignTreeNode(stmtNo)->getChildren()[1]);

    bool matchLHS(PKB::IsModifies(stmtNo, varName));
    bool matchRHS(Utils::IsSubTree(*RHS, *exprTree));

    return (matchLHS && matchRHS);
}

bool PKB::HasExactPattern(TreeNode* exprTree) {
    for (TreeNode* node : GetAllAssignTreeNodes()) {
        if (Utils::IsSameTree(*node->getChildren()[1], *exprTree)) {
            return true;
        }
    }

    return false;
}

bool PKB::HasSubPattern(TreeNode* exprTree) {
    for (TreeNode* node : GetAllAssignTreeNodes()) {
        if (Utils::IsSubTree(*node->getChildren()[1], *exprTree)) {
            return true;
        }
    }

    return false;
}

bool PKB::IsExactRHS(unsigned int stmtNo, TreeNode* exprTree) {
    TreeNode* node(GetAssignTreeNode(stmtNo));
    return Utils::IsSameTree(*node->getChildren()[1], *exprTree);
}

bool PKB::IsSubRHS(unsigned int stmtNo, TreeNode* exprTree) {
    TreeNode* node(GetAssignTreeNode(stmtNo));
    return Utils::IsSubTree(*node->getChildren()[1], *exprTree);
}

/* END - Deprecated */

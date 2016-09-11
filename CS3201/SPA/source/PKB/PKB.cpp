#include <map>
#include <set>
#include <string>
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
Table<unsigned int, string> PKB::stmtTable_;
Table<unsigned int, TreeNode*> PKB::assignTable_;

Table<unsigned int, string> PKB::modifiesTable_;
Table<string, string> PKB::modifiesProcedureTable_;

Table<unsigned int, string> PKB::usesTable_;
Table<string, string> PKB::usesProcedureTable_;

Table<unsigned int, unsigned int> PKB::parentTable_;
TransitiveTable<unsigned int, unsigned int> PKB::parentTransitiveTable_;

Table<unsigned int, unsigned int> PKB::followsTable_;
TransitiveTable<unsigned int, unsigned int> PKB::followsTransitiveTable_;

PKB::PKB() {}

PKB::~PKB() {}

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

void PKB::GenerateConstantTable(vector<string> constants) {
    unsigned int i = 1;
    for (auto &constant : constants) {
        if (!constantTable_.hasValue(constant)) {
            constantTable_.insert(i++, constant);
        }
    }
}

bool PKB::HasConstant(string constantValue) {
    return constantTable_.hasValue(constantValue);
}

string PKB::GetConstantValue(unsigned int index) {
    return constantTable_.getValue(index);
}

unsigned int PKB::GetConstantIndex(string constantValue) {
    return constantTable_.getKey(constantValue);
}

set<string> PKB::GetAllConstantValues() {
    return constantTable_.getValues();
}

void PKB::PrintConstantTable() {
    constantTable_.printTable();
}

/* END   - Constant table functions */
/* START - Variable table functions */

void PKB::GenerateVariableTable(vector<string> variableNames) {
    unsigned int i = 1;
    for (auto &variableName : variableNames) {
        if (!variableTable_.hasValue(variableName)) {
            variableTable_.insert(i++, variableName);
        }
    }
}

bool PKB::HasVariable(string variableName) {
    return variableTable_.hasValue(variableName);
}

string PKB::GetVariableName(unsigned int index) {
    return variableTable_.getValue(index);
}

unsigned int PKB::GetVariableIndex(string variableName) {
    return variableTable_.getKey(variableName);
}

set<string> PKB::GetAllVariableNames() {
    return variableTable_.getValues();
}

void PKB::PrintVariableTable() {
    variableTable_.printTable();
}

/* END   - Variable table functions */
/* START - Procedure table functions */

void PKB::GenerateProcedureTable(vector<string> procedureNames) {
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
    return procedureTable_.getValue(index);
}

unsigned int PKB::GetProcedureIndex(string procedureName) {
    return procedureTable_.getKey(procedureName);
}

set<string> PKB::GetAllProcedures() {
    return procedureTable_.getValues();
}

void PKB::PrintProcedureTable() {
    procedureTable_.printTable();
}

/* END   - Procedure table functions */
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
    return stmtTable_.getValue(stmtNumber);
}

set<unsigned int> PKB::GetSymbolStmtNumbers(string symbol) {
    return stmtTable_.getKeys(symbol);
}

set<unsigned int> PKB::GetSymbolStmtNumbers(Symbol symbol) {
    return stmtTable_.getKeys(Constants::SymbolToString(symbol));
}

void PKB::PrintStmtTable() {
    stmtTable_.printTable();
}

/* END   - Stmt table functions */
/* START - Assign table functions */

void PKB::GenerateAssignTable(std::map<unsigned int, TreeNode*> assigns) {
    for (auto &assign : assigns) {
        assignTable_.insert(assign.first, assign.second);
    }
}

TreeNode* PKB::GetAssignTreeNode(unsigned int stmtNumber) {
    return assignTable_.getValue(stmtNumber);
}

set<TreeNode*> PKB::GetAllAssignTreeNodes() {
    return assignTable_.getValues();
}

/* END   - Assign table functions */
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

bool PKB::IsModifiesProcedure(std::string procedureName, string variableName) {
    return modifiesProcedureTable_.hasKeyToValue(procedureName, variableName);
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
/* END   - Stmt table functions */
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

bool PKB::IsUsesProcedure(std::string procedureName, std::string variableName) {
    return usesProcedureTable_.hasKeyToValue(procedureName, variableName);
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

bool PKB::IsParentTransitive(unsigned int parent, unsigned int child) {
    return parentTable_.hasKeyToValue(parent, child);
}

unsigned int PKB::GetParent(unsigned int child) {
    return parentTable_.getKey(child);
}

set<unsigned int> PKB::GetChildren(unsigned int parent) {
    return parentTable_.getValues(parent);
}

set<unsigned int> PKB::GetParentsTransitive(unsigned child) {
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

bool PKB::IsFollowsTransitive(unsigned int follows, unsigned int following) {
    return followsTransitiveTable_.hasKeyToValue(follows, following);
}

unsigned int PKB::GetFollows(unsigned int following) {
    return followsTable_.getKey(following);
}

unsigned int PKB::GetFollowing(unsigned int follows) {
    return followsTable_.getValue(follows);
}

set<unsigned int> PKB::GetFollowsTransitive(unsigned following) {
    return followsTransitiveTable_.getKeys(following);
}

set<unsigned int> PKB::GetFollowingTransitive(unsigned follows) {
    return followsTransitiveTable_.getValues(follows);
}

void PKB::PrintFollowsTable() {
    followsTable_.printTable();
}

void PKB::PrintFollowsTransitiveTable() {
    followsTransitiveTable_.printTable();
}

/* END   - Follows table functions */
/* START - Other functions */

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

/* START - Other functions */

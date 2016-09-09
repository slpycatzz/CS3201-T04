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

void PKB::PrintStmtTable() {
    stmtTable_.printTable();
}

/* END   - Stmt table functions */
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

void PKB::PrintFollowsTable() {
    followsTable_.printTable();
}

void PKB::PrintFollowsTransitiveTable() {
    followsTransitiveTable_.printTable();
}

/* END   - Follows table functions */

//// TODO(pixelducky): update this impl
//bool PKB::IsVariableExist(string variable_name) {
//    return true;
//}


//// TODO(pixelducky): update this impl
//bool PKB::IsProcedureExist(string procedure_name) {
//  return false;
//}
//


//vector<string> PKB::UsesVariable(int line_number) {
//  return PKB::uses_table.getValues(line_number);
//}
//
//vector<string> PKB::UsesVariable(string procedure_name) {
//  return PKB::procedure_uses_table.getValues(procedure_name);
//}
//
//vector<int> PKB::UsedByLineNumber(string variable_name) {
//  return PKB::uses_table.getKeys(variable_name);
//}
//
//vector<string> PKB::UsedByProcedure(string variable_name) {
//  return PKB::procedure_uses_table.getKeys(variable_name);
//}


//
//vector<string> PKB::ModifiesVariable(int line_number) {
//  return PKB::modifies_table.getValues(line_number);
//}
//
//vector<string> PKB::ModifiesVariable(string procedure_name) {
//  return PKB::procedure_modifies_table.getValues(procedure_name);
//}
//
//vector<int> PKB::ModifiedByLineNumber(string variable_name) {
//  return PKB::modifies_table.getKeys(variable_name);
//}
//
//vector<string> PKB::ModifiedByProcedure(string variable_name) {
//  return PKB::procedure_modifies_table.getKeys(variable_name);
//}


//// TODO(pixelducky): update this impl
//bool PKB::IsParent(int line_number1, int line_number2) {
//  return false;
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsParent(int line_number1, vector<int> line_numbers) {
//  return false;
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsParentTransitive(int line_number1, int line_number2) {
//  return false;
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsParentTransitive(int line_number1, vector<int> line_numbers) {
//  return false;
//}


//// TODO(pixelducky): update this impl
//bool PKB::IsFollows(int line_number1, int line_number2) {
//  return false;
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsFollows(int line_number1, vector<int> line_numbers) {
//  return false;
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsFollowsTransitive(int line_number1, int line_number2) {
//  return false;
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsFollowsTransitive(int line_number1, vector<int> line_numbers) {
//  return false;
//}

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Constants.h"
#include "PKB/AST.h"
#include "PKB/Table.h"
#include "PKB/PKB.h"
#include "TreeNode.h"

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

void PKB::generateConstantTable(std::set<string> constants) {
    unsigned int i = 1;
    for (auto &constant : constants) {
        constantTable_.insert(i++, constant);
    }
}

void PKB::printConstantTable() {
    constantTable_.printTable();
}

/* END   - Constant table functions */
/* START - Variable table functions */

void PKB::generateVariableTable(std::set<string> variableNames) {
    unsigned int i = 1;
    for (auto &variableName : variableNames) {
        variableTable_.insert(i++, variableName);
    }
}

void PKB::printVariableTable() {
    variableTable_.printTable();
}

/* END   - Variable table functions */
/* START - Procedure table functions */

void PKB::generateProcedureTable(std::set<string> procedureNames) {
    unsigned int i = 1;
    for (auto &procedureName : procedureNames) {
        numberOfProcedure_++;
        procedureTable_.insert(i++, procedureName);
    }
}

void PKB::printProcedureTable() {
    procedureTable_.printTable();
}

/* END   - Procedure table functions */
/* START - Stmt table functions */

void PKB::generateStmtTable(std::map<unsigned int, string> stmts) {
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

void PKB::printStmtTable() {
    stmtTable_.printTable();
}

/* END   - Stmt table functions */

//
//void PKB::InsertVariable(string variable_name, int line_number) {
//    variableTable.insert(variable_name, line_number);
//}

//void PKB::InsertVariable(string variable_name, vector<int> line_numbers) {
//  for (auto line_number : line_numbers) {
//    PKB::InsertVariable(variable_name, line_number);
//  }
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsVariableExist(string variable_name) {
//    return true;
//}
//
//void PKB::InsertProcedure(string procedure_name, int line_number) {
//  PKB::procedure_table.insert(procedure_name, line_number);
//}
//
//void PKB::InsertProcedure(string procedure_name, vector<int> line_numbers) {
//  for (auto line_number : line_numbers) {
//    PKB::InsertProcedure(procedure_name, line_number);
//  }
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsProcedureExist(string procedure_name) {
//  return false;
//}
//
//void PKB::InsertUses(int line_number, string variable_name) {
//  PKB::uses_table.insert(line_number, variable_name);
//}
//
//void PKB::InsertUses(int line_number, vector<string> variable_names) {
//  for (auto variable_name : variable_names) {
//    PKB::InsertUses(line_number, variable_name);
//  }
//}
//
//void PKB::InsertUses(string procedure_name, string variable_name) {
//  PKB::procedure_uses_table.insert(procedure_name, variable_name);
//}
//
//void PKB::InsertUses(string procedure_name, vector<string> variable_names) {
//  for (auto variable_name : variable_names) {
//    PKB::InsertUses(procedure_name, variable_name);
//  }
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
//void PKB::InsertModifies(int line_number, string variable_name) {
//  PKB::modifies_table.insert(line_number, variable_name);
//}
//
//void PKB::InsertModifies(int line_number, vector<string> variable_names) {
//  for (auto variable_name : variable_names) {
//    PKB::InsertModifies(line_number, variable_name);
//  }
//}
//
//void PKB::InsertModifies(string procedure_name, string variable_name) {
//  PKB::procedure_modifies_table.insert(procedure_name, variable_name);
//}
//
//void PKB::InsertModifies(string procedure_name, vector<string> variable_names) {
//  for (auto variable_name : variable_names) {
//    PKB::InsertModifies(procedure_name, variable_name);
//  }
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
//
//void PKB::InsertCalls(string procedure_name1, string procedure_name2) {
//  PKB::calls_table.insert(procedure_name1, procedure_name2);
//}
//
//void PKB::InsertCalls(string procedure_name1, vector<string> procedure_names) {
//  for (auto procedure_name2 : procedure_names) {
//    PKB::InsertCalls(procedure_name1, procedure_name2);
//  }
//}
//
//void PKB::InsertCallsTransitive(string procedure_name1, string procedure_name2) {
//  PKB::calls_transitive_table.insert(procedure_name1, procedure_name2);
//}
//
//void PKB::InsertCallsTransitive(string procedure_name1, vector<string> procedure_names) {
//  for (auto procedure_name2 : procedure_names) {
//    PKB::InsertCallsTransitive(procedure_name1, procedure_name2);
//  }
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsCalls(string procedure_name1, string procedure_name2) {
//  return false;
//}
//// TODO(pixelducky): update this impl
//bool PKB::IsCallsTransitive(string procedure_name1, string procedure_name2) {
//  return false;
//}
//
//void PKB::InsertParent(int line_number1, int line_number2) {
//  PKB::parent_table.insert(line_number1, line_number2);
//}
//
//void PKB::InsertParent(int line_number1, vector<int> line_numbers) {
//  for (auto line_number2 : line_numbers) {
//    PKB::InsertParent(line_number1, line_number2);
//  }
//}
//
//void PKB::InsertParentTransitive(int line_number1, int line_number2) {
//  PKB::parent_transitive_table.insert(line_number1, line_number2);
//}
//
//void PKB::InsertParentTransitive(int line_number1, vector<int> line_numbers) {
//  for (auto line_number2 : line_numbers) {
//    PKB::InsertParentTransitive(line_number1, line_number2);
//  }
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
//
//void PKB::InsertFollows(int line_number1, int line_number2) {
//  PKB::follows_table.insert(line_number1, line_number2);
//}
//
//void PKB::InsertFollows(int line_number1, vector<int> line_numbers) {
//  for (auto line_number2 : line_numbers) {
//    PKB::InsertFollows(line_number1, line_number2);
//  }
//}
//
//void PKB::InsertFollowsTransitive(int line_number1, int line_number2) {
//  PKB::follows_transitive_table.insert(line_number1, line_number2);
//}
//
//void PKB::InsertFollowsTransitive(int line_number1, vector<int> line_numbers) {
//  for (auto line_number2 : line_numbers) {
//    PKB::InsertFollowsTransitive(line_number1, line_number2);
//  }
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

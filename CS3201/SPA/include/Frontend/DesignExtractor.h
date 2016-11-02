#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Constants.h"
#include "PKB/PKB.h"

class DesignExtractor {
 public:
    DesignExtractor();
    static DesignExtractor& getInstance();
    void resetInstance();

    void populatePKB(std::map<StmtNumber, unsigned int> stmtsLevels, unsigned int tableMaximumSize);

    void setProcedureData(ProcedureName procedureName, StmtNumber firstStmtNumber, StmtNumber lastStmtNumber);
    void setStmtListData(Symbol stmtSymbol, StmtNumber stmtNumber);
    void setContainerStmtData(Symbol stmtSymbol, StmtNumber stmtNumber, VariableName controlVariableName);
    void setIfStmtData(StmtNumber stmtNumber, StmtNumber thenLastStmtNumber, StmtNumber elseFirstStmtNumber);
    void setCallStmtData(StmtNumber stmtNumber, ProcedureName currentProcedureName, ProcedureName procedureName);
    void setAssignStmtData(Symbol stmtSymbol, StmtNumber stmtNumber, VariableName variable);
    void setAssignExpressionData(StmtNumber stmtNumber, std::vector<std::string> expression);

    void computeNextTransitive(CFGNode* controlFlowGraphNode, Matrix &nextTransitiveMatrix);

    void computeAffects(CFGNode* controlFlowGraphNode, Matrix &affectsMatrix, VectorTable<StmtNumber, StmtNumber> &affectsTable);

 private:
    void populateGenericTables();
    void populateStmtRelatedTables();
    void populateExpressionTables();
    void populateDesignAbstractionTables();

    void precomputeCalls();
    void precomputeModifies();
    void precomputeUses();
    void precomputeParent();
    void precomputeFollows();
    void precomputeNext();

    int getParentOfStmtNumber(StmtNumber stmtNumber);
    int getFollowOfStmtNumber(StmtNumber stmtNumber);
    int getFollowingOfStmtNumber(StmtNumber stmtNumber);
    std::map<StmtNumber, Symbol> getNextStmtNumbers(StmtNumber stmtNumber);

    static DesignExtractor instance;

    /* PKB tables helper variables. */
    std::map<StmtNumber, unsigned int> stmtsLevels_;
    std::map<StmtNumber, StmtNumber> thenLastStmt_;
    std::map<StmtNumber, StmtNumber> elseFirstStmt_;
    std::map<StmtNumber, ProcedureName> proceduresLastStmt_;

    /* For PKB generic table generation. */
    std::set<ConstantValue> constants_;
    std::set<ProcedureName> procedureNames_;
    std::set<VariableName> variableNames_;
    std::map<StmtNumber, VariableName> controlVariableNames_;
    std::map<StmtNumber, ProcedureName> call_;
    std::map<StmtNumber, Symbol> stmts_;
    std::map<StmtNumber, Symbol> stmtlists_;
    std::map<StmtNumber, ProcedureName> proceduresFirstStmt_;

    /* For PKB expression tables generation. */
    std::map<StmtNumber, std::vector<std::string>> expressions_;

    /* For PKB design abstraction table generation. */
    std::map<ProcedureName, std::set<ProcedureName>> calls_;
    std::map<StmtNumber, VariableName> modifies_;
    std::map<StmtNumber, std::set<std::string>> uses_;
};


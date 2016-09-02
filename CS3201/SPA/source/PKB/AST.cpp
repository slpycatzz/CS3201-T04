#include "PKB/AST.h"

AST *AST::instance = 0;

AST::AST() {}

AST* AST::getInstance() {
    if (!instance) {
        instance = new AST();
    }

    return instance;
}

void AST::deleteInstance() {
    instance = 0;
}

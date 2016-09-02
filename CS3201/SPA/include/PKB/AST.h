#pragma once

#include "Tree.h"

class AST : public Tree {
 public:
    static AST* getInstance();
    static void deleteInstance();

 private:
    AST();
    static AST *instance;
};

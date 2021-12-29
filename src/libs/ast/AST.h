#pragma once
#include <cstdint>
#include <cassert>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "ASTNode.h"


using namespace std;
namespace weaver {

    /**
     * The abstract syntax tree for Weaver.
     */
    class AST {
    public:
        AST() : _root(nullptr) {}

        void setRoot(ASTNode* root) { _root = root; };
        string toString();
        string toStringSubtree(ASTNode* node, uint16_t indent);
        ASTNode* getRoot() { return _root; }
    private:
        ASTNode *_root;

    };

    ASTNode* fromFile(string); 
    ASTNode* fromString(string); 
}
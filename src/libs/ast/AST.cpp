#include "AST.h"
#include <sstream>
#include "../program/Program.h"

using namespace std;
using namespace weaver;

string AST::toString() {
    stringstream ss;

    ASTNode* root = getRoot();
    ss << string(0, ' ') << root->toString(true) << endl ;
    for (int i = 0; i < root->getNumChildren(); i ++) {
        ASTNode* current = root->getChild(i);
        ss << toStringSubtree(current, 2);
        ss << endl;
    }

    return ss.str();
}

string AST::toStringSubtree(ASTNode *node, uint16_t indent) {
    stringstream ss;

    ss << string(indent, ' ') << node->toString(true) << endl ;
    for (int i = 0; i < node->getNumChildren(); i ++) {
        ASTNode* current = node->getChild(i);
        ss << toStringSubtree(current, indent + 2);
    }

    return ss.str();
}
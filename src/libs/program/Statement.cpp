#include "Statement.h"
#include "Program.h"

using namespace weaver;

bool Statement::operator==(const Statement &other) const {
    if (getThreadID() !=  other.getThreadID())
        return false;

    return *getNode() == *other.getNode();
}


Statement* Statement::create(ASTNode *node, Program *program, uint16_t thread) {
    assert(node != nullptr && "Fatal Error");

    bool allowed = false;

    // we only allow a subset of ASTNode to be turned into a statement
    if (node->isAssign() || node->isAssume() || node->isBool() || node->isNoAction() || node->isAtomic()) {
        allowed = true;
    }

    assert(allowed && "Not allowed to make into a statement!\n");

    Statement* stmt = new Statement(node, thread);

    Statement* eq = program->getEquivalentStatement(stmt);

    if (eq == nullptr) {
        program->addStatementToPool(stmt);
        return  stmt;
    }
    else {
        delete stmt;
        return eq;
    }
}

string Statement::toString() const {
    stringstream ss;

    string thread = "Main";
    if (getThreadID() != 0) {
        thread = to_string(getThreadID());
    }

    ss << getNode()->getReadableText() << " : Thread: " << thread;

    return ss.str();
}
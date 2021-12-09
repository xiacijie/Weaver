#include "Program.h"
#include "../verifier/SMTInterpol.h"

using namespace weaver;
using namespace std;

//vector<Thread>& Program::getParallelThreads(uint32_t state) {
//    const auto& it = _threads.find(state);
//    assert(it == _threads.end() && "This is not a parallel state");
//    return it->second;
//}


const unordered_set<Statement*>& Program::getStatementsByThread(uint16_t threadID) const {
    const auto& it = _statementsByThread.find(threadID);

    assert(it != _statementsByThread.end() && "Error");
    return it->second;
}

string Program::dependentStatementsToString() {
    stringstream ss;
    ss << "Dependent Statements:" << endl;

    for (const auto& it : _dependenceRelation) {
        Statement* stmt = it.first;
        ss << stmt->toString() << " ---- " << "{";

        vector<string> dependentStatements;
        for (const auto& s : it.second) {
            dependentStatements.push_back(s->toString());
        }

        ss << join(dependentStatements, " | ") << "}";

        ss << endl;
    }

    return ss.str();
}

string Program::independentStatementsToString() {
    stringstream ss;
    ss << "Independent Statements:" << endl;
    unordered_map<Statement*, unordered_set<Statement*>> statementsProduct;

    for (int i = 0; i < _statementPool.size(); i ++) {
        for (int j = i + 1; j < _statementPool.size(); j ++) {
            Statement* s1 = _statementPool[i];
            Statement* s2 = _statementPool[j];

            statementsProduct[s1].insert(s2);
            statementsProduct[s2].insert(s1);

            statementsProduct[s1].insert(s1);
            statementsProduct[s2].insert(s2);
        }
    }

    for (const auto& it : statementsProduct) {
        Statement* stmt = it.first;
        ss << stmt->toString() << " ---- " << "{";

        vector<string> independentStatements;
        for (const auto& s : it.second) {
            if (_dependenceRelation.find(stmt) == _dependenceRelation.end() ||
                _dependenceRelation[stmt].find(s) == _dependenceRelation[stmt].end()) {
                independentStatements.push_back(s->toString());
            }
        }
        ss << join(independentStatements, " | ") << "}";
        ss << endl;
    }

    return ss.str();
}

//string Program::parallelStatesToString() {
//    stringstream ss;
//    ss << "This Program has " << _threads.size() << " parallel states in total" << endl;
//    ss << "Parallel States: " << endl;
//    for (auto& it : _threads) {
//        ss << it.first << " ";
//    }
//
//    ss << endl << endl;
//
//    for (auto& it : _threads) {
//        ss  << "Parallel State: "<< it.first << endl;
//        ss << it.second.size() << " threads in total" << endl;
//        for (auto& t: it.second) {
//            ss << "# Thread: " << t.getID() << endl;
//            ss << t.getCFG().toString(t.getAlphabet()) << endl;
//        }
//    }
//
//    return ss.str();
//}

//vector<Thread>& Program::createParallelThreads(uint32_t state, uint32_t numThreads) {
//    vector<Thread> threads;
//    _threads[state] = threads;
//
//    for (int i = 0 ; i < numThreads; i++) {
//        Thread thread(i+1);
//        _threads[state].push_back(thread);
//    }
//
//    return _threads[state];
//}


Program::~Program() {
    deallocateASTNodePool();
    deallocateStatementPool();
}

void Program::buildDependenceRelation() {

    TheoremProverBase* verifier = new SMTInterpol(this);

    for (int i = 0; i < _statementPool.size(); i ++) {
        for (int j = i + 1; j < _statementPool.size(); j ++) {
            Statement* s1 = _statementPool[i];
            Statement* s2 = _statementPool[j];

            bool isInDependent = verifier->checkIndependenceRelation(s1, s2);
            if (!isInDependent) {
                addDependentStatements(s1, s2);
            }

        }
    }

    delete verifier;
}

ASTNode* Program::getEquivalentASTNode(ASTNode *node) {
    for (const auto& n : _nodePool) {
        if (*node == *n) {
            return n;
        }
    }

    return nullptr;
}

Statement* Program::getEquivalentStatement(Statement *stmt) {
    for (const auto& s : _statementPool) {
        if (*stmt == *s) {
            return s;
        }
    }

    return nullptr;
}


void Program::addDependentStatements(Statement *first, Statement *second) {
    // note that dependence relation is symmetric and reflexive
    _dependenceRelation[first].insert(first);
    _dependenceRelation[second].insert(second);

    _dependenceRelation[first].insert(second);
    _dependenceRelation[second].insert(first);
}

bool Program::isDependent(Statement *first, Statement *second) {
    if (_dependenceRelation.find(first) != _dependenceRelation.end()) {
        if (_dependenceRelation[first].find(second) != _dependenceRelation[first].end()) {
            return true;
        }
    }

//    if (_dependenceRelation.find(second) != _dependenceRelation.end()) {
//        if (_dependenceRelation[second].find(first) != _dependenceRelation[second].end()) {
//            return true;
//        }
//    }

    return false;
}
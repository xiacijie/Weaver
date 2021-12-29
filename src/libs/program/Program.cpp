#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "antlr4-runtime.h"
#include "Program.h"
#include "../verifier/SMTInterpol.h"
#include "WeaverLexer.h"
#include "WeaverParser.h"
#include "ASTBuilder.h"
#include "CFGBuilder.h"
#include "util.h"
#include "log.h"


using namespace weaver;
using namespace std;
using namespace antlr4;
using namespace antlrcpp;

//vector<Thread>& Program::getParallelThreads(uint32_t state) {
//    const auto& it = _threads.find(state);
//    assert(it == _threads.end() && "This is not a parallel state");
//    return it->second;
//}

extern logg::Logger logger;

void Program::init(string fileName) {

    if (!(fileName.substr(fileName.find_last_of(".") + 1) == "wvr")) {
        logger.error("Currently only '.wvr' files are supported."); 
        exit(1); 
    } 

    InputType t = wvr; 

    ifstream stream;
    stream.open(fileName);

    if (!stream.good()) {
        throw invalid_argument( "Cannot open file {}" + fileName );
    }

    ANTLRInputStream input(stream);
	this->init(t, input); 

}

void Program::init(InputType t, string contents) {

    ANTLRInputStream input(contents);
	this->init(t, input); 
}

void Program::init(InputType t, ANTLRInputStream input) {

    if (t != wvr) {
        throw invalid_argument( "Currently only '.wvr' files are supported.");
    }

    WeaverLexer lexer(&input);
	CommonTokenStream tokens(&lexer);

	tokens.fill();

	WeaverParser parser(&tokens);

	WeaverParser::ProgramContext* tree = parser.program();

    if (parser.getNumberOfSyntaxErrors() > 0) {
        cerr << "Weaver: Syntax Error!\n" << endl;
        abort();
    }

    ASTBuilder builder(this, tree);
    builder.build();

    cout << _ast.toString() << endl;
    cout << _vTable.toString() << endl;

    CFGBuilder cfgBuilder(this);
    cout << "Building CFG..." << endl;
    cfgBuilder.build();
    cout << this->getCFG().toString() << endl;

    cout << "Alphabet size: " << this->getAlphabet().size() << endl;

    this->buildDependenceRelation();

    cout << this->independentStatementsToString() << endl;

    cout << this->dependentStatementsToString() << endl;
}

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

        ss << util::join(dependentStatements, " | ") << "}";

        ss << endl;
    }

    return ss.str();
}

string Program::independentStatementsToString() {
    stringstream ss;
    ss << "Independent Statements:" << endl;

    for (const auto& it : _independenceRelation) {
        Statement* stmt = it.first;
        ss << stmt->toString() << " ---- " << "{";

        vector<string> independentStatements;
        for (const auto& s : it.second) {
            independentStatements.push_back(s->toString());
        }

        ss << util::join(independentStatements, " | ") << "}";

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
            if (isInDependent) {
                addIndependentStatements(s1, s2);
            }
            else {
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

void Program::addIndependentStatements(Statement *first, Statement *second) {
    _independenceRelation[first].insert(second);
    _independenceRelation[second].insert(first);
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
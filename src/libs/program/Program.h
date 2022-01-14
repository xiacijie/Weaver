#pragma once
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include "VariableTable.h"
#include "AST.h"
#include "NFA.h"
#include "antlr4-runtime.h"
#include "MathSAT.h"
#include "Yices.h"
#include "SMTInterpol.h"

using namespace std;

namespace weaver {

    enum InputType { wvr, c };

    /**
     * The abstract program
     *
     * This class stores all the global info of per run of the Weaver,
     * such as the variable table, abstract syntax tree...
     */
    class Program {

    public:
        Program() :
            _totalThreads(0),
            _SMTInterpol(&_vTable),
            _MathSAT(&_vTable),
            _Yices(&_vTable)
            {}

        ~Program();

        VariableTable& getVariableTable()  { return _vTable; }
        AST& getAST() { return _ast; }
        NFA& getCFG() { return _cfg; }
        Alphabet& getAlphabet() { return _alphabet; }
        SMTInterpol* getSMTInterpol() { return &_SMTInterpol; }
        Yices* getYices() { return &_Yices; }
        MathSAT* getMathSAT() { return &_MathSAT; }

        void init(string); // filename 
        void init(InputType, string); // code 
        void init(InputType, antlr4::ANTLRInputStream); // code 
        void addASTNodeToPool(ASTNode* node) { _nodePool.push_back(node); }
        void addStatementToPool(Statement* stmt) { _statementPool.push_back(stmt); _statementsByThread[stmt->getThreadID()].insert(stmt); }

        /**
         * Get equivalent ASTNode that was created earlier
         * So we don't create duplicated ASTNodes
         *
         * For example, 'a = a + 1' and 'a = a + 1' are two equivalent ASTNodes
         * if none, return nullptr
         * @param node
         * @return
         */
        ASTNode* getEquivalentASTNode(ASTNode* node);

        Statement* getEquivalentStatement(Statement* stmt);

        /**
         * Build the dependence relation of the statements
         */
        void buildDependenceRelation();

        const unordered_map<Statement*, unordered_set<Statement*>>& getDependenceRelation() { return _dependenceRelation; }
        const unordered_map<Statement*, unordered_set<Statement*>>& getIndependenceRelation() { return _independenceRelation; }
        const unordered_set<Statement*>& getDependentStatements(Statement* statement) { return _dependenceRelation[statement]; };
        bool isDependent(Statement* first, Statement* second);

        size_t getNumASTNodes() { return _nodePool.size(); }

        string dependentStatementsToString();
        string independentStatementsToString();

        uint16_t getNextThreadID() { return _totalThreads++; }
        uint16_t getTotalNumThreads() const { return _totalThreads; }

        const unordered_set<Statement*>& getStatementsByThread(uint16_t threadID) const;

    private:
        void deallocateASTNodePool() { for (const auto& n : _nodePool) delete n; }
        void deallocateStatementPool() { for (const auto& n : _statementPool) delete n; }
        void addDependentStatements(Statement* first, Statement* second);
        void addIndependentStatements(Statement* first, Statement* second);

        VariableTable _vTable;
        AST _ast;
        NFA _cfg;
        vector<ASTNode*> _nodePool;
        vector<Statement*> _statementPool;
        Alphabet _alphabet;

        uint16_t _totalThreads;

        unordered_map<Statement*, unordered_set<Statement*>> _dependenceRelation;
        unordered_map<Statement*, unordered_set<Statement*>> _independenceRelation;

        // disjoint statements by their thread number
        unordered_map<uint16_t, unordered_set<Statement*>> _statementsByThread;

        //theorem provers
        SMTInterpol _SMTInterpol;
        Yices _Yices;
        MathSAT _MathSAT;
    };

}



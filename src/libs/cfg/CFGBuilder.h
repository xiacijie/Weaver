#pragma once

#include "../automata/DFA.h"
#include "../program/Program.h"

namespace weaver {
    class CFGBuilder {
    public:
        /**
         * Turn the AST into the control flow graph
         * This builds the CFG for the whole program
         *
         * @param program
         */
        CFGBuilder(Program* program) :
                _program(program),
                _root(program->getAST().getRoot()),
                _cfg(&program->getCFG()),
                _alphabet(program->getAlphabet()),
                _threadID(program->getNextThreadID())
                {}

        /**
         *
         * @return the last state of the control flow automata
         */
        uint32_t build();

    private:
        /**
         *  This builds the CFG for a single block
         *
         * @param cfg
         * @param block
         * @param alphabet
         * @param program
         */
        CFGBuilder(NFA* cfg, ASTNode* block, Alphabet& alphabet, Program* program):
                _cfg(cfg),
                _root(block),
                _program(program),
                _alphabet(alphabet),
                _threadID(program->getNextThreadID())
        {}

        ASTNode* _root;
        NFA* _cfg;
        Program* _program;
        Alphabet& _alphabet;
        uint16_t _threadID;

        /**
         *
         * @param fromState
         * @param statement
         * @param toState
         *
         * Wrapper over _cfg->addTransition
         * Update the alphabet in the Program everytime we add a transition
         */
        void addTransition(uint32_t fromState, Statement* statement, uint32_t toState);

        /**
         * This is for block node or program node
         *
         * @param node
         * @param currentState
         * @return
         */
        uint32_t iterateThroughStatements(ASTNode* node, uint32_t currentState);

        uint32_t statementHandleDispatch(ASTNode* node, uint32_t currentState);
        uint32_t handleStatementWithNoControlFlow(ASTNode* node, uint32_t currentState);
        uint32_t handleNonDeterConditionalStatement(ASTNode* node, uint32_t currentState);
        uint32_t handleConditionalStatement(ASTNode* node, uint32_t currentState);
        uint32_t handleNonDeterLoopStatement(ASTNode* node, uint32_t currentState);
        uint32_t handleLoopStatement(ASTNode* node, uint32_t currentState);
        uint32_t handleParallelStatement(ASTNode* node, uint32_t currentState);

        Statement* createStatement(ASTNode* node) const { return Statement::create(node, _program, _threadID); }
    };

}
#pragma once
#include <cstdint>
#include <string>

using namespace std;

namespace weaver {
    class Program;
    class ASTNode;

    /**
     * The class for the statement in the program
     * Statement is a wrapper over ASTNode
     */
    class Statement {
    public:

        /**
        *
        * @param node the abstract syntax tree node
        * @param program
        * @param thread the thread this statement belongs to. 0 means this is the main thread.
        */
        static Statement* create(ASTNode* node, Program* program, uint16_t thread=0);

        bool operator== (const Statement& other) const;

        uint16_t getThreadID() const { return _threadID; }
        ASTNode* getNode() const { return _node; }

        string toString() const;
    private:

        Statement(ASTNode* node, uint16_t thread):
                _node(node),
                _threadID(thread) {}

        ASTNode* _node;
        uint16_t _threadID;
    };
}



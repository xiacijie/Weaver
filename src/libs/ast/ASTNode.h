#pragma once
#include <cassert>
#include <vector>
#include <string>

using namespace std;

namespace weaver {
    class Program;

    enum DataType {
        Int, Bool, NoType
    };

    const string DataTypeLabels[] = {"Int", "Bool", "NoType"};

    enum NodeType {
        Programme,
        Parallel,
        Block,
        Identifier,
        Constant,
        Addition, Subtraction, Multiplication,
        Assignment,
        Conditional,
        Loop,
        Assert,
        Assume,
        Atomic,
        Negation,
        CompareGT, CompareGTE, CompareLT, CompareLTE, CompareEQ, CompareNEQ,
        And, Or, Not,
        NoAction
    };

    const string NodeTypeLabels[] = {"Programme",
                                     "Parallel",
                                     "Block",
                                     "Identifier",
                                     "Constant",
                                     "Addition", "Subtraction", "Multiplication",
                                     "Assignment",
                                     "Conditional",
                                     "Loop",
                                     "Assert",
                                     "Assume",
                                     "Atomic",
                                     "Negation",
                                     "CompareGT", "CompareGTE", "CompareLT", "CompareLTE", "CompareEQ", "CompareNEQ",
                                     "And", "Or", "Not",
                                     "NoAction"
    };

    enum BooleanValue {
        True, False, NonDeter
    };

    const string BooleanValueLabels[3] = {"True", "False", "NonDeter"};

    /**
     * The abstract syntax tree node for Weaver
     */
    class ASTNode{
    public:

        static ASTNode* create(NodeType nType, DataType dType, Program* program, const vector<ASTNode*>& children={});
        static ASTNode* create(int64_t value, Program* program);
        static ASTNode* create(BooleanValue value, Program* program);
        static ASTNode* create(const string& varName, DataType dType, Program* program);
        static ASTNode* createNoAction(Program* program);

        bool operator== (const ASTNode& other) const;

        size_t getNumChildren() const { return _children.size(); }
        ASTNode* getChild(uint16_t i) const {
            assert(i < getNumChildren() && "Child Index Out of Range!\n");
            return _children[i];
        };

        DataType getDataType() const { return _dType; }
        NodeType getNodeType() const { return _nType; }

        /* Methods for checking the type of this AST Node*/
        bool isConstant() const { return _nType == NodeType:: Constant; }
        bool isConditional() const { return _nType == NodeType::Conditional; }
        bool isLoop() const { return _nType == NodeType::Loop; }
        bool isAssert() const { return _nType == NodeType::Assert; }
        bool isAssume() const { return _nType == NodeType::Assume; }
        bool isInt() const { return _dType == DataType::Int; }
        bool isBool() const { return _dType == DataType::Bool; }
        bool isIntConst() const { return isConstant() && isInt(); }
        bool isBoolConst() const { return isConstant() && isBool(); }
        bool isNonDeterBool() const { return isBoolConst() && _boolVal == BooleanValue::NonDeter; }
        bool isAssign() const { return _nType == NodeType::Assignment; }
        bool isProgram() const { return _nType == NodeType::Programme; }
        bool isParallel() const { return _nType == NodeType::Parallel; }
        bool isAtomic() const { return _nType == NodeType::Atomic; }
        bool isBlock() const { return _nType == NodeType::Block; }
        bool isAdd() const { return _nType == NodeType::Addition; }
        bool isSub() const { return _nType == NodeType::Subtraction; }
        bool isMult() const { return _nType == NodeType::Multiplication; }
        bool isAnd() const { return _nType == NodeType::And; }
        bool isOr() const { return _nType == NodeType::Or; }
        bool isNegation() const { return _nType == NodeType::Negation; }
        bool isNot() const { return _nType == NodeType::Not; }
        bool isNoAction() const { return _nType == NodeType::NoAction; }

        bool isId() const { return _nType == NodeType::Identifier; }

        bool isComparison() const { return _nType == NodeType::CompareEQ || _nType == NodeType::CompareNEQ ||
            _nType == NodeType::CompareGT || _nType == NodeType::CompareGTE || _nType == NodeType::CompareLT ||
            _nType == NodeType::CompareLTE;
        }

        bool isBinaryOperation() const { return isAdd() || isSub() || isMult() || isAnd() || isOr() || isComparison(); }
        bool isUnaryOperation() const { return isNegation() || isNot(); }

        int64_t getInt() const { assert(isIntConst() && "Not an Integer Constant!\n"); return _intVal; }
        BooleanValue getBool() const { assert(isBoolConst() && "Not a Boolean Constant!\n"); return _boolVal; }
        string getIdName() const { assert(isId() && "Not an Identifier!\n"); return _idName; }


        string toString(bool readableText=false) const;

        uint32_t getNodeID() const { return _nodeID; }
        ASTNode* getParent() const { return _parent; }

        string getReadableText() const;

    private:
        ASTNode(NodeType nType, DataType dType, uint32_t nodeID) :
                _nType(nType) ,
                _dType(dType),
                _nodeID(nodeID),
                _parent(nullptr)
        {};

        void addChild(ASTNode* node) { _children.push_back(node); node->setParent(this); }
        void setInt(int64_t i) { assert(isIntConst() && "Not an Integer!\n"); _intVal = i; }
        void setBool(BooleanValue b) { assert(isBoolConst() && "Not a Boolean!\n"); _boolVal = b; }
        void setIdName(string s) { assert(isId() && "Not an Identifier!\n"); _idName = s; }
        void setParent(ASTNode* parent) { _parent = parent; }

        static string getBinaryOpText(NodeType type);
        static string getUnaryOpText(NodeType type);

        /**
         * If have equivalent one, the node will be deleted and get the equivalent one.
         * Otherwise, it will be kept and be returned
         * @param node
         * @return
         */
        static ASTNode* getEquivalentASTNode(ASTNode* node, Program* program);

        uint32_t _nodeID;

        ASTNode* _parent;
        DataType _dType;
        NodeType _nType;

        // Not a good design though, should be put those three fields into a union
        int64_t _intVal;
        BooleanValue _boolVal;
        string _idName;

        vector<ASTNode*> _children;
    };

}

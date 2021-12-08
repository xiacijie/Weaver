#include "ASTNode.h"
#include <sstream>
#include "../program/Program.h"

using namespace std;
using namespace weaver;

bool ASTNode::operator==(const ASTNode &other) const {
    if (other.isNoAction() && isNoAction())
        return true;

    if (other.getNodeType() != getNodeType()
    || other.getDataType() != getDataType()
    || other.getNumChildren() != getNumChildren()
    ) {
        return false;
    }

    if (isAssign()) {
        return *getChild(0) == *other.getChild(0)
        && *getChild(1) == *other.getChild(1);
    }

    if (getDataType() == DataType::NoType
        && other.getDataType() == DataType::NoType) {
        return false;
    }

    if (getNumChildren() == 0) {
        if (isId()) {
            return getIdName() == other.getIdName();
        }
        else if (isIntConst()) {
            return getInt() == other.getInt();
        }
        else if (isBoolConst()) {
            return getBool() == other.getBool();
        }
        else {
            assert(false && "Error in ASTNode ==\n");
        }
    }
    else {
        bool allEq = true;
        for (int i =0; i < getNumChildren(); i ++) {
            allEq = allEq && (*getChild(i) == *other.getChild(i));
        }

        return allEq;
    }
}

ASTNode* ASTNode::create(NodeType nType, DataType dType, Program *program, const vector<ASTNode*>& children) {
    auto newNode = new ASTNode(nType, dType, program->getNumASTNodes());

    for (const auto& c: children) {
        newNode->addChild(c);
    }

    return getEquivalentASTNode(newNode, program);
}

ASTNode* ASTNode::create(int64_t value, Program *program) {
    auto newNode = new ASTNode(NodeType::Constant, DataType::Int, program->getNumASTNodes());
    newNode->setInt(value);

    return getEquivalentASTNode(newNode, program);
}

ASTNode* ASTNode::create(BooleanValue value, Program *program) {
    auto newNode = new ASTNode(NodeType::Constant, DataType::Bool, program->getNumASTNodes());
    newNode->setBool(value);

    return getEquivalentASTNode(newNode, program);
}


ASTNode* ASTNode::create(const string& varName, DataType dType, Program *program) {
    auto newNode = new ASTNode(NodeType::Identifier, dType, program->getNumASTNodes());
    newNode->setIdName(varName);

    return getEquivalentASTNode(newNode, program);
}

ASTNode* ASTNode::createNoAction(Program *program) {
    auto newNode = new ASTNode(NodeType::NoAction, DataType::NoType, program->getNumASTNodes());

    return getEquivalentASTNode(newNode, program);
}


ASTNode* ASTNode::getEquivalentASTNode(ASTNode *node, Program* program) {
    ASTNode* eqNode = program->getEquivalentASTNode(node);

    if (eqNode) {
        delete node;
        return eqNode;
    }

    program->addASTNodeToPool(node);
    return node;
}

string ASTNode::getBinaryOpText(NodeType type) {
    switch (type) {
        case NodeType::And:
            return "&&";
        case NodeType::Or:
            return "||";
        case NodeType::Addition:
            return "+";
        case NodeType::Multiplication:
            return "*";
        case NodeType::Subtraction:
            return "-";
        case NodeType::CompareEQ:
            return "==";
        case NodeType::CompareNEQ:
            return "!=";
        case NodeType::CompareLTE:
            return "<=";
        case NodeType::CompareLT:
            return "<";
        case NodeType::CompareGTE:
            return ">=";
        case NodeType::CompareGT:
            return ">";
        default:
            assert(false && "Error\n");
    }
}

string ASTNode::getUnaryOpText(NodeType type) {
    switch (type) {
        case NodeType::Not:
            return "!";
        case NodeType::Negation:
            return "-";
        default:
            assert(false && "Error\n");
    }
}



string ASTNode::toString(bool readableText) const {
    string s;
    string nodeTypeLabel = NodeTypeLabels[_nType];
    switch (_nType) {
        case NodeType::Constant:
        {
            s = nodeTypeLabel +" ";
            switch (_dType) {
                case DataType::Int: s = nodeTypeLabel + " " + to_string(getInt()); break;
                case DataType::Bool: s = nodeTypeLabel + " " + BooleanValueLabels[getBool()]; break;
                case DataType::NoType: assert(false && "Constant Node must have a data type!\n"); break;
            }
            break;
        }
        case NodeType::Identifier: s = nodeTypeLabel + " " + getIdName(); break;
        default:
            s = nodeTypeLabel;
            break;
    }

    string optText;
    if (readableText) {
        optText = "{" + getReadableText() + "}";
    }

    return s + " : " + DataTypeLabels[_dType] + " [ID: " + to_string(getNodeID()) +"] " + optText;
}

string ASTNode::getReadableText() const {
    if (isNoAction()) {
        return "NoAction";
    }

    if (isAssign()) {
        return getChild(0)->getReadableText() + " = " + getChild(1)->getReadableText();
    }

    if (isId()) {
        return getIdName();
    }

    if (isConstant()){
        if (isIntConst()) {
            return to_string(getInt());
        }
        else if (isBoolConst()) {
            return BooleanValueLabels[getBool()];
        }
        else {
            assert(false && "Error\n");
        }
    }

    if (isAtomic()) {
        vector<string> statementsText;
        statementsText.reserve(getChild(0)->getNumChildren());

        for (int i = 0; i < getChild(0)->getNumChildren(); i ++) {
            statementsText.push_back(getChild(0)->getChild(i)->getReadableText());
        }

        return join(statementsText, "; ");
    }

    if (isBinaryOperation()) {
        string binaryOp = getBinaryOpText(getNodeType());
        return getChild(0)->getReadableText() + " " + binaryOp + " " + getChild(1)->getReadableText();
    }
    else if (isUnaryOperation()) {
        string unaryOp = getUnaryOpText(getNodeType());
        return unaryOp + "(" + getChild(0)->getReadableText() + ")";
    }

    return "";
}
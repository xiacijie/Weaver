#include "ASTBuilder.h"
#include <sstream>

using namespace weaver;
using namespace antlrcpp;
using namespace std;
using namespace antlr4;

void ASTBuilder::build() {
    _ast->setRoot(visitProgram(_parserContext).as<ASTNode*>());
}

bool ASTBuilder::isBinaryBooleanOperation(NodeType type) {
    return type == NodeType::CompareGT || type == NodeType::CompareGTE || type == NodeType::CompareNEQ
        || type == NodeType::CompareEQ || type == NodeType::CompareLTE || type == NodeType::CompareLT
        || type == NodeType::And || type == NodeType::Or;
}

DataType ASTBuilder::getDataType(WeaverParser::TypeContext *ctx) {
    if (ctx->BOOL())  return DataType::Bool;
    else if (ctx->INT()) return DataType::Int;
    else assert(false && "Error in getDataType!\n");
}

void ASTBuilder::throwError(const string &errorMessage) {
    cerr << "Error: " << errorMessage << endl;
    abort();
}

void ASTBuilder::ensureVarIsNotDeclared(const string &varName) {
    if (_varTable->isVarDeclared(varName)) {
        stringstream ss;
        ss << "Variable '" << varName << "' is already declared!" << endl;
        throwError(ss.str());
    }
}

void ASTBuilder::ensureVarIsDeclared(const string &varName) {
    if (!_varTable->isVarDeclared(varName)) {
        stringstream ss;
        ss << "Variable '" << varName << "' is not declared!" << endl;
        throwError(ss.str());
    }
}

void ASTBuilder::ensureVarHasType(const string &varName, DataType type) {
    if (_varTable->getVarType(varName) != type) {
        stringstream ss;
        ss << "Assignment of variable '" << varName << "' type is unmatched!";
        throwError(ss.str());
    }
}

void ASTBuilder::ensureVarIsInitialized(const string &varName) {
    if (!_varTable->isVarInitialized(varName)) {
        stringstream ss;
        ss << "Variable '" << varName << "' is not initialized!";
        throwError(ss.str());
    }
}

void ASTBuilder::declareVariable(const string &varName, DataType type) {
    ensureVarIsNotDeclared(varName);
    _varTable->declareVar(varName, type);
}

Any ASTBuilder::visitVarDeclarationStatement(WeaverParser::VarDeclarationStatementContext *ctx) {

    WeaverParser::VarDeclarationBodyContext* body = ctx->varDeclarationBody();

    if (dynamic_cast<WeaverParser::ProgramContext*>(ctx->parent->parent) == NULL ) {
        throwError("Cannot declare a Variable inside a Block!");
    }

    vector<ASTNode*> assignments;

    for (const auto& singleVarDecl : body->singleVarDeclaration()) {
        bool isArrayDecl = false;
        if (singleVarDecl->LEFT_SQUARE_BRACKET()) { // it is array declaration
            string varName = singleVarDecl->IDENTIFIER()->getText();
            DataType arrayType = toArrayType(getDataType(ctx->type()));

            declareVariable(varName, arrayType);
        }
        else {
            string varName = singleVarDecl->assignment() ? singleVarDecl->assignment()->IDENTIFIER()->getText()
                                                         : singleVarDecl->IDENTIFIER()->getText();

            declareVariable(varName, getDataType(ctx->type()));
        }


        // if it is an assignment
        if (singleVarDecl->assignment()) {
            ASTNode* assignmentNode = visitAssignment(singleVarDecl->assignment()).as<ASTNode*>();
            assignments.push_back(assignmentNode);
        }
    }

    if (assignments.empty()) {
        return nullptr;
    }
    else  if (assignments.size() == 1) {
        return assignments[0];
    }
    else {
        auto atomicNode = ASTNode::create(NodeType::Atomic, DataType::NoType, _program,assignments);
        return atomicNode;
    }

}

Any ASTBuilder::visitProgram(WeaverParser::ProgramContext *ctx) {
    if (ctx->statement().empty()) {
        throwError("Empty Program!");
    }

    vector<ASTNode*> children;

    for (WeaverParser::StatementContext* s : ctx->statement()) {
        Any statementNode = WeaverParserBaseVisitor::visitStatement(s);
        if (statementNode.isNotNull()) {
            if (statementNode.is<ASTNode*>()) {
                ASTNode* node = statementNode.as<ASTNode*>();
                children.push_back(node);
            }
        }
    }

    auto programNode = ASTNode::create(NodeType::Programme, DataType::NoType, _program, children);

//    if (_varTable->anyUninitializedVar()) {
//
//        stringstream ss;
//        ss << "Error: Not all variables are initialized!" << endl;
//        for (const auto& it : _varTable->getGlobalVariables()) {
//            if (!_varTable->isInitializedVarDeclared(it.first)) {
//                ss << "Variable '" << it.first << "' " << "not initialized!";
//            }
//        }
//
//        throwError(ss.str());
//    }

    if (!_hasAssertion) {
        throwError("Program has no Assertions!");
    }

    return programNode;
}

Any ASTBuilder::visitAssignmentStatement(WeaverParser::AssignmentStatementContext *ctx) {
    ASTNode* assignmentStatement = visitAssignment(ctx->assignment()).as<ASTNode*>();
    return assignmentStatement;
}

Any ASTBuilder::visitAssignment(WeaverParser::AssignmentContext *ctx) {
    string varName = ctx->IDENTIFIER()->getText();

    // verify if the variable to be assigned is declared or not
    ensureVarIsDeclared(varName);

    auto idNode = ASTNode::create(varName, _varTable->getVarType(varName), _program);

    ASTNode* expressionNode = visitExpression(ctx->expression()).as<ASTNode*>();

    ensureVarHasType(varName, idNode->getDataType());

//    //init this var
//    _varTable->initializeVar(varName);

    auto assignNode = ASTNode::create(NodeType::Assignment, DataType::NoType, _program, {idNode, expressionNode});
    return assignNode;
}

Any ASTBuilder::visitOperand(WeaverParser::OperandContext *ctx) {

    if (ctx->INTEGER() && ctx->MINUS()) {
        int64_t intVal = stol(ctx->INTEGER()->getText());

        auto constIntNode = ASTNode::create(intVal, _program);

        auto negationNode = ASTNode::create(NodeType::Negation, DataType::Int, _program, {constIntNode});
        return negationNode;
    }
    else if (ctx->INTEGER()) {
        int64_t intVal = stol(ctx->INTEGER()->getText());
        auto constIntNode = ASTNode::create(intVal, _program);
        return constIntNode;
    }
    else if (ctx->IDENTIFIER()) {
        string varName = ctx->IDENTIFIER()->getText();
        ensureVarIsDeclared(varName);

//        ensureVarIsInitialized(varName);

        auto varNode = ASTNode::create(varName, _varTable->getVarType(varName), _program);
        return varNode;
    }
    else if (ctx->boolean()) {
        return visitBoolean(ctx->boolean()).as<ASTNode*>();
    }
    else {
        assert(false && "Error in visit Operand!\n");
        return nullptr;
    }
}

Any ASTBuilder::visitBoolean(WeaverParser::BooleanContext *ctx) {
    BooleanValue value;

    if (ctx->FALSE()) value = BooleanValue::False;
    else if (ctx->TRUE()) value = BooleanValue::True;
    else if (ctx->Q_MARK()) value = BooleanValue::NonDeter;
    else {
        assert(false && "Error in visit boolean!\n");
        return nullptr;
    }

    ASTNode* boolNode = ASTNode::create(value, _program);
    return boolNode;
}

Any ASTBuilder::visitExpression(WeaverParser::ExpressionContext *ctx) {
    if (ctx->LEFT_PAREN()) {
        ASTNode* expressionNode = visitExpression(ctx->expression(0)).as<ASTNode*>();
        return expressionNode;
    }

    if (ctx->MINUS() && ctx->expression().size() == 1) { //unary negation
        ASTNode* expressionNode =  visitExpression(ctx->expression(0)).as<ASTNode*>();
        if (!expressionNode->isInt()) {
            throwError("Only Integer can be negated!");
        }

        ASTNode* negationNode = ASTNode::create(NodeType::Negation, DataType::Int, _program, {expressionNode});
        return negationNode;
    }

    if (ctx->PLUS() && ctx->expression().size() == 1) {
        ASTNode* expressionNode = visitExpression(ctx->expression(0)).as<ASTNode*>();
        if (!expressionNode->isInt()) {
            throwError("Only Integer can be + !");
        }

        return expressionNode;
    }

    if (ctx->NOT() && ctx->expression().size() == 1) {
        ASTNode* expressionNode = visitExpression(ctx->expression(0)).as<ASTNode*>();
        if (!expressionNode->isBool()) {
            throwError("Only Boolean can be complemented!");
        }

        ASTNode* notNode = ASTNode::create(NodeType::Not, DataType::Bool, _program, {expressionNode});

        return notNode;
    }

    if (ctx->STAR()) {
        ASTNode* expressionNode1 = visitExpression(ctx->expression(0)).as<ASTNode*>();
        ASTNode* expressionNode2 = visitExpression(ctx->expression(1)).as<ASTNode*>();

        if (!expressionNode1->isInt() || !expressionNode2->isInt()) {
            throwError("Only Integers can be multiplied!");
        }

        ASTNode* multiNode = ASTNode::create(NodeType::Multiplication, DataType::Int, _program, {expressionNode1, expressionNode2});

        return multiNode;
    }

    if (ctx->PLUS()) {
        ASTNode* expressionNode1 = visitExpression(ctx->expression(0)).as<ASTNode*>();
        ASTNode* expressionNode2 = visitExpression(ctx->expression(1)).as<ASTNode*>();

        if (!expressionNode1->isInt() || !expressionNode2->isInt()) {
            throwError("Only Integers can be added!");
        }

        ASTNode* addNode = ASTNode::create(NodeType::Addition, DataType::Int, _program, {expressionNode1, expressionNode2});

        return addNode;
    }

    if (ctx->MINUS()) {
        ASTNode* expressionNode1 = visitExpression(ctx->expression(0)).as<ASTNode*>();
        ASTNode* expressionNode2 = visitExpression(ctx->expression(1)).as<ASTNode*>();

        if (!expressionNode1->isInt() || !expressionNode2->isInt()) {
            throwError("Only Integers can be -!");
        }

        ASTNode* minusNode = ASTNode::create(NodeType::Subtraction, DataType::Int, _program, {expressionNode1, expressionNode2});

        return minusNode;
    }

    if (ctx->AND()) {
        ASTNode* expressionNode1 = visitExpression(ctx->expression(0)).as<ASTNode*>();
        ASTNode* expressionNode2 = visitExpression(ctx->expression(1)).as<ASTNode*>();

        if (!expressionNode1->isBool() || !expressionNode2->isBool()) {
            throwError("Only Booleans can be &&!");
        }

        ASTNode* andNode = ASTNode::create(NodeType::And, DataType::Bool, _program, {expressionNode1, expressionNode2});

        return andNode;
    }

    if (ctx->OR()) {
        ASTNode* expressionNode1 = visitExpression(ctx->expression(0)).as<ASTNode*>();
        ASTNode* expressionNode2 = visitExpression(ctx->expression(1)).as<ASTNode*>();

        if (!expressionNode1->isBool() || !expressionNode2->isBool()) {
            throwError("Only Booleans can be ||!");
        }

        ASTNode* orNode = ASTNode::create(NodeType::Or, DataType::Bool, _program, {expressionNode1, expressionNode2});

        return orNode;
    }

    if (ctx->comparisonOp()) {
        NodeType nodeType;
        const auto& comparisonOp = ctx->comparisonOp();

        if (comparisonOp->GT()) nodeType = NodeType::CompareGT;
        else if (comparisonOp->LT()) nodeType = NodeType::CompareLT;
        else if (comparisonOp->LTE()) nodeType = NodeType::CompareLTE;
        else if (comparisonOp->GTE()) nodeType = NodeType::CompareGTE;
        else if (comparisonOp->EQ()) nodeType = NodeType::CompareEQ;
        else if (comparisonOp->NEQ()) nodeType = NodeType::CompareNEQ;
        else {
            assert(false && "Error");
        }

        ASTNode* expressionNode1 = visitExpression(ctx->expression(0)).as<ASTNode*>();
        ASTNode* expressionNode2 = visitExpression(ctx->expression(1)).as<ASTNode*>();

        if (comparisonOp->GT() || comparisonOp->LT() || comparisonOp->LTE() || comparisonOp->GTE()) {
            if (!expressionNode1->isInt() || !expressionNode2->isInt()) {
                throwError("Invalid comparison operation!");
            }
        }

        if (expressionNode1->isBool() != expressionNode2->isBool()) {
            throwError("Compare over two different type!");
        }

        ASTNode* cmpNode = ASTNode::create(nodeType, DataType::Bool, _program, {expressionNode1, expressionNode2});
        return cmpNode;
    }

    if (ctx->operand()) {
        return visitOperand(ctx->operand()).as<ASTNode*>();
    }

    if (ctx->selectExpression()) {
        return visitSelectExpression(ctx->selectExpression()).as<ASTNode*>();
    }

    assert(false && "Error");
}

Any ASTBuilder::visitBlock(WeaverParser::BlockContext *ctx) {
    if (ctx->statement().empty()) {
        throwError("Empty block is not allowed!");
    }

    vector<ASTNode*> children;

    for (WeaverParser::StatementContext* s : ctx->statement()) {

        Any statementNode = WeaverParserBaseVisitor::visitStatement(s);

        if (statementNode.isNotNull()) {
            if (statementNode.is<ASTNode *>()) {
                ASTNode *node = statementNode.as<ASTNode *>();
                children.push_back(node);
            }
        }
    }

    auto blockNode = ASTNode::create(NodeType::Block, DataType::NoType, _program, children);

    return blockNode;
}

Any ASTBuilder::visitAtomicStatement(WeaverParser::AtomicStatementContext *ctx) {
    if (ctx->block()->statement().empty()) {
        throwError("Empty Atomic is not allowed!");
    }

    vector<ASTNode*> statements;

    for (WeaverParser::StatementContext* s : ctx->block()->statement()) {

        Any statementNode = WeaverParserBaseVisitor::visitStatement(s);

        if (statementNode.isNotNull()) {
            if (statementNode.is<ASTNode *>()) {
                ASTNode *node = statementNode.as<ASTNode *>();

                if (!node->isAssign()) {
                    throwError("Invalid Atomic Syntax, only Assignment Statement can be in Atomic!");
                }

                statements.push_back(node);
            }
        }
    }

    auto atomicNode = ASTNode::create(NodeType::Atomic, DataType::NoType, _program, statements);
    return atomicNode;
}



Any ASTBuilder::visitConditionalStatement(WeaverParser::ConditionalStatementContext *ctx) {
    ASTNode* conditionNode = visitExpression(ctx->expression()).as<ASTNode*>();

    if (!conditionNode->isBool()) {
        throwError("If statement only accepts boolean value as the condition!");
    }

    ASTNode* ifTrueTargetNode = WeaverParserBaseVisitor::visitStatement(ctx->statement(0)).as<ASTNode*>();

    if (ctx->statement().size() == 1) {
        return ASTNode::create(NodeType::Conditional, DataType::NoType, _program, {conditionNode, ifTrueTargetNode});
    }
    else {
        ASTNode* ifFalseTargetNode = WeaverParserBaseVisitor::visitStatement(ctx->statement(1)).as<ASTNode*>();
        return ASTNode::create(NodeType::Conditional, DataType::NoType, _program, {conditionNode, ifTrueTargetNode, ifFalseTargetNode});
    }
}

Any ASTBuilder::visitWhileStatement(WeaverParser::WhileStatementContext *ctx) {
    ASTNode* conditionNode = visitExpression(ctx->expression()).as<ASTNode*>();

    if (!conditionNode->isBool()) {
        throwError("While statement only accepts boolean value as the condition!");
    }

    ASTNode* blockNode = visitBlock(ctx->block()).as<ASTNode*>();

    if (ctx->block()->statement().empty()) {
        throwError("While body cannot be empty!");
    }

    return ASTNode::create(NodeType::Loop, DataType::NoType, _program, {conditionNode, blockNode});
}

Any ASTBuilder::visitAssertStatement(WeaverParser::AssertStatementContext *ctx) {
    _hasAssertion = true;

    ASTNode* conditionNode = visitExpression(ctx->expression()).as<ASTNode*>();

    if (!conditionNode->isBool()) {
        throwError("Assert statement only accepts boolean value as the condition!");
    }

    return ASTNode::create(NodeType::Assert, DataType::NoType, _program, {conditionNode});
}

Any ASTBuilder::visitAssumeStatement(WeaverParser::AssumeStatementContext *ctx) {
    const auto& expression = ctx->expression();

    ASTNode* conditionNode = visitExpression(ctx->expression()).as<ASTNode*>();

    if (!conditionNode->isBool()) {
        throwError("Assume statement only accepts boolean value as the condition!");
    }

    return ASTNode::create(NodeType::Assume, DataType::NoType, _program, {conditionNode});
}

Any ASTBuilder::visitParallelStatement(WeaverParser::ParallelStatementContext *ctx) {
    int64_t numParBodies = stol(ctx->INTEGER()->getText());

    if (dynamic_cast<WeaverParser::ProgramContext*>(ctx->parent->parent) == nullptr ) {
        throwError("Cannot write parallel statement inside a block!");
    }

    if (numParBodies <= 1) {
        throwError("Number of Parallel Bodies must be greater than 1!");
    }

    if (numParBodies != ctx->block().size()) {
        throwError("Unmatched number of Parallel Bodies!");
    }

    vector<ASTNode*> children;
    for (const auto& block : ctx->block()) {
        children.push_back(visitBlock(block).as<ASTNode*>());
    }

    auto parallelNode = ASTNode::create(NodeType::Parallel, DataType::NoType, _program, children);
    return parallelNode;
}

Any ASTBuilder::visitIncreaseStatement(WeaverParser::IncreaseStatementContext *ctx) {
    string varName = ctx->IDENTIFIER()->getText();

    ensureVarIsDeclared(varName);
    ensureVarHasType(varName, DataType::Int);

    ASTNode* idNode = ASTNode::create(varName, DataType::Int, _program);

    ASTNode* constOneNode = ASTNode::create(1, _program);

    ASTNode* addExpr = ASTNode::create(NodeType::Addition, DataType::Int, _program,{idNode, constOneNode});
    ASTNode* assignmentNode = ASTNode::create(NodeType::Assignment, DataType::Int, _program, {idNode, addExpr});

    return assignmentNode;
}

Any ASTBuilder::visitDecreaseStatement(WeaverParser::DecreaseStatementContext *ctx) {
    string varName = ctx->IDENTIFIER()->getText();

    ensureVarIsDeclared(varName);
    ensureVarHasType(varName, DataType::Int);

    ASTNode* idNode = ASTNode::create(varName, DataType::Int, _program);
    
    ASTNode* constOneNode = ASTNode::create(1, _program);

    ASTNode* subExpr = ASTNode::create(NodeType::Subtraction, DataType::Int, _program,{idNode, constOneNode});
    ASTNode* assignmentNode = ASTNode::create(NodeType::Assignment, DataType::Int, _program, {idNode, subExpr});

    return assignmentNode;
}

DataType ASTBuilder::toArrayType(DataType type) {
    switch (type) {
        case DataType::Int:
            return DataType::IntArray;
        default:
            assert(false && "Error");
    }
}

DataType ASTBuilder::fromArrayType(DataType type) {
    switch (type) {
        case DataType::IntArray:
            return DataType::Int;
        default:
            assert(false && "Error");
    }
}

bool ASTBuilder::isArrayType(DataType type) {
    switch (type) {
        case DataType::IntArray:
            return true;
        default:
            return false;
    }
}

Any ASTBuilder::visitStoreStatement(WeaverParser::StoreStatementContext *ctx) {
    string varName = ctx->IDENTIFIER()->getText();
    ensureVarIsDeclared(varName);

    ASTNode* expressionNode = visitExpression(ctx->expression(1)).as<ASTNode*>();
    DataType type = toArrayType(expressionNode->getDataType());
    ensureVarHasType(varName, type);

    ASTNode* arrayIdNode = ASTNode::create(varName, type, _program);
    ASTNode* indexNode = visitExpression(ctx->expression(0)).as<ASTNode*>();
    if (!indexNode->isInt()) {
        throwError("Array index must have integer type!");
    }

    ASTNode* storeNode = ASTNode::create(NodeType::Store, DataType::NoType, _program, {arrayIdNode, indexNode, expressionNode});
    return storeNode;
}

Any ASTBuilder::visitSelectExpression(WeaverParser::SelectExpressionContext *ctx) {
    string varName = ctx->IDENTIFIER()->getText();
    ensureVarIsDeclared(varName);
    DataType type = _varTable->getVarType(varName);

    if (!isArrayType(type)) {
        throwError("Array dereference on a non Array variable!");
    }

    ASTNode* arrayNode = ASTNode::create(varName,type, _program);

    ASTNode* indexNode = visitExpression(ctx->expression()).as<ASTNode*>();
    if (!indexNode->isInt()) {
        throwError("Array Index must have Int Type!");
    }

    DataType expressionType = fromArrayType(type);
    ASTNode* selectNode = ASTNode::create(NodeType::Select, expressionType, _program, {arrayNode, indexNode});
    return selectNode;
}
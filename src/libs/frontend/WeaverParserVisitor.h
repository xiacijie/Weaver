
// Generated from /home/xiacijie/Project/Weaver/src/grammar/WeaverParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "WeaverParser.h"


namespace weaver {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by WeaverParser.
 */
class  WeaverParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by WeaverParser.
   */
    virtual antlrcpp::Any visitProgram(WeaverParser::ProgramContext *context) = 0;

    virtual antlrcpp::Any visitType(WeaverParser::TypeContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(WeaverParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitSingleVarDeclaration(WeaverParser::SingleVarDeclarationContext *context) = 0;

    virtual antlrcpp::Any visitVarDeclarationBody(WeaverParser::VarDeclarationBodyContext *context) = 0;

    virtual antlrcpp::Any visitVarDeclarationStatement(WeaverParser::VarDeclarationStatementContext *context) = 0;

    virtual antlrcpp::Any visitOperand(WeaverParser::OperandContext *context) = 0;

    virtual antlrcpp::Any visitExpression(WeaverParser::ExpressionContext *context) = 0;

    virtual antlrcpp::Any visitComparisonOp(WeaverParser::ComparisonOpContext *context) = 0;

    virtual antlrcpp::Any visitBoolean(WeaverParser::BooleanContext *context) = 0;

    virtual antlrcpp::Any visitStatement(WeaverParser::StatementContext *context) = 0;

    virtual antlrcpp::Any visitBlock(WeaverParser::BlockContext *context) = 0;

    virtual antlrcpp::Any visitAssignmentStatement(WeaverParser::AssignmentStatementContext *context) = 0;

    virtual antlrcpp::Any visitIncreaseStatement(WeaverParser::IncreaseStatementContext *context) = 0;

    virtual antlrcpp::Any visitDecreaseStatement(WeaverParser::DecreaseStatementContext *context) = 0;

    virtual antlrcpp::Any visitAssertStatement(WeaverParser::AssertStatementContext *context) = 0;

    virtual antlrcpp::Any visitAssumeStatement(WeaverParser::AssumeStatementContext *context) = 0;

    virtual antlrcpp::Any visitWhileStatement(WeaverParser::WhileStatementContext *context) = 0;

    virtual antlrcpp::Any visitConditionalStatement(WeaverParser::ConditionalStatementContext *context) = 0;

    virtual antlrcpp::Any visitParallelStatement(WeaverParser::ParallelStatementContext *context) = 0;

    virtual antlrcpp::Any visitAtomicStatement(WeaverParser::AtomicStatementContext *context) = 0;

    virtual antlrcpp::Any visitStoreStatement(WeaverParser::StoreStatementContext *context) = 0;

    virtual antlrcpp::Any visitSelectStatement(WeaverParser::SelectStatementContext *context) = 0;


};

}  // namespace weaver

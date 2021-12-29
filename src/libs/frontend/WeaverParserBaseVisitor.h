
// Generated from /home/xiacijie/Project/Weaver/src/grammar/WeaverParser.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "WeaverParserVisitor.h"


namespace weaver {

/**
 * This class provides an empty implementation of WeaverParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  WeaverParserBaseVisitor : public WeaverParserVisitor {
public:

  virtual antlrcpp::Any visitProgram(WeaverParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitType(WeaverParser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(WeaverParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSingleVarDeclaration(WeaverParser::SingleVarDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarDeclarationBody(WeaverParser::VarDeclarationBodyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarDeclarationStatement(WeaverParser::VarDeclarationStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOperand(WeaverParser::OperandContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpression(WeaverParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComparisonOp(WeaverParser::ComparisonOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBoolean(WeaverParser::BooleanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatement(WeaverParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlock(WeaverParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignmentStatement(WeaverParser::AssignmentStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIncreaseStatement(WeaverParser::IncreaseStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDecreaseStatement(WeaverParser::DecreaseStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssertStatement(WeaverParser::AssertStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssumeStatement(WeaverParser::AssumeStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhileStatement(WeaverParser::WhileStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConditionalStatement(WeaverParser::ConditionalStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParallelStatement(WeaverParser::ParallelStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtomicStatement(WeaverParser::AtomicStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStoreStatement(WeaverParser::StoreStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSelectStatement(WeaverParser::SelectStatementContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace weaver

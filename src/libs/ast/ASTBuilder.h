#pragma once
#include "../frontend/WeaverParserBaseVisitor.h"
#include "../frontend/WeaverLexer.h"
#include "../program/Program.h"
#include "AST.h"
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace antlrcpp;
using namespace antlr4;
namespace  weaver {


    class ASTBuilder : public WeaverParserBaseVisitor{
    public:
        ASTBuilder(Program* program, WeaverParser::ProgramContext *parserContext):
            _program(program),
            _varTable(&(program->getVariableTable())),
            _hasAssertion(false),
            _ast(&(program->getAST())),
            _parserContext(parserContext) {}

        void build();

    private:
        Any visitProgram(WeaverParser::ProgramContext *ctx) override;

        Any visitVarDeclarationStatement(WeaverParser::VarDeclarationStatementContext *ctx) override;
        Any visitAssignment(WeaverParser::AssignmentContext *ctx) override;
        Any visitAssignmentStatement(WeaverParser::AssignmentStatementContext *ctx) override;
        Any visitExpression(WeaverParser::ExpressionContext *ctx) override;
        Any visitOperand(WeaverParser::OperandContext *ctx) override;
        Any visitBoolean(WeaverParser::BooleanContext *ctx) override;
        Any visitConditionalStatement(WeaverParser::ConditionalStatementContext *ctx) override;
        Any visitWhileStatement(WeaverParser::WhileStatementContext *ctx) override;
        Any visitAssertStatement(WeaverParser::AssertStatementContext *ctx) override;
        Any visitAssumeStatement(WeaverParser::AssumeStatementContext *ctx) override;
        Any visitBlock(WeaverParser::BlockContext *ctx) override;
        Any visitAtomicStatement(WeaverParser::AtomicStatementContext *ctx) override;
        Any visitParallelStatement(WeaverParser::ParallelStatementContext *ctx) override;
        Any visitIncreaseStatement(WeaverParser::IncreaseStatementContext* ctx) override;
        Any visitDecreaseStatement(WeaverParser::DecreaseStatementContext* ctx) override;
        Any visitStoreStatement(WeaverParser::StoreStatementContext *ctx) override;

        void declareVariable(const string& varName, DataType type);
        void ensureVarIsNotDeclared(const string& varName);
        void ensureVarIsInitialized(const string& varName);
        void ensureVarIsDeclared(const string& varName);
        void ensureVarHasType(const string& varName, DataType type);
        void throwError(const string& errorMessage);

        WeaverParser::ProgramContext *_parserContext;
        DataType getDataType(WeaverParser::TypeContext* ctx);
        bool isBinaryBooleanOperation(NodeType type);
        VariableTable* _varTable;
        Program* _program;
        AST* _ast;

        bool _hasAssertion;

    };



}
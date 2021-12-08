
// Generated from /home/xiacijie/Project/Weaver/src/grammar/WeaverParser.g4 by ANTLR 4.9.2


#include "WeaverParserVisitor.h"

#include "WeaverParser.h"


using namespace antlrcpp;
using namespace weaver;
using namespace antlr4;

WeaverParser::WeaverParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

WeaverParser::~WeaverParser() {
  delete _interpreter;
}

std::string WeaverParser::getGrammarFileName() const {
  return "WeaverParser.g4";
}

const std::vector<std::string>& WeaverParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& WeaverParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

WeaverParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::ProgramContext::EOF() {
  return getToken(WeaverParser::EOF, 0);
}

std::vector<WeaverParser::StatementContext *> WeaverParser::ProgramContext::statement() {
  return getRuleContexts<WeaverParser::StatementContext>();
}

WeaverParser::StatementContext* WeaverParser::ProgramContext::statement(size_t i) {
  return getRuleContext<WeaverParser::StatementContext>(i);
}


size_t WeaverParser::ProgramContext::getRuleIndex() const {
  return WeaverParser::RuleProgram;
}


antlrcpp::Any WeaverParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::ProgramContext* WeaverParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, WeaverParser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(45);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << WeaverParser::INT)
      | (1ULL << WeaverParser::BOOL)
      | (1ULL << WeaverParser::IF)
      | (1ULL << WeaverParser::WHILE)
      | (1ULL << WeaverParser::ASSERT)
      | (1ULL << WeaverParser::ASSUME)
      | (1ULL << WeaverParser::ATOMIC)
      | (1ULL << WeaverParser::PARALLEL)
      | (1ULL << WeaverParser::IDENTIFIER)
      | (1ULL << WeaverParser::LEFT_BRACE))) != 0)) {
      setState(42);
      statement();
      setState(47);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(48);
    match(WeaverParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

WeaverParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::TypeContext::INT() {
  return getToken(WeaverParser::INT, 0);
}

tree::TerminalNode* WeaverParser::TypeContext::BOOL() {
  return getToken(WeaverParser::BOOL, 0);
}


size_t WeaverParser::TypeContext::getRuleIndex() const {
  return WeaverParser::RuleType;
}


antlrcpp::Any WeaverParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::TypeContext* WeaverParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 2, WeaverParser::RuleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(50);
    _la = _input->LA(1);
    if (!(_la == WeaverParser::INT

    || _la == WeaverParser::BOOL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

WeaverParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::AssignmentContext::IDENTIFIER() {
  return getToken(WeaverParser::IDENTIFIER, 0);
}

tree::TerminalNode* WeaverParser::AssignmentContext::ASSIGN() {
  return getToken(WeaverParser::ASSIGN, 0);
}

WeaverParser::ExpressionContext* WeaverParser::AssignmentContext::expression() {
  return getRuleContext<WeaverParser::ExpressionContext>(0);
}


size_t WeaverParser::AssignmentContext::getRuleIndex() const {
  return WeaverParser::RuleAssignment;
}


antlrcpp::Any WeaverParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::AssignmentContext* WeaverParser::assignment() {
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 4, WeaverParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(52);
    match(WeaverParser::IDENTIFIER);
    setState(53);
    match(WeaverParser::ASSIGN);
    setState(54);
    expression(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SingleVarDeclarationContext ------------------------------------------------------------------

WeaverParser::SingleVarDeclarationContext::SingleVarDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::SingleVarDeclarationContext::IDENTIFIER() {
  return getToken(WeaverParser::IDENTIFIER, 0);
}

WeaverParser::AssignmentContext* WeaverParser::SingleVarDeclarationContext::assignment() {
  return getRuleContext<WeaverParser::AssignmentContext>(0);
}


size_t WeaverParser::SingleVarDeclarationContext::getRuleIndex() const {
  return WeaverParser::RuleSingleVarDeclaration;
}


antlrcpp::Any WeaverParser::SingleVarDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitSingleVarDeclaration(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::SingleVarDeclarationContext* WeaverParser::singleVarDeclaration() {
  SingleVarDeclarationContext *_localctx = _tracker.createInstance<SingleVarDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 6, WeaverParser::RuleSingleVarDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(58);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(56);
      match(WeaverParser::IDENTIFIER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(57);
      assignment();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclarationBodyContext ------------------------------------------------------------------

WeaverParser::VarDeclarationBodyContext::VarDeclarationBodyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<WeaverParser::SingleVarDeclarationContext *> WeaverParser::VarDeclarationBodyContext::singleVarDeclaration() {
  return getRuleContexts<WeaverParser::SingleVarDeclarationContext>();
}

WeaverParser::SingleVarDeclarationContext* WeaverParser::VarDeclarationBodyContext::singleVarDeclaration(size_t i) {
  return getRuleContext<WeaverParser::SingleVarDeclarationContext>(i);
}

std::vector<tree::TerminalNode *> WeaverParser::VarDeclarationBodyContext::COMA() {
  return getTokens(WeaverParser::COMA);
}

tree::TerminalNode* WeaverParser::VarDeclarationBodyContext::COMA(size_t i) {
  return getToken(WeaverParser::COMA, i);
}


size_t WeaverParser::VarDeclarationBodyContext::getRuleIndex() const {
  return WeaverParser::RuleVarDeclarationBody;
}


antlrcpp::Any WeaverParser::VarDeclarationBodyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitVarDeclarationBody(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::VarDeclarationBodyContext* WeaverParser::varDeclarationBody() {
  VarDeclarationBodyContext *_localctx = _tracker.createInstance<VarDeclarationBodyContext>(_ctx, getState());
  enterRule(_localctx, 8, WeaverParser::RuleVarDeclarationBody);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(60);
    singleVarDeclaration();
    setState(65);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == WeaverParser::COMA) {
      setState(61);
      match(WeaverParser::COMA);
      setState(62);
      singleVarDeclaration();
      setState(67);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclarationStatementContext ------------------------------------------------------------------

WeaverParser::VarDeclarationStatementContext::VarDeclarationStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

WeaverParser::TypeContext* WeaverParser::VarDeclarationStatementContext::type() {
  return getRuleContext<WeaverParser::TypeContext>(0);
}

WeaverParser::VarDeclarationBodyContext* WeaverParser::VarDeclarationStatementContext::varDeclarationBody() {
  return getRuleContext<WeaverParser::VarDeclarationBodyContext>(0);
}

tree::TerminalNode* WeaverParser::VarDeclarationStatementContext::SEMICOLON() {
  return getToken(WeaverParser::SEMICOLON, 0);
}


size_t WeaverParser::VarDeclarationStatementContext::getRuleIndex() const {
  return WeaverParser::RuleVarDeclarationStatement;
}


antlrcpp::Any WeaverParser::VarDeclarationStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitVarDeclarationStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::VarDeclarationStatementContext* WeaverParser::varDeclarationStatement() {
  VarDeclarationStatementContext *_localctx = _tracker.createInstance<VarDeclarationStatementContext>(_ctx, getState());
  enterRule(_localctx, 10, WeaverParser::RuleVarDeclarationStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(68);
    type();
    setState(69);
    varDeclarationBody();
    setState(70);
    match(WeaverParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OperandContext ------------------------------------------------------------------

WeaverParser::OperandContext::OperandContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::OperandContext::MINUS() {
  return getToken(WeaverParser::MINUS, 0);
}

tree::TerminalNode* WeaverParser::OperandContext::INTEGER() {
  return getToken(WeaverParser::INTEGER, 0);
}

WeaverParser::BooleanContext* WeaverParser::OperandContext::boolean() {
  return getRuleContext<WeaverParser::BooleanContext>(0);
}

tree::TerminalNode* WeaverParser::OperandContext::IDENTIFIER() {
  return getToken(WeaverParser::IDENTIFIER, 0);
}


size_t WeaverParser::OperandContext::getRuleIndex() const {
  return WeaverParser::RuleOperand;
}


antlrcpp::Any WeaverParser::OperandContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitOperand(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::OperandContext* WeaverParser::operand() {
  OperandContext *_localctx = _tracker.createInstance<OperandContext>(_ctx, getState());
  enterRule(_localctx, 12, WeaverParser::RuleOperand);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(77);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case WeaverParser::MINUS: {
        enterOuterAlt(_localctx, 1);
        setState(72);
        match(WeaverParser::MINUS);
        setState(73);
        match(WeaverParser::INTEGER);
        break;
      }

      case WeaverParser::INTEGER: {
        enterOuterAlt(_localctx, 2);
        setState(74);
        match(WeaverParser::INTEGER);
        break;
      }

      case WeaverParser::TRUE:
      case WeaverParser::FALSE:
      case WeaverParser::Q_MARK: {
        enterOuterAlt(_localctx, 3);
        setState(75);
        boolean();
        break;
      }

      case WeaverParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 4);
        setState(76);
        match(WeaverParser::IDENTIFIER);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

WeaverParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

WeaverParser::OperandContext* WeaverParser::ExpressionContext::operand() {
  return getRuleContext<WeaverParser::OperandContext>(0);
}

tree::TerminalNode* WeaverParser::ExpressionContext::LEFT_PAREN() {
  return getToken(WeaverParser::LEFT_PAREN, 0);
}

std::vector<WeaverParser::ExpressionContext *> WeaverParser::ExpressionContext::expression() {
  return getRuleContexts<WeaverParser::ExpressionContext>();
}

WeaverParser::ExpressionContext* WeaverParser::ExpressionContext::expression(size_t i) {
  return getRuleContext<WeaverParser::ExpressionContext>(i);
}

tree::TerminalNode* WeaverParser::ExpressionContext::RIGHT_PAREN() {
  return getToken(WeaverParser::RIGHT_PAREN, 0);
}

tree::TerminalNode* WeaverParser::ExpressionContext::MINUS() {
  return getToken(WeaverParser::MINUS, 0);
}

tree::TerminalNode* WeaverParser::ExpressionContext::PLUS() {
  return getToken(WeaverParser::PLUS, 0);
}

tree::TerminalNode* WeaverParser::ExpressionContext::NOT() {
  return getToken(WeaverParser::NOT, 0);
}

tree::TerminalNode* WeaverParser::ExpressionContext::STAR() {
  return getToken(WeaverParser::STAR, 0);
}

WeaverParser::ComparisonOpContext* WeaverParser::ExpressionContext::comparisonOp() {
  return getRuleContext<WeaverParser::ComparisonOpContext>(0);
}

tree::TerminalNode* WeaverParser::ExpressionContext::AND() {
  return getToken(WeaverParser::AND, 0);
}

tree::TerminalNode* WeaverParser::ExpressionContext::OR() {
  return getToken(WeaverParser::OR, 0);
}


size_t WeaverParser::ExpressionContext::getRuleIndex() const {
  return WeaverParser::RuleExpression;
}


antlrcpp::Any WeaverParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}


WeaverParser::ExpressionContext* WeaverParser::expression() {
   return expression(0);
}

WeaverParser::ExpressionContext* WeaverParser::expression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  WeaverParser::ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  WeaverParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 14;
  enterRecursionRule(_localctx, 14, WeaverParser::RuleExpression, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(91);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      setState(80);
      operand();
      break;
    }

    case 2: {
      setState(81);
      match(WeaverParser::LEFT_PAREN);
      setState(82);
      expression(0);
      setState(83);
      match(WeaverParser::RIGHT_PAREN);
      break;
    }

    case 3: {
      setState(85);
      match(WeaverParser::MINUS);
      setState(86);
      expression(9);
      break;
    }

    case 4: {
      setState(87);
      match(WeaverParser::PLUS);
      setState(88);
      expression(8);
      break;
    }

    case 5: {
      setState(89);
      match(WeaverParser::NOT);
      setState(90);
      expression(7);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(114);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(112);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(93);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(94);
          match(WeaverParser::STAR);
          setState(95);
          expression(7);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(96);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(97);
          match(WeaverParser::PLUS);
          setState(98);
          expression(6);
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(99);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(100);
          match(WeaverParser::MINUS);
          setState(101);
          expression(5);
          break;
        }

        case 4: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(102);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(103);
          comparisonOp();
          setState(104);
          expression(4);
          break;
        }

        case 5: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(106);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(107);
          match(WeaverParser::AND);
          setState(108);
          expression(3);
          break;
        }

        case 6: {
          _localctx = _tracker.createInstance<ExpressionContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleExpression);
          setState(109);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(110);
          match(WeaverParser::OR);
          setState(111);
          expression(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(116);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ComparisonOpContext ------------------------------------------------------------------

WeaverParser::ComparisonOpContext::ComparisonOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::ComparisonOpContext::EQ() {
  return getToken(WeaverParser::EQ, 0);
}

tree::TerminalNode* WeaverParser::ComparisonOpContext::NEQ() {
  return getToken(WeaverParser::NEQ, 0);
}

tree::TerminalNode* WeaverParser::ComparisonOpContext::LTE() {
  return getToken(WeaverParser::LTE, 0);
}

tree::TerminalNode* WeaverParser::ComparisonOpContext::GTE() {
  return getToken(WeaverParser::GTE, 0);
}

tree::TerminalNode* WeaverParser::ComparisonOpContext::LT() {
  return getToken(WeaverParser::LT, 0);
}

tree::TerminalNode* WeaverParser::ComparisonOpContext::GT() {
  return getToken(WeaverParser::GT, 0);
}


size_t WeaverParser::ComparisonOpContext::getRuleIndex() const {
  return WeaverParser::RuleComparisonOp;
}


antlrcpp::Any WeaverParser::ComparisonOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitComparisonOp(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::ComparisonOpContext* WeaverParser::comparisonOp() {
  ComparisonOpContext *_localctx = _tracker.createInstance<ComparisonOpContext>(_ctx, getState());
  enterRule(_localctx, 16, WeaverParser::RuleComparisonOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(117);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << WeaverParser::EQ)
      | (1ULL << WeaverParser::NEQ)
      | (1ULL << WeaverParser::LTE)
      | (1ULL << WeaverParser::GTE)
      | (1ULL << WeaverParser::LT)
      | (1ULL << WeaverParser::GT))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanContext ------------------------------------------------------------------

WeaverParser::BooleanContext::BooleanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::BooleanContext::TRUE() {
  return getToken(WeaverParser::TRUE, 0);
}

tree::TerminalNode* WeaverParser::BooleanContext::FALSE() {
  return getToken(WeaverParser::FALSE, 0);
}

tree::TerminalNode* WeaverParser::BooleanContext::Q_MARK() {
  return getToken(WeaverParser::Q_MARK, 0);
}


size_t WeaverParser::BooleanContext::getRuleIndex() const {
  return WeaverParser::RuleBoolean;
}


antlrcpp::Any WeaverParser::BooleanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitBoolean(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::BooleanContext* WeaverParser::boolean() {
  BooleanContext *_localctx = _tracker.createInstance<BooleanContext>(_ctx, getState());
  enterRule(_localctx, 18, WeaverParser::RuleBoolean);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(119);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << WeaverParser::TRUE)
      | (1ULL << WeaverParser::FALSE)
      | (1ULL << WeaverParser::Q_MARK))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

WeaverParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

WeaverParser::AssignmentStatementContext* WeaverParser::StatementContext::assignmentStatement() {
  return getRuleContext<WeaverParser::AssignmentStatementContext>(0);
}

WeaverParser::IncreaseStatementContext* WeaverParser::StatementContext::increaseStatement() {
  return getRuleContext<WeaverParser::IncreaseStatementContext>(0);
}

WeaverParser::DecreaseStatementContext* WeaverParser::StatementContext::decreaseStatement() {
  return getRuleContext<WeaverParser::DecreaseStatementContext>(0);
}

WeaverParser::WhileStatementContext* WeaverParser::StatementContext::whileStatement() {
  return getRuleContext<WeaverParser::WhileStatementContext>(0);
}

WeaverParser::AssertStatementContext* WeaverParser::StatementContext::assertStatement() {
  return getRuleContext<WeaverParser::AssertStatementContext>(0);
}

WeaverParser::AssumeStatementContext* WeaverParser::StatementContext::assumeStatement() {
  return getRuleContext<WeaverParser::AssumeStatementContext>(0);
}

WeaverParser::ConditionalStatementContext* WeaverParser::StatementContext::conditionalStatement() {
  return getRuleContext<WeaverParser::ConditionalStatementContext>(0);
}

WeaverParser::VarDeclarationStatementContext* WeaverParser::StatementContext::varDeclarationStatement() {
  return getRuleContext<WeaverParser::VarDeclarationStatementContext>(0);
}

WeaverParser::AtomicStatementContext* WeaverParser::StatementContext::atomicStatement() {
  return getRuleContext<WeaverParser::AtomicStatementContext>(0);
}

WeaverParser::ParallelStatementContext* WeaverParser::StatementContext::parallelStatement() {
  return getRuleContext<WeaverParser::ParallelStatementContext>(0);
}

WeaverParser::BlockContext* WeaverParser::StatementContext::block() {
  return getRuleContext<WeaverParser::BlockContext>(0);
}


size_t WeaverParser::StatementContext::getRuleIndex() const {
  return WeaverParser::RuleStatement;
}


antlrcpp::Any WeaverParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::StatementContext* WeaverParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 20, WeaverParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(132);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(121);
      assignmentStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(122);
      increaseStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(123);
      decreaseStatement();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(124);
      whileStatement();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(125);
      assertStatement();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(126);
      assumeStatement();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(127);
      conditionalStatement();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(128);
      varDeclarationStatement();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(129);
      atomicStatement();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(130);
      parallelStatement();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(131);
      block();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

WeaverParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::BlockContext::LEFT_BRACE() {
  return getToken(WeaverParser::LEFT_BRACE, 0);
}

tree::TerminalNode* WeaverParser::BlockContext::RIGHT_BRACE() {
  return getToken(WeaverParser::RIGHT_BRACE, 0);
}

std::vector<WeaverParser::StatementContext *> WeaverParser::BlockContext::statement() {
  return getRuleContexts<WeaverParser::StatementContext>();
}

WeaverParser::StatementContext* WeaverParser::BlockContext::statement(size_t i) {
  return getRuleContext<WeaverParser::StatementContext>(i);
}


size_t WeaverParser::BlockContext::getRuleIndex() const {
  return WeaverParser::RuleBlock;
}


antlrcpp::Any WeaverParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::BlockContext* WeaverParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 22, WeaverParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(134);
    match(WeaverParser::LEFT_BRACE);
    setState(138);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << WeaverParser::INT)
      | (1ULL << WeaverParser::BOOL)
      | (1ULL << WeaverParser::IF)
      | (1ULL << WeaverParser::WHILE)
      | (1ULL << WeaverParser::ASSERT)
      | (1ULL << WeaverParser::ASSUME)
      | (1ULL << WeaverParser::ATOMIC)
      | (1ULL << WeaverParser::PARALLEL)
      | (1ULL << WeaverParser::IDENTIFIER)
      | (1ULL << WeaverParser::LEFT_BRACE))) != 0)) {
      setState(135);
      statement();
      setState(140);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(141);
    match(WeaverParser::RIGHT_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentStatementContext ------------------------------------------------------------------

WeaverParser::AssignmentStatementContext::AssignmentStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

WeaverParser::AssignmentContext* WeaverParser::AssignmentStatementContext::assignment() {
  return getRuleContext<WeaverParser::AssignmentContext>(0);
}

tree::TerminalNode* WeaverParser::AssignmentStatementContext::SEMICOLON() {
  return getToken(WeaverParser::SEMICOLON, 0);
}


size_t WeaverParser::AssignmentStatementContext::getRuleIndex() const {
  return WeaverParser::RuleAssignmentStatement;
}


antlrcpp::Any WeaverParser::AssignmentStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitAssignmentStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::AssignmentStatementContext* WeaverParser::assignmentStatement() {
  AssignmentStatementContext *_localctx = _tracker.createInstance<AssignmentStatementContext>(_ctx, getState());
  enterRule(_localctx, 24, WeaverParser::RuleAssignmentStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(143);
    assignment();
    setState(144);
    match(WeaverParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IncreaseStatementContext ------------------------------------------------------------------

WeaverParser::IncreaseStatementContext::IncreaseStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::IncreaseStatementContext::IDENTIFIER() {
  return getToken(WeaverParser::IDENTIFIER, 0);
}

tree::TerminalNode* WeaverParser::IncreaseStatementContext::INC() {
  return getToken(WeaverParser::INC, 0);
}

tree::TerminalNode* WeaverParser::IncreaseStatementContext::SEMICOLON() {
  return getToken(WeaverParser::SEMICOLON, 0);
}


size_t WeaverParser::IncreaseStatementContext::getRuleIndex() const {
  return WeaverParser::RuleIncreaseStatement;
}


antlrcpp::Any WeaverParser::IncreaseStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitIncreaseStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::IncreaseStatementContext* WeaverParser::increaseStatement() {
  IncreaseStatementContext *_localctx = _tracker.createInstance<IncreaseStatementContext>(_ctx, getState());
  enterRule(_localctx, 26, WeaverParser::RuleIncreaseStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    match(WeaverParser::IDENTIFIER);
    setState(147);
    match(WeaverParser::INC);
    setState(148);
    match(WeaverParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DecreaseStatementContext ------------------------------------------------------------------

WeaverParser::DecreaseStatementContext::DecreaseStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::DecreaseStatementContext::IDENTIFIER() {
  return getToken(WeaverParser::IDENTIFIER, 0);
}

tree::TerminalNode* WeaverParser::DecreaseStatementContext::DEC() {
  return getToken(WeaverParser::DEC, 0);
}

tree::TerminalNode* WeaverParser::DecreaseStatementContext::SEMICOLON() {
  return getToken(WeaverParser::SEMICOLON, 0);
}


size_t WeaverParser::DecreaseStatementContext::getRuleIndex() const {
  return WeaverParser::RuleDecreaseStatement;
}


antlrcpp::Any WeaverParser::DecreaseStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitDecreaseStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::DecreaseStatementContext* WeaverParser::decreaseStatement() {
  DecreaseStatementContext *_localctx = _tracker.createInstance<DecreaseStatementContext>(_ctx, getState());
  enterRule(_localctx, 28, WeaverParser::RuleDecreaseStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(150);
    match(WeaverParser::IDENTIFIER);
    setState(151);
    match(WeaverParser::DEC);
    setState(152);
    match(WeaverParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssertStatementContext ------------------------------------------------------------------

WeaverParser::AssertStatementContext::AssertStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::AssertStatementContext::ASSERT() {
  return getToken(WeaverParser::ASSERT, 0);
}

tree::TerminalNode* WeaverParser::AssertStatementContext::LEFT_PAREN() {
  return getToken(WeaverParser::LEFT_PAREN, 0);
}

WeaverParser::ExpressionContext* WeaverParser::AssertStatementContext::expression() {
  return getRuleContext<WeaverParser::ExpressionContext>(0);
}

tree::TerminalNode* WeaverParser::AssertStatementContext::RIGHT_PAREN() {
  return getToken(WeaverParser::RIGHT_PAREN, 0);
}

tree::TerminalNode* WeaverParser::AssertStatementContext::SEMICOLON() {
  return getToken(WeaverParser::SEMICOLON, 0);
}


size_t WeaverParser::AssertStatementContext::getRuleIndex() const {
  return WeaverParser::RuleAssertStatement;
}


antlrcpp::Any WeaverParser::AssertStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitAssertStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::AssertStatementContext* WeaverParser::assertStatement() {
  AssertStatementContext *_localctx = _tracker.createInstance<AssertStatementContext>(_ctx, getState());
  enterRule(_localctx, 30, WeaverParser::RuleAssertStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(154);
    match(WeaverParser::ASSERT);
    setState(155);
    match(WeaverParser::LEFT_PAREN);
    setState(156);
    expression(0);
    setState(157);
    match(WeaverParser::RIGHT_PAREN);
    setState(158);
    match(WeaverParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssumeStatementContext ------------------------------------------------------------------

WeaverParser::AssumeStatementContext::AssumeStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::AssumeStatementContext::ASSUME() {
  return getToken(WeaverParser::ASSUME, 0);
}

tree::TerminalNode* WeaverParser::AssumeStatementContext::LEFT_PAREN() {
  return getToken(WeaverParser::LEFT_PAREN, 0);
}

WeaverParser::ExpressionContext* WeaverParser::AssumeStatementContext::expression() {
  return getRuleContext<WeaverParser::ExpressionContext>(0);
}

tree::TerminalNode* WeaverParser::AssumeStatementContext::RIGHT_PAREN() {
  return getToken(WeaverParser::RIGHT_PAREN, 0);
}

tree::TerminalNode* WeaverParser::AssumeStatementContext::SEMICOLON() {
  return getToken(WeaverParser::SEMICOLON, 0);
}


size_t WeaverParser::AssumeStatementContext::getRuleIndex() const {
  return WeaverParser::RuleAssumeStatement;
}


antlrcpp::Any WeaverParser::AssumeStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitAssumeStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::AssumeStatementContext* WeaverParser::assumeStatement() {
  AssumeStatementContext *_localctx = _tracker.createInstance<AssumeStatementContext>(_ctx, getState());
  enterRule(_localctx, 32, WeaverParser::RuleAssumeStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(160);
    match(WeaverParser::ASSUME);
    setState(161);
    match(WeaverParser::LEFT_PAREN);
    setState(162);
    expression(0);
    setState(163);
    match(WeaverParser::RIGHT_PAREN);
    setState(164);
    match(WeaverParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhileStatementContext ------------------------------------------------------------------

WeaverParser::WhileStatementContext::WhileStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::WhileStatementContext::WHILE() {
  return getToken(WeaverParser::WHILE, 0);
}

tree::TerminalNode* WeaverParser::WhileStatementContext::LEFT_PAREN() {
  return getToken(WeaverParser::LEFT_PAREN, 0);
}

WeaverParser::ExpressionContext* WeaverParser::WhileStatementContext::expression() {
  return getRuleContext<WeaverParser::ExpressionContext>(0);
}

tree::TerminalNode* WeaverParser::WhileStatementContext::RIGHT_PAREN() {
  return getToken(WeaverParser::RIGHT_PAREN, 0);
}

WeaverParser::BlockContext* WeaverParser::WhileStatementContext::block() {
  return getRuleContext<WeaverParser::BlockContext>(0);
}


size_t WeaverParser::WhileStatementContext::getRuleIndex() const {
  return WeaverParser::RuleWhileStatement;
}


antlrcpp::Any WeaverParser::WhileStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitWhileStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::WhileStatementContext* WeaverParser::whileStatement() {
  WhileStatementContext *_localctx = _tracker.createInstance<WhileStatementContext>(_ctx, getState());
  enterRule(_localctx, 34, WeaverParser::RuleWhileStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(166);
    match(WeaverParser::WHILE);
    setState(167);
    match(WeaverParser::LEFT_PAREN);
    setState(168);
    expression(0);
    setState(169);
    match(WeaverParser::RIGHT_PAREN);
    setState(170);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalStatementContext ------------------------------------------------------------------

WeaverParser::ConditionalStatementContext::ConditionalStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::ConditionalStatementContext::IF() {
  return getToken(WeaverParser::IF, 0);
}

tree::TerminalNode* WeaverParser::ConditionalStatementContext::LEFT_PAREN() {
  return getToken(WeaverParser::LEFT_PAREN, 0);
}

WeaverParser::ExpressionContext* WeaverParser::ConditionalStatementContext::expression() {
  return getRuleContext<WeaverParser::ExpressionContext>(0);
}

tree::TerminalNode* WeaverParser::ConditionalStatementContext::RIGHT_PAREN() {
  return getToken(WeaverParser::RIGHT_PAREN, 0);
}

std::vector<WeaverParser::StatementContext *> WeaverParser::ConditionalStatementContext::statement() {
  return getRuleContexts<WeaverParser::StatementContext>();
}

WeaverParser::StatementContext* WeaverParser::ConditionalStatementContext::statement(size_t i) {
  return getRuleContext<WeaverParser::StatementContext>(i);
}

tree::TerminalNode* WeaverParser::ConditionalStatementContext::ELSE() {
  return getToken(WeaverParser::ELSE, 0);
}


size_t WeaverParser::ConditionalStatementContext::getRuleIndex() const {
  return WeaverParser::RuleConditionalStatement;
}


antlrcpp::Any WeaverParser::ConditionalStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitConditionalStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::ConditionalStatementContext* WeaverParser::conditionalStatement() {
  ConditionalStatementContext *_localctx = _tracker.createInstance<ConditionalStatementContext>(_ctx, getState());
  enterRule(_localctx, 36, WeaverParser::RuleConditionalStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(172);
    match(WeaverParser::IF);
    setState(173);
    match(WeaverParser::LEFT_PAREN);
    setState(174);
    expression(0);
    setState(175);
    match(WeaverParser::RIGHT_PAREN);
    setState(176);
    statement();
    setState(179);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
    case 1: {
      setState(177);
      match(WeaverParser::ELSE);
      setState(178);
      statement();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParallelStatementContext ------------------------------------------------------------------

WeaverParser::ParallelStatementContext::ParallelStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::ParallelStatementContext::PARALLEL() {
  return getToken(WeaverParser::PARALLEL, 0);
}

tree::TerminalNode* WeaverParser::ParallelStatementContext::LEFT_PAREN() {
  return getToken(WeaverParser::LEFT_PAREN, 0);
}

tree::TerminalNode* WeaverParser::ParallelStatementContext::INTEGER() {
  return getToken(WeaverParser::INTEGER, 0);
}

tree::TerminalNode* WeaverParser::ParallelStatementContext::RIGHT_PAREN() {
  return getToken(WeaverParser::RIGHT_PAREN, 0);
}

std::vector<WeaverParser::BlockContext *> WeaverParser::ParallelStatementContext::block() {
  return getRuleContexts<WeaverParser::BlockContext>();
}

WeaverParser::BlockContext* WeaverParser::ParallelStatementContext::block(size_t i) {
  return getRuleContext<WeaverParser::BlockContext>(i);
}


size_t WeaverParser::ParallelStatementContext::getRuleIndex() const {
  return WeaverParser::RuleParallelStatement;
}


antlrcpp::Any WeaverParser::ParallelStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitParallelStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::ParallelStatementContext* WeaverParser::parallelStatement() {
  ParallelStatementContext *_localctx = _tracker.createInstance<ParallelStatementContext>(_ctx, getState());
  enterRule(_localctx, 38, WeaverParser::RuleParallelStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(181);
    match(WeaverParser::PARALLEL);
    setState(182);
    match(WeaverParser::LEFT_PAREN);
    setState(183);
    match(WeaverParser::INTEGER);
    setState(184);
    match(WeaverParser::RIGHT_PAREN);
    setState(188);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(185);
        block(); 
      }
      setState(190);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomicStatementContext ------------------------------------------------------------------

WeaverParser::AtomicStatementContext::AtomicStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* WeaverParser::AtomicStatementContext::ATOMIC() {
  return getToken(WeaverParser::ATOMIC, 0);
}

WeaverParser::BlockContext* WeaverParser::AtomicStatementContext::block() {
  return getRuleContext<WeaverParser::BlockContext>(0);
}


size_t WeaverParser::AtomicStatementContext::getRuleIndex() const {
  return WeaverParser::RuleAtomicStatement;
}


antlrcpp::Any WeaverParser::AtomicStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<WeaverParserVisitor*>(visitor))
    return parserVisitor->visitAtomicStatement(this);
  else
    return visitor->visitChildren(this);
}

WeaverParser::AtomicStatementContext* WeaverParser::atomicStatement() {
  AtomicStatementContext *_localctx = _tracker.createInstance<AtomicStatementContext>(_ctx, getState());
  enterRule(_localctx, 40, WeaverParser::RuleAtomicStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(191);
    match(WeaverParser::ATOMIC);
    setState(192);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool WeaverParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 7: return expressionSempred(dynamic_cast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool WeaverParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 6);
    case 1: return precpred(_ctx, 5);
    case 2: return precpred(_ctx, 4);
    case 3: return precpred(_ctx, 3);
    case 4: return precpred(_ctx, 2);
    case 5: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> WeaverParser::_decisionToDFA;
atn::PredictionContextCache WeaverParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN WeaverParser::_atn;
std::vector<uint16_t> WeaverParser::_serializedATN;

std::vector<std::string> WeaverParser::_ruleNames = {
  "program", "type", "assignment", "singleVarDeclaration", "varDeclarationBody", 
  "varDeclarationStatement", "operand", "expression", "comparisonOp", "boolean", 
  "statement", "block", "assignmentStatement", "increaseStatement", "decreaseStatement", 
  "assertStatement", "assumeStatement", "whileStatement", "conditionalStatement", 
  "parallelStatement", "atomicStatement"
};

std::vector<std::string> WeaverParser::_literalNames = {
  "", "'int'", "'bool'", "'if'", "'else'", "'while'", "'assert'", "'assume'", 
  "'atomic'", "'parallel'", "'true'", "'false'", "", "", "'++'", "'--'", 
  "'=='", "'!='", "'<='", "'>='", "'<'", "'>'", "'&&'", "'||'", "'!'", "'*'", 
  "'\u003F'", "'+'", "'-'", "'='", "':'", "','", "';'", "'{'", "'}'", "'('", 
  "')'"
};

std::vector<std::string> WeaverParser::_symbolicNames = {
  "", "INT", "BOOL", "IF", "ELSE", "WHILE", "ASSERT", "ASSUME", "ATOMIC", 
  "PARALLEL", "TRUE", "FALSE", "IDENTIFIER", "INTEGER", "INC", "DEC", "EQ", 
  "NEQ", "LTE", "GTE", "LT", "GT", "AND", "OR", "NOT", "STAR", "Q_MARK", 
  "PLUS", "MINUS", "ASSIGN", "COLON", "COMA", "SEMICOLON", "LEFT_BRACE", 
  "RIGHT_BRACE", "LEFT_PAREN", "RIGHT_PAREN", "NEWLINE", "TAB", "WHITE_SPACE", 
  "COMMENT", "LINE_COMMENT"
};

dfa::Vocabulary WeaverParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> WeaverParser::_tokenNames;

WeaverParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x2b, 0xc5, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 
       0x9, 0x14, 0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x3, 0x2, 
       0x7, 0x2, 0x2e, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x31, 0xb, 0x2, 0x3, 
       0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
       0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x3d, 0xa, 0x5, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x42, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 
       0x45, 0xb, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 
       0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x50, 0xa, 0x8, 
       0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 
       0x5e, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
       0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
       0x3, 0x9, 0x7, 0x9, 0x73, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x76, 0xb, 
       0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 
       0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 
       0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x87, 0xa, 0xc, 0x3, 0xd, 0x3, 
       0xd, 0x7, 0xd, 0x8b, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x8e, 0xb, 0xd, 
       0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 
       0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
       0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
       0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
       0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
       0x3, 0x14, 0x5, 0x14, 0xb6, 0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 
       0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0xbd, 0xa, 0x15, 0xc, 0x15, 
       0xe, 0x15, 0xc0, 0xb, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 
       0x16, 0x2, 0x3, 0x10, 0x17, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 
       0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 
       0x28, 0x2a, 0x2, 0x5, 0x3, 0x2, 0x3, 0x4, 0x3, 0x2, 0x12, 0x17, 0x4, 
       0x2, 0xc, 0xd, 0x1c, 0x1c, 0x2, 0xcc, 0x2, 0x2f, 0x3, 0x2, 0x2, 0x2, 
       0x4, 0x34, 0x3, 0x2, 0x2, 0x2, 0x6, 0x36, 0x3, 0x2, 0x2, 0x2, 0x8, 
       0x3c, 0x3, 0x2, 0x2, 0x2, 0xa, 0x3e, 0x3, 0x2, 0x2, 0x2, 0xc, 0x46, 
       0x3, 0x2, 0x2, 0x2, 0xe, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x10, 0x5d, 0x3, 
       0x2, 0x2, 0x2, 0x12, 0x77, 0x3, 0x2, 0x2, 0x2, 0x14, 0x79, 0x3, 0x2, 
       0x2, 0x2, 0x16, 0x86, 0x3, 0x2, 0x2, 0x2, 0x18, 0x88, 0x3, 0x2, 0x2, 
       0x2, 0x1a, 0x91, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x94, 0x3, 0x2, 0x2, 0x2, 
       0x1e, 0x98, 0x3, 0x2, 0x2, 0x2, 0x20, 0x9c, 0x3, 0x2, 0x2, 0x2, 0x22, 
       0xa2, 0x3, 0x2, 0x2, 0x2, 0x24, 0xa8, 0x3, 0x2, 0x2, 0x2, 0x26, 0xae, 
       0x3, 0x2, 0x2, 0x2, 0x28, 0xb7, 0x3, 0x2, 0x2, 0x2, 0x2a, 0xc1, 0x3, 
       0x2, 0x2, 0x2, 0x2c, 0x2e, 0x5, 0x16, 0xc, 0x2, 0x2d, 0x2c, 0x3, 
       0x2, 0x2, 0x2, 0x2e, 0x31, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x2d, 0x3, 0x2, 
       0x2, 0x2, 0x2f, 0x30, 0x3, 0x2, 0x2, 0x2, 0x30, 0x32, 0x3, 0x2, 0x2, 
       0x2, 0x31, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33, 0x7, 0x2, 0x2, 0x3, 
       0x33, 0x3, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0x9, 0x2, 0x2, 0x2, 0x35, 
       0x5, 0x3, 0x2, 0x2, 0x2, 0x36, 0x37, 0x7, 0xe, 0x2, 0x2, 0x37, 0x38, 
       0x7, 0x1f, 0x2, 0x2, 0x38, 0x39, 0x5, 0x10, 0x9, 0x2, 0x39, 0x7, 
       0x3, 0x2, 0x2, 0x2, 0x3a, 0x3d, 0x7, 0xe, 0x2, 0x2, 0x3b, 0x3d, 0x5, 
       0x6, 0x4, 0x2, 0x3c, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x3b, 0x3, 0x2, 
       0x2, 0x2, 0x3d, 0x9, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x43, 0x5, 0x8, 0x5, 
       0x2, 0x3f, 0x40, 0x7, 0x21, 0x2, 0x2, 0x40, 0x42, 0x5, 0x8, 0x5, 
       0x2, 0x41, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x42, 0x45, 0x3, 0x2, 0x2, 0x2, 
       0x43, 0x41, 0x3, 0x2, 0x2, 0x2, 0x43, 0x44, 0x3, 0x2, 0x2, 0x2, 0x44, 
       0xb, 0x3, 0x2, 0x2, 0x2, 0x45, 0x43, 0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 
       0x5, 0x4, 0x3, 0x2, 0x47, 0x48, 0x5, 0xa, 0x6, 0x2, 0x48, 0x49, 0x7, 
       0x22, 0x2, 0x2, 0x49, 0xd, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x4b, 0x7, 0x1e, 
       0x2, 0x2, 0x4b, 0x50, 0x7, 0xf, 0x2, 0x2, 0x4c, 0x50, 0x7, 0xf, 0x2, 
       0x2, 0x4d, 0x50, 0x5, 0x14, 0xb, 0x2, 0x4e, 0x50, 0x7, 0xe, 0x2, 
       0x2, 0x4f, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x4c, 0x3, 0x2, 0x2, 0x2, 
       0x4f, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x50, 
       0xf, 0x3, 0x2, 0x2, 0x2, 0x51, 0x52, 0x8, 0x9, 0x1, 0x2, 0x52, 0x5e, 
       0x5, 0xe, 0x8, 0x2, 0x53, 0x54, 0x7, 0x25, 0x2, 0x2, 0x54, 0x55, 
       0x5, 0x10, 0x9, 0x2, 0x55, 0x56, 0x7, 0x26, 0x2, 0x2, 0x56, 0x5e, 
       0x3, 0x2, 0x2, 0x2, 0x57, 0x58, 0x7, 0x1e, 0x2, 0x2, 0x58, 0x5e, 
       0x5, 0x10, 0x9, 0xb, 0x59, 0x5a, 0x7, 0x1d, 0x2, 0x2, 0x5a, 0x5e, 
       0x5, 0x10, 0x9, 0xa, 0x5b, 0x5c, 0x7, 0x1a, 0x2, 0x2, 0x5c, 0x5e, 
       0x5, 0x10, 0x9, 0x9, 0x5d, 0x51, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x53, 
       0x3, 0x2, 0x2, 0x2, 0x5d, 0x57, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x59, 0x3, 
       0x2, 0x2, 0x2, 0x5d, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x74, 0x3, 0x2, 
       0x2, 0x2, 0x5f, 0x60, 0xc, 0x8, 0x2, 0x2, 0x60, 0x61, 0x7, 0x1b, 
       0x2, 0x2, 0x61, 0x73, 0x5, 0x10, 0x9, 0x9, 0x62, 0x63, 0xc, 0x7, 
       0x2, 0x2, 0x63, 0x64, 0x7, 0x1d, 0x2, 0x2, 0x64, 0x73, 0x5, 0x10, 
       0x9, 0x8, 0x65, 0x66, 0xc, 0x6, 0x2, 0x2, 0x66, 0x67, 0x7, 0x1e, 
       0x2, 0x2, 0x67, 0x73, 0x5, 0x10, 0x9, 0x7, 0x68, 0x69, 0xc, 0x5, 
       0x2, 0x2, 0x69, 0x6a, 0x5, 0x12, 0xa, 0x2, 0x6a, 0x6b, 0x5, 0x10, 
       0x9, 0x6, 0x6b, 0x73, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x6d, 0xc, 0x4, 0x2, 
       0x2, 0x6d, 0x6e, 0x7, 0x18, 0x2, 0x2, 0x6e, 0x73, 0x5, 0x10, 0x9, 
       0x5, 0x6f, 0x70, 0xc, 0x3, 0x2, 0x2, 0x70, 0x71, 0x7, 0x19, 0x2, 
       0x2, 0x71, 0x73, 0x5, 0x10, 0x9, 0x4, 0x72, 0x5f, 0x3, 0x2, 0x2, 
       0x2, 0x72, 0x62, 0x3, 0x2, 0x2, 0x2, 0x72, 0x65, 0x3, 0x2, 0x2, 0x2, 
       0x72, 0x68, 0x3, 0x2, 0x2, 0x2, 0x72, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x72, 
       0x6f, 0x3, 0x2, 0x2, 0x2, 0x73, 0x76, 0x3, 0x2, 0x2, 0x2, 0x74, 0x72, 
       0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 0x3, 0x2, 0x2, 0x2, 0x75, 0x11, 0x3, 
       0x2, 0x2, 0x2, 0x76, 0x74, 0x3, 0x2, 0x2, 0x2, 0x77, 0x78, 0x9, 0x3, 
       0x2, 0x2, 0x78, 0x13, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7a, 0x9, 0x4, 0x2, 
       0x2, 0x7a, 0x15, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x87, 0x5, 0x1a, 0xe, 
       0x2, 0x7c, 0x87, 0x5, 0x1c, 0xf, 0x2, 0x7d, 0x87, 0x5, 0x1e, 0x10, 
       0x2, 0x7e, 0x87, 0x5, 0x24, 0x13, 0x2, 0x7f, 0x87, 0x5, 0x20, 0x11, 
       0x2, 0x80, 0x87, 0x5, 0x22, 0x12, 0x2, 0x81, 0x87, 0x5, 0x26, 0x14, 
       0x2, 0x82, 0x87, 0x5, 0xc, 0x7, 0x2, 0x83, 0x87, 0x5, 0x2a, 0x16, 
       0x2, 0x84, 0x87, 0x5, 0x28, 0x15, 0x2, 0x85, 0x87, 0x5, 0x18, 0xd, 
       0x2, 0x86, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x86, 0x7c, 0x3, 0x2, 0x2, 0x2, 
       0x86, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x86, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x86, 
       0x7f, 0x3, 0x2, 0x2, 0x2, 0x86, 0x80, 0x3, 0x2, 0x2, 0x2, 0x86, 0x81, 
       0x3, 0x2, 0x2, 0x2, 0x86, 0x82, 0x3, 0x2, 0x2, 0x2, 0x86, 0x83, 0x3, 
       0x2, 0x2, 0x2, 0x86, 0x84, 0x3, 0x2, 0x2, 0x2, 0x86, 0x85, 0x3, 0x2, 
       0x2, 0x2, 0x87, 0x17, 0x3, 0x2, 0x2, 0x2, 0x88, 0x8c, 0x7, 0x23, 
       0x2, 0x2, 0x89, 0x8b, 0x5, 0x16, 0xc, 0x2, 0x8a, 0x89, 0x3, 0x2, 
       0x2, 0x2, 0x8b, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8a, 0x3, 0x2, 0x2, 
       0x2, 0x8c, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8f, 0x3, 0x2, 0x2, 0x2, 
       0x8e, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x90, 0x7, 0x24, 0x2, 0x2, 
       0x90, 0x19, 0x3, 0x2, 0x2, 0x2, 0x91, 0x92, 0x5, 0x6, 0x4, 0x2, 0x92, 
       0x93, 0x7, 0x22, 0x2, 0x2, 0x93, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x94, 
       0x95, 0x7, 0xe, 0x2, 0x2, 0x95, 0x96, 0x7, 0x10, 0x2, 0x2, 0x96, 
       0x97, 0x7, 0x22, 0x2, 0x2, 0x97, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x98, 
       0x99, 0x7, 0xe, 0x2, 0x2, 0x99, 0x9a, 0x7, 0x11, 0x2, 0x2, 0x9a, 
       0x9b, 0x7, 0x22, 0x2, 0x2, 0x9b, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x9c, 
       0x9d, 0x7, 0x8, 0x2, 0x2, 0x9d, 0x9e, 0x7, 0x25, 0x2, 0x2, 0x9e, 
       0x9f, 0x5, 0x10, 0x9, 0x2, 0x9f, 0xa0, 0x7, 0x26, 0x2, 0x2, 0xa0, 
       0xa1, 0x7, 0x22, 0x2, 0x2, 0xa1, 0x21, 0x3, 0x2, 0x2, 0x2, 0xa2, 
       0xa3, 0x7, 0x9, 0x2, 0x2, 0xa3, 0xa4, 0x7, 0x25, 0x2, 0x2, 0xa4, 
       0xa5, 0x5, 0x10, 0x9, 0x2, 0xa5, 0xa6, 0x7, 0x26, 0x2, 0x2, 0xa6, 
       0xa7, 0x7, 0x22, 0x2, 0x2, 0xa7, 0x23, 0x3, 0x2, 0x2, 0x2, 0xa8, 
       0xa9, 0x7, 0x7, 0x2, 0x2, 0xa9, 0xaa, 0x7, 0x25, 0x2, 0x2, 0xaa, 
       0xab, 0x5, 0x10, 0x9, 0x2, 0xab, 0xac, 0x7, 0x26, 0x2, 0x2, 0xac, 
       0xad, 0x5, 0x18, 0xd, 0x2, 0xad, 0x25, 0x3, 0x2, 0x2, 0x2, 0xae, 
       0xaf, 0x7, 0x5, 0x2, 0x2, 0xaf, 0xb0, 0x7, 0x25, 0x2, 0x2, 0xb0, 
       0xb1, 0x5, 0x10, 0x9, 0x2, 0xb1, 0xb2, 0x7, 0x26, 0x2, 0x2, 0xb2, 
       0xb5, 0x5, 0x16, 0xc, 0x2, 0xb3, 0xb4, 0x7, 0x6, 0x2, 0x2, 0xb4, 
       0xb6, 0x5, 0x16, 0xc, 0x2, 0xb5, 0xb3, 0x3, 0x2, 0x2, 0x2, 0xb5, 
       0xb6, 0x3, 0x2, 0x2, 0x2, 0xb6, 0x27, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb8, 
       0x7, 0xb, 0x2, 0x2, 0xb8, 0xb9, 0x7, 0x25, 0x2, 0x2, 0xb9, 0xba, 
       0x7, 0xf, 0x2, 0x2, 0xba, 0xbe, 0x7, 0x26, 0x2, 0x2, 0xbb, 0xbd, 
       0x5, 0x18, 0xd, 0x2, 0xbc, 0xbb, 0x3, 0x2, 0x2, 0x2, 0xbd, 0xc0, 
       0x3, 0x2, 0x2, 0x2, 0xbe, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xbf, 0x3, 
       0x2, 0x2, 0x2, 0xbf, 0x29, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xbe, 0x3, 0x2, 
       0x2, 0x2, 0xc1, 0xc2, 0x7, 0xa, 0x2, 0x2, 0xc2, 0xc3, 0x5, 0x18, 
       0xd, 0x2, 0xc3, 0x2b, 0x3, 0x2, 0x2, 0x2, 0xd, 0x2f, 0x3c, 0x43, 
       0x4f, 0x5d, 0x72, 0x74, 0x86, 0x8c, 0xb5, 0xbe, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

WeaverParser::Initializer WeaverParser::_init;

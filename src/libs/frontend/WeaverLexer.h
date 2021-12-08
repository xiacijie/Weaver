
// Generated from /home/xiacijie/Project/Weaver/src/grammar/WeaverLexer.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"


namespace weaver {


class  WeaverLexer : public antlr4::Lexer {
public:
  enum {
    INT = 1, BOOL = 2, IF = 3, ELSE = 4, WHILE = 5, ASSERT = 6, ASSUME = 7, 
    ATOMIC = 8, PARALLEL = 9, TRUE = 10, FALSE = 11, IDENTIFIER = 12, INTEGER = 13, 
    INC = 14, DEC = 15, EQ = 16, NEQ = 17, LTE = 18, GTE = 19, LT = 20, 
    GT = 21, AND = 22, OR = 23, NOT = 24, STAR = 25, Q_MARK = 26, PLUS = 27, 
    MINUS = 28, ASSIGN = 29, COLON = 30, COMA = 31, SEMICOLON = 32, LEFT_BRACE = 33, 
    RIGHT_BRACE = 34, LEFT_PAREN = 35, RIGHT_PAREN = 36, NEWLINE = 37, TAB = 38, 
    WHITE_SPACE = 39, COMMENT = 40, LINE_COMMENT = 41
  };

  explicit WeaverLexer(antlr4::CharStream *input);
  ~WeaverLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace weaver

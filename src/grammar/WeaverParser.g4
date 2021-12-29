parser grammar WeaverParser;

options {
	tokenVocab = WeaverLexer;
}

/**
 Parser Rules
 */
program: statement* EOF;

type: INT | BOOL;

assignment : IDENTIFIER ASSIGN expression;
singleVarDeclaration: IDENTIFIER | IDENTIFIER LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET | assignment;
varDeclarationBody: singleVarDeclaration (COMA singleVarDeclaration)*;
varDeclarationStatement: type varDeclarationBody SEMICOLON;

operand: MINUS INTEGER | INTEGER | boolean | IDENTIFIER;

expression:
            operand
            | LEFT_PAREN expression RIGHT_PAREN
            | MINUS expression
            | PLUS expression
            | NOT expression
            | expression STAR expression
            | expression PLUS expression
            | expression MINUS expression
            | expression comparisonOp expression
            | expression AND expression
            | expression OR expression
            ;

comparisonOp: EQ|NEQ|LTE|GTE|LT|GT;

boolean: TRUE | FALSE | Q_MARK;

statement: assignmentStatement
           | increaseStatement
           | decreaseStatement
           | whileStatement
           | assertStatement
           | assumeStatement
           | storeStatement
           | selectStatement
           | conditionalStatement 
           | varDeclarationStatement
           | atomicStatement
           | parallelStatement
           | block
           ;

block: LEFT_BRACE statement* RIGHT_BRACE;
assignmentStatement: assignment SEMICOLON;
increaseStatement: IDENTIFIER INC SEMICOLON;
decreaseStatement: IDENTIFIER DEC SEMICOLON;
assertStatement: ASSERT LEFT_PAREN expression RIGHT_PAREN SEMICOLON;
assumeStatement: ASSUME LEFT_PAREN expression RIGHT_PAREN SEMICOLON;
whileStatement: WHILE LEFT_PAREN expression RIGHT_PAREN block;
conditionalStatement: IF LEFT_PAREN expression RIGHT_PAREN statement (ELSE statement)?;
parallelStatement: PARALLEL LEFT_PAREN INTEGER RIGHT_PAREN block*;
atomicStatement: ATOMIC block;
storeStatement: IDENTIFIER LEFT_SQUARE_BRACKET INTEGER RIGHT_SQUARE_BRACKET ASSIGN expression;
selectStatement: IDENTIFIER LEFT_SQUARE_BRACKET INTEGER RIGHT_SQUARE_BRACKET;

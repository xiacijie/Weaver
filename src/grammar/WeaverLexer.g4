lexer grammar WeaverLexer;

/**
 Lexer Rules
 */


//keywords
INT: 'int';
BOOL: 'bool';

IF: 'if';
ELSE: 'else';
WHILE: 'while';
ASSERT: 'assert';
ASSUME: 'assume';
ATOMIC: 'atomic';
PARALLEL: 'parallel';

TRUE: 'true';
FALSE: 'false';

IDENTIFIER: ALPHA(ALPHA|DIGIT)*;
INTEGER: DIGIT+;

INC: '++';
DEC: '--';

EQ:'==';
NEQ:'!=';
LTE:'<=';
GTE: '>=';
LT: '<';
GT: '>';

AND: '&&';
OR: '||';
NOT: '!';

STAR: '*';
Q_MARK: '?';
PLUS:'+';
MINUS: '-';

ASSIGN: '=';
COLON: ':';
COMA: ',';
SEMICOLON: ';';
LEFT_BRACE: '{';
RIGHT_BRACE: '}';
LEFT_PAREN:'(';
RIGHT_PAREN: ')';
LEFT_SQUARE_BRACKET: '[';
RIGHT_SQUARE_BRACKET: ']';



NEWLINE: [\r\n]+ -> skip;
TAB: [\t]+ ->skip;
WHITE_SPACE: [ ]+ ->skip;
COMMENT
    : '/*' .*? '*/' -> skip
;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
;


fragment DIGIT: [0-9];
fragment ALPHA: [a-zA-Z_];

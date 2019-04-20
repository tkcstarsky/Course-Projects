lexer grammar C1Lexer;

tokens {
    Comma,
    SemiColon,
    Assign,
    LeftBracket,
    RightBracket,
    LeftBrace,
    RightBrace,
    LeftParen,
    RightParen,
    If,
    Else,
    While,
    Const,
    Equal,
    NonEqual,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,

    Int,
    Float,
    Void,

    Identifier,
    IntConst,
    FloatConst
}
Comma: ',';
SemiColon: ';';
Assign: '=';

LeftParen: '(';
RightParen: ')';
LeftBracket: '[';
RightBracket: ']';
LeftBrace: '{';
RightBrace: '}';

If: 'if';
Else: 'else';
While: 'while';
Const: 'const';
Equal: '==';
NonEqual: '!=';
Less: '<';
Greater: '>';
LessEqual: '<=';
GreaterEqual: '>=';

Plus: '+' ;
Minus: '-' ;
Multiply: '*' ;
Divide: '/' ;
Modulo: '%' ;

Int: 'int';
Float: 'float';
Void: 'void';

Identifier: ([a-z]|[A-Z]|'_')([a-z]|[A-Z]|'_'|[0-9])*;
IntConst: (('0X'|'0x')([1-9]|[a-fA-F])([0-9]|[a-fA-F])*)|
          ('0'[1-7]([0-7])*)|
          ([1-9]([0-9])*)|
          '0';
FloatConst: ([0-9]([0-9])*('.'[0-9]([0-9])*)?|[0-9]([0-9])*'.'|'.'[0-9]([0-9])*)(('e'|'E')('-'|'+')?[0-9]([0-9])*)?;

WhiteSpace: [ \t\r\n]+ -> skip;
Comment:  (('//'.*?(('\n'|'\r\n')|('\\'('\n'|'\r\n').*?('\n'|'\r\n'))))|
          ('/*'.*?'*/')|
          ('/\\'('\n'|'\r\n')'/'.*?('\n'|'\r\n')))  ->skip;

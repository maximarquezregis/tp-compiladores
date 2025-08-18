%{
#include <stdlib.h>
#include <stdio.h>
int yylex(void);
void yyerror(const char *s);
%}

%token INT VOID BOOL MAIN RETURN ID INT_VAL BOOL_VAL
%token AND OR NEG
%token IF WHILE ELSE DO

%right '='
%left '+' '-'
%left '*'
%left AND OR
%right UMINUS NEG

%%

program: INT M
       | VOID M
       ;

M: MAIN '(' ')' '{' P '}'
 ;

P: D ';' P
 | stmt ';' P
 |
 ;

D: INT ID
 | BOOL ID
 ;

stmt: ID '=' expr
    | expr
    | RETURN expr
    | RETURN
    ;

expr: INT_VAL
    | BOOL_VAL
    | ID
    | expr '+' expr
    | expr '*' expr {$$ = }
    | expr '-' expr
    | '-' expr %prec UMINUS
    | '(' expr ')'
    | expr AND expr
    | expr OR expr
    | NEG expr %prec NEG
    ;

%%

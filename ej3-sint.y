%{
#include <stdlib.h>
#include <stdio.h>
#include <ast.h>
int yylex(void);
void yyerror(const char *s);
%}

%token INT VOID BOOL MAIN RETURN ID INT_VAL BOOL_VAL
%token AND OR NEG

%right '='
%left OR
%left AND
%left '+' '-'
%left '*' '/'
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

stmt: ID '=' expr { $$ = new_binary_node(ASSIGN, $1, $3); }
    | expr        { $$ = $1; }
    | RETURN expr { $$ = new_unary_node(RETURN, $2); }
    | RETURN      { $$ = new_unary_node(RETURN, void); }
    ;

expr: expr AND expr { $$ = new_binary_node(AND, $1, $3); }
    | expr OR expr  { $$ = new_binary_node(OR, $1, $3); }
    | sum_expr      { $$ = $1; }
    ;

sum_expr: sum_expr '+' term { $$ = new_binary_node(ADDITION, $1, $3); }
        | sum_expr '-' term { $$ = new_binary_node(SUBTRACTION, $1, $3); }
        | term              { $$ = $1; }
        ;

term: term '*' factor { $$ = new_binary_node(MULTIPLICATION, $1, $3); }
    | term '/' factor { $$ = new_binary_node(DIVISION, $1, $3); }
    | factor          { $$ = $1; }
    ;

factor: '-' factor %prec UMINUS { $$ = new_unary_node(MINUS, $2); }
        | NEG factor %prec NEG  { $$ = new_unary_node(NEG, $2); }
        | INT_VAL               { $$ = new_leaf_node(INT, $1); }
        | BOOL_VAL              { $$ = new_leaf_node(BOOL, $1); }
        | ID                    { $$ = new_leaf_node(ID, $1); }
        | '(' expr ')'          { $$ = $2; }
        ;

%%

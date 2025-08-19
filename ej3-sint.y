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

expr: expr AND expr { $$ = new_and_node($1,$3); }
    | expr OR expr  { $$ = new_or_node($1,$3); }
    | sum_expr      { $$ = $1; }
    ;

sum_expr: sum_expr '+' term { $$ = new_addition_node($1,$3); }
        | sum_expr '-' term { $$ = new_subtraction_node($1,$3); }
        | term              { $$ = $1; }
        ;

term: term '*' factor { $$ = new_multiplication_node($1,$3); }
    | factor          { $$ = $1; }
    ;

factor: '-' factor %prec UMINUS { $$ = new_minus_node($2); }
        | NEG factor %prec NEG    { $$ = new_neg_node($2); }
        | INT_VAL                 { $$ = new_int_node($1); }
        | BOOL_VAL                { $$ = new_bool_node($1); }
        | ID                      { $$ = new_id_node($1); }
        | '(' expr ')'            { $$ = $2; }
        ;

%%

%{
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
int yylex(void);
void yyerror(const char *s);
%}

%union {
    int ival;
    char* sval;
    AST_NODE* node;
}

%token INT VOID BOOL MAIN RETURN ID INT_VAL BOOL_VAL
%token AND OR NEG

%type <node> program M P D stmt expr
%type <sval> ID
%type <ival> INT_VAL BOOL_VAL

%right '='
%left OR
%left AND
%left '+' '-'
%left '*' '/'
%right UMINUS NEG

%start program

%%

program
    : INT M  { returnInt = true; }
    | VOID M { returnInt = false; }
    ;

M
    : MAIN '(' ')' '{' P '}' { }
    ;

P
    : P D ';'    { add_sentence($2); }
    | P stmt ';' { add_sentence($2); }
    |
    ;

D
    : INT ID  {
        AST_NODE* id = new_leaf_node(NULL, TYPE_ID, $2);
        $$ = new_unary_node(NULL, OP_DECL_INT, id);
      }
    | BOOL ID {
        AST_NODE* id = new_leaf_node(NULL, TYPE_ID, $2);
        $$ = new_unary_node(NULL, OP_DECL_BOOL, id);
      }
    ;

stmt
    : ID '=' expr {
        AST_NODE* id = new_leaf_node(NULL, TYPE_ID, $1);
        $$ = new_binary_node(NULL, OP_ASSIGN, id, $3);
      }
    | expr        { $$ = $1; }
    | RETURN expr { $$ = new_unary_node(NULL, OP_RETURN, $2); }
    | RETURN      { $$ = new_unary_node(NULL, OP_RETURN, NULL); }
    ;

expr
    : INT_VAL               { $$ = new_leaf_node(NULL, TYPE_INT, &$1); }
    | BOOL_VAL              { $$ = new_leaf_node(NULL, TYPE_BOOL, &$1); }
    | ID                    { $$ = new_leaf_node(NULL, TYPE_ID, $1); }
    | expr '+' expr         { $$ = new_binary_node(NULL, OP_ADDITION, $1, $3); }
    | expr '*' expr         { $$ = new_binary_node(NULL, OP_MULTIPLICATION, $1, $3); }
    | expr '/' expr         { $$ = new_binary_node(NULL, OP_DIVISION, $1, $3); }
    | expr '-' expr         { $$ = new_binary_node(NULL, OP_SUBTRACTION, $1, $3); }
    | '-' expr %prec UMINUS { $$ = new_unary_node(NULL, OP_MINUS, $2); }
    | '(' expr ')'          { $$ = $2; }
    | expr AND expr         { $$ = new_binary_node(NULL, OP_AND, $1, $3); }
    | expr OR expr          { $$ = new_binary_node(NULL, OP_OR, $1, $3); }
    | NEG expr %prec NEG    { $$ = new_unary_node(NULL, OP_NEG, $2); }
    ;

%%

void yyerror(const char *s){
    fprintf(stderr,"-> ERROR sintactico: %s\n", s);
}
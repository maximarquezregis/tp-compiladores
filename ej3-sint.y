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

%type <ival> INT_VAL BOOL_VAL
%type <sval> ID
%type <node> INT VOID BOOL MAIN RETURN AND OR NEG program P D stmt expr

%right '='
%left OR
%left AND
%left '+' '-'
%left '*' '/'
%right UMINUS NEG

%start program

%%

program: INT M { new_unary_node(NULL, OP_DECL_INT, $2); }
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

stmt: ID '=' expr { $$.node = new_binary_node(OP_ASSIGN, $1.leaf, $3.node); }
    | expr        { $$ = $1; }
    | RETURN expr { $$.node = new_unary_node(OP_RETURN, $2.node); }
    | RETURN      { $$.node = new_unary_node(OP_RETURN, NULL); }
    ;

expr: INT_VAL               { $$.leaf = new_leaf_node(TYPE_INT, &$1.ival); }
    | BOOL_VAL              { $$.leaf = new_leaf_node(TYPE_BOOL, &$1.ival); }
    | ID                    { $$.leaf = new_leaf_node(TYPE_ID, $1.sval); }
    | expr '+' expr         { $$.node = new_binary_node(OP_ADDITION, $1.node, $3.node); }
    | expr '*' expr         { $$.node = new_binary_node(OP_MULTIPLICATION, $1.node, $3.node); }
    | expr '/' expr         { $$.node = new_binary_node(OP_DIVISION, $1.node, $3.node); }
    | expr '-' expr         { $$.node = new_binary_node(OP_SUBTRACTION, $1.node, $3.node); }
    | '-' expr %prec UMINUS { $$.node = new_unary_node(OP_MINUS, $2.node); }
    | '(' expr ')'          { $$ = $2; }
    | expr AND expr         { $$.node = new_binary_node(OP_AND, $1.node, $3.node); }
    | expr OR expr          { $$.node = new_binary_node(OP_OR, $1.node, $3.node); }
    | NEG expr %prec NEG    { $$.node = new_unary_node(OP_NEG, $2.node); }
    ;

%%

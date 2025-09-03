%{
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ast.h"
#include "pas.h"
#include "print_funcs.h"
#include "interpreter.h"
#include "ej3-sint.tab.h"
#include "error_handling.h"
int yyparse(void);
void yyerror(const char *s);
char *strdup(const char *s);
extern int yylineno;
%}

%option noyywrap
%option yylineno

letra [a-zA-Z]
digito [0-9]+

%%

"int"                  { return INT; }
"void"                 { return VOID; }
{digito}+              { yylval.ival = atoi(yytext); return INT_VAL; }
"true"                 { yylval.ival = 1; return BOOL_VAL; }
"false"                { yylval.ival = 0; return BOOL_VAL; }
"return"               { return RETURN; }
"bool"                 { return BOOL; }
"main"                 { return MAIN; }
{letra}({letra}|[0-9])* { yylval.sval = strdup(yytext); return ID; }
"&&"                   { return AND; }
"||"                   { return OR; }
"!"                    { return NEG; }
[+\-*/%=]              { return *yytext; }
[;(){}]                { return *yytext; }
[ \t\r\n]+             { }
.                      { lexical_error(yylineno, yytext); }

%%

int main(int argc,char *argv[]){
    ++argv; --argc;
    if (argc > 0) yyin = fopen(argv[0],"r");
    else yyin = stdin;
    yyparse();
    print_program_horizontal();
    extern int alreadyReturned;
    if (returnInt || returnBool) {
        printf("Returned value: %d\n", interpreter(head_ast));
    } else {
        interpreter(head_ast);
    }
    for (AST_ROOT* cur = head_ast; cur != NULL; cur = cur->next) {
       genCode(cur->sentence);
    }
    printCodeToFile("assembly/gen_assembly.txt");
    print_id_table();
    return 0;
}
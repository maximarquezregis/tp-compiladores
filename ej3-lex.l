%{
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ast.h"
#include "print_funcs.h"
#include "ej3-sint.tab.h"
int yyparse(void);
void yyerror(const char *s);
char *strdup(const char *s);
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
.                      { }

%%

int main(int argc,char *argv[]){
    ++argv; --argc;
    if (argc > 0) yyin = fopen(argv[0],"r");
    else yyin = stdin;
    yyparse();
    print_program_horizontal();
    print_id_table();
    for (AST_ROOT* cur = head_ast; cur != NULL; cur = cur->next) {
        genCode(cur->sentence);
    }
    printCodeToFile("file.txt");
    return 0;
}
%{
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "ej3-sint.tab.h"
int yyparse(void);
%}

%option noyywrap
%option yylineno

letra [a-zA-Z]
digito [0-9]+

%%

{digito}+               { yylval.ival = atoi(yytext); return INT_VAL; }
"true"                  { yylval.ival = 1; return BOOL_VAL; }
"false"                 { yylval.ival = 0; return BOOL_VAL; }
"return"                { return RETURN; }
"bool"                  { return BOOL; }
"main"                  { return MAIN; }
{letra}({letra}|[0-9])* { yylval.sval = strdup(yytext); return ID; }
"&&"                    { return AND; }
"||"                    { return OR; }
"!"                     { return NEG; }
[+\-*/%=]               { return *yytext; }
[;(){}]                 { return *yytext; }
[ \t\r\n]+              { }
.                       { }

%%

void yyerror(const char *s){
    fprintf(stderr,"-> ERROR sintactico en la linea %d: %s\n", yylineno, s);
}

int main(int argc,char *argv[]){
    ++argv; --argc;
    if (argc > 0) yyin = fopen(argv[0],"r");
    else yyin = stdin;
    yyparse();
    return 0;
}

%{
#include <stdlib.h>
#include <stdio.h>
#include "ej3-sint.tab.h"
int yyparse(void);
%}

%option noyywrap
%option yylineno

letra [a-zA-Z]
digito [0-9]+

%%

{digito}+               { return INT_VAL; }
"true"                  { return BOOL_VAL; }
"false"                 { return BOOL_VAL; }
"if"                    { return IF; }
"int"                   { return INT; }
"while"                 { return WHILE; }
"else"                  { return ELSE; }
"do"                    { return DO; }
"return"                { return RETURN; }
"bool"                  { return BOOL; }
"main"                  { return MAIN; }
"&&"                    { return AND; }
"||"                    { return OR; }
"!"                     { return NEG; }
[+*/%=]                 { return *yytext; }
[;(){}]                 { return *yytext; }
{letra}({letra}|[0-9])* { return ID; }
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

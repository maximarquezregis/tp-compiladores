#!/bin/bash

flex ej3-lex.l
bison -d ej3-sint.y
gcc lex.yy.c ej3-sint.tab.c -o parser

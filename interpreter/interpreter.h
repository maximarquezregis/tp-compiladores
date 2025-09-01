#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "id_table.h"

extern int alreadyReturned;

int interpreter(AST_NODE* tree);

#endif
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "id_table.h"

int interpreter(AST_NODE* tree);

#endif
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "id_table.h"

typedef enum {
	INT_TYPE,
	BOOL_TYPE,
} TYPE;

typedef struct {
    void *value;
    TYPE type;
} ReturnValueNode;

int interpreter(AST_NODE* tree);

#endif
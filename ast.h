#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

typedef enum {
    UNARY,
    BINARY,
} OPERATOR_ARITY;

typedef enum {
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_MINUS,
    OP_AND,
    OP_OR,
    OP_NEG,
    OP_ASSIGN,
    OP_RETURN,
    OP_DECL_INT,
	OP_DECL_BOOL
} OPERATOR;

typedef enum {
    TYPE_INT,
    TYPE_BOOL,
    TYPE_ID
} LEAF_TYPE;

typedef struct AST_ROOT {
    AST_ROOT *next;
    AST_NODE *sentence;
    int amount;
    // flag used for checking if the program should return an integer or void
    // assuming there's no other types main can return
    int returnInt? = true;
}

typedef struct AST_NODE {
    struct AST_NODE *father;
    OPERATOR_ARITY arity;
    OPERATOR op;
    struct AST_NODE *left;
    // for unary nodes, use only the left child
    struct AST_NODE *right;
} AST_NODE;

typedef struct AST_LEAF {
    struct AST_NODE *father;
    LEAF_TYPE type;
    void *value;
} AST_LEAF;

AST_NODE* new_unary_node(AST_NODE* fath, OPERATOR opt, AST_NODE* left);
AST_NODE* new_binary_node(AST_NODE* fath, OPERATOR opt, AST_NODE* left, AST_NODE* right);
AST_LEAF* new_leaf_node(AST_NODE* fath, LEAF_TYPE type, void* v);

#endif //AST_H
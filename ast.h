#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef enum {
    UNARY,
    BINARY,
} OPERATOR_ARITY;

typedef enum {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    MINUS,
    AND,
    OR,
    NEG,
    ASSIGN,
    RETURN
} OPERATOR;

typedef enum {
    INT,
    BOOL,
    ID
} LEAF_TYPE;

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

AST_NODE* new_unary_node(OPERATOR opt, AST_NODE* n){
}

AST_NODE* new_binary_node(OPERATOR opt, AST_NODE* left, AST_NODE* right ){
}
#endif //AST_H
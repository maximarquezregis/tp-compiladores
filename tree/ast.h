#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "id_table.h"

#define true 1
#define false 0

struct ID_TABLE;

typedef struct AST_NODE AST_NODE;
typedef struct AST_ROOT AST_ROOT;
typedef struct INT_LEAF INT_LEAF;
typedef struct BOOL_LEAF BOOL_LEAF;

// flag used for checking if the program should return an integer or void
// assuming there's no other types main can return
extern int returnInt;
extern int returnBool;
extern AST_ROOT *head_ast;
extern AST_ROOT *end_ast;

typedef enum {
    UNARY,
    BINARY
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
    OP_DECL_BOOL,
} OPERATOR;

typedef enum {
	TYPE_INT,
	TYPE_BOOL,
	TYPE_ID
} LEAF_TYPE;

struct INT_LEAF {
    LEAF_TYPE type;
    int value;
};

struct BOOL_LEAF {
    LEAF_TYPE type;
    int value;
};

union LEAF {
    INT_LEAF int_leaf;
    BOOL_LEAF bool_leaf;
    ID_TABLE* id_leaf;
};

struct AST_NODE {
    struct AST_NODE *father;
    OPERATOR_ARITY arity;
	// undefined if is_leaf is true and not a decl/return/etc
    OPERATOR op;
    struct AST_NODE *left;
    struct AST_NODE *right;
    // flag for checking if the NODE is a leaf
    int is_leaf;
    // this fields will remain NULL if is_leaf is false
    LEAF_TYPE leaf_type;
    union LEAF *value;
    // source line where node was created
    int line;
};

struct AST_ROOT {
    AST_NODE *sentence;
    AST_ROOT *next;
};

AST_NODE* new_unary_node(OPERATOR opt, AST_NODE* left);
AST_NODE* new_binary_node(OPERATOR opt, AST_NODE* left, AST_NODE* right);
AST_NODE* new_leaf_node(LEAF_TYPE type, void* v);
void create_root(AST_NODE* tree);
void add_sentence(AST_NODE* tree);
void print_program(void);
void print_node(AST_NODE *node, const char *prefix, int is_last);

#endif
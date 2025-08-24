#include "ast.h"

int returnInt;
AST_ROOT *head = NULL;
AST_ROOT *end = NULL;

// function that allocates memory for each new node and initializes its fields by default
static AST_NODE* alloc_node(void) {
    AST_NODE* n = (AST_NODE*) malloc(sizeof(AST_NODE));
    n->father = NULL;
    n->left = n->right = NULL;
    n->is_leaf = 0;
    n->value = NULL;
    return n;
}

AST_NODE* new_binary_node(AST_NODE* fath, OPERATOR opt, AST_NODE* left, AST_NODE* right) {
    AST_NODE* n = alloc_node();
    n->arity = BINARY;
    n->op = opt;
    n->left = left;
    n->right = right;
    if (left) left->father = n;
    if (right) right->father = n;
    n->father = fath;
    return n;
}

AST_NODE* new_unary_node(AST_NODE* fath, OPERATOR opt, AST_NODE* left) {
    AST_NODE* n = alloc_node();
    n->arity = UNARY;
    n->op = opt;
    n->left = left;
    if (left) left->father = n;
    n->father = fath;
    return n;
}

AST_NODE* new_leaf_node(AST_NODE* fath, LEAF_TYPE type, void* v) {
    AST_NODE* n = alloc_node();
    n->arity = UNARY;
    n->is_leaf = 1;
    n->leaf_type = type;
    switch (type) {
        case TYPE_INT:
        case TYPE_BOOL:
            n->value = malloc(sizeof(int));
            memcpy(n->value, v, sizeof(int));
            break;
        case TYPE_ID:
            n->value = strdup((char*)v);
            break;
    }
    n->father = fath;
    return n;
}

// creates the root list
void create_root(AST_NODE* tree) {
    head = (AST_ROOT*) malloc(sizeof(AST_ROOT));
    head->sentence = tree;
    head->next = NULL;
    end = head;
}

// add new node in root list
void add_sentence(AST_NODE* tree) {
    /* if there is no root list yet, create one
       this works like a singleton pattern, only create one root list */
    if (head == NULL) {
        create_root(tree);
        return;
    } else {
        AST_ROOT *aux = malloc(sizeof(AST_ROOT));
        aux->sentence = tree;
        aux->next = NULL;
        end->next = aux;
        end = aux;
        return;
    }
}
#include "ast.h"
#include <stdbool.h>

int returnInt;
AST_ROOT *head_ast = NULL;
AST_ROOT *end_ast = NULL;

// function that allocates memory for each new node and initializes its fields by default
static AST_NODE* alloc_node(void) {
    AST_NODE* node = (AST_NODE*) malloc(sizeof(AST_NODE));
    node->father = NULL;
    node->left = node->right = NULL;
    node->is_leaf = false;
    node->value = NULL;
    return node;
}

AST_NODE* new_binary_node(OPERATOR opt, AST_NODE* left, AST_NODE* right) {
    AST_NODE* node = alloc_node();
    node->arity = BINARY;
    node->op = opt;
    node->left = left;
    node->right = right;
    if (left) left->father = node;
    if (right) right->father = node;
    return node;
}

AST_NODE* new_unary_node(OPERATOR opt, AST_NODE* left) {
    AST_NODE* node = alloc_node();
    node->arity = UNARY;
    node->op = opt;
    node->left = left;
	// for unary nodes we always use the left child
    if (left) left->father = node;
    return node;
}

AST_NODE* new_leaf_node(LEAF_TYPE type, void* v) {
    AST_NODE* node = alloc_node();
    node->arity = UNARY;
    node->is_leaf = true;
    node->leaf_type = type;

    switch (type) {
        case TYPE_INT: {
			// allocate memory for the union INT_LEAF and set its fields
            node->value = malloc(sizeof(union LEAF));
            node->value->int_leaf.type = type;
            node->value->int_leaf.value = *(int*)v;
            break;
        }
        case TYPE_BOOL: {
			// allocate memory for the BOOL_LEAF and set its fields
            node->value = malloc(sizeof(union LEAF));
            node->value->bool_leaf.type = TYPE_BOOL;
            node->value->bool_leaf.value = (*(int*)v) ? true : false;
            break;
        }
        case TYPE_ID: {
			// finds the memory direction of the node in the symbols' table that contains id
			ID_TABLE* aux = find((char*) v);
			if (aux == NULL) {
				fprintf(stderr, "ERROR: variable '%s' not declared\n", (char*) v);
        		exit(EXIT_FAILURE);
			} else {
				node->value = malloc(sizeof(union LEAF));
                node->value->id_leaf = aux;
			}
            break;
        }
    }
    return node;
}

// creates the root list
void create_root(AST_NODE* tree) {
    head_ast = (AST_ROOT*) malloc(sizeof(AST_ROOT));
    head_ast->sentence = tree;
    head_ast->next = NULL;
    end_ast = head_ast;
}

// add new node in root list
void add_sentence(AST_NODE* tree) {
    /* if there is no root list yet, create one
       this works like a singleton pattern, only create one root list */
    if (head_ast == NULL) {
        create_root(tree);
        return;
    } else {
        AST_ROOT *aux = malloc(sizeof(AST_ROOT));
        aux->sentence = tree;
        aux->next = NULL;
        end_ast->next = aux;
        end_ast = aux;
        return;
    }
}

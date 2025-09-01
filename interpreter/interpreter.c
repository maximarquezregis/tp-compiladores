#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

/*
 * Recursively evaluates an AST node and returns its integer value.
 * Booleans are represented as 0 (false) or 1 (true).
 */
static int eval(AST_NODE *tree) {
    if (!tree) {
        fprintf(stderr, "ERROR: NULL node in eval()\n");
        exit(EXIT_FAILURE);
    }

    if (tree->is_leaf) {
        switch (tree->leaf_type) {
            case TYPE_INT:
                return tree->value->int_leaf.value;
            case TYPE_BOOL:
                return tree->value->bool_leaf.value ? 1 : 0;
            case TYPE_ID: {
                ID_TABLE *id = tree->value->id_leaf;
                if (!id) {
                    fprintf(stderr, "ERROR: nonexistent identifier (NULL)\n");
                    exit(EXIT_FAILURE);
                }
                if (id->data == NULL) {
                    fprintf(stderr, "ERROR: variable '%s' used before initialization\n", id->id_name);
                    exit(EXIT_FAILURE);
                }
                return *(int*)id->data;
            }
        }
        fprintf(stderr, "ERROR: unknown leaf type\n");
        exit(EXIT_FAILURE);
    }

    switch (tree->op) {
        case OP_ADDITION:
            return eval(tree->left) + eval(tree->right);
        case OP_SUBTRACTION:
            return eval(tree->left) - eval(tree->right);
        case OP_MULTIPLICATION:
            return eval(tree->left) * eval(tree->right);
        case OP_DIVISION: {
            int denom = eval(tree->right);
            if (denom == 0) {
                fprintf(stderr, "ERROR: division by zero\n");
                exit(EXIT_FAILURE);
            }
            return eval(tree->left) / denom;
        }
        case OP_MINUS:
            return -eval(tree->left);
        case OP_AND:
            return (eval(tree->left) && eval(tree->right)) ? 1 : 0;
        case OP_OR:
            return (eval(tree->left) || eval(tree->right)) ? 1 : 0;
        case OP_NEG:
            return !eval(tree->left);
        case OP_ASSIGN: {
            // Left must be a TYPE_ID leaf
            if (!tree->left || !tree->left->is_leaf || tree->left->leaf_type != TYPE_ID) {
                fprintf(stderr, "ERROR: invalid left-hand side of assignment\n");
                exit(EXIT_FAILURE);
            }
            ID_TABLE *id = tree->left->value->id_leaf;
            int value = eval(tree->right);
            switch (id->id_type) {
                case CONST_INT: {
                    add_data(id->id_name, CONST_INT, &value);
                    break;
                }
                case CONST_BOOL: {
                    int b = value ? 1 : 0;
                    add_data(id->id_name, CONST_BOOL, &b);
                    value = b;
                    break;
                }
                case UNKNOWN:
                    fprintf(stderr, "ERROR: assignment to unknown identifier '%s'\n", id->id_name);
                    exit(EXIT_FAILURE);
            }
            return value;
        }
        case OP_DECL_INT:
        case OP_DECL_BOOL:
            // Declaration already recorded in symbol table by parser; do not evaluate identifier
            return 0;
        case OP_RETURN: {
            if (!tree->left) {
                // return without expression
                return 0;
            }
            int return_value = eval(tree->left);
            printf("%d \n", return_value);
            return return_value;
        }
   }

    fprintf(stderr, "ERROR: unknown operator in interpreter\n");
    exit(EXIT_FAILURE);
}

/* Public function: interprets (evaluates) a tree */
int interpreter(AST_NODE *tree) {
    return eval(tree);
}

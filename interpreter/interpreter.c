#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

int alreadyReturned = 0;

/*
 * Recursively evaluates an AST node and stores its type and value in ‘ret’.
 * Booleans are represented as 0 (false) or 1 (true).
 * Performs type checking on every operation and variable.
 */
static void eval(AST_NODE *tree, ReturnValueNode *ret) {
    if (!tree) {
        fprintf(stderr, "ERROR(line %d): NULL node in eval()\n", -1);
        exit(EXIT_FAILURE);
    }
    int line = tree->line;
    if (tree->is_leaf) {
        switch (tree->leaf_type) {
            case TYPE_INT:
                ret->type = INT_TYPE;
                ret->value = malloc(sizeof(int));
                *(int*)ret->value = tree->value->int_leaf.value;
                return;
            case TYPE_BOOL:
                ret->type = BOOL_TYPE;
                ret->value = malloc(sizeof(int));
                *(int*)ret->value = tree->value->bool_leaf.value;
                return;
            case TYPE_ID: {
                ID_TABLE *id = tree->value->id_leaf;
                if (!id) {
                    fprintf(stderr, "ERROR(line %d): nonexistent identifier (NULL)\n", line);
                    exit(EXIT_FAILURE);
                }
                if (id->data == NULL) {
                    fprintf(stderr, "ERROR(line %d): variable '%s' used before initialization\n", line, id->id_name);
                    exit(EXIT_FAILURE);
                }
                if (id->id_type == CONST_INT) {
                    ret->type = INT_TYPE;
                } else if (id->id_type == CONST_BOOL) {
                    ret->type = BOOL_TYPE;
                } else {
                    fprintf(stderr, "ERROR: identifier '%s' has unknown type\n", id->id_name);
                    exit(EXIT_FAILURE);
                }
                ret->value = malloc(sizeof(int));
                *(int*)ret->value = *(int*)id->data;
                return;
            }
        }
        fprintf(stderr, "ERROR(line %d): unknown leaf type\n", line);
        exit(EXIT_FAILURE);
    }
    ReturnValueNode left;
    ReturnValueNode right;
    switch (tree->op) {
        case OP_ADDITION:
            eval(tree->left, &left);
            eval(tree->right, &right);
            if (left.type != INT_TYPE || right.type != INT_TYPE) {
                fprintf(stderr, "ERROR: addition is only for integers\n");
                exit(EXIT_FAILURE);
            }
            ret->type = INT_TYPE;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = (*(int*)left.value) + (*(int*)right.value); 
            free(left.value);
            free(right.value);
            return;
        case OP_SUBTRACTION:
            eval(tree->left, &left);
            eval(tree->right, &right);
            if (left.type != INT_TYPE || right.type != INT_TYPE) {
                fprintf(stderr, "ERROR: substraction is only for integers\n");
                exit(EXIT_FAILURE);
            }
            ret->type = INT_TYPE;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = (*(int*)left.value) - (*(int*)right.value);
            free(left.value);
            free(right.value);
            return;
        case OP_MULTIPLICATION:
            eval(tree->left, &left);
            eval(tree->right, &right);
            if (left.type != INT_TYPE || right.type != INT_TYPE) {
                fprintf(stderr, "ERROR: multiplication is only for integers\n");
                exit(EXIT_FAILURE);
            }
            ret->type = INT_TYPE;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = (*(int*)left.value) * (*(int*)right.value);
            free(left.value);
            free(right.value);
            return;
        case OP_DIVISION: {
            eval(tree->left, &left);
            eval(tree->right, &right);
            if (left.type != INT_TYPE || right.type != INT_TYPE) {
                fprintf(stderr, "ERROR: divison is only for integers\n");
                exit(EXIT_FAILURE);
            }
            ret->type = INT_TYPE;
            int denom = (*(int*)right.value);
            if (denom == 0) {
                fprintf(stderr, "ERROR(line %d): division by zero\n", line);
                exit(EXIT_FAILURE);
            }
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = (*(int*)left.value) / (*(int*)right.value);
            free(left.value);
            free(right.value);
            return;
        }
        case OP_MINUS:
            eval(tree->left, &left);
            if (left.type != INT_TYPE) {
                fprintf(stderr, "ERROR: minus is only for integers\n");
                exit(EXIT_FAILURE);
            }
            ret->type = INT_TYPE;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = - (*(int*)left.value);
            free(left.value);
            return;
        case OP_AND:
            eval(tree->left, &left);
            eval(tree->right, &right);
            if (left.type != BOOL_TYPE || right.type != BOOL_TYPE) {
                fprintf(stderr, "ERROR: AND is only for booleans \n");
                exit(EXIT_FAILURE);
            }
            ret->type = BOOL_TYPE;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = (*(int*)left.value) && (*(int*)right.value) ? 1 : 0;
            free(left.value);
            free(right.value);
            return;
        case OP_OR:
            eval(tree->left, &left);
            eval(tree->right, &right);
            if (left.type != BOOL_TYPE || right.type != BOOL_TYPE) {
                fprintf(stderr, "ERROR: OR is only for booleans \n");
                exit(EXIT_FAILURE);
            }
            ret->type = BOOL_TYPE;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = (*(int*)left.value) || (*(int*)right.value) ? 1 : 0;
            free(left.value);
            free(right.value);
            return;        
        case OP_NEG:
            eval(tree->left, &left);
            if (left.type != BOOL_TYPE) {
                fprintf(stderr, "ERROR: NEG is only for booleans \n");
                exit(EXIT_FAILURE);
            }
            ret->type = BOOL_TYPE;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = !(*(int*)left.value) ? 1 : 0;
            free(left.value);
            return;
        case OP_ASSIGN: {
            // Left must be a TYPE_ID leaf
            if (!tree->left || !tree->left->is_leaf || tree->left->leaf_type != TYPE_ID) {
                fprintf(stderr, "ERROR(line %d): invalid left-hand side of assignment\n", line);
                exit(EXIT_FAILURE);
            }
            ID_TABLE *id = tree->left->value->id_leaf;

            eval(tree->right, &right);

            if ((id->id_type == CONST_INT && right.type != INT_TYPE) ||
                (id->id_type == CONST_BOOL && right.type != BOOL_TYPE)) {
                fprintf(stderr, "ERROR: type mismatch in assignment to variable '%s'\n", id->id_name);
                exit(EXIT_FAILURE);
            }

            ret->type = right.type;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = (*(int*)right.value);
            switch (id->id_type) {
                case CONST_INT: {
                    add_data(id->id_name, CONST_INT, ret->value);
                    break;
                }
                case CONST_BOOL: {
                    int b = (*(int*)ret->value) ? 1 : 0;
                    add_data(id->id_name, CONST_BOOL, &b);
                    break;
                }
                case UNKNOWN:
                    fprintf(stderr, "ERROR(line %d): assignment to unknown identifier '%s'\n", line, id->id_name);
                    exit(EXIT_FAILURE);
            }
            free(right.value);
            return;
        }
        case OP_DECL_INT:
        case OP_DECL_BOOL:
            // Declaration already recorded in symbol table by parser; do not evaluate identifier
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = 0;
            ret->type = (tree->op == OP_DECL_INT) ? INT_TYPE : BOOL_TYPE;
            return;
        case OP_RETURN: {
            if (!alreadyReturned) {
                if (!tree->left && returnInt) {
                    fprintf(stderr, "ERROR(line %d): main returns void when it should return int\n", line);
                    exit(EXIT_FAILURE);
                } else if (!returnInt && tree->left) {
                    fprintf(stderr, "ERROR(line %d): main returns int when it should return void\n", line);
                    exit(EXIT_FAILURE);
                } else if (tree->left) {
                    eval(tree->left, &left);
                    if (returnInt && left.type != INT_TYPE) {
                        fprintf(stderr, "ERROR(line %d): main should return int\n", line);
                        exit(EXIT_FAILURE);
                    }
                    ret->type = left.type;
                    ret->value = malloc(sizeof(int));
                    *(int*)ret->value = (*(int*)left.value);
                    int return_value = *(int*)ret->value;
                    alreadyReturned = 1;
                    free(left.value);
                    return;
                } else {
                    if (!returnInt) {
                        alreadyReturned = 1;
                        return;
                    }
                }
            } else {
                fprintf(stderr, "WARNING(line %d): return statement ignored, already returned once\n", line);
                return;
            }
        }
    }

    fprintf(stderr, "ERROR(line %d): unknown operator in interpreter\n", line);
    exit(EXIT_FAILURE);
}

/* Public function: interprets (evaluates) a tree */
int interpreter(AST_NODE *tree) {
    ReturnValueNode ret;
    eval(tree, &ret);
    return *(int*)ret.value;
}

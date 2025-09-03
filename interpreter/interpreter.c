#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "error_handling.h"

int alreadyReturned = 0;

/*
 * Recursively evaluates an AST node and stores its type and value in ‘ret’.
 * Booleans are represented as 0 (false) or 1 (true).
 * Performs type checking on every operation and variable.
 */
static void eval(AST_NODE *tree, ReturnValueNode *ret) {
    int line = tree->line;
    if (alreadyReturned){
        warning_already_returned(line);
    }
    if (!tree) {
        error_null_node(-1);
    }
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
                    error_noexistent_id(line);
                }
                if (id->data == NULL) {
                    error_variable_used_before_init(line, id->id_name);
                }
                if (id->id_type == CONST_INT) {
                    ret->type = INT_TYPE;
                } else if (id->id_type == CONST_BOOL) {
                    ret->type = BOOL_TYPE;
                } else {
                    error_id_unknown_type(line, id->id_name);
                }
                ret->value = malloc(sizeof(int));
                *(int*)ret->value = *(int*)id->data;
                return;
            }
        }
        error_unknown_leaf_type(line);
    }
    ReturnValueNode left;
    ReturnValueNode right;
    switch (tree->op) {
        case OP_ADDITION:
            eval(tree->left, &left);
            eval(tree->right, &right);
            if (left.type != INT_TYPE || right.type != INT_TYPE) {
                additional_error(line);
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
                substraction_error(line);
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
                multiplication_error(line);
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
                division_error(line);
            }
            ret->type = INT_TYPE;
            int denom = (*(int*)right.value);
            if (denom == 0) {
                division_zero_error(line);
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
               minus_error(line); 
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
                and_error(line);
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
                or_error(line);
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
                neg_error(line);
            }
            ret->type = BOOL_TYPE;
            ret->value = malloc(sizeof(int));
            *(int*)ret->value = !(*(int*)left.value) ? 1 : 0;
            free(left.value);
            return;
        case OP_ASSIGN: {
            // Left must be a TYPE_ID leaf
            if (!tree->left || !tree->left->is_leaf || tree->left->leaf_type != TYPE_ID) {
                assign_error(line);
            }
            ID_TABLE *id = tree->left->value->id_leaf;

            eval(tree->right, &right);

            if ((id->id_type == CONST_INT && right.type != INT_TYPE) ||
                (id->id_type == CONST_BOOL && right.type != BOOL_TYPE)) {
                fprintf(stderr, "ERROR(line %d): type mismatch in assignment to variable '%s'\n", line, id->id_name);
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
            if (tree->left) {
                if (returnInt) {
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
                } else if (returnBool) {
                    eval(tree->left, &left);
                    if (left.type != BOOL_TYPE) {
                        fprintf(stderr, "ERROR(line %d): main should return bool\n", line);
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
                    fprintf(stderr, "ERROR(line %d): main should return void\n", line);
                    exit(EXIT_FAILURE);
                }
            } else {
                if (returnInt) {
                    fprintf(stderr, "ERROR(line %d): main returns void when it should return int\n", line);
                    exit(EXIT_FAILURE);
                } else if (returnBool) {
                    fprintf(stderr, "ERROR(line %d): main returns void when it should return bool\n", line);
                    exit(EXIT_FAILURE);
                } else {
                    alreadyReturned = 1;
                    return;
                }
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

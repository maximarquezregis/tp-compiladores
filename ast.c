#include "ast.h"
#include <stdbool.h>
#include <stdio.h>

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

// function that frees memory of node and its children
void free_mem(AST_NODE* node) {
	if (node->is_leaf) {
		free(node->value);
	} else if (node->arity == UNARY && !node->is_leaf) {
		free_mem(node->left);
	} else {
		free_mem(node->left);
		free_mem(node->right);
	}

	free(node);
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


// Buffer for save all the instructions (pseudo-assembly)
Instr code[1000];   // 1000 instructions for example
int code_size = 0;  // Number of instructions saved


// Function for save instructions in the buffer
void emit(InstrType t, const char* arg) {
    code[code_size].type = t;
    if(arg) strcpy(code[code_size].arg, arg);
    else code[code_size].arg[0] = '\0';     //If received NULL, save empty string
    code_size++;
}


// Generate the pseudo-assembly
void genCode(AST_NODE* node) {
    if (!node) return;

    if (node->is_leaf) {
		char buf[32];
        switch (node->leaf_type){
            case TYPE_INT:
                sprintf(buf, "%d", node->value->int_leaf.value);
                emit(I_LOADVAL, buf);
                break;
			case TYPE_BOOL:
                sprintf(buf, "%d", node->value->bool_leaf.value);
                emit(I_LOADVAL, buf);
                break;
			case TYPE_ID:
                emit(I_LOAD, node->value->id_leaf->id_name);
                break;
		}
	} else {
		switch (node->op){
            genCode(node->left);
            genCode(node->right);
            case OP_ADDITION:       emit(I_ADD, NULL); break;
            case OP_SUBTRACTION:    emit(I_SUB, NULL); break;
			case OP_MULTIPLICATION: emit(I_MUL, NULL); break;
			case OP_DIVISION:       emit(I_DIV, NULL); break;
			case OP_MINUS:          emit(I_MIN, NULL); break;
			case OP_RETURN:         emit(I_RET, NULL); break;
			case OP_DECL_INT:       emit(I_DECL, NULL); break;
			case OP_DECL_BOOL:      emit(I_DECL, NULL); break;
			case OP_AND:            emit(I_AND, NULL); break;
			case OP_OR:             emit(I_OR, NULL); break;
			case OP_NEG:            emit(I_NEG, NULL); break;
            case OP_ASSIGN:
                genCode(node->right);
                emit(I_STORE, node->left->value);
                break;
		}
	}
}

void printCodeToFile(const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Can't open the file provided");
        return;
    }

    for (int i = 0; i < code_size; i++) {
        switch (code[i].type) {
            case I_LOADVAL:
                fprintf(f, "LOADVAL %s\n", code[i].arg);
                break;
            case I_LOAD:
                fprintf(f, "LOAD %s\n", code[i].arg);
                break;
            case I_STORE:
                fprintf(f, "STORE %s\n", code[i].arg);
                break;
            case I_ADD:
                fprintf(f, "ADD\n");
                break;
            case I_SUB:
                fprintf(f, "SUB\n");
                break;
            case I_MUL:
                fprintf(f, "MUL\n");
                break;
            case I_DIV:
                fprintf(f, "DIV\n");
                break;
            case I_MIN:
                fprintf(f, "MINUS\n");
                break;
            case I_RET:
                fprintf(f, "RET\n");
                break;
            case I_AND:
                fprintf(f, "AND\n");
                break;
            case I_OR:
                fprintf(f, "OR\n");
                break;
            case I_NEG:
                fprintf(f, "NEG\n");
                break;
            default:
                fprintf(f, "UNKNOWN\n");
                break;
        }
    }

    fclose(f);
}
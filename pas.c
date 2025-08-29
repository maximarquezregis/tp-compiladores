#include "pas.h"
#include "ast.h"
#include <stdbool.h>
#include <stdio.h>

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
            case OP_ADDITION:
                genCode(node->left);
                genCode(node->right);
                emit(I_ADD, NULL);
                break;
            case OP_SUBTRACTION:
                genCode(node->left);
                genCode(node->right);
                emit(I_SUB, NULL);
                break;
			case OP_MULTIPLICATION:
                genCode(node->left);
                genCode(node->right);
                emit(I_MUL, NULL);
                break;
			case OP_DIVISION:
                genCode(node->left);
                genCode(node->right);
                emit(I_DIV, NULL);
                break;
			case OP_MINUS:
                genCode(node->left);
                emit(I_MIN, NULL);
                break;
			case OP_RETURN:
                if (node->left) genCode(node->left);
                emit(I_RET, NULL);
                break;
			case OP_DECL_INT:
                emit(I_DECL, node->left->value->id_leaf->id_name);
                break;
			case OP_DECL_BOOL:
                emit(I_DECL, node->left->value->id_leaf->id_name);
                break;
			case OP_AND:
                genCode(node->left);
                genCode(node->right);
                emit(I_AND, NULL);
                break;
			case OP_OR:
                genCode(node->left);
                genCode(node->right);
                emit(I_OR, NULL);
                break;
			case OP_NEG:
                genCode(node->left);
                emit(I_NEG, NULL);
                break;
            case OP_ASSIGN:
                genCode(node->right);
                emit(I_STORE, node->left->value->id_leaf->id_name);
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
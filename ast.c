#include "ast.h"
#include <stdbool.h>

int returnInt;
AST_ROOT *head = NULL;
AST_ROOT *end = NULL;

// function that allocates memory for each new node and initializes its fields by default
static AST_NODE* alloc_node(void) {
    AST_NODE* node = (AST_NODE*) malloc(sizeof(AST_NODE));
    node->father = NULL;
    node->left = node->right = NULL;
    node->is_leaf = false;
    node->value = NULL;
    return node;
}

AST_NODE* new_binary_node(AST_NODE* fath, OPERATOR opt, AST_NODE* left, AST_NODE* right) {
    AST_NODE* node = alloc_node();
    node->arity = BINARY;
    node->op = opt;
    node->left = left;
    node->right = right;
    if (left) left->father = node;
    if (right) right->father = node;
    node->father = fath;
    return node;
}

AST_NODE* new_unary_node(AST_NODE* fath, OPERATOR opt, AST_NODE* left) {
    AST_NODE* node = alloc_node();
    node->arity = UNARY;
    node->op = opt;
    node->left = left;
	// for unary nodes we always use the left child
    if (left) left->father = node;
    node->father = fath;
    return node;
}

AST_NODE* new_leaf_node(AST_NODE* fath, LEAF_TYPE type, void* v) {
    AST_NODE* node = alloc_node();
    node->arity = UNARY;
    node->is_leaf = true;
    node->leaf_type = type;
    switch (type) {
        case TYPE_INT:
        case TYPE_BOOL:
			// allocate memory for an integer or bool and copy the value
            node->value = malloc(sizeof(int));
            memcpy(node->value, v, sizeof(int));
            break;
        case TYPE_ID:
			// duplicate the string and store it in the node
			id_table =
            node->value = strdup((char*)v);
            break;
    }
    node->father = fath;
    return node;
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

/* function for interpreter */
int interpreter(AST_NODE* tree) {
    if (!tree) return 0;
    if (tree->is_leaf) {
        switch (tree->leaf_type){
            case TYPE_INT:      tree->value;
            case TYPE_BOOL:     tree->value;
            case TYPE_ID:       global_values[tree->value];

            default:          error("Interpreter: unknown tree type %d\n", tree->leaf_type);
        }
    } else {
        switch (tree->op){
            case OP_ADDITION:           return interpreter(tree->left) + interpreter(tree->right);
            case OP_SUBTRACTION:        return interpreter(tree->left) - interpreter(tree->right);
            case OP_MULTIPLICATION:     return interpreter(tree->left) * interpreter(tree->right);
            case OP_DIVISION:           return interpreter(tree->left) / interpreter(tree->right);
            case OP_MINUS:              return - interpreter(tree->left);
            case OP_RETURN:             return interpreter(tree->left) + interpreter(tree->right);
            case OP_DECL_INT:           return interpreter(tree->left) + interpreter(tree->right);
            case OP_DECL_BOOL:          return interpreter(tree->left) + interpreter(tree->right);
            case OP_AND:                return interpreter(tree->left) && interpreter(tree->right);
            case OP_OR:                 return interpreter(tree->left) || interpreter(tree->right);
            case OP_NEG:                return ! interpreter(tree->left);
            case OP_ASSIGN:             return global_values[tree->left->value] = interpreter(tree->right);

            default:          error("Interpreter: unknown tree type %d\n", tree->op);
        }
    }
    return 0;
}

/* funciones para imprimir el arbol */
static const char *op_to_string(OPERATOR op) {
    switch (op) {
        case OP_ADDITION:        return "+";
        case OP_SUBTRACTION:     return "-";
        case OP_MULTIPLICATION:  return "*";
        case OP_DIVISION:        return "/";
        case OP_MINUS:           return "UMINUS";
        case OP_AND:             return "&&";
        case OP_OR:              return "||";
        case OP_NEG:             return "!";
        case OP_ASSIGN:          return "=";
        case OP_RETURN:          return "return";
        case OP_DECL_INT:        return "decl int";
        case OP_DECL_BOOL:       return "decl bool";
        default:                 return "?";
    }
}

static void print_leaf(AST_NODE *n) {
    switch (n->leaf_type) {
        case TYPE_INT:
            printf("INT(%d)", *(int*)n->value);
            break;
        case TYPE_BOOL:
            printf("BOOL(%s)", (*(int*)n->value) ? "true" : "false");
            break;
        case TYPE_ID:
            printf("ID(%s)", (char*)n->value);
            break;
    }
}

void print_node(AST_NODE *node, const char *prefix, int is_last) {
    if (!node) return;

    // Dibujar el prefijo de la rama
    printf("%s", prefix);
    printf(is_last ? "└── " : "├── ");

    // Imprimir contenido del nodo
    if (node->is_leaf) {
        print_leaf(node);
    } else {
        printf("%s", op_to_string(node->op));
    }
    printf("\n");

    // Preparar nuevo prefijo
    char new_prefix[512];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");

    // Recolectar hijos (para binarios y unarios)
    AST_NODE *children[2];
    int count = 0;
    if (node->left)  children[count++] = node->left;
    if (node->arity == BINARY && node->right)
        children[count++] = node->right;

    for (int i = 0; i < count; ++i) {
        print_node(children[i], new_prefix, i == count - 1);
    }
}

void print_program(void) {
    printf("=== AST del programa ===\n");
    AST_ROOT *cur = head;
    int idx = 0;
    while (cur) {
        printf("Sentencia %d\n", idx++);
        print_node(cur->sentence, "", 1);
        cur = cur->next;
    }
    printf("========================\n");
}
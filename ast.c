#include "ast.h"

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
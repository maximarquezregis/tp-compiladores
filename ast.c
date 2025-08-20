#include "ast.h"

AST_NODE* new_binary_node(OPERATOR opt, AST_NODE* left, AST_NODE* right ){
    AST_NODE* node = (AST_NODE*) malloc(sizeof(AST_NODE));
    node->father = NULL;
    node->arity = BINARY;
    node->op = opt;
    node->left = left;
    node->right = right;
    return node;
}

AST_NODE* new_unary_node(OPERATOR opt, AST_NODE* n){
  AST_NODE* node = (AST_NODE*) malloc(sizeof(AST_NODE));
  node->father = NULL;
  node->arity = UNARY;
  node->op = opt;
  node->left = n;
  return node;
}

AST_LEAF* new_leaf_node(LEAF_TYPE type, void* v){
  AST_LEAF* node = (AST_LEAF*) malloc(sizeof(AST_LEAF));
  node->father = NULL;
  node->type = type;
  node->value = cast(type);

  switch (type){
    case INT:
      memcpy(node->value, v, sizeof(int));
      break;

    case ID:
      *(char**)node->value = strdup((char*)v);
      break;
  }

  return node;
}

void* cast(LEAF_TYPE t){
  switch (t)
  {
  case INT:
    return malloc(sizeof(int));
    break;

  case ID:
    return malloc(sizeof(char*));

    default:
    break;
  }
}
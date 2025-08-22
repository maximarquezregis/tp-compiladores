#include "ast.h"

AST_NODE* new_binary_node(AST_NODE* fath, OPERATOR opt, AST_NODE* left, AST_NODE* right ){
  AST_NODE* node = (AST_NODE*) malloc(sizeof(AST_NODE));
  node->father = fath;
  node->arity = BINARY;
  node->op = opt;
  node->left = left;
  node->right = right;
  return node;
}

AST_NODE* new_unary_node(AST_NODE* fath, OPERATOR opt, AST_NODE* lft){
  AST_NODE* node = (AST_NODE*) malloc(sizeof(AST_NODE));
  node->father = fath;
  node->arity = UNARY;
  node->op = opt;
  node->left = lft;
  return node;
}

AST_LEAF* new_leaf_node(AST_NODE* fath, LEAF_TYPE type, void* v){
  AST_LEAF* node = (AST_LEAF*) malloc(sizeof(AST_LEAF));
  node->father = fath;
  node->type = type;

  switch (type){
    case TYPE_INT:
      node->value = malloc(sizeof(int));
      memcpy(node->value, v, sizeof(int));
      break;

    case TYPE_BOOL:
      node->value = malloc(sizeof(int));
      memcpy(node->value, v, sizeof(int));
      break;

    case TYPE_ID:
      node->value = malloc(sizeof(char*));
      *(char**)node->value = strdup((char*)v);
      break;
  }

  return node;
}
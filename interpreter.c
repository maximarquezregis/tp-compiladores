/* function for interpreter */
int interpreter(AST_NODE* tree) {
	if (!tree) {
		fprintf(stderr, "ERROR: nothing to interpret \n");
		exit(EXIT_FAILURE);
	}
	if (tree->is_leaf) {
		switch (tree->leaf_type){
			case TYPE_INT: (int) tree->value->int_leaf.value;
			case TYPE_BOOL: (int) tree->value->bool_leaf.value;
			case TYPE_ID: () global_values[tree->value];

				// default:          error("Interpreter: unknown tree type %d\n", tree->leaf_type);
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
				//case OP_ASSIGN:             return global_values[tree->left->value] = interpreter(tree->right);

				// default:          error("Interpreter: unknown tree type %d\n", tree->op);
		}
	}
	return 0;
}
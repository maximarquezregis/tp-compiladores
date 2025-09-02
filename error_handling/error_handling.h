#ifndef ERROR_HANDLING
#define ERROR_HANDLING

void redeclaration_variable(int line, char* variable);
void variable_not_declared(int line, char* variable);
void type_mismatch(int linea, char* variable);
void allocate_mem_error();

#endif 
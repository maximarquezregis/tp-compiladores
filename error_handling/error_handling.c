#include "error_handling.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void redeclaration_variable(int line, char* variable){
    fprintf(stderr, "ERROR(line %d): variable '%s' redeclared\n", line, variable);
    exit(EXIT_FAILURE);
}

void variable_not_declared(int line, char* variable){
    fprintf(stderr, "ERROR(line %d): variable '%s' not declared\n", line, variable);
    exit(EXIT_FAILURE);
}

void type_mismatch(int line, char* variable){
    fprintf(stderr, "ERROR(line %d): type mismatch in assignment to variable '%s'\n", line, variable);
	exit(EXIT_FAILURE);
}

void allocate_mem_error(){
    perror("allocate_mem() error");
    exit(EXIT_FAILURE);
}
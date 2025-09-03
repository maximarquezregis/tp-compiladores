#include "error_handling.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void error_redeclaration_variable(int line, char* variable){
    fprintf(stderr, "ERROR(line %d): variable '%s' redeclared\n", line, variable);
    exit(EXIT_FAILURE);
}

void error_variable_not_declared(int line, char* variable){
    fprintf(stderr, "ERROR(line %d): variable '%s' not declared\n", line, variable);
    exit(EXIT_FAILURE);
}

void error_type_mismatch(int line, char* variable){
    fprintf(stderr, "ERROR(line %d): type mismatch in assignment to variable '%s'\n", line, variable);
	exit(EXIT_FAILURE);
}

void allocate_mem_error(){
    perror("allocate_mem() error");
    exit(EXIT_FAILURE);
}

void warning_already_returned(int line){
    fprintf(stderr, "WARNING(line %d): return statement ignored, already returned once\n", line);
    return;
}

void error_null_node(int line){
    fprintf(stderr, "ERROR(line %d): NULL node in eval()\n", line);
    exit(EXIT_FAILURE);
}

void error_noexistent_id(int line){
    fprintf(stderr, "ERROR(line %d): nonexistent identifier '%s'\n", line);
    exit(EXIT_FAILURE);
}

void error_variable_used_before_init(int line, char* variable){
    fprintf(stderr, "ERROR(line %d): variable '%s' used before initialization\n", line, variable);
    exit(EXIT_FAILURE);
}

void error_id_unknown_type(int line, char* variable){
    fprintf(stderr, "ERROR(line %d): identifier '%s' has unknown type\n", line, variable);
    exit(EXIT_FAILURE);
}

void error_unknown_leaf_type(int line){
    fprintf(stderr, "ERROR(line %d): unknown leaf type\n", line);
    exit(EXIT_FAILURE);
}

void additional_error(int line){
    fprintf(stderr, "ERROR(linea %d): addition is only for integers\n", line);
    exit(EXIT_FAILURE);
}

void substraction_error(int line){
    fprintf(stderr, "ERROR(line %d): substraction is only for integers\n", line);
    exit(EXIT_FAILURE);
}

void multiplication_error(int line){
    fprintf(stderr, "ERROR(line %d): multiplication is only for integers\n", line);
    exit(EXIT_FAILURE);
}

void division_error(int line){
    fprintf(stderr, "ERROR(line %d): divison is only for integers\n", line);
    exit(EXIT_FAILURE);
}

void division_zero_error(int line){
    fprintf(stderr, "ERROR(line %d): division by zero\n", line);
    exit(EXIT_FAILURE);
}

void minus_error(int line){
    fprintf(stderr, "ERROR(line %d): minus is only for integers\n", line);
    exit(EXIT_FAILURE);
}

void and_error(int line){
    fprintf(stderr, "ERROR(line %d): AND is only for booleans \n", line);
    exit(EXIT_FAILURE);
}

void or_error(int line){
    fprintf(stderr, "ERROR(line %d): OR is only for booleans \n", line);
    exit(EXIT_FAILURE);
}

void neg_error(int line){
    fprintf(stderr, "ERROR(line %d): NEG is only for booleans \n", line);
    exit(EXIT_FAILURE);
}

void assign_error(int line){
    fprintf(stderr, "ERROR(line %d): invalid left-hand side of assignment\n", line);
    exit(EXIT_FAILURE);
}

void error_return_int(int line){
    fprintf(stderr, "ERROR(line %d): main should return int\n", line);
    exit(EXIT_FAILURE);
}

void error_return_bool(int line){
    fprintf(stderr, "ERROR(line %d): main should return bool\n", line);
    exit(EXIT_FAILURE);
}

void error_return_void(int line){
    fprintf(stderr, "ERROR(line %d): main should return void\n", line);
    exit(EXIT_FAILURE);
}

void error_unespected_return(int line, char* type){
    if (type == "int"){
        fprintf(stderr, "ERROR(line %d): main returns void when it should return int\n", line);
    } else {
        fprintf(stderr, "ERROR(line %d): main returns void when it should return bool\n", line);
    }
    exit(EXIT_FAILURE);   
}

void error_unknown_operator(int line){
    fprintf(stderr, "ERROR(line %d): unknown operator in interpreter\n", line);
    exit(EXIT_FAILURE);
}

void lexical_error(int line, char* msg){
    fprintf(stderr, "Lexical ERROR(line %d): unknown character '%s'\n", line, msg);
    exit(EXIT_FAILURE);
}

void syntax_error(int line, char* msg){
    fprintf(stderr,"Syntax ERROR(line %d): %s\n", line, msg);
}
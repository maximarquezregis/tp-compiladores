#define _GNU_SOURCE
#include "id_table.h"
#include <errno.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern int yylineno;

ID_TABLE* head_table = NULL;
ID_TABLE* end_table = NULL;

// creates a new node with id_name = name and returns its memory direction
ID_TABLE* add_id(char* name, ID_TYPE type) {
	// Check for redeclaration BEFORE adding to table
	if(find(name) != NULL) {
		fprintf(stderr, "ERROR(line %d): variable '%s' redeclared\n", yylineno, name);
        exit(EXIT_FAILURE);
	}

	ID_TABLE* aux = allocate_mem();
	if (end_table == NULL) {
		end_table = head_table = aux;
	} else {
		end_table->next = aux;
		end_table = aux;
	}

	end_table->id_name = strdup((char*)name);
	end_table->id_type = type;
	return end_table;
}

// adds data to the variable name node
void add_data(char* name, ID_TYPE type, void* data) {
	ID_TABLE* aux = find(name);
	if (aux == NULL) {
		fprintf(stderr, "ERROR(line %d): variable '%s' not declared\n", yylineno, name);
        exit(EXIT_FAILURE);
	}
	if (aux->id_type != type) {
		fprintf(stderr, "ERROR(line %d): type mismatch in assignment to variable '%s'\n", yylineno, name);
		exit(EXIT_FAILURE);
	}

	// Only free if data was previously allocated
	if (aux->data != NULL) {
		free(aux->data);
	}

	switch(type) {
		// allocate memory and copy data into the node
        case CONST_BOOL:
            aux->data = malloc(sizeof(int));
            memcpy(aux->data, data, sizeof(int));
            return;
        case CONST_INT:
            aux->data = malloc(sizeof(int));
            memcpy(aux->data, data, sizeof(int));
            return;
        case UNKNOWN:
            fprintf(stderr, "ERROR(line %d): variable '%s' not declared\n", yylineno, name);
        	exit(EXIT_FAILURE);
    }

	return;
}

/* returns the memory direction of the node with id_name = name
   if the node is not found, returns NULL */
ID_TABLE* find(char* name) {
	for (ID_TABLE* cur = head_table; cur; cur = cur->next) {
        if (cur->id_name && strcmp(cur->id_name, name) == 0) {
            return cur;
        }
    }
    return NULL;
}

// allocate memory for a node in the simbols' table
ID_TABLE* allocate_mem() {
	ID_TABLE* aux = malloc(sizeof(ID_TABLE));
	if (!aux) {
        perror("allocate_mem() error");
        exit(EXIT_FAILURE);
    }

	// initializes all data to NULL
	aux->id_name = NULL;
	aux->id_type = UNKNOWN;
	aux->data = NULL;
	aux->next = NULL;

	return aux;
}

// retrieves data of id from table
void* get_data(char* name) {
	ID_TABLE* aux = find(name);
	return aux->data;
}
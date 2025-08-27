#include "id_table.h"
#include <errno.h>
#include <stdlib.h>
#include <errno.h>

ID_TABLE* head_table = NULL;
ID_TABLE* end_table = NULL;

// creates a new node with id_name = name and returns its memory direction
ID_TABLE* add_id(char* name, ID_TYPE type) {
	ID_TABLE* aux = allocate_mem();
	if (end_table == NULL) {
		end_table = head_table = aux;
	} else {
		end_table->next = aux;
	}
	if(find(name) != NULL) {
		fprintf(stderr, "ERROR: variable '%s' redeclared\n", name);
        exit(EXIT_FAILURE);
	}
	end_table = aux;
	end_table->id_name = strdup((char*)name);
	end_table->id_type = type;
	return end_table;
}

// adds data to the variable name node
void add_data(char* name, ID_TYPE type, void* data) {
	ID_TABLE* aux = find(name);
	if (aux == NULL) {
		fprintf(stderr, "ERROR: variable '%s' no declarada\n", name);
        exit(EXIT_FAILURE);
	}
	aux->id_type = type;

	switch(type) {
        case CONST_BOOL:
            aux->data = malloc(sizeof(int));
            memcpy(aux->data, data, sizeof(int));
            return;
        case CONST_INT:
            aux->data = malloc(sizeof(int));
            memcpy(aux->data, data, sizeof(int));
            return;
        case UNKNOWN:
            fprintf(stderr, "ERROR: variable '%s' no declarada\n", name);
        	exit(EXIT_FAILURE);
    }
	aux->data = malloc(sizeof(int));
	memcpy(aux->data, data, sizeof(int));
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
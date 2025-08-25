#include "id_table.h"
#include <errno.h>

// creates a new node with id_name = name and returns its memory direction
ID_TABLE* add_id(char* name) {
	ID_TABLE* aux = allocate_mem();
	if (end == NULL) {
		end = head = aux;
	} else {
		end->next = aux;
	}
	if(find(name) != NULL) {
		perror("ERROR: variable %s redeclarada", name);
		exit();
	}
	end = aux;
	end->id_name = strdup((char*)name);
	return end;
}

void add_data(char* name, LEAF_TYPE type, void* data) {
	ID_TABLE* aux = find(name);
	if (aux == NULL) {
		aux = add_id(name);
	}
	aux->id_type = type;
	memcpy(aux->data, data, sizeof(int));
	return;
}

// returns the memory direction of the node with id_name = name
ID_TABLE* find(char* name) {
	ID_TABLE* aux = malloc(sizeof(ID_TABLE*));
	aux = head;
	while(strcmp(aux->id_name, name) && aux != NULL) {
		aux = aux->next;
	}

	return aux;
}

ID_TABLE* allocate_mem() {
	ID_TABLE* aux = malloc(sizeof(ID_TABLE));

	aux->id_name = NULL;
	aux->id_type = NULL;
	aux->data = NULL;
	aux->next = NULL;

	return aux;
}
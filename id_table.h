#ifndef PREPROYECTO_ID_TABLE_H
#define PREPROYECTO_ID_TABLE_H

#include "ast.h"
#include <string.h>

extern ID_TABLE* head;
extern ID_TABLE* end;

typedef struct ID_TABLE;

struct ID_TABLE {
	char* id_name;
	LEAF_TYPE id_type;
	int* data;
	ID_TABLE* next;
}

ID_TABLE* add_id(char* name);
void add_data(char* name, LEAF_TYPE type, void* data);
ID_TABLE* find(char* name);

#endif
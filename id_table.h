#ifndef ID_TABLE_H
#define ID_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct ID_TABLE ID_TABLE;

// pointers to the start and end of id_table
extern ID_TABLE* head_table;
extern ID_TABLE* end_table;

typedef enum {
	CONST_INT,
	CONST_BOOL,
	UNKNOWN
} ID_TYPE;

// node type for id_Table
struct ID_TABLE {
	char* id_name;
	ID_TYPE id_type;
	void* data;
	ID_TABLE* next;
};

ID_TABLE* allocate_mem();
ID_TABLE* add_id(char* name, ID_TYPE type);
void add_data(char* name, ID_TYPE type, void* data);
ID_TABLE* find(char* name);
void print_id_table(void);

#endif
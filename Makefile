# Variables
LEX     = flex
BISON   = bison
CC      = gcc
CFLAGS  = -g -Wall -Wextra -std=c11 -I. -Itree -IsymbolTable -IprintUtilities -Iinterpreter -Iassembly
TARGET  = parser

# Sources
GEN_LEX_SRC   = lex.yy.c
GEN_Y_TAB_C   = ej3-sint.tab.c
GEN_Y_TAB_H   = ej3-sint.tab.h
LEX_FILE      = ej3-lex.l
YACC_FILE     = ej3-sint.y

SRCS = tree/ast.c symbolTable/id_table.c printUtilities/print_funcs.c interpreter/interpreter.c assembly/pas.c
OBJS = $(SRCS:.c=.o) $(GEN_LEX_SRC:.c=.o) $(GEN_Y_TAB_C:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(GEN_LEX_SRC) $(GEN_Y_TAB_C) $(GEN_Y_TAB_H) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Generate Bison files
$(GEN_Y_TAB_C) $(GEN_Y_TAB_H): $(YACC_FILE)
	$(BISON) -d $(YACC_FILE)

# Generate Flex file
$(GEN_LEX_SRC): $(LEX_FILE) $(GEN_Y_TAB_H)
	$(LEX) $(LEX_FILE)

# Compile C sources
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile sources in subdirectories
tree/%.o: tree/%.c
	$(CC) $(CFLAGS) -c $< -o $@

symbolTable/%.o: symbolTable/%.c
	$(CC) $(CFLAGS) -c $< -o $@

printUtilities/%.o: printUtilities/%.c
	$(CC) $(CFLAGS) -c $< -o $@

interpreter/%.o: interpreter/%.c
	$(CC) $(CFLAGS) -c $< -o $@

assembly/%.o: assembly/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(GEN_LEX_SRC) $(GEN_Y_TAB_C) $(GEN_Y_TAB_H)
	rm -f tree/*.o symbolTable/*.o printUtilities/*.o interpreter/*.o assembly/*.o
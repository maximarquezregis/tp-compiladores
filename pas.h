#include "ast.h"

// Instruction types for the pseudo-assembly
typedef enum {
    I_LOAD,
    I_LOADVAL,
    I_STORE,
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_MIN,
    I_AND,
    I_OR,
    I_NEG,
    I_RET,
    I_DECL
} InstrType;

// Instruction representation for the pseudo-assembly
typedef struct {
    InstrType type;
    char arg[32]; // Buffer size (variable name or value)
} Instr;


void emit(InstrType t, const char* arg);
void genCode(AST_NODE* node);
void printCodeToFile(const char* filename);
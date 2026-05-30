#ifndef TAC_H
#define TAC_H

#include "ast.h"

typedef enum {
    TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV,
    TAC_ASSIGN, TAC_LABEL, TAC_GOTO, TAC_IFGOTO,
    TAC_PARAM, TAC_CALL, TAC_RETURN,
    TAC_INPUT, TAC_OUTPUT
} TacOp;

typedef struct TacInstr {
    TacOp op;
    char *result;   // temporary variable or variable name
    char *arg1;
    char *arg2;
    struct TacInstr *next;
} TacInstr;

typedef struct {
    TacInstr *head;
    TacInstr *tail;
    int temp_counter;
} TacProgram;

TacProgram* tac_create();
void tac_add_instr(TacProgram *prog, TacOp op, const char *result, const char *arg1, const char *arg2);
char* tac_new_temp(TacProgram *prog);
void tac_print(TacProgram *prog);
void tac_free(TacProgram *prog);

// TAC generation from AST
TacProgram* tac_generate(ASTNode *node);

#endif
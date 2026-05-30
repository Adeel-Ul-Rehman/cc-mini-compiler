#include "tac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TacProgram* tac_create() {
    TacProgram *prog = (TacProgram*)malloc(sizeof(TacProgram));
    prog->head = NULL;
    prog->tail = NULL;
    prog->temp_counter = 0;
    return prog;
}

void tac_add_instr(TacProgram *prog, TacOp op, const char *result, const char *arg1, const char *arg2) {
    TacInstr *instr = (TacInstr*)malloc(sizeof(TacInstr));
    instr->op = op;
    instr->result = result ? strdup(result) : NULL;
    instr->arg1 = arg1 ? strdup(arg1) : NULL;
    instr->arg2 = arg2 ? strdup(arg2) : NULL;
    instr->next = NULL;
    
    if (!prog->head) {
        prog->head = prog->tail = instr;
    } else {
        prog->tail->next = instr;
        prog->tail = instr;
    }
}

char* tac_new_temp(TacProgram *prog) {
    char *temp = (char*)malloc(16);
    sprintf(temp, "t%d", ++prog->temp_counter);
    return temp;
}

void tac_print(TacProgram *prog) {
    if (!prog || !prog->head) {
        printf("No TAC generated.\n");
        return;
    }
    
    printf("\n=== Three Address Code (TAC) ===\n");
    TacInstr *instr = prog->head;
    
    while (instr) {
        if (instr->op == TAC_LABEL) {
            printf("%s:\n", instr->result);
        } else if (instr->op == TAC_GOTO) {
            printf("goto %s\n", instr->result);
        } else if (instr->op == TAC_IFGOTO) {
            printf("if %s goto %s\n", instr->result, instr->arg1);
        } else if (instr->op == TAC_INPUT) {
            printf("input %s\n", instr->result);
        } else if (instr->op == TAC_OUTPUT) {
            printf("output %s\n", instr->result);
        } else if (instr->op == TAC_RETURN) {
            printf("return %s\n", instr->result ? instr->result : "");
        } else if (instr->op == TAC_ADD) {
            printf("%s = %s + %s\n", instr->result, instr->arg1, instr->arg2);
        } else if (instr->op == TAC_SUB) {
            printf("%s = %s - %s\n", instr->result, instr->arg1, instr->arg2);
        } else if (instr->op == TAC_MUL) {
            printf("%s = %s * %s\n", instr->result, instr->arg1, instr->arg2);
        } else if (instr->op == TAC_DIV) {
            printf("%s = %s / %s\n", instr->result, instr->arg1, instr->arg2);
        } else if (instr->op == TAC_ASSIGN) {
            printf("%s = %s\n", instr->result, instr->arg1);
        }
        instr = instr->next;
    }
    printf("==============================\n");
}

void tac_free(TacProgram *prog) {
    if (!prog) return;
    TacInstr *instr = prog->head;
    while (instr) {
        TacInstr *next = instr->next;
        if (instr->result) free(instr->result);
        if (instr->arg1) free(instr->arg1);
        if (instr->arg2) free(instr->arg2);
        free(instr);
        instr = next;
    }
    free(prog);
}

// Forward declarations
static void tac_gen_stmt(TacProgram *prog, ASTNode *node);
static void tac_gen_expr(TacProgram *prog, ASTNode *node, char **result);
static TacProgram *return_instrs = NULL;

static void tac_gen_assign(TacProgram *prog, ASTNode *node) {
    char *expr_result = NULL;
    tac_gen_expr(prog, node->data.assign.expr, &expr_result);
    if (expr_result) {
        tac_add_instr(prog, TAC_ASSIGN, node->data.assign.name, expr_result, NULL);
        free(expr_result);
    }
}

static void tac_gen_binary(TacProgram *prog, ASTNode *node, char **result) {
    char *left_result = NULL;
    char *right_result = NULL;
    
    tac_gen_expr(prog, node->data.binary.left, &left_result);
    tac_gen_expr(prog, node->data.binary.right, &right_result);
    
    if (!left_result || !right_result) {
        *result = NULL;
        return;
    }
    
    char *temp = tac_new_temp(prog);
    TacOp op;
    
    if (strcmp(node->data.binary.op, "+") == 0) op = TAC_ADD;
    else if (strcmp(node->data.binary.op, "-") == 0) op = TAC_SUB;
    else if (strcmp(node->data.binary.op, "*") == 0) op = TAC_MUL;
    else if (strcmp(node->data.binary.op, "/") == 0) op = TAC_DIV;
    else {
        *result = left_result;
        free(right_result);
        return;
    }
    
    tac_add_instr(prog, op, temp, left_result, right_result);
    *result = temp;
    free(left_result);
    free(right_result);
}

static void tac_gen_if(TacProgram *prog, ASTNode *node) {
    char *cond_result = NULL;
    tac_gen_expr(prog, node->data.if_stmt.cond, &cond_result);
    
    char *label_else = tac_new_temp(prog);
    char *label_end = tac_new_temp(prog);
    
    tac_add_instr(prog, TAC_IFGOTO, cond_result, label_else, NULL);
    free(cond_result);
    
    tac_gen_stmt(prog, node->data.if_stmt.then_branch);
    tac_add_instr(prog, TAC_GOTO, label_end, NULL, NULL);
    
    tac_add_instr(prog, TAC_LABEL, label_else, NULL, NULL);
    if (node->data.if_stmt.else_branch) {
        tac_gen_stmt(prog, node->data.if_stmt.else_branch);
    }
    tac_add_instr(prog, TAC_LABEL, label_end, NULL, NULL);
    free(label_else);
    free(label_end);
}

static void tac_gen_while(TacProgram *prog, ASTNode *node) {
    char *label_start = tac_new_temp(prog);
    char *label_end = tac_new_temp(prog);
    
    tac_add_instr(prog, TAC_LABEL, label_start, NULL, NULL);
    
    char *cond_result = NULL;
    tac_gen_expr(prog, node->data.while_stmt.cond, &cond_result);
    tac_add_instr(prog, TAC_IFGOTO, cond_result, label_end, NULL);
    free(cond_result);
    
    tac_gen_stmt(prog, node->data.while_stmt.body);
    tac_add_instr(prog, TAC_GOTO, label_start, NULL, NULL);
    tac_add_instr(prog, TAC_LABEL, label_end, NULL, NULL);
    free(label_start);
    free(label_end);
}

static void tac_gen_for(TacProgram *prog, ASTNode *node) {
    tac_gen_stmt(prog, node->data.for_stmt.init);
    
    char *label_start = tac_new_temp(prog);
    char *label_end = tac_new_temp(prog);
    
    tac_add_instr(prog, TAC_LABEL, label_start, NULL, NULL);
    
    char *cond_result = NULL;
    tac_gen_expr(prog, node->data.for_stmt.cond, &cond_result);
    tac_add_instr(prog, TAC_IFGOTO, cond_result, label_end, NULL);
    free(cond_result);
    
    tac_gen_stmt(prog, node->data.for_stmt.body);
    tac_gen_stmt(prog, node->data.for_stmt.inc);
    tac_add_instr(prog, TAC_GOTO, label_start, NULL, NULL);
    tac_add_instr(prog, TAC_LABEL, label_end, NULL, NULL);
    free(label_start);
    free(label_end);
}

static void tac_gen_output(TacProgram *prog, ASTNode *node) {
    char *expr_result = NULL;
    tac_gen_expr(prog, node->data.output.expr, &expr_result);
    if (expr_result) {
        tac_add_instr(prog, TAC_OUTPUT, expr_result, NULL, NULL);
        free(expr_result);
    }
}

static void tac_gen_input(TacProgram *prog, ASTNode *node) {
    tac_add_instr(prog, TAC_INPUT, node->data.input.var, NULL, NULL);
}

static void tac_gen_return(TacProgram *prog, ASTNode *node) {
    if (node->data.ret.expr) {
        char *expr_result = NULL;
        tac_gen_expr(prog, node->data.ret.expr, &expr_result);
        if (expr_result) {
            // Store return for later - will add at end
            if (!return_instrs) return_instrs = tac_create();
            tac_add_instr(return_instrs, TAC_RETURN, expr_result, NULL, NULL);
            free(expr_result);
        }
    }
}

static void tac_gen_expr(TacProgram *prog, ASTNode *node, char **result) {
    if (!node) {
        *result = NULL;
        return;
    }
    
    switch (node->type) {
        case NODE_INT_LIT: {
            char *temp = tac_new_temp(prog);
            char val[32];
            sprintf(val, "%d", node->data.ival);
            tac_add_instr(prog, TAC_ASSIGN, temp, val, NULL);
            *result = temp;
            break;
        }
        case NODE_FLOAT_LIT: {
            char *temp = tac_new_temp(prog);
            char val[64];
            sprintf(val, "%f", node->data.fval);
            tac_add_instr(prog, TAC_ASSIGN, temp, val, NULL);
            *result = temp;
            break;
        }
        case NODE_VAR: {
            *result = strdup(node->data.sval);
            break;
        }
        case NODE_BINARY: {
            tac_gen_binary(prog, node, result);
            break;
        }
        default: {
            *result = NULL;
            break;
        }
    }
}

static void tac_gen_block(TacProgram *prog, ASTNode *node) {
    ASTNode *stmt = node->data.block.stmts;
    while (stmt) {
        if (stmt->type == NODE_STMT_LIST) {
            tac_gen_stmt(prog, stmt->data.list.node);
            stmt = stmt->data.list.next;
        } else {
            tac_gen_stmt(prog, stmt);
            stmt = NULL;
        }
    }
}

static void tac_gen_stmt(TacProgram *prog, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_DECL:
            break;
        case NODE_ASSIGN:
            tac_gen_assign(prog, node);
            break;
        case NODE_IF:
            tac_gen_if(prog, node);
            break;
        case NODE_WHILE:
            tac_gen_while(prog, node);
            break;
        case NODE_FOR:
            tac_gen_for(prog, node);
            break;
        case NODE_RETURN:
            tac_gen_return(prog, node);
            break;
        case NODE_INPUT:
            tac_gen_input(prog, node);
            break;
        case NODE_OUTPUT:
            tac_gen_output(prog, node);
            break;
        case NODE_BLOCK:
            tac_gen_block(prog, node);
            break;
        case NODE_STMT_LIST:
            tac_gen_stmt(prog, node->data.list.node);
            tac_gen_stmt(prog, node->data.list.next);
            break;
        case NODE_FUNC:
            tac_gen_block(prog, node->data.func.body);
            break;
        default:
            break;
    }
}

TacProgram* tac_generate(ASTNode *node) {
    if (!node) return NULL;
    
    return_instrs = NULL;
    TacProgram *prog = tac_create();
    
    if (node->type == NODE_PROGRAM) {
        ASTNode *funcs = node->data.program.funcs;
        while (funcs) {
            if (funcs->type == NODE_FUNC_LIST) {
                tac_gen_stmt(prog, funcs->data.list.node);
                funcs = funcs->data.list.next;
            } else {
                tac_gen_stmt(prog, funcs);
                funcs = NULL;
            }
        }
    } else {
        tac_gen_stmt(prog, node);
    }
    
    // Append return instructions at the end
    if (return_instrs) {
        TacInstr *instr = return_instrs->head;
        while (instr) {
            tac_add_instr(prog, instr->op, instr->result, instr->arg1, instr->arg2);
            instr = instr->next;
        }
        tac_free(return_instrs);
    }
    
    return prog;
}
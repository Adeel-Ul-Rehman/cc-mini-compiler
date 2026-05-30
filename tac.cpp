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
        switch (instr->op) {
            case TAC_LABEL:
                printf("%s:\n", instr->result);
                break;
            case TAC_GOTO:
                printf("goto %s\n", instr->result);
                break;
            case TAC_IFGOTO:
                printf("if %s != 0 goto %s\n", instr->result, instr->arg1);
                break;
            case TAC_INPUT:
                printf("input %s\n", instr->result);
                break;
            case TAC_OUTPUT:
                printf("output %s\n", instr->result);
                break;
            case TAC_RETURN:
                printf("return %s\n", instr->result ? instr->result : "");
                break;
            case TAC_ASSIGN:
                if (instr->arg2) {
                    printf("%s = %s %s %s\n", instr->result, instr->arg1, 
                           instr->op == TAC_ADD ? "+" : (instr->op == TAC_SUB ? "-" : 
                           (instr->op == TAC_MUL ? "*" : "/")), instr->arg2);
                } else {
                    printf("%s = %s\n", instr->result, instr->arg1);
                }
                break;
            case TAC_ADD:
            case TAC_SUB:
            case TAC_MUL:
            case TAC_DIV:
                {
                    const char *op_str = "";
                    switch (instr->op) {
                        case TAC_ADD: op_str = "+"; break;
                        case TAC_SUB: op_str = "-"; break;
                        case TAC_MUL: op_str = "*"; break;
                        case TAC_DIV: op_str = "/"; break;
                        default: break;
                    }
                    printf("%s = %s %s %s\n", instr->result, instr->arg1, op_str, instr->arg2);
                }
                break;
            default:
                break;
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
    const char *op_str = node->data.binary.op;
    
    if (strcmp(op_str, "+") == 0) op = TAC_ADD;
    else if (strcmp(op_str, "-") == 0) op = TAC_SUB;
    else if (strcmp(op_str, "*") == 0) op = TAC_MUL;
    else if (strcmp(op_str, "/") == 0) op = TAC_DIV;
    else {
        op = TAC_ASSIGN;
        tac_add_instr(prog, TAC_ASSIGN, temp, left_result, NULL);
        *result = temp;
        free(left_result);
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
        tac_add_instr(prog, TAC_RETURN, expr_result, NULL, NULL);
        if (expr_result) free(expr_result);
    } else {
        tac_add_instr(prog, TAC_RETURN, NULL, NULL, NULL);
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

static void tac_gen_stmt(TacProgram *prog, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
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
        case NODE_BLOCK: {
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
            break;
        }
        case NODE_STMT_LIST:
            tac_gen_stmt(prog, node->data.list.node);
            tac_gen_stmt(prog, node->data.list.next);
            break;
        case NODE_FUNC: {
            // Generate code for function body
            tac_gen_stmt(prog, node->data.func.body);
            break;
        }
        default:
            break;
    }
}

TacProgram* tac_generate(ASTNode *node) {
    if (!node) return NULL;
    
    TacProgram *prog = tac_create();
    
    if (node->type == NODE_PROGRAM) {
        // Handle program node
        ASTNode *current = node->data.program.funcs;
        while (current) {
            if (current->type == NODE_FUNC_LIST) {
                // Extract the actual function node
                ASTNode *func = current->data.list.node;
                tac_gen_stmt(prog, func);
                current = current->data.list.next;
            } else {
                tac_gen_stmt(prog, current);
                current = NULL;
            }
        }
    } else {
        tac_gen_stmt(prog, node);
    }
    
    return prog;
}
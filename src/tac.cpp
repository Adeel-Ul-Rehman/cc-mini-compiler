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
        } else if (instr->op == TAC_ARRAY_STORE) {
            printf("%s[%s] = %s\n", instr->result, instr->arg1, instr->arg2);
        } else if (instr->op == TAC_ARRAY_LOAD) {
            printf("%s = %s[%s]\n", instr->result, instr->arg1, instr->arg2);
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

static void gen_expr(TacProgram *prog, ASTNode *node, char **result);
static void gen_stmt(TacProgram *prog, ASTNode *node);

static void gen_assign(TacProgram *prog, ASTNode *node) {
    char *expr_res = NULL;
    gen_expr(prog, node->data.assign.expr, &expr_res);
    if (expr_res) {
        tac_add_instr(prog, TAC_ASSIGN, node->data.assign.name, expr_res, NULL);
        free(expr_res);
    }
}

static void gen_binary(TacProgram *prog, ASTNode *node, char **result) {
    char *left = NULL, *right = NULL;
    gen_expr(prog, node->data.binary.left, &left);
    gen_expr(prog, node->data.binary.right, &right);
    
    if (!left || !right) {
        *result = NULL;
        return;
    }
    
    char *temp = tac_new_temp(prog);
    TacOp op;
    if (strcmp(node->data.binary.op, "+") == 0) op = TAC_ADD;
    else if (strcmp(node->data.binary.op, "-") == 0) op = TAC_SUB;
    else if (strcmp(node->data.binary.op, "*") == 0) op = TAC_MUL;
    else op = TAC_DIV;
    
    tac_add_instr(prog, op, temp, left, right);
    *result = temp;
    free(left);
    free(right);
}

static void gen_if(TacProgram *prog, ASTNode *node) {
    char *cond = NULL;
    gen_expr(prog, node->data.if_stmt.cond, &cond);
    char *l_else = tac_new_temp(prog);
    char *l_end = tac_new_temp(prog);
    tac_add_instr(prog, TAC_IFGOTO, cond, l_else, NULL);
    free(cond);
    gen_stmt(prog, node->data.if_stmt.then_branch);
    tac_add_instr(prog, TAC_GOTO, l_end, NULL, NULL);
    tac_add_instr(prog, TAC_LABEL, l_else, NULL, NULL);
    if (node->data.if_stmt.else_branch) gen_stmt(prog, node->data.if_stmt.else_branch);
    tac_add_instr(prog, TAC_LABEL, l_end, NULL, NULL);
    free(l_else);
    free(l_end);
}

static void gen_while(TacProgram *prog, ASTNode *node) {
    char *l_start = tac_new_temp(prog);
    char *l_end = tac_new_temp(prog);
    tac_add_instr(prog, TAC_LABEL, l_start, NULL, NULL);
    char *cond = NULL;
    gen_expr(prog, node->data.while_stmt.cond, &cond);
    tac_add_instr(prog, TAC_IFGOTO, cond, l_end, NULL);
    free(cond);
    gen_stmt(prog, node->data.while_stmt.body);
    tac_add_instr(prog, TAC_GOTO, l_start, NULL, NULL);
    tac_add_instr(prog, TAC_LABEL, l_end, NULL, NULL);
    free(l_start);
    free(l_end);
}

static void gen_for(TacProgram *prog, ASTNode *node) {
    gen_stmt(prog, node->data.for_stmt.init);
    char *l_start = tac_new_temp(prog);
    char *l_end = tac_new_temp(prog);
    tac_add_instr(prog, TAC_LABEL, l_start, NULL, NULL);
    char *cond = NULL;
    gen_expr(prog, node->data.for_stmt.cond, &cond);
    tac_add_instr(prog, TAC_IFGOTO, cond, l_end, NULL);
    free(cond);
    gen_stmt(prog, node->data.for_stmt.body);
    gen_stmt(prog, node->data.for_stmt.inc);
    tac_add_instr(prog, TAC_GOTO, l_start, NULL, NULL);
    tac_add_instr(prog, TAC_LABEL, l_end, NULL, NULL);
    free(l_start);
    free(l_end);
}

static void gen_output(TacProgram *prog, ASTNode *node) {
    char *expr = NULL;
    gen_expr(prog, node->data.output.expr, &expr);
    if (expr) {
        tac_add_instr(prog, TAC_OUTPUT, expr, NULL, NULL);
        free(expr);
    }
}

static void gen_input(TacProgram *prog, ASTNode *node) {
    tac_add_instr(prog, TAC_INPUT, node->data.input.var, NULL, NULL);
}

static void gen_return(TacProgram *prog, ASTNode *node) {
    if (node->data.ret.expr) {
        char *expr = NULL;
        gen_expr(prog, node->data.ret.expr, &expr);
        if (expr) {
            tac_add_instr(prog, TAC_RETURN, expr, NULL, NULL);
            free(expr);
        }
    } else {
        tac_add_instr(prog, TAC_RETURN, NULL, NULL, NULL);
    }
}

static void gen_array_decl(TacProgram *prog, ASTNode *node) {
    ASTNode *init = node->data.array_decl.init_list;
    if (init) {
        int idx = 0;
        ASTNode *cur = init;
        while (cur) {
            if (cur->type == NODE_ARRAY_INIT) {
                char *val = NULL;
                gen_expr(prog, cur->data.list.node, &val);
                if (val) {
                    char idx_str[32];
                    sprintf(idx_str, "%d", idx);
                    tac_add_instr(prog, TAC_ARRAY_STORE, node->data.array_decl.name, idx_str, val);
                    free(val);
                }
                cur = cur->data.list.next;
                idx++;
            } else {
                cur = NULL;
            }
        }
    }
}

static void gen_array_assign(TacProgram *prog, ASTNode *node) {
    ASTNode *access = node->data.array_assign.lvalue;
    char *idx = NULL, *val = NULL;
    gen_expr(prog, access->data.array_access.index, &idx);
    gen_expr(prog, node->data.array_assign.expr, &val);
    if (idx && val) {
        tac_add_instr(prog, TAC_ARRAY_STORE, access->data.array_access.name, idx, val);
        free(idx);
        free(val);
    }
}

static void gen_array_load(TacProgram *prog, ASTNode *node, char **result) {
    char *idx = NULL;
    gen_expr(prog, node->data.array_access.index, &idx);
    if (idx) {
        char *temp = tac_new_temp(prog);
        tac_add_instr(prog, TAC_ARRAY_LOAD, temp, node->data.array_access.name, idx);
        *result = temp;
        free(idx);
    } else {
        *result = NULL;
    }
}

static void gen_expr(TacProgram *prog, ASTNode *node, char **result) {
    if (!node) {
        *result = NULL;
        return;
    }
    
    switch (node->type) {
        case NODE_INT_LIT: {
            char *temp = tac_new_temp(prog);
            char buf[32];
            sprintf(buf, "%d", node->data.ival);
            tac_add_instr(prog, TAC_ASSIGN, temp, buf, NULL);
            *result = temp;
            break;
        }
        case NODE_FLOAT_LIT: {
            char *temp = tac_new_temp(prog);
            char buf[64];
            sprintf(buf, "%f", node->data.fval);
            tac_add_instr(prog, TAC_ASSIGN, temp, buf, NULL);
            *result = temp;
            break;
        }
        case NODE_VAR:
            *result = strdup(node->data.sval);
            break;
        case NODE_BINARY:
            gen_binary(prog, node, result);
            break;
        case NODE_ARRAY_ACCESS:
            gen_array_load(prog, node, result);
            break;
        default:
            *result = NULL;
            break;
    }
}

static void gen_block(TacProgram *prog, ASTNode *node) {
    ASTNode *stmt = node->data.block.stmts;
    while (stmt) {
        if (stmt->type == NODE_STMT_LIST) {
            gen_stmt(prog, stmt->data.list.node);
            stmt = stmt->data.list.next;
        } else {
            gen_stmt(prog, stmt);
            stmt = NULL;
        }
    }
}

static void gen_stmt(TacProgram *prog, ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_DECL:
            break;
        case NODE_ARRAY_DECL:
            gen_array_decl(prog, node);
            break;
        case NODE_ASSIGN:
            gen_assign(prog, node);
            break;
        case NODE_ARRAY_ASSIGN:
            gen_array_assign(prog, node);
            break;
        case NODE_IF:
            gen_if(prog, node);
            break;
        case NODE_WHILE:
            gen_while(prog, node);
            break;
        case NODE_FOR:
            gen_for(prog, node);
            break;
        case NODE_RETURN:
            gen_return(prog, node);
            break;
        case NODE_INPUT:
            gen_input(prog, node);
            break;
        case NODE_OUTPUT:
            gen_output(prog, node);
            break;
        case NODE_BLOCK:
            gen_block(prog, node);
            break;
        case NODE_STMT_LIST:
            gen_stmt(prog, node->data.list.node);
            gen_stmt(prog, node->data.list.next);
            break;
        case NODE_FUNC:
            gen_block(prog, node->data.func.body);
            break;
        default:
            break;
    }
}

TacProgram* tac_generate(ASTNode *node) {
    if (!node) return NULL;
    TacProgram *prog = tac_create();
    
    if (node->type == NODE_PROGRAM) {
        ASTNode *funcs = node->data.program.funcs;
        while (funcs) {
            if (funcs->type == NODE_FUNC_LIST) {
                gen_stmt(prog, funcs->data.list.node);
                funcs = funcs->data.list.next;
            } else {
                gen_stmt(prog, funcs);
                funcs = NULL;
            }
        }
    } else {
        gen_stmt(prog, node);
    }
    return prog;
}
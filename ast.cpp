#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* ast_program(ASTNode *decls, ASTNode *funcs) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->data.program.decls = decls;
    node->data.program.funcs = funcs;
    return node;
}

ASTNode* ast_decl_list(ASTNode *next, ASTNode *node) {
    ASTNode *list = (ASTNode*)malloc(sizeof(ASTNode));
    list->type = NODE_DECL_LIST;
    list->data.list.next = next;
    list->data.list.node = node;
    return list;
}

ASTNode* ast_declaration(ASTNode *type, char *name, ASTNode *init) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_DECL;
    node->data.decl.type = type;
    node->data.decl.name = strdup(name);
    node->data.decl.init = init;
    return node;
}

ASTNode* ast_func_list(ASTNode *next, ASTNode *node) {
    ASTNode *list = (ASTNode*)malloc(sizeof(ASTNode));
    list->type = NODE_FUNC_LIST;
    list->data.list.next = next;
    list->data.list.node = node;
    return list;
}

ASTNode* ast_function(ASTNode *ret_type, char *name, ASTNode *params, ASTNode *body) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_FUNC;
    node->data.func.ret_type = ret_type;
    node->data.func.name = strdup(name);
    node->data.func.params = params;
    node->data.func.body = body;
    return node;
}

ASTNode* ast_param_list(ASTNode *next, ASTNode *type, char *name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_PARAM_LIST;
    node->data.param.next = next;
    node->data.param.type = type;
    node->data.param.name = strdup(name);
    return node;
}

ASTNode* ast_block(ASTNode *stmts) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_BLOCK;
    node->data.block.stmts = stmts;
    return node;
}

ASTNode* ast_stmt_list(ASTNode *next, ASTNode *node) {
    ASTNode *list = (ASTNode*)malloc(sizeof(ASTNode));
    list->type = NODE_STMT_LIST;
    list->data.list.next = next;
    list->data.list.node = node;
    return list;
}

ASTNode* ast_assignment(char *name, ASTNode *expr) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN;
    node->data.assign.name = strdup(name);
    node->data.assign.expr = expr;
    return node;
}

ASTNode* ast_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_IF;
    node->data.if_stmt.cond = cond;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch;
    return node;
}

ASTNode* ast_while(ASTNode *cond, ASTNode *body) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->data.while_stmt.cond = cond;
    node->data.while_stmt.body = body;
    return node;
}

ASTNode* ast_for(ASTNode *init, ASTNode *cond, ASTNode *inc, ASTNode *body) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_FOR;
    node->data.for_stmt.init = init;
    node->data.for_stmt.cond = cond;
    node->data.for_stmt.inc = inc;
    node->data.for_stmt.body = body;
    return node;
}

ASTNode* ast_return(ASTNode *expr) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_RETURN;
    node->data.ret.expr = expr;
    return node;
}

ASTNode* ast_input(char *var) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_INPUT;
    node->data.input.var = strdup(var);
    return node;
}

ASTNode* ast_output(ASTNode *expr) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_OUTPUT;
    node->data.output.expr = expr;
    return node;
}

ASTNode* ast_binary(const char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_BINARY;
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

ASTNode* ast_int_lit(int val) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_INT_LIT;
    node->data.ival = val;
    return node;
}

ASTNode* ast_float_lit(float val) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_FLOAT_LIT;
    node->data.fval = val;
    return node;
}

ASTNode* ast_bool_lit(int val) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_BOOL_LIT;
    node->data.ival = val;
    return node;
}

ASTNode* ast_string_lit(char *val) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_STRING_LIT;
    node->data.sval = strdup(val);
    return node;
}

ASTNode* ast_var(char *name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_VAR;
    node->data.sval = strdup(name);
    return node;
}

ASTNode* ast_type(char *name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NODE_TYPE;
    node->data.sval = strdup(name);
    return node;
}

void ast_print(ASTNode *node, int indent) {
    if (!node) return;
    for (int i=0;i<indent;i++) printf(" ");
    switch(node->type) {
        case NODE_PROGRAM: printf("Program\n"); ast_print(node->data.program.decls, indent+2); ast_print(node->data.program.funcs, indent+2); break;
        case NODE_DECL_LIST: ast_print(node->data.list.node, indent); ast_print(node->data.list.next, indent); break;
        case NODE_DECL: printf("Decl: %s\n", node->data.decl.name); ast_print(node->data.decl.type, indent+2); if(node->data.decl.init) ast_print(node->data.decl.init, indent+2); break;
        case NODE_FUNC_LIST: ast_print(node->data.list.node, indent); ast_print(node->data.list.next, indent); break;
        case NODE_FUNC: printf("Func: %s\n", node->data.func.name); ast_print(node->data.func.ret_type, indent+2); ast_print(node->data.func.params, indent+2); ast_print(node->data.func.body, indent+2); break;
        case NODE_PARAM_LIST: printf("Param: %s\n", node->data.param.name); ast_print(node->data.param.type, indent+2); ast_print(node->data.param.next, indent); break;
        case NODE_BLOCK: printf("Block\n"); ast_print(node->data.block.stmts, indent+2); break;
        case NODE_STMT_LIST: ast_print(node->data.list.node, indent); ast_print(node->data.list.next, indent); break;
        case NODE_ASSIGN: printf("Assign: %s\n", node->data.assign.name); ast_print(node->data.assign.expr, indent+2); break;
        case NODE_IF: printf("If\n"); ast_print(node->data.if_stmt.cond, indent+2); ast_print(node->data.if_stmt.then_branch, indent+2); if(node->data.if_stmt.else_branch) ast_print(node->data.if_stmt.else_branch, indent+2); break;
        case NODE_WHILE: printf("While\n"); ast_print(node->data.while_stmt.cond, indent+2); ast_print(node->data.while_stmt.body, indent+2); break;
        case NODE_FOR: printf("For\n"); ast_print(node->data.for_stmt.init, indent+2); ast_print(node->data.for_stmt.cond, indent+2); ast_print(node->data.for_stmt.inc, indent+2); ast_print(node->data.for_stmt.body, indent+2); break;
        case NODE_RETURN: printf("Return\n"); ast_print(node->data.ret.expr, indent+2); break;
        case NODE_INPUT: printf("Input: %s\n", node->data.input.var); break;
        case NODE_OUTPUT: printf("Output\n"); ast_print(node->data.output.expr, indent+2); break;
        case NODE_BINARY: printf("Binary: %s\n", node->data.binary.op); ast_print(node->data.binary.left, indent+2); ast_print(node->data.binary.right, indent+2); break;
        case NODE_INT_LIT: printf("Int: %d\n", node->data.ival); break;
        case NODE_FLOAT_LIT: printf("Float: %f\n", node->data.fval); break;
        case NODE_BOOL_LIT: printf("Bool: %d\n", node->data.ival); break;
        case NODE_STRING_LIT: printf("String: %s\n", node->data.sval); break;
        case NODE_VAR: printf("Var: %s\n", node->data.sval); break;
        case NODE_TYPE: printf("Type: %s\n", node->data.sval); break;
    }
}

void ast_free(ASTNode *node) {
    if (!node) return;
    free(node);
}
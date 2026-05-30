#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ASTNode* new_node(NodeType type) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->inferred_type = NULL;
    return node;
}

ASTNode* ast_program(ASTNode *decls, ASTNode *funcs) {
    ASTNode *node = new_node(NODE_PROGRAM);
    node->data.program.decls = decls;
    node->data.program.funcs = funcs;
    return node;
}

ASTNode* ast_decl_list(ASTNode *next, ASTNode *node) {
    ASTNode *list = new_node(NODE_DECL_LIST);
    // Store in correct order - next is previous list
    if (next) {
        // Find tail
        ASTNode *tail = next;
        while (tail->data.list.next) {
            tail = tail->data.list.next;
        }
        tail->data.list.next = list;
        list->data.list.node = node;
        list->data.list.next = NULL;
        return next;
    } else {
        list->data.list.node = node;
        list->data.list.next = NULL;
        return list;
    }
}

ASTNode* ast_declaration(ASTNode *type, char *name, ASTNode *init) {
    ASTNode *node = new_node(NODE_DECL);
    node->data.decl.type = type;
    node->data.decl.name = strdup(name);
    node->data.decl.init = init;
    if (init) {
        ast_set_type(node, ast_get_type(init));
    } else {
        ast_set_type(node, ast_get_type(type));
    }
    return node;
}

ASTNode* ast_func_list(ASTNode *next, ASTNode *node) {
    ASTNode *list = new_node(NODE_FUNC_LIST);
    if (next) {
        ASTNode *tail = next;
        while (tail->data.list.next) {
            tail = tail->data.list.next;
        }
        tail->data.list.next = list;
        list->data.list.node = node;
        list->data.list.next = NULL;
        return next;
    } else {
        list->data.list.node = node;
        list->data.list.next = NULL;
        return list;
    }
}

ASTNode* ast_function(ASTNode *ret_type, char *name, ASTNode *params, ASTNode *body) {
    ASTNode *node = new_node(NODE_FUNC);
    node->data.func.ret_type = ret_type;
    node->data.func.name = strdup(name);
    node->data.func.params = params;
    node->data.func.body = body;
    ast_set_type(node, ast_get_type(ret_type));
    return node;
}

ASTNode* ast_param_list(ASTNode *next, ASTNode *type, char *name) {
    ASTNode *node = new_node(NODE_PARAM_LIST);
    if (next) {
        ASTNode *tail = next;
        while (tail->data.param.next) {
            tail = tail->data.param.next;
        }
        tail->data.param.next = node;
        node->data.param.type = type;
        node->data.param.name = strdup(name);
        node->data.param.next = NULL;
        return next;
    } else {
        node->data.param.type = type;
        node->data.param.name = strdup(name);
        node->data.param.next = NULL;
        return node;
    }
}

ASTNode* ast_block(ASTNode *stmts) {
    ASTNode *node = new_node(NODE_BLOCK);
    node->data.block.stmts = stmts;
    return node;
}

ASTNode* ast_stmt_list(ASTNode *next, ASTNode *node) {
    ASTNode *list = new_node(NODE_STMT_LIST);
    // Maintain order: new node goes to the end
    if (next) {
        ASTNode *tail = next;
        while (tail->data.list.next) {
            tail = tail->data.list.next;
        }
        tail->data.list.next = list;
        list->data.list.node = node;
        list->data.list.next = NULL;
        return next;
    } else {
        list->data.list.node = node;
        list->data.list.next = NULL;
        return list;
    }
}

ASTNode* ast_assignment(char *name, ASTNode *expr) {
    ASTNode *node = new_node(NODE_ASSIGN);
    node->data.assign.name = strdup(name);
    node->data.assign.expr = expr;
    ast_set_type(node, ast_get_type(expr));
    return node;
}

ASTNode* ast_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = new_node(NODE_IF);
    node->data.if_stmt.cond = cond;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch;
    ast_set_type(node, "void");
    return node;
}

ASTNode* ast_while(ASTNode *cond, ASTNode *body) {
    ASTNode *node = new_node(NODE_WHILE);
    node->data.while_stmt.cond = cond;
    node->data.while_stmt.body = body;
    ast_set_type(node, "void");
    return node;
}

ASTNode* ast_for(ASTNode *init, ASTNode *cond, ASTNode *inc, ASTNode *body) {
    ASTNode *node = new_node(NODE_FOR);
    node->data.for_stmt.init = init;
    node->data.for_stmt.cond = cond;
    node->data.for_stmt.inc = inc;
    node->data.for_stmt.body = body;
    ast_set_type(node, "void");
    return node;
}

ASTNode* ast_return(ASTNode *expr) {
    ASTNode *node = new_node(NODE_RETURN);
    node->data.ret.expr = expr;
    ast_set_type(node, ast_get_type(expr));
    return node;
}

ASTNode* ast_input(char *var) {
    ASTNode *node = new_node(NODE_INPUT);
    node->data.input.var = strdup(var);
    ast_set_type(node, "void");
    return node;
}

ASTNode* ast_output(ASTNode *expr) {
    ASTNode *node = new_node(NODE_OUTPUT);
    node->data.output.expr = expr;
    ast_set_type(node, "void");
    return node;
}

ASTNode* ast_binary(const char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = new_node(NODE_BINARY);
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    const char *ltype = ast_get_type(left);
    const char *rtype = ast_get_type(right);
    if (strcmp(ltype, "float") == 0 || strcmp(rtype, "float") == 0) {
        ast_set_type(node, "float");
    } else {
        ast_set_type(node, "int");
    }
    return node;
}

ASTNode* ast_int_lit(int val) {
    ASTNode *node = new_node(NODE_INT_LIT);
    node->data.ival = val;
    ast_set_type(node, "int");
    return node;
}

ASTNode* ast_float_lit(float val) {
    ASTNode *node = new_node(NODE_FLOAT_LIT);
    node->data.fval = val;
    ast_set_type(node, "float");
    return node;
}

ASTNode* ast_bool_lit(int val) {
    ASTNode *node = new_node(NODE_BOOL_LIT);
    node->data.ival = val;
    ast_set_type(node, "int");
    return node;
}

ASTNode* ast_string_lit(char *val) {
    ASTNode *node = new_node(NODE_STRING_LIT);
    node->data.sval = strdup(val);
    ast_set_type(node, "string");
    return node;
}

ASTNode* ast_var(char *name) {
    ASTNode *node = new_node(NODE_VAR);
    node->data.sval = strdup(name);
    ast_set_type(node, "unknown");
    return node;
}

ASTNode* ast_type(char *name) {
    ASTNode *node = new_node(NODE_TYPE);
    node->data.sval = strdup(name);
    ast_set_type(node, name);
    return node;
}

const char* ast_get_type(ASTNode *node) {
    if (!node) return "unknown";
    if (node->inferred_type) return node->inferred_type;
    switch (node->type) {
        case NODE_INT_LIT: return "int";
        case NODE_FLOAT_LIT: return "float";
        case NODE_BOOL_LIT: return "int";
        case NODE_STRING_LIT: return "string";
        case NODE_TYPE: return node->data.sval;
        case NODE_VAR: return "unknown";
        default: return "unknown";
    }
}

void ast_set_type(ASTNode *node, const char *type) {
    if (!node) return;
    if (node->inferred_type) free(node->inferred_type);
    node->inferred_type = strdup(type);
}

void ast_print(ASTNode *node, int indent) {
    if (!node) return;
    for (int i=0;i<indent;i++) printf(" ");
    switch(node->type) {
        case NODE_PROGRAM: printf("Program\n"); ast_print(node->data.program.decls, indent+2); ast_print(node->data.program.funcs, indent+2); break;
        case NODE_DECL_LIST: ast_print(node->data.list.node, indent); ast_print(node->data.list.next, indent); break;
        case NODE_DECL: printf("Decl: %s (%s)\n", node->data.decl.name, node->inferred_type ? node->inferred_type : "?"); ast_print(node->data.decl.type, indent+2); if(node->data.decl.init) ast_print(node->data.decl.init, indent+2); break;
        case NODE_FUNC_LIST: ast_print(node->data.list.node, indent); ast_print(node->data.list.next, indent); break;
        case NODE_FUNC: printf("Func: %s (%s)\n", node->data.func.name, node->inferred_type ? node->inferred_type : "?"); ast_print(node->data.func.ret_type, indent+2); ast_print(node->data.func.params, indent+2); ast_print(node->data.func.body, indent+2); break;
        case NODE_PARAM_LIST: printf("Param: %s (%s)\n", node->data.param.name, node->inferred_type ? node->inferred_type : "?"); ast_print(node->data.param.type, indent+2); ast_print(node->data.param.next, indent); break;
        case NODE_BLOCK: printf("Block\n"); ast_print(node->data.block.stmts, indent+2); break;
        case NODE_STMT_LIST: ast_print(node->data.list.node, indent); ast_print(node->data.list.next, indent); break;
        case NODE_ASSIGN: printf("Assign: %s (%s)\n", node->data.assign.name, node->inferred_type ? node->inferred_type : "?"); ast_print(node->data.assign.expr, indent+2); break;
        case NODE_IF: printf("If\n"); ast_print(node->data.if_stmt.cond, indent+2); ast_print(node->data.if_stmt.then_branch, indent+2); if(node->data.if_stmt.else_branch) ast_print(node->data.if_stmt.else_branch, indent+2); break;
        case NODE_WHILE: printf("While\n"); ast_print(node->data.while_stmt.cond, indent+2); ast_print(node->data.while_stmt.body, indent+2); break;
        case NODE_FOR: printf("For\n"); ast_print(node->data.for_stmt.init, indent+2); ast_print(node->data.for_stmt.cond, indent+2); ast_print(node->data.for_stmt.inc, indent+2); ast_print(node->data.for_stmt.body, indent+2); break;
        case NODE_RETURN: printf("Return (%s)\n", node->inferred_type ? node->inferred_type : "?"); ast_print(node->data.ret.expr, indent+2); break;
        case NODE_INPUT: printf("Input: %s\n", node->data.input.var); break;
        case NODE_OUTPUT: printf("Output\n"); ast_print(node->data.output.expr, indent+2); break;
        case NODE_BINARY: printf("Binary: %s (%s)\n", node->data.binary.op, node->inferred_type ? node->inferred_type : "?"); ast_print(node->data.binary.left, indent+2); ast_print(node->data.binary.right, indent+2); break;
        case NODE_INT_LIT: printf("Int: %d\n", node->data.ival); break;
        case NODE_FLOAT_LIT: printf("Float: %f\n", node->data.fval); break;
        case NODE_BOOL_LIT: printf("Bool: %d\n", node->data.ival); break;
        case NODE_STRING_LIT: printf("String: %s\n", node->data.sval); break;
        case NODE_VAR: printf("Var: %s (%s)\n", node->data.sval, node->inferred_type ? node->inferred_type : "?"); break;
        case NODE_TYPE: printf("Type: %s\n", node->data.sval); break;
        default: break;
    }
}

// Constant folding helper - returns a new constant node if expression is constant
ASTNode* ast_try_fold(ASTNode *node) {
    if (!node) return NULL;
    
    if (node->type == NODE_BINARY) {
        ASTNode *left = node->data.binary.left;
        ASTNode *right = node->data.binary.right;
        
        if (left && right && 
            (left->type == NODE_INT_LIT || left->type == NODE_FLOAT_LIT) &&
            (right->type == NODE_INT_LIT || right->type == NODE_FLOAT_LIT)) {
            
            const char *op = node->data.binary.op;
            float l = (left->type == NODE_INT_LIT) ? left->data.ival : left->data.fval;
            float r = (right->type == NODE_INT_LIT) ? right->data.ival : right->data.fval;
            float result = 0;
            
            if (strcmp(op, "+") == 0) result = l + r;
            else if (strcmp(op, "-") == 0) result = l - r;
            else if (strcmp(op, "*") == 0) result = l * r;
            else if (strcmp(op, "/") == 0) result = l / r;
            else return node;
            
            if (result == (int)result && left->type == NODE_INT_LIT && right->type == NODE_INT_LIT) {
                return ast_int_lit((int)result);
            } else {
                return ast_float_lit(result);
            }
        }
    }
    return node;
}

void ast_free(ASTNode *node) {
    if (!node) return;
    if (node->inferred_type) free(node->inferred_type);
    free(node);
}
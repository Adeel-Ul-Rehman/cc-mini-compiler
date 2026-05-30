#include "optimizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_constant_node(ASTNode *node) {
    if (!node) return 0;
    return (node->type == NODE_INT_LIT || node->type == NODE_FLOAT_LIT);
}

int eval_constant_int(ASTNode *node) {
    if (!node) return 0;
    if (node->type == NODE_INT_LIT) return node->data.ival;
    if (node->type == NODE_FLOAT_LIT) return (int)node->data.fval;
    return 0;
}

float eval_constant_float(ASTNode *node) {
    if (!node) return 0.0;
    if (node->type == NODE_INT_LIT) return (float)node->data.ival;
    if (node->type == NODE_FLOAT_LIT) return node->data.fval;
    return 0.0;
}

static ASTNode* fold_binary_constant(ASTNode *node) {
    if (node->type != NODE_BINARY) return node;
    
    ASTNode *left = node->data.binary.left;
    ASTNode *right = node->data.binary.right;
    
    // Both operands are constants
    if (is_constant_node(left) && is_constant_node(right)) {
        const char *op = node->data.binary.op;
        float left_val = eval_constant_float(left);
        float right_val = eval_constant_float(right);
        float result = 0;
        
        if (strcmp(op, "+") == 0) result = left_val + right_val;
        else if (strcmp(op, "-") == 0) result = left_val - right_val;
        else if (strcmp(op, "*") == 0) result = left_val * right_val;
        else if (strcmp(op, "/") == 0) result = left_val / right_val;
        else {
            // For comparison operators, evaluate to int
            if (strcmp(op, "==") == 0) result = (left_val == right_val) ? 1 : 0;
            else if (strcmp(op, "!=") == 0) result = (left_val != right_val) ? 1 : 0;
            else if (strcmp(op, "<") == 0) result = (left_val < right_val) ? 1 : 0;
            else if (strcmp(op, ">") == 0) result = (left_val > right_val) ? 1 : 0;
            else if (strcmp(op, "<=") == 0) result = (left_val <= right_val) ? 1 : 0;
            else if (strcmp(op, ">=") == 0) result = (left_val >= right_val) ? 1 : 0;
            else return node;
        }
        
        // Check if result is integer (no fractional part)
        if (result == (int)result && left->type == NODE_INT_LIT && right->type == NODE_INT_LIT) {
            ASTNode *new_node = ast_int_lit((int)result);
            ast_set_type(new_node, "int");
            return new_node;
        } else {
            ASTNode *new_node = ast_float_lit(result);
            ast_set_type(new_node, "float");
            return new_node;
        }
    }
    
    return node;
}

static void optimize_stmt(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_ASSIGN:
            if (node->data.assign.expr) {
                node->data.assign.expr = fold_constants(node->data.assign.expr);
            }
            break;
        case NODE_IF:
            if (node->data.if_stmt.cond) {
                node->data.if_stmt.cond = fold_constants(node->data.if_stmt.cond);
            }
            optimize_stmt(node->data.if_stmt.then_branch);
            if (node->data.if_stmt.else_branch) {
                optimize_stmt(node->data.if_stmt.else_branch);
            }
            break;
        case NODE_WHILE:
            if (node->data.while_stmt.cond) {
                node->data.while_stmt.cond = fold_constants(node->data.while_stmt.cond);
            }
            optimize_stmt(node->data.while_stmt.body);
            break;
        case NODE_FOR:
            optimize_stmt(node->data.for_stmt.init);
            if (node->data.for_stmt.cond) {
                node->data.for_stmt.cond = fold_constants(node->data.for_stmt.cond);
            }
            optimize_stmt(node->data.for_stmt.body);
            optimize_stmt(node->data.for_stmt.inc);
            break;
        case NODE_RETURN:
            if (node->data.ret.expr) {
                node->data.ret.expr = fold_constants(node->data.ret.expr);
            }
            break;
        case NODE_OUTPUT:
            if (node->data.output.expr) {
                node->data.output.expr = fold_constants(node->data.output.expr);
            }
            break;
        case NODE_BLOCK: {
            ASTNode *stmt = node->data.block.stmts;
            while (stmt) {
                if (stmt->type == NODE_STMT_LIST) {
                    optimize_stmt(stmt->data.list.node);
                    stmt = stmt->data.list.next;
                } else {
                    optimize_stmt(stmt);
                    stmt = NULL;
                }
            }
            break;
        }
        case NODE_STMT_LIST:
            optimize_stmt(node->data.list.node);
            optimize_stmt(node->data.list.next);
            break;
        case NODE_FUNC:
            optimize_stmt(node->data.func.body);
            break;
        default:
            break;
    }
}

ASTNode* fold_constants(ASTNode *node) {
    if (!node) return node;
    
    switch (node->type) {
        case NODE_BINARY:
            node->data.binary.left = fold_constants(node->data.binary.left);
            node->data.binary.right = fold_constants(node->data.binary.right);
            return fold_binary_constant(node);
        case NODE_ASSIGN:
            node->data.assign.expr = fold_constants(node->data.assign.expr);
            return node;
        default:
            return node;
    }
}

void optimize_ast(ASTNode *node) {
    if (!node) return;
    
    if (node->type == NODE_PROGRAM) {
        ASTNode *funcs = node->data.program.funcs;
        while (funcs) {
            if (funcs->type == NODE_FUNC_LIST) {
                optimize_stmt(funcs->data.list.node);
                funcs = funcs->data.list.next;
            } else {
                optimize_stmt(funcs);
                funcs = NULL;
            }
        }
    } else {
        optimize_stmt(node);
    }
}
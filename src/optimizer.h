#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ast.h"

// Constant folding on AST
void optimize_ast(ASTNode *node);

// Check if a node is a constant (int or float literal)
int is_constant_node(ASTNode *node);

// Evaluate constant binary expression
int eval_constant_int(ASTNode *node);
float eval_constant_float(ASTNode *node);

// Fold constants in an expression
ASTNode* fold_constants(ASTNode *node);

#endif
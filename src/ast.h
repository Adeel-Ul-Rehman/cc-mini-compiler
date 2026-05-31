#ifndef AST_H
#define AST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NODE_PROGRAM, NODE_DECL_LIST, NODE_DECL, NODE_FUNC_LIST, NODE_FUNC,
    NODE_PARAM_LIST, NODE_BLOCK, NODE_STMT_LIST, NODE_ASSIGN,
    NODE_IF, NODE_WHILE, NODE_FOR, NODE_RETURN, NODE_INPUT, NODE_OUTPUT,
    NODE_BINARY, NODE_INT_LIT, NODE_FLOAT_LIT, NODE_BOOL_LIT, NODE_STRING_LIT,
    NODE_VAR, NODE_TYPE,
    NODE_ARRAY_DECL, NODE_ARRAY_ACCESS, NODE_ARRAY_INIT, NODE_ARRAY_ASSIGN, NODE_INPUT_ARRAY
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char *inferred_type;
    union {
        struct {
            struct ASTNode *decls;
            struct ASTNode *funcs;
        } program;
        struct {
            struct ASTNode *next;
            struct ASTNode *node;
        } list;
        struct {
            struct ASTNode *type;
            char *name;
            struct ASTNode *init;
        } decl;
        struct {
            struct ASTNode *ret_type;
            char *name;
            struct ASTNode *params;
            struct ASTNode *body;
        } func;
        struct {
            struct ASTNode *next;
            struct ASTNode *type;
            char *name;
        } param;
        struct {
            struct ASTNode *stmts;
        } block;
        struct {
            char *name;
            struct ASTNode *expr;
        } assign;
        struct {
            struct ASTNode *cond;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stmt;
        struct {
            struct ASTNode *cond;
            struct ASTNode *body;
        } while_stmt;
        struct {
            struct ASTNode *init;
            struct ASTNode *cond;
            struct ASTNode *inc;
            struct ASTNode *body;
        } for_stmt;
        struct {
            struct ASTNode *expr;
        } ret;
        struct {
            char *var;
        } input;
        struct {
            struct ASTNode *expr;
        } output;
        struct {
            const char *op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary;
        int ival;
        float fval;
        char *sval;
        // Array nodes
        struct {
            struct ASTNode *type;
            char *name;
            struct ASTNode *size;
            struct ASTNode *init_list;
        } array_decl;
        struct {
            char *name;
            struct ASTNode *index;
        } array_access;
        struct {
            struct ASTNode *lvalue;
            struct ASTNode *expr;
        } array_assign;
        struct {
            struct ASTNode *access;
        } input_array;
    } data;
} ASTNode;

// Constructor functions
ASTNode* ast_program(ASTNode *decls, ASTNode *funcs);
ASTNode* ast_decl_list(ASTNode *next, ASTNode *node);
ASTNode* ast_declaration(ASTNode *type, char *name, ASTNode *init);
ASTNode* ast_func_list(ASTNode *next, ASTNode *node);
ASTNode* ast_function(ASTNode *ret_type, char *name, ASTNode *params, ASTNode *body);
ASTNode* ast_param_list(ASTNode *next, ASTNode *type, char *name);
ASTNode* ast_block(ASTNode *stmts);
ASTNode* ast_stmt_list(ASTNode *next, ASTNode *node);
ASTNode* ast_assignment(char *name, ASTNode *expr);
ASTNode* ast_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch);
ASTNode* ast_while(ASTNode *cond, ASTNode *body);
ASTNode* ast_for(ASTNode *init, ASTNode *cond, ASTNode *inc, ASTNode *body);
ASTNode* ast_return(ASTNode *expr);
ASTNode* ast_input(char *var);
ASTNode* ast_output(ASTNode *expr);
ASTNode* ast_binary(const char *op, ASTNode *left, ASTNode *right);
ASTNode* ast_int_lit(int val);
ASTNode* ast_float_lit(float val);
ASTNode* ast_bool_lit(int val);
ASTNode* ast_string_lit(char *val);
ASTNode* ast_var(char *name);
ASTNode* ast_type(char *name);

// Array functions
ASTNode* ast_array_decl(ASTNode *type, char *name, ASTNode *size, ASTNode *init);
ASTNode* ast_array_access(char *name, ASTNode *index);
ASTNode* ast_array_init_list(ASTNode *next, ASTNode *value);
ASTNode* ast_array_assign(ASTNode *lvalue, ASTNode *expr);
ASTNode* ast_input_array(ASTNode *access);

// Type functions
const char* ast_get_type(ASTNode *node);
void ast_set_type(ASTNode *node, const char *type);

// Print and free
void ast_print(ASTNode *node, int indent);
void ast_free(ASTNode *node);

// Optimization
ASTNode* ast_try_fold(ASTNode *node);

#ifdef __cplusplus
}
#endif

#endif
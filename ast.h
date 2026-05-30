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
    NODE_VAR, NODE_TYPE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char *inferred_type;   // for type checking (e.g., "int", "float", "bool", "void", "string")
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
    } data;
} ASTNode;

// Constructor functions (same as before)
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

// Type helper functions
const char* ast_get_type(ASTNode *node);
void ast_set_type(ASTNode *node, const char *type);

void ast_print(ASTNode *node, int indent);
void ast_free(ASTNode *node);

#ifdef __cplusplus
}
#endif

#endif
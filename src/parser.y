%{
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "ast.h"
#include "symbol_table.h"
#include "error_handler.h"

extern int yylex();
extern int yylineno;
void yyerror(const char *s);

ASTNode *program_root;
extern SymbolTable *symtab;
extern ErrorList *error_list;

int semantic_errors = 0;

const char* get_type_name(ASTNode *type) {
    if (!type || type->type != NODE_TYPE) return "unknown";
    return type->data.sval;
}

void semantic_error(const char *msg) {
    error_list_add(error_list, yylineno, "%s", msg);
    semantic_errors = 1;
}

const char* get_factor_type(ASTNode *node) {
    if (!node) return "unknown";
    switch (node->type) {
        case NODE_INT_LIT: return "int";
        case NODE_FLOAT_LIT: return "float";
        case NODE_BOOL_LIT: return "int";
        case NODE_STRING_LIT: return "string";
        case NODE_ARRAY_ACCESS:
            return "int";  // Array elements are int for now
        case NODE_VAR: {
            char *name = node->data.sval;
            Symbol *sym = lookup_symbol(symtab, name);
            if (sym && sym->type) {
                if (sym->is_array) {
                    return "int";  // Array element type
                }
                return get_type_name(sym->type);
            }
            return "unknown";
        }
        default: return "unknown";
    }
}
%}

%union {
    int ival;
    float fval;
    char *sval;
    struct ASTNode *node;
}

%token TYPE_VOID TYPE_INT TYPE_FLOAT TYPE_BOOL
%token IF ELSE WHILE FOR RETURN INPUT OUTPUT
%token EQ NE LE GE AND OR ASSIGN
%token PLUS MINUS STAR SLASH LT GT
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA
%token LBRACK RBRACK
%token <ival> INT_LIT BOOL_LIT
%token <fval> FLOAT_LIT
%token <sval> IDENTIFIER STRING_LIT

%type <node> program top_list top
%type <node> function decl
%type <node> type_spec param_opt param_list
%type <node> block block_item_list block_item
%type <node> stmt stmt_list
%type <node> assign_stmt if_stmt while_stmt for_stmt return_stmt input_stmt output_stmt
%type <node> expr logical_or logical_and equality relational additive multiplicative factor
%type <node> array_decl array_access array_init_list

%start program

%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left STAR SLASH

%%

program:
    top_list   { program_root = ast_program(NULL, $1); }
    ;

top_list:
    /* empty */               { $$ = NULL; }
    | top_list top            { $$ = ast_func_list($1, $2); }
    ;

top:
    decl                      { $$ = $1; }
    | function                { $$ = $1; }
    ;

decl:
    type_spec IDENTIFIER SEMICOLON {
        if (lookup_symbol_current_scope(symtab, $2)) {
            error_list_add(error_list, yylineno, "Variable '%s' already declared in this scope", $2);
        } else {
            insert_symbol(symtab, $2, $1);
        }
        $$ = ast_declaration($1, $2, NULL);
    }
    | type_spec IDENTIFIER ASSIGN expr SEMICOLON {
        const char *spec = get_type_name($1);
        const char *expr_type = "unknown";
        if ($4) {
            if ($4->type == NODE_FLOAT_LIT) expr_type = "float";
            else if ($4->type == NODE_INT_LIT) expr_type = "int";
            else if ($4->type == NODE_BOOL_LIT) expr_type = "int";
            else if ($4->type == NODE_STRING_LIT) expr_type = "string";
            else if ($4->type == NODE_VAR) {
                Symbol *sym = lookup_symbol(symtab, $4->data.sval);
                if (sym && sym->type) expr_type = get_type_name(sym->type);
            }
            else expr_type = ast_get_type($4);
        }
        if (strcmp(spec, expr_type) != 0) {
            error_list_add(error_list, yylineno, "Type mismatch in initialization of '%s' (%s vs %s)", $2, spec, expr_type);
        } else {
            if (lookup_symbol_current_scope(symtab, $2)) {
                error_list_add(error_list, yylineno, "Variable '%s' already declared in this scope", $2);
            } else {
                insert_symbol(symtab, $2, $1);
            }
        }
        $$ = ast_declaration($1, $2, $4);
    }
    | type_spec IDENTIFIER LBRACK expr RBRACK SEMICOLON {
        // Array declaration without initialization
        // Check if size is constant
        $$ = ast_array_decl($1, $2, $4, NULL);
        // Insert into symbol table
        if (lookup_symbol_current_scope(symtab, $2)) {
            error_list_add(error_list, yylineno, "Array '%s' already declared", $2);
        } else {
            // Get size if constant
            int size = 0;
            if ($4 && $4->type == NODE_INT_LIT) {
                size = $4->data.ival;
            }
            insert_array_symbol(symtab, $2, $1, size);
        }
    }
    | type_spec IDENTIFIER LBRACK expr RBRACK ASSIGN LBRACE array_init_list RBRACE SEMICOLON {
        // Array declaration with initialization
        $$ = ast_array_decl($1, $2, $4, $8);
        if (lookup_symbol_current_scope(symtab, $2)) {
            error_list_add(error_list, yylineno, "Array '%s' already declared", $2);
        } else {
            int size = 0;
            if ($4 && $4->type == NODE_INT_LIT) {
                size = $4->data.ival;
            }
            insert_array_symbol(symtab, $2, $1, size);
        }
    }
    | error SEMICOLON {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid declaration, skipping to next semicolon");
        $$ = NULL;
    }
    ;

function:
    type_spec IDENTIFIER LPAREN param_opt RPAREN block {
        if (lookup_symbol_current_scope(symtab, $2)) {
            error_list_add(error_list, yylineno, "Function '%s' already declared", $2);
        } else {
            insert_symbol(symtab, $2, $1);
        }
        $$ = ast_function($1, $2, $4, $6);
    }
    | error RPAREN block {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid function declaration, skipping to closing parenthesis");
        $$ = NULL;
    }
    ;

param_opt:
    /* empty */      { $$ = NULL; }
    | param_list     { $$ = $1; }
    ;

param_list:
    type_spec IDENTIFIER {
        insert_symbol(symtab, $2, $1);
        $$ = ast_param_list(NULL, $1, $2);
    }
    | param_list COMMA type_spec IDENTIFIER {
        insert_symbol(symtab, $4, $3);
        $$ = ast_param_list($1, $3, $4);
    }
    ;

block:
    LBRACE { enter_scope(symtab); } block_item_list RBRACE { exit_scope(symtab); $$ = ast_block($3); }
    | error RBRACE {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid block content, skipping to closing brace");
        $$ = NULL;
    }
    ;

block_item_list:
    /* empty */               { $$ = NULL; }
    | block_item_list block_item { $$ = ast_stmt_list($1, $2); }
    ;

block_item:
    decl                      { $$ = $1; }
    | stmt                    { $$ = $1; }
    ;

stmt_list:
    /* empty */               { $$ = NULL; }
    | stmt_list stmt          { $$ = ast_stmt_list($1, $2); }
    ;

stmt:
    assign_stmt SEMICOLON     { $$ = $1; }
    | if_stmt                 { $$ = $1; }
    | while_stmt              { $$ = $1; }
    | for_stmt                { $$ = $1; }
    | return_stmt SEMICOLON   { $$ = $1; }
    | input_stmt SEMICOLON    { $$ = $1; }
    | output_stmt SEMICOLON   { $$ = $1; }
    | block                   { $$ = $1; }
    | error SEMICOLON {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid statement, skipping to semicolon");
        $$ = NULL;
    }
    ;

assign_stmt:
    IDENTIFIER ASSIGN expr {
        Symbol *sym = lookup_symbol(symtab, $1);
        if (!sym) {
            error_list_add(error_list, yylineno, "Variable '%s' not declared", $1);
        } else if (sym->is_array) {
            error_list_add(error_list, yylineno, "Cannot assign to array '%s' without index", $1);
        } else {
            const char *var_type = get_type_name(sym->type);
            const char *expr_type = "unknown";
            if ($3) {
                if ($3->type == NODE_FLOAT_LIT) expr_type = "float";
                else if ($3->type == NODE_INT_LIT) expr_type = "int";
                else if ($3->type == NODE_BOOL_LIT) expr_type = "int";
                else if ($3->type == NODE_STRING_LIT) expr_type = "string";
                else if ($3->type == NODE_VAR) {
                    Symbol *s = lookup_symbol(symtab, $3->data.sval);
                    if (s && s->type) expr_type = get_type_name(s->type);
                }
                else expr_type = ast_get_type($3);
            }
            if (strcmp(var_type, expr_type) != 0) {
                error_list_add(error_list, yylineno, "Type mismatch assigning '%s' to '%s'", expr_type, var_type);
            }
        }
        $$ = ast_assignment($1, $3);
    }
    | IDENTIFIER LBRACK expr RBRACK ASSIGN expr {
        // Array element assignment: arr[index] = value
        Symbol *sym = lookup_symbol(symtab, $1);
        if (!sym) {
            error_list_add(error_list, yylineno, "Array '%s' not declared", $1);
        } else if (!sym->is_array) {
            error_list_add(error_list, yylineno, "'%s' is not an array", $1);
        } else {
            const char *expr_type = "unknown";
            if ($6) {
                if ($6->type == NODE_FLOAT_LIT) expr_type = "float";
                else if ($6->type == NODE_INT_LIT) expr_type = "int";
                else if ($6->type == NODE_BOOL_LIT) expr_type = "int";
                else if ($6->type == NODE_STRING_LIT) expr_type = "string";
                else if ($6->type == NODE_VAR) {
                    Symbol *s = lookup_symbol(symtab, $6->data.sval);
                    if (s && s->type) expr_type = get_type_name(s->type);
                }
                else expr_type = ast_get_type($6);
            }
            // Array elements are int type for now
            if (strcmp("int", expr_type) != 0 && strcmp("float", expr_type) != 0) {
                error_list_add(error_list, yylineno, "Type mismatch in array assignment");
            }
        }
        ASTNode *access = ast_array_access($1, $3);
        $$ = ast_array_assign(access, $6);
    }
    ;

if_stmt:
    IF LPAREN expr RPAREN stmt ELSE stmt   { $$ = ast_if($3, $5, $7); }
    | IF LPAREN expr RPAREN stmt           { $$ = ast_if($3, $5, NULL); }
    | IF error stmt ELSE stmt {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid if condition, skipping");
        $$ = NULL;
    }
    ;

while_stmt:
    WHILE LPAREN expr RPAREN stmt   { $$ = ast_while($3, $5); }
    | WHILE error stmt {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid while condition, skipping");
        $$ = NULL;
    }
    ;

for_stmt:
    FOR LPAREN assign_stmt SEMICOLON expr SEMICOLON assign_stmt RPAREN stmt
        { $$ = ast_for($3, $5, $7, $9); }
    | FOR error stmt {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid for loop declaration, skipping");
        $$ = NULL;
    }
    ;

return_stmt:
    RETURN expr { $$ = ast_return($2); }
    | RETURN error {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid return statement, skipping");
        $$ = NULL;
    }
    ;

input_stmt:
    INPUT LPAREN IDENTIFIER RPAREN {
        Symbol *sym = lookup_symbol(symtab, $3);
        if (!sym) {
            error_list_add(error_list, yylineno, "Variable '%s' not declared", $3);
        }
        $$ = ast_input($3);
    }
    | INPUT LPAREN IDENTIFIER LBRACK expr RBRACK RPAREN {
        // Input into array element: input(arr[index])
        Symbol *sym = lookup_symbol(symtab, $3);
        if (!sym) {
            error_list_add(error_list, yylineno, "Array '%s' not declared", $3);
        } else if (!sym->is_array) {
            error_list_add(error_list, yylineno, "'%s' is not an array", $3);
        }
        ASTNode *access = ast_array_access($3, $5);
        $$ = ast_input_array(access);
    }
    | INPUT error RPAREN {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid input statement, skipping");
        $$ = NULL;
    }
    ;

output_stmt:
    OUTPUT LPAREN expr RPAREN { $$ = ast_output($3); }
    | OUTPUT error RPAREN {
        yyclearin;
        error_list_add(error_list, yylineno, "Invalid output statement, skipping");
        $$ = NULL;
    }
    ;

array_init_list:
    expr { $$ = ast_array_init_list(NULL, $1); }
    | array_init_list COMMA expr { $$ = ast_array_init_list($1, $3); }
    ;

expr:
    logical_or   { $$ = $1; ast_set_type($$, ast_get_type($1)); }
    ;

logical_or:
    logical_and                 { $$ = $1; ast_set_type($$, ast_get_type($1)); }
    | logical_or OR logical_and { $$ = ast_binary("||", $1, $3); ast_set_type($$, "int"); }
    ;

logical_and:
    equality                     { $$ = $1; ast_set_type($$, ast_get_type($1)); }
    | logical_and AND equality   { $$ = ast_binary("&&", $1, $3); ast_set_type($$, "int"); }
    ;

equality:
    relational                   { $$ = $1; ast_set_type($$, ast_get_type($1)); }
    | equality EQ relational     { $$ = ast_binary("==", $1, $3); ast_set_type($$, "int"); }
    | equality NE relational     { $$ = ast_binary("!=", $1, $3); ast_set_type($$, "int"); }
    ;

relational:
    additive                     { $$ = $1; ast_set_type($$, ast_get_type($1)); }
    | relational LT additive     { $$ = ast_binary("<", $1, $3); ast_set_type($$, "int"); }
    | relational GT additive     { $$ = ast_binary(">", $1, $3); ast_set_type($$, "int"); }
    | relational LE additive     { $$ = ast_binary("<=", $1, $3); ast_set_type($$, "int"); }
    | relational GE additive     { $$ = ast_binary(">=", $1, $3); ast_set_type($$, "int"); }
    ;

additive:
    multiplicative               { $$ = $1; ast_set_type($$, ast_get_type($1)); }
    | additive PLUS multiplicative   { $$ = ast_binary("+", $1, $3); ast_set_type($$, "int"); }
    | additive MINUS multiplicative  { $$ = ast_binary("-", $1, $3); ast_set_type($$, "int"); }
    ;

multiplicative:
    factor                       { $$ = $1; ast_set_type($$, ast_get_type($1)); }
    | multiplicative STAR factor { $$ = ast_binary("*", $1, $3); ast_set_type($$, "int"); }
    | multiplicative SLASH factor { $$ = ast_binary("/", $1, $3); ast_set_type($$, "int"); }
    ;

factor:
    INT_LIT            { $$ = ast_int_lit($1); ast_set_type($$, "int"); }
    | FLOAT_LIT        { $$ = ast_float_lit($1); ast_set_type($$, "float"); }
    | BOOL_LIT         { $$ = ast_bool_lit($1); ast_set_type($$, "int"); }
    | IDENTIFIER       {
        $$ = ast_var($1);
        Symbol *sym = lookup_symbol(symtab, $1);
        if (!sym) {
            error_list_add(error_list, yylineno, "Variable '%s' not declared", $1);
            ast_set_type($$, "unknown");
        } else if (sym->is_array) {
            error_list_add(error_list, yylineno, "Array '%s' used without index", $1);
            ast_set_type($$, "unknown");
        } else {
            ast_set_type($$, get_type_name(sym->type));
        }
    }
    | STRING_LIT       { $$ = ast_string_lit($1); ast_set_type($$, "string"); }
    | IDENTIFIER LBRACK expr RBRACK {
        $$ = ast_array_access($1, $3);
        Symbol *sym = lookup_symbol(symtab, $1);
        if (!sym) {
            error_list_add(error_list, yylineno, "Array '%s' not declared", $1);
            ast_set_type($$, "unknown");
        } else if (!sym->is_array) {
            error_list_add(error_list, yylineno, "'%s' is not an array", $1);
            ast_set_type($$, "unknown");
        } else {
            ast_set_type($$, "int");  // Array elements are int
        }
    }
    | LPAREN expr RPAREN  { $$ = $2; ast_set_type($$, ast_get_type($2)); }
    ;

type_spec:
    TYPE_VOID   { $$ = ast_type("void"); }
    | TYPE_INT    { $$ = ast_type("int"); }
    | TYPE_FLOAT { $$ = ast_type("float"); }
    | TYPE_BOOL  { $$ = ast_type("bool"); }
    ;

%%

void yyerror(const char *s) {
    error_list_add(error_list, yylineno, "Syntax error: %s", s);
    semantic_errors = 1;
}
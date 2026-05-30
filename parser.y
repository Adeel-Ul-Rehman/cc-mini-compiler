%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

extern int yylex();
extern int yylineno;
void yyerror(const char *s);

ASTNode *program_root;
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
%token <ival> INT_LIT BOOL_LIT
%token <fval> FLOAT_LIT
%token <sval> IDENTIFIER STRING_LIT

%type <node> program declaration_list declaration function_list function
%type <node> param_list statement_list statement
%type <node> assignment_stmt if_stmt while_stmt for_stmt return_stmt input_stmt output_stmt
%type <node> block expression logical_or logical_and equality relational additive multiplicative
%type <node> factor type_spec

%start program

%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left STAR SLASH

%%

program:
    declaration_list function_list   { program_root = ast_program($1, $2); }
    ;

declaration_list:
    /* empty */            { $$ = NULL; }
    | declaration_list declaration { $$ = ast_decl_list($1, $2); }
    ;

declaration:
    type_spec IDENTIFIER SEMICOLON   { $$ = ast_declaration($1, $2, NULL); }
    | type_spec IDENTIFIER ASSIGN expression SEMICOLON { $$ = ast_declaration($1, $2, $4); }
    ;

function_list:
    /* empty */            { $$ = NULL; }
    | function_list function { $$ = ast_func_list($1, $2); }
    ;

function:
    type_spec IDENTIFIER LPAREN RPAREN block
        { $$ = ast_function($1, $2, NULL, $5); }
    | type_spec IDENTIFIER LPAREN param_list RPAREN block
        { $$ = ast_function($1, $2, $4, $6); }
    ;

param_list:
    type_spec IDENTIFIER
        { $$ = ast_param_list(NULL, $1, $2); }
    | param_list COMMA type_spec IDENTIFIER
        { $$ = ast_param_list($1, $3, $4); }
    ;

block:
    LBRACE statement_list RBRACE { $$ = ast_block($2); }
    ;

statement_list:
    /* empty */            { $$ = NULL; }
    | statement_list statement { $$ = ast_stmt_list($1, $2); }
    ;

statement:
    assignment_stmt SEMICOLON { $$ = $1; }
    | if_stmt              { $$ = $1; }
    | while_stmt           { $$ = $1; }
    | for_stmt             { $$ = $1; }
    | return_stmt SEMICOLON { $$ = $1; }
    | input_stmt SEMICOLON { $$ = $1; }
    | output_stmt SEMICOLON { $$ = $1; }
    | block                { $$ = $1; }
    ;

assignment_stmt:
    IDENTIFIER ASSIGN expression { $$ = ast_assignment($1, $3); }
    ;

if_stmt:
    IF LPAREN expression RPAREN statement ELSE statement { $$ = ast_if($3, $5, $7); }
    | IF LPAREN expression RPAREN statement { $$ = ast_if($3, $5, NULL); }
    ;

while_stmt:
    WHILE LPAREN expression RPAREN statement { $$ = ast_while($3, $5); }
    ;

for_stmt:
    FOR LPAREN assignment_stmt SEMICOLON expression SEMICOLON assignment_stmt RPAREN statement
        { $$ = ast_for($3, $5, $7, $9); }
    ;

return_stmt:
    RETURN expression { $$ = ast_return($2); }
    ;

input_stmt:
    INPUT LPAREN IDENTIFIER RPAREN { $$ = ast_input($3); }
    ;

output_stmt:
    OUTPUT LPAREN expression RPAREN { $$ = ast_output($3); }
    ;

expression:
    logical_or { $$ = $1; }
    ;

logical_or:
    logical_and { $$ = $1; }
    | logical_or OR logical_and { $$ = ast_binary("||", $1, $3); }
    ;

logical_and:
    equality { $$ = $1; }
    | logical_and AND equality { $$ = ast_binary("&&", $1, $3); }
    ;

equality:
    relational { $$ = $1; }
    | equality EQ relational { $$ = ast_binary("==", $1, $3); }
    | equality NE relational { $$ = ast_binary("!=", $1, $3); }
    ;

relational:
    additive { $$ = $1; }
    | relational LT additive { $$ = ast_binary("<", $1, $3); }
    | relational GT additive { $$ = ast_binary(">", $1, $3); }
    | relational LE additive { $$ = ast_binary("<=", $1, $3); }
    | relational GE additive { $$ = ast_binary(">=", $1, $3); }
    ;

additive:
    multiplicative { $$ = $1; }
    | additive PLUS multiplicative { $$ = ast_binary("+", $1, $3); }
    | additive MINUS multiplicative { $$ = ast_binary("-", $1, $3); }
    ;

multiplicative:
    factor { $$ = $1; }
    | multiplicative STAR factor { $$ = ast_binary("*", $1, $3); }
    | multiplicative SLASH factor { $$ = ast_binary("/", $1, $3); }
    ;

factor:
    INT_LIT              { $$ = ast_int_lit($1); }
    | FLOAT_LIT          { $$ = ast_float_lit($1); }
    | BOOL_LIT           { $$ = ast_bool_lit($1); }
    | IDENTIFIER         { $$ = ast_var($1); }
    | STRING_LIT         { $$ = ast_string_lit($1); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

type_spec:
    TYPE_VOID  { $$ = ast_type("void"); }
    | TYPE_INT    { $$ = ast_type("int"); }
    | TYPE_FLOAT { $$ = ast_type("float"); }
    | TYPE_BOOL  { $$ = ast_type("bool"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", yylineno, s);
}
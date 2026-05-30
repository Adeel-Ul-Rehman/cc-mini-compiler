#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Symbol {
    char *name;
    ASTNode *type;
    int scope_level;
    struct Symbol *next;
} Symbol;

typedef struct SymbolTable {
    Symbol **buckets;
    int size;
    int current_scope;
} SymbolTable;

SymbolTable* create_symbol_table();
void destroy_symbol_table(SymbolTable *st);
void enter_scope(SymbolTable *st);
void exit_scope(SymbolTable *st);
void insert_symbol(SymbolTable *st, char *name, ASTNode *type);
Symbol* lookup_symbol_current_scope(SymbolTable *st, char *name);
Symbol* lookup_symbol(SymbolTable *st, char *name);
void print_symbol_table(SymbolTable *st);

#ifdef __cplusplus
}
#endif

#endif
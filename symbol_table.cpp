#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 211

static unsigned int hash(char *name) {
    unsigned int h = 0;
    while(*name) h = (h << 5) + *name++;
    return h % TABLE_SIZE;
}

SymbolTable* create_symbol_table() {
    SymbolTable *st = (SymbolTable*)malloc(sizeof(SymbolTable));
    st->buckets = (Symbol**)calloc(TABLE_SIZE, sizeof(Symbol*));
    st->size = TABLE_SIZE;
    st->current_scope = 0;
    return st;
}

void destroy_symbol_table(SymbolTable *st) {
    for(int i=0;i<TABLE_SIZE;i++) {
        Symbol *s = st->buckets[i];
        while(s) {
            Symbol *next = s->next;
            free(s->name);
            free(s);
            s = next;
        }
    }
    free(st->buckets);
    free(st);
}

void enter_scope(SymbolTable *st) {
    st->current_scope++;
}

void exit_scope(SymbolTable *st) {
    for(int i=0;i<TABLE_SIZE;i++) {
        Symbol *prev = NULL;
        Symbol *s = st->buckets[i];
        while(s) {
            if(s->scope_level == st->current_scope) {
                if(prev) prev->next = s->next;
                else st->buckets[i] = s->next;
                Symbol *to_free = s;
                s = s->next;
                free(to_free->name);
                free(to_free);
            } else {
                prev = s;
                s = s->next;
            }
        }
    }
    st->current_scope--;
}


Symbol* lookup_symbol_current_scope(SymbolTable *st, char *name) {
    unsigned int idx = hash(name);
    Symbol *s = st->buckets[idx];
    while(s) {
        if(strcmp(s->name, name)==0 && s->scope_level == st->current_scope) return s;
        s = s->next;
    }
    return NULL;
}

void insert_symbol(SymbolTable *st, char *name, ASTNode *type) {
    unsigned int idx = hash(name);
    Symbol *sym = (Symbol*)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->scope_level = st->current_scope;
    sym->next = st->buckets[idx];
    st->buckets[idx] = sym;
}

Symbol* lookup_symbol(SymbolTable *st, char *name) {
    unsigned int idx = hash(name);
    Symbol *s = st->buckets[idx];
    while(s) {
        if(strcmp(s->name, name)==0) return s;
        s = s->next;
    }
    return NULL;
}

void print_symbol_table(SymbolTable *st) {
    printf("\n--- Symbol Table ---\n");
    for (int scope = 0; scope <= st->current_scope; scope++) {
        printf("Scope %d:\n", scope);
        for (int i = 0; i < st->size; i++) {
            Symbol *s = st->buckets[i];
            while (s) {
                if (s->scope_level == scope) {
                    const char *type_name = "unknown";
                    if (s->type && s->type->type == NODE_TYPE)
                        type_name = s->type->data.sval;
                    printf("  %s : %s\n", s->name, type_name);
                }
                s = s->next;
            }
        }
    }
}
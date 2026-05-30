#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "tac.h"

typedef struct {
    char **names;
    char **values;
    int count;
    int capacity;
} Memory;

Memory* memory_create();
void memory_set(Memory *mem, const char *name, const char *value);
char* memory_get(Memory *mem, const char *name);
void memory_free(Memory *mem);

void interpreter_execute(TacProgram *prog);

#endif
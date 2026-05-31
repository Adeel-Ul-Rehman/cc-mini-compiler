#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ArrayData {
    char *name;
    int size;
    char **elements;
    struct ArrayData *next;
} ArrayData;

typedef struct Memory {
    char **names;
    char **values;
    int count;
    int capacity;
    ArrayData *arrays;
} Memory;

Memory* memory_create() {
    Memory *mem = (Memory*)malloc(sizeof(Memory));
    mem->names = NULL;
    mem->values = NULL;
    mem->count = 0;
    mem->capacity = 0;
    mem->arrays = NULL;
    return mem;
}

static void array_store(Memory *mem, const char *name, int index, const char *value) {
    ArrayData *arr = mem->arrays;
    while (arr) {
        if (strcmp(arr->name, name) == 0) {
            if (index >= 0 && index < arr->size) {
                if (arr->elements[index]) free(arr->elements[index]);
                arr->elements[index] = strdup(value);
            }
            return;
        }
        arr = arr->next;
    }
    
    arr = (ArrayData*)malloc(sizeof(ArrayData));
    arr->name = strdup(name);
    arr->size = 100;
    arr->elements = (char**)calloc(arr->size, sizeof(char*));
    arr->next = mem->arrays;
    mem->arrays = arr;
    if (index >= 0 && index < arr->size) {
        arr->elements[index] = strdup(value);
    }
}

static char* array_load(Memory *mem, const char *name, int index) {
    ArrayData *arr = mem->arrays;
    while (arr) {
        if (strcmp(arr->name, name) == 0) {
            if (index >= 0 && index < arr->size) {
                if (arr->elements[index]) return arr->elements[index];
                return "0";
            }
            return "0";
        }
        arr = arr->next;
    }
    return "0";
}

void memory_set(Memory *mem, const char *name, const char *value) {
    if (!mem || !name) return;
    for (int i = 0; i < mem->count; i++) {
        if (strcmp(mem->names[i], name) == 0) {
            free(mem->values[i]);
            mem->values[i] = strdup(value);
            return;
        }
    }
    if (mem->count >= mem->capacity) {
        mem->capacity = mem->capacity == 0 ? 10 : mem->capacity * 2;
        mem->names = (char**)realloc(mem->names, mem->capacity * sizeof(char*));
        mem->values = (char**)realloc(mem->values, mem->capacity * sizeof(char*));
    }
    mem->names[mem->count] = strdup(name);
    mem->values[mem->count] = strdup(value);
    mem->count++;
}

char* memory_get(Memory *mem, const char *name) {
    if (!mem || !name) return NULL;
    for (int i = 0; i < mem->count; i++) {
        if (strcmp(mem->names[i], name) == 0) return mem->values[i];
    }
    return NULL;
}

void memory_free(Memory *mem) {
    if (!mem) return;
    for (int i = 0; i < mem->count; i++) {
        free(mem->names[i]);
        free(mem->values[i]);
    }
    free(mem->names);
    free(mem->values);
    ArrayData *arr = mem->arrays;
    while (arr) {
        ArrayData *next = arr->next;
        free(arr->name);
        for (int i = 0; i < arr->size; i++) {
            if (arr->elements[i]) free(arr->elements[i]);
        }
        free(arr->elements);
        free(arr);
        arr = next;
    }
    free(mem);
}

static char* get_value(Memory *mem, const char *name) {
    if (!name) return NULL;
    if (name[0] >= '0' && name[0] <= '9') return (char*)name;
    char *val = memory_get(mem, name);
    if (val) return val;
    return (char*)name;
}

void interpreter_execute(TacProgram *prog) {
    if (!prog || !prog->head) {
        printf("No TAC to execute.\n");
        return;
    }
    
    Memory *mem = memory_create();
    
    typedef struct Label {
        char *name;
        TacInstr *instr;
        struct Label *next;
    } Label;
    
    Label *labels = NULL;
    TacInstr *curr = prog->head;
    while (curr) {
        if (curr->op == TAC_LABEL) {
            Label *l = (Label*)malloc(sizeof(Label));
            l->name = strdup(curr->result);
            l->instr = curr->next;
            l->next = labels;
            labels = l;
        }
        curr = curr->next;
    }
    
    auto find_label = [&](const char *name) -> TacInstr* {
        Label *l = labels;
        while (l) {
            if (strcmp(l->name, name) == 0) return l->instr;
            l = l->next;
        }
        return NULL;
    };
    
    printf("\n=== Program Output ===\n");
    
    TacInstr *instr = prog->head;
    int step = 0;
    while (instr && step < 500) {
        step++;
        switch (instr->op) {
            case TAC_ASSIGN: {
                char *val = get_value(mem, instr->arg1);
                if (val) memory_set(mem, instr->result, val);
                break;
            }
            case TAC_ADD: {
                char *l = get_value(mem, instr->arg1);
                char *r = get_value(mem, instr->arg2);
                if (l && r) {
                    float res = atof(l) + atof(r);
                    char buf[64];
                    sprintf(buf, "%g", res);
                    memory_set(mem, instr->result, buf);
                }
                break;
            }
            case TAC_SUB: {
                char *l = get_value(mem, instr->arg1);
                char *r = get_value(mem, instr->arg2);
                if (l && r) {
                    float res = atof(l) - atof(r);
                    char buf[64];
                    sprintf(buf, "%g", res);
                    memory_set(mem, instr->result, buf);
                }
                break;
            }
            case TAC_MUL: {
                char *l = get_value(mem, instr->arg1);
                char *r = get_value(mem, instr->arg2);
                if (l && r) {
                    float res = atof(l) * atof(r);
                    char buf[64];
                    sprintf(buf, "%g", res);
                    memory_set(mem, instr->result, buf);
                }
                break;
            }
            case TAC_DIV: {
                char *l = get_value(mem, instr->arg1);
                char *r = get_value(mem, instr->arg2);
                if (l && r && atof(r) != 0) {
                    float res = atof(l) / atof(r);
                    char buf[64];
                    sprintf(buf, "%g", res);
                    memory_set(mem, instr->result, buf);
                }
                break;
            }
            case TAC_OUTPUT: {
                char *val = get_value(mem, instr->result);
                if (val && val[0] != 't') {
                    printf("%s\n", val);
                }
                break;
            }
            case TAC_INPUT: {
                char buf[256];
                printf("? ");
                fflush(stdout);
                fgets(buf, sizeof(buf), stdin);
                buf[strcspn(buf, "\n")] = '\0';
                memory_set(mem, instr->result, buf);
                break;
            }
            case TAC_RETURN:
                break;
            case TAC_GOTO: {
                TacInstr *target = find_label(instr->result);
                if (target) { instr = target; continue; }
                break;
            }
            case TAC_IFGOTO: {
                char *cond_val = get_value(mem, instr->result);
                int cond = cond_val ? atoi(cond_val) : 0;
                if (cond != 0) {
                    TacInstr *target = find_label(instr->arg1);
                    if (target) { instr = target; continue; }
                }
                break;
            }
            case TAC_ARRAY_STORE: {
                char *idx = get_value(mem, instr->arg1);
                char *val = get_value(mem, instr->arg2);
                if (idx && val) {
                    array_store(mem, instr->result, atoi(idx), val);
                }
                break;
            }
            case TAC_ARRAY_LOAD: {
                char *idx = get_value(mem, instr->arg2);
                if (idx) {
                    char *val = array_load(mem, instr->arg1, atoi(idx));
                    memory_set(mem, instr->result, val);
                }
                break;
            }
            case TAC_LABEL:
                break;
            default:
                break;
        }
        instr = instr->next;
    }
    
    if (step >= 500) printf("\n!!! Stopped after 500 steps !!!\n");
    printf("\n========================\n");
    
    Label *l = labels;
    while (l) {
        Label *next = l->next;
        free(l->name);
        free(l);
        l = next;
    }
    memory_free(mem);
}
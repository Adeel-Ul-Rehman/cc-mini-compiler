#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Memory* memory_create() {
    Memory *mem = (Memory*)malloc(sizeof(Memory));
    mem->names = NULL;
    mem->values = NULL;
    mem->count = 0;
    mem->capacity = 0;
    return mem;
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
        if (strcmp(mem->names[i], name) == 0) {
            return mem->values[i];
        }
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
    free(mem);
}

static char* get_value(Memory *mem, const char *name) {
    if (!name) return NULL;
    char *val = memory_get(mem, name);
    if (val) return val;
    return (char*)name;
}

static int is_number(const char *str) {
    if (!str || *str == '\0') return 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '.') continue;
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

void interpreter_execute(TacProgram *prog) {
    if (!prog || !prog->head) {
        printf("No TAC to execute.\n");
        return;
    }
    
    Memory *mem = memory_create();
    
    // Collect labels
    struct Label {
        char *name;
        TacInstr *instr;
        struct Label *next;
    } *labels = NULL;
    
    TacInstr *curr = prog->head;
    while (curr) {
        if (curr->op == TAC_LABEL) {
            struct Label *l = (struct Label*)malloc(sizeof(struct Label));
            l->name = strdup(curr->result);
            l->instr = curr->next;
            l->next = labels;
            labels = l;
        }
        curr = curr->next;
    }
    
    auto find_label = [&](const char *name) -> TacInstr* {
        struct Label *l = labels;
        while (l) {
            if (strcmp(l->name, name) == 0) return l->instr;
            l = l->next;
        }
        return NULL;
    };
    
    printf("\n=== Program Output ===\n");
    
    TacInstr *instr = prog->head;
    while (instr) {
        switch (instr->op) {
            case TAC_ASSIGN: {
                char *val = get_value(mem, instr->arg1);
                if (val) {
                    memory_set(mem, instr->result, val);
                }
                break;
            }
            case TAC_ADD: {
                char *left = get_value(mem, instr->arg1);
                char *right = get_value(mem, instr->arg2);
                if (left && right && is_number(left) && is_number(right)) {
                    float result = atof(left) + atof(right);
                    char res_str[64];
                    sprintf(res_str, "%g", result);
                    memory_set(mem, instr->result, res_str);
                }
                break;
            }
            case TAC_SUB: {
                char *left = get_value(mem, instr->arg1);
                char *right = get_value(mem, instr->arg2);
                if (left && right && is_number(left) && is_number(right)) {
                    float result = atof(left) - atof(right);
                    char res_str[64];
                    sprintf(res_str, "%g", result);
                    memory_set(mem, instr->result, res_str);
                }
                break;
            }
            case TAC_MUL: {
                char *left = get_value(mem, instr->arg1);
                char *right = get_value(mem, instr->arg2);
                if (left && right && is_number(left) && is_number(right)) {
                    float result = atof(left) * atof(right);
                    char res_str[64];
                    sprintf(res_str, "%g", result);
                    memory_set(mem, instr->result, res_str);
                }
                break;
            }
            case TAC_DIV: {
                char *left = get_value(mem, instr->arg1);
                char *right = get_value(mem, instr->arg2);
                if (left && right && is_number(left) && is_number(right)) {
                    float result = atof(left) / atof(right);
                    char res_str[64];
                    sprintf(res_str, "%g", result);
                    memory_set(mem, instr->result, res_str);
                }
                break;
            }
            case TAC_OUTPUT: {
                char *val = get_value(mem, instr->result);
                if (val) {
                    if (val[0] == '"') {
                        char *s = strdup(val + 1);
                        s[strlen(s)-1] = '\0';
                        printf("%s\n", s);
                        free(s);
                    } else {
                        printf("%s\n", val);
                    }
                }
                break;
            }
            case TAC_INPUT: {
                char input_buf[256];
                printf("? ");
                fflush(stdout);
                fgets(input_buf, sizeof(input_buf), stdin);
                input_buf[strcspn(input_buf, "\n")] = '\0';
                memory_set(mem, instr->result, input_buf);
                break;
            }
            case TAC_RETURN:
                break;
            case TAC_GOTO: {
                TacInstr *target = find_label(instr->result);
                if (target) instr = target;
                break;
            }
            case TAC_IFGOTO: {
                char *cond_val = get_value(mem, instr->result);
                int cond_int = atoi(cond_val);
                if (cond_int == 0) {
                    TacInstr *target = find_label(instr->arg1);
                    if (target) instr = target;
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
    
    printf("\n========================\n");
    
    // Cleanup labels
    struct Label *l = labels;
    while (l) {
        struct Label *next = l->next;
        free(l->name);
        free(l);
        l = next;
    }
    
    memory_free(mem);
}
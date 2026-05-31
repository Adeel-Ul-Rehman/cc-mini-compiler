#include "error_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

ErrorList* error_list_create() {
    ErrorList *list = (ErrorList*)malloc(sizeof(ErrorList));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

void error_list_add(ErrorList *list, int line, const char *format, ...) {
    if (!list) return;
    
    va_list args;
    va_start(args, format);
    char buffer[512];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    Error *err = (Error*)malloc(sizeof(Error));
    err->line = line;
    err->message = strdup(buffer);
    err->next = NULL;
    
    if (!list->head) {
        list->head = list->tail = err;
    } else {
        list->tail->next = err;
        list->tail = err;
    }
    list->count++;
}

void error_list_print(ErrorList *list) {
    if (!list || !list->head) return;
    
    printf("\n=== Errors Found (%d) ===\n", list->count);
    Error *err = list->head;
    while (err) {
        printf("Error at line %d: %s\n", err->line, err->message);
        err = err->next;
    }
    printf("==========================\n");
}

int error_list_has_errors(ErrorList *list) {
    return list && list->head != NULL;
}

void error_list_free(ErrorList *list) {
    if (!list) return;
    Error *err = list->head;
    while (err) {
        Error *next = err->next;
        free(err->message);
        free(err);
        err = next;
    }
    free(list);
}
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Error {
    int line;
    char *message;
    struct Error *next;
} Error;

typedef struct {
    Error *head;
    Error *tail;
    int count;
} ErrorList;

ErrorList* error_list_create();
void error_list_add(ErrorList *list, int line, const char *format, ...);
void error_list_print(ErrorList *list);
int error_list_has_errors(ErrorList *list);
void error_list_free(ErrorList *list);

#ifdef __cplusplus
}
#endif

#endif
#ifndef _H3_H
#define _H3_H

#define MAX_LINE_LEN 2048

#include "forward_list.h"

/* https://stackoverflow.com/questions/19068705/undefined-reference-when-calling-inline-function
 * https://gcc.gnu.org/onlinedocs/gcc/Inline.html
 * https://stackoverflow.com/questions/16245521/c99-inline-function-in-c-file
 * In summary, use static inline instead of inline in header files for -gnu11.  */

static inline int any_rand() {
    return (rand() % 2) * 2 - 1;
}

/* Standard style of compare functions give by
 * https://en.cppreference.com/w/c/algorithm/qsort  */

static inline int double_dec(const void *a, const void *b) {
    double arg1 = *(double *)(((struct node *)a)->data);
    double arg2 = *(double *)(((struct node *)b)->data);
    if (arg1 < arg2) return 1;
    if (arg1 > arg2) return -1;
    return 0;
}

static inline int double_inc(const void *a, const void *b) {
    double arg1 = *(double *)(((struct node *)a)->data);
    double arg2 = *(double *)(((struct node *)b)->data);
    if (arg1 > arg2) return 1;
    if (arg1 < arg2) return -1;
    return 0;
}

static inline int int_dec(const void *a, const void *b) {
    int arg1 = *(int *)(((struct node *)a)->data);
    int arg2 = *(int *)(((struct node *)b)->data);
    if (arg1 < arg2) return 1;
    if (arg1 > arg2) return -1;
    return 0;
}

static inline int int_inc(const void *a, const void *b) {
    int arg1 = *(int *)(((struct node *)a)->data);
    int arg2 = *(int *)(((struct node *)b)->data);
    if (arg1 > arg2) return 1;
    if (arg1 < arg2) return -1;
    return 0;
}

static inline int string_dec(const void *a, const void *b) {
    return strcmp((char *) (((struct node *)b)->data), (char *) (((struct node *)a)->data));
}

static inline int string_inc(const void *a, const void *b) {
    return strcmp((char *) (((struct node *)a)->data), (char *) (((struct node *)b)->data));
}

// Do not define a global variable in the header file.
// Instead, declare it in the header file, and define it in the source file.
// Otherwise, "error: 'extern' variable has an initializer [-Werror,-Wextern-initializer]"
// or "multiple definition of `cmp'".
// "extern" is necessary, otherwise "warning: size of symbol `cmp' changed".
// https://stackoverflow.com/questions/14526153/multiple-definition-of-a-global-variable
extern int (*const cmp[3][3])(const void *, const void *);

void fprint_forward(FILE *fp, struct node *head, int datatype);
int get_datatype(char *ifn);
int get_sortingtype(char *sortingtype);
void read_file(struct node **head_ref, const char *ifn, int datatype);
void h3(char *ifn, int datatype, int sortingtype);
void write_file(struct node **head_ref, int datatype, int sortingtype);

#endif

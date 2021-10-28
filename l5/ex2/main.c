#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab5_dlist.h"

dlistValueType getValueType(char *fileName) {
    if (!strcmp(fileName, "rand_int.txt"))
        return DLIST_INT;
    else if (!strcmp(fileName, "rand_char*.txt") || !strcmp(fileName, "rand_string.txt"))
        // Windows does not permit file names with '*', so we have to use "rand_string.txt" just for debugging.
        return DLIST_STR;
    else if (!strcmp(fileName, "rand_double.txt"))
        return DLIST_DOUBLE;
    else
        return DLIST_UNKOWN;
}

dlistSortMethod getSortMethod(char *order) {
    if (!strcmp(order, "rand"))
        return DLIST_SORT_RAND;
    else if (!strcmp(order, "inc"))
        return DLIST_SORT_INC;
    else if (!strcmp(order, "dec"))
        return DLIST_SORT_DEC;
    else
        return DLIST_SORT_UNKOWN;
}

void getKeyFromLine(char (*key)[], char *line) {  // char[] *key
    /* https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
     * https://blog.csdn.net/hustfoxy/article/details/23473805/
     * https://man7.org/linux/man-pages/man3/strtok_r.3.html
     * https://stackoverflow.com/questions/7218625/what-are-the-differences-between-strtok-and-strsep-in-c
     * static char *save_ptr;
     * char *tmp = strtok_r(line, "=", &save_ptr);  linux only
     * https://www.geeksforgeeks.org/strdup-strdndup-functions-c/
     * https://en.cppreference.com/w/c/experimental/dynamic/strdup
     * https://en.cppreference.com/w/c/string/byte/strdup  */
    char *tmp = strdup(line);
    strtok(tmp, "=");
    for (size_t i = 0; i <= strlen(tmp); i++)
        (*key)[i] = *(tmp + i);
    free(tmp);
 }

void getValueFromLine(dlistValue *value, char *line, dlistValueType type) {
    /* line = strtok(line, "=");
     * unnecessary, only used to avoid "error: unused parameter 'line' [-Werror,-Wunused-parameter]
     * WARNING: NEVER DO THIS! IT WILL BE UNABLE TO PERFORM ANY DEBUGGING BY GDB!
     * TO SET BREAKPOINTS AT ANYWHERE WILL CAUSE SIGSEGV AT getValueFromLine()
     * ==388==LeakSanitizer has encountered a fatal error.
     * ==388==HINT: For debugging, try setting environment variable LSAN_OPTIONS=verbosity=1:log_threads=1
     * ==388==HINT: LeakSanitizer does not work under ptrace (strace, gdb, etc)
     * [Inferior 1 (process 388) exited with code 01]
     * [1]    569 segmentation fault  ./l5 rand_int.txt inc  */
    strtok(line, "=");  // line remains unchanged before!
    char *raw_value = strtok(NULL, "\n");
    switch (type) {
        case DLIST_INT:
            value->intValue = atoi(raw_value);
            break;
        case DLIST_STR:
            value->strValue = (char *) malloc(sizeof(char) * (strlen(raw_value) + 1));
            strcpy(value->strValue, raw_value);
            break;
        case DLIST_DOUBLE:
            value->doubleValue = strtod(raw_value, NULL);
            break;
        default:
            break;
    }
}

int run(char *fileName, char *order) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) exit(EXIT_FAILURE);
    dlistValueType type = getValueType(fileName);
    dlistSortMethod method = getSortMethod(order);
    if (type == DLIST_UNKOWN || method == DLIST_SORT_UNKOWN) exit(EXIT_FAILURE);
    dlist listDst = createDlist(type), list = createDlist(type);
    char line[2048], key[2048];
    dlistValue value;
    while (fgets(line, sizeof(line), fp)) {
        getKeyFromLine(&key, line);
        getValueFromLine(&value, line, type);
        dlistAppend(list, key, value);
    }
    fclose(fp);
    dlistSort(list, listDst, method);
    dlistSort(list, listDst, method);
    dlistPrint(listDst);
    dlistFree(list);
    dlistFree(listDst);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) return -1;
    run(argv[1], argv[2]);
    return 0;
}

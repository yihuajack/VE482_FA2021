#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    size_t i;
    char *chunk1 = (char *)malloc(sizeof(char) * 3221225472);
    memset(chunk1, 0, 3221225472);
    printf("chunk1 memory thrashing\n");
    char *chunk2 = (char *)malloc(sizeof(char) * 1073741824);
    memset(chunk2, 0, 1073741824);
    printf("chunk2 memory must be thrashing\n");
    free(chunk1);
    free(chunk2);
    return 0;
}


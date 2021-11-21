//
// Created by Yihua on 2021/10/25.
//

#include "algorithm.h"

// https://github.com/git/git/blob/master/stable-qsort.c
// Johannes Schindelin on 2019/10/01.
void merge_sort(void *ptr, size_t count, size_t size, int (*const comp)(const void *, const void *), char *t) {
    char *temp, *p1 = ptr, *p2;
    size_t n1, n2;
    if (count <= 1)
        return;
    n1 = count / 2;
    n2 = count - n1;
    p2 = (char *)ptr + n1 * size;
    merge_sort(p1, n1, size, comp, t);
    merge_sort(p2, n2, size, comp, t);
    temp = t;
    while (n1 > 0 && n2 > 0) {
        if (comp(p1, p2) <= 0) {
            memcpy(temp, p1, size);
            temp += size;
            p1 += size;
            --n1;
        } else {
            memcpy(temp, p2, size);
            temp += size;
            p2 += size;
            --n2;
        }
    }
    if (n1 > 0)
        memcpy(temp, p1, n1 * size);
    memcpy(ptr, t, (count - n2) * size);
}

void msort(void *ptr, size_t count, size_t size, int (*const comp)(const void *, const void *)) {
    char buf[1024];
    if (count * size < sizeof(buf)) {
        merge_sort(ptr, count, size, comp, buf);
    } else {
        char *tmp = (char *) malloc(sizeof(char) * count * size);
        merge_sort(ptr, count, size, comp, tmp);
        free(tmp);
    }
}

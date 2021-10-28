//
// Created by Yihua on 2021/10/25.
//

#ifndef H3_ALGORITHM_H
#define H3_ALGORITHM_H

#include <stdlib.h>
#include <string.h>

void merge_sort(void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *), char *t);
void msort(void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *));

#endif //H3_ALGORITHM_H

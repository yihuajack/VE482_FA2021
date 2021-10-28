//
// Created by Yihua on 2021/10/26.
//

#include <stdio.h>
#include <stdlib.h>
#include "lab5_dlist.h"

struct node {
    char* key;
    dlistValue value;
    struct node *next;
};

int any_rand() {
    return (rand() % 2) * 2 - 1;
}

int double_dec(const void *a, const void *b) {
    double arg1 = *(double *)(((struct node *)a)->value);
    double arg2 = *(double *)(((struct node *)b)->value);
    if (arg1 < arg2) return 1;
    if (arg1 > arg2) return -1;
    return 0;
}

int double_inc(const void *a, const void *b) {
    double arg1 = *(double *)(((struct node *)a)->value);
    double arg2 = *(double *)(((struct node *)b)->value);
    if (arg1 > arg2) return 1;
    if (arg1 < arg2) return -1;
    return 0;
}

int int_dec(const void *a, const void *b) {
    int arg1 = *(int *)(((struct node *)a)->value);
    int arg2 = *(int *)(((struct node *)b)->value);
    if (arg1 < arg2) return 1;
    if (arg1 > arg2) return -1;
    return 0;
}

int int_inc(const void *a, const void *b) {
    int arg1 = *(int *)(((struct node *)a)->value);
    int arg2 = *(int *)(((struct node *)b)->value);
    if (arg1 > arg2) return 1;
    if (arg1 < arg2) return -1;
    return 0;
}

int string_dec(const void *a, const void *b) {
    return strcmp((char *) (((struct node *)b)->value), (char *) (((struct node *)a)->value));
}

int string_inc(const void *a, const void *b) {
    return strcmp((char *) (((struct node *)a)->value), (char *) (((struct node *)b)->value));
}

static dlistValueType staticValueType;

dlist createDlist(dlistValueType type) {
    struct node **head_ref = (struct node **) malloc(sizeof(struct node *));
    staticValueType = type;
    return head_ref;
}

int dlistIsEmpty(dlist_const this) {
    return !*(struct node **)this;
}

void dlistAppend(dlist this, const char* key, dlistValue value) {
    struct node **head_ref = this;
    switch (staticValueType) {
        case DLIST_STR:
            append_list(head_ref, key, value.strValue);
            break;
        case DLIST_INT:
            append_list(head_ref, key, &value.intValue);
            break;
        case DLIST_DOUBLE:
            append_list(head_ref, key, &value.doubleValue);
            break;
        default:
            fprintf(stderr, "Invalid value type.\n");
            break;
    }
}

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method) {
    if (method != DLIST_SORT_UNKOWN) {
        size_t len = length(*(struct node **)this);
        if (len) {
            size_t i;
            struct node *l_temp = malloc(sizeof(struct node) * len);
            struct node *temp = *(struct node **)this;
            for (i = 0; i < len; i++) {
                memcpy(l_temp + i, temp, sizeof(struct node));
                temp = temp->next;
            }
            switch (staticValueType) {
                case DLIST_STR:
                    msort(l_temp, len, sizeof(struct node), cmp[0][method - 1]);
                    break;
                case DLIST_INT:
                    msort(l_temp, len, sizeof(struct node), cmp[1][method - 1]);
                    break;
                case DLIST_DOUBLE:
                    msort(l_temp, len, sizeof(struct node), cmp[2][method - 1]);
                    break;
                default:
                    fprintf(stderr, "Invalid value type.\n");
                    break;
            }
            if (dlistIsEmpty(listDst))
                clear(listDst);
            temp = *(struct node **)listDst;
            for (i = 0; i < len; i++) {
                temp->key = l_temp[i].key;
                temp->value = l_temp[i].value;
                temp = temp->next;
            }
            free(l_temp);
        }
    }
}

void dlistPrint(dlist_const this) {
    switch (staticValueType) {
        case DLIST_STR:
            fprint_forward(stdout, *(struct node **)this, 0);
            break;
        case DLIST_INT:
            fprint_forward(stdout, *(struct node **)this, 1);
            break;
        case DLIST_DOUBLE:
            fprint_forward(stdout, *(struct node **)this, 2);
            break;
        default:
            fprintf(stderr, "Invalid value type.\n");
            break;
    }
}

void dlistFree(dlist this) {
    clear(this);
    free(this);
}

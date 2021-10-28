//
// Created by Yihua on 2021/10/26.
//

#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"
#include "lab5_dlist.h"

struct node {
    char* key;
    dlistValue value;
    struct node *next;
};

struct fwd_list {
    struct node *head;
    struct node *tail;
    dlistValueType type;
};

int any_rand() {
    return (rand() % 2) * 2 - 1;
}

int double_dec(const void *a, const void *b) {
    double arg1 = ((struct node *)a)->value.doubleValue;
    double arg2 = ((struct node *)b)->value.doubleValue;
    if (arg1 < arg2) return 1;
    if (arg1 > arg2) return -1;
    return 0;
}

int double_inc(const void *a, const void *b) {
    double arg1 = ((struct node *)a)->value.doubleValue;
    double arg2 = ((struct node *)b)->value.doubleValue;
    if (arg1 > arg2) return 1;
    if (arg1 < arg2) return -1;
    return 0;
}

int int_dec(const void *a, const void *b) {
    int arg1 = ((struct node *)a)->value.intValue;
    int arg2 = ((struct node *)b)->value.intValue;
    if (arg1 < arg2) return 1;
    if (arg1 > arg2) return -1;
    return 0;
}

int int_inc(const void *a, const void *b) {
    int arg1 = ((struct node *)a)->value.intValue;
    int arg2 = ((struct node *)b)->value.intValue;
    if (arg1 > arg2) return 1;
    if (arg1 < arg2) return -1;
    return 0;
}

int string_dec(const void *a, const void *b) {
    return strcmp(((struct node *)b)->value.strValue, ((struct node *)a)->value.strValue);
}

int string_inc(const void *a, const void *b) {
    return strcmp(((struct node *)a)->value.strValue, ((struct node *)b)->value.strValue);
}

int (*const cmp[3][3])(const void *, const void *) = {
        {any_rand, string_inc, string_dec},
        {any_rand, int_inc, int_dec},
        {any_rand, double_inc, double_dec}
};

dlist createDlist(dlistValueType type) {
    struct fwd_list *list = (struct fwd_list *) malloc(sizeof(struct fwd_list));
    list->head = NULL;
    list->tail = NULL;
    list->type = type;
    return list;
}

int dlistIsEmpty(dlist_const this) {
    return !((struct fwd_list *)this)->head;
}

void dlistAppend(dlist this, const char* key, dlistValue value) {
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    new_node->key = (char *) malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(new_node->key, key);
    struct fwd_list *list = this;
    if (list->type == DLIST_STR) {
        new_node->value.strValue = (char *) malloc(sizeof(char) * (strlen(value.strValue) + 1));
        strcpy(new_node->value.strValue, value.strValue);
    } else {
        new_node->value = value;
    }
    /* For test, we have value allocated in getValueFromLine() before.
     * Thus, if allocate memory again for new_node->value.strValue, the memory of value remains not freed.
     * This will cause 10,000 blocks of memory leak.
     * However, JOJ's getValueFromLine() does not allocate new memory for value,
     * so to pass JOJ we still need to allocate memory for new_node->value.strValue,
     * otherwise JOJ will gives Runtime Error (250) "Program exit with non zero value -6!"
     * and Stderr is "double free or corruption (out)"  */
    new_node->next = NULL;
    if (!list->head) {
        list->head = new_node;
        list->tail = list->head;
    } else {
        list->tail->next = new_node;
        list->tail = list->tail->next;
    }
}

size_t length_list(const struct fwd_list *list) {
    size_t len = 0;
    struct node *current = list->head;
    while (current) {
        len++;
        current = current->next;
    }
    return len;
}

void clear_list(struct fwd_list *list) {
    struct node *current = list->head, *next;
    if (list->type == DLIST_STR) {
        while (current) {
            next = current->next;
            free(current->key);
            free(current->value.strValue);
            free(current);
            current = next;
        }
    } else {
        while (current) {
            next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    list->head = NULL;
    list->tail = NULL;
}

void dlistFree(dlist this) {
    clear_list((struct fwd_list *)this);
    free(this);
}

void msort_fwdlist(const struct fwd_list *listSrc, struct fwd_list *listDst, int (*const cmp)(const void *, const void *)) {
    size_t len = length_list(listSrc);
    if (len) {
        size_t i;
        struct node *l_temp = (struct node *) malloc(sizeof(struct node) * len);
        struct node *temp = listSrc->head;
        for (i = 0; i < len; i++) {
            memcpy(l_temp + i, temp, sizeof(struct node));
            temp = temp->next;
        }
        msort(l_temp, len, sizeof(struct node), cmp);
        if (listDst->head)  // Assume elements of listDst are allocated by malloc() before.
            clear_list(listDst);
        struct node *new_node = (struct node*) malloc(sizeof(struct node));
        new_node->key = (char *) malloc(sizeof(char) * (strlen(l_temp[0].key) + 1));
        strcpy(new_node->key, l_temp[0].key);
        if (listSrc->type == DLIST_STR) {
            new_node->value.strValue = (char *) malloc(sizeof(char) * (strlen(l_temp[0].value.strValue) + 1));
            strcpy(new_node->value.strValue, l_temp[0].value.strValue);
        } else {
            new_node->value = l_temp[0].value;
        }
        new_node->next = NULL;
        listDst->head = new_node;
        listDst->tail = listDst->head;
        for (i = 1; i < len; i++) {
            struct node *new_node = (struct node*) malloc(sizeof(struct node));
            // Declaration shadows a local variable.
            new_node->key = (char *) malloc(sizeof(char) * (strlen(l_temp[i].key)) + 1);
            strcpy(new_node->key, l_temp[i].key);
            if (listSrc->type == DLIST_STR) {
                new_node->value.strValue = (char *) malloc(sizeof(char) * (strlen(l_temp[i].value.strValue) + 1));
                strcpy(new_node->value.strValue, l_temp[i].value.strValue);
            } else {
                new_node->value = l_temp[i].value;
            }
            new_node->next = NULL;
            listDst->tail->next = new_node;
            listDst->tail = listDst->tail->next;
        }
        free(l_temp);
    }
}

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method) {
    if (method != DLIST_SORT_UNKOWN) {
        switch (((struct fwd_list *) this)->type) {
            case DLIST_STR:
                msort_fwdlist(this, listDst, cmp[0][method - 1]);
                break;
            case DLIST_INT:
                msort_fwdlist(this, listDst, cmp[1][method - 1]);
                break;
            case DLIST_DOUBLE:
                msort_fwdlist(this, listDst, cmp[2][method - 1]);
                break;
            default:
                fprintf(stderr, "Invalid data type.\n");
                break;
        }
    }
}

void dlistPrint(dlist_const this) {
    if (this && ((struct fwd_list *)this)->head) {
        struct node *current = ((struct fwd_list *)this)->head;
        while (current) {
            switch (((struct fwd_list *)this)->type) {
                case DLIST_INT:
                    printf("%s=%d\n", current->key, current->value.intValue);
                    break;
                case DLIST_DOUBLE:
                    printf("%s=%f\n", current->key, current->value.doubleValue);
                    break;
                case DLIST_STR:
                    printf("%s=%s\n", current->key, current->value.strValue);
                    break;
                default:
                    break;
            }
            current = current->next;
        }
    }
}

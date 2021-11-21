#ifndef _FORWARD_LIST_H
#define _FORWARD_LIST_H

#include "algorithm.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char *str;
    void *data;
    struct node *next;
};

struct fwd_list {
    struct node *head;
    struct node *tail;
    int type;
};

struct node *add_list(struct node *list1, struct node *list2);
void append_fwdlist(struct fwd_list *list, const char *str, void *data);
void append_list(struct node** head_ref, const char *str, void *data);
void append_node(struct node** prev_ref, const char *str, void *data);
void *at(struct node *head, size_t pos);
void clear(struct node **head_ref);
void clear_list(struct fwd_list *list);
size_t count(struct node* head, void *data);
void erase_at(struct node **head_ref, size_t pos);
void insert_after(struct node *prev_node, char *str, void *data);
size_t length(struct node *head);
size_t length_list(const struct fwd_list *list);
bool loop(struct node *head);
struct node *merge_list_recursive(struct node *subl1, struct node *subl2);
void merge_sort_list(struct node **head_ref);
void msort_fwdlist(const struct fwd_list *listSrc, struct fwd_list *listDst, int (*const cmp)(const void *, const void *));
void msort_list(struct node **head_ref, int (*cmp)(const void *, const void *));
void print_forward(struct node *head);
void print_middle_fast(struct node *head);
void print_middle_slow(struct node* head);
void print_reverse_at(struct node* head, size_t pos);
void push_end(struct node **head_ref, char *str, void *data);
void push_front(struct node **head_ref, char *str, void *data);
void qsort_list(struct node **head_ref, int (*cmp)(const void *, const void *));
void remove_data(struct node **head_ref, void *data);
void remove_loop(struct node *slow, struct node *head);
bool search(struct node *head, void *data);
bool search_recursive(struct node *head, void *data);
void split_list(struct node *src, struct node **head_ref, struct node **tail_ref);

#endif

#ifndef _FORWARD_LIST_H
#define _FORWARD_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char *str;
    void *data;
    struct node *next;
};

/* struct fwd_list {
 *     struct node *first;
 *     size_t len;
 * }  */

void append_node(struct node** prev_ref, char *str, void *data);
struct node *add_list(struct node *list1, struct node *list2);
void append_list(struct node** head_ref, char *str, void *data);
void *at(struct node *head, size_t pos);
void clear(struct node **head_ref);
size_t count(struct node* head, void *data);
void erase_at(struct node **head_ref, size_t pos);
void insert_after(struct node *prev_node, char *str, void *data);
size_t length(struct node *head);
// void list_initializer();
bool loop(struct node *head);
void print_forward(struct node *head);
void print_middle_fast(struct node *head);
void print_middle_slow(struct node* head);
void print_reverse_at(struct node* head, size_t pos);
void push_end(struct node **head_ref, char *str, void *data);
void push_front(struct node **head_ref, char *str, void *data);
void remove_data(struct node **head_ref, void *data);
void remove_loop(struct node *slow, struct node *head);
bool search(struct node *head, void *data);
bool search_recursive(struct node *head, void *data);
void sort_list(struct node **head_ref, int (*cmp)(const void *, const void *));

#endif

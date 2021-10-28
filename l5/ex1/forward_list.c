#include "forward_list.h"

struct node *add_list(struct node *list1, struct node *list2) {
    struct node *res = NULL, *temp = NULL, *prev = NULL;
    int carry = 0, sum;
    while (list1 != NULL || list2 != NULL) {
        sum = carry + (list1 ? *(int *)(list1->data) : 0) + (list2 ? *(int *)(list2->data) : 0);
        carry = (sum >= 10) ? 1 : 0;
        sum = sum % 10;
        struct node *new_node = (struct node *) malloc(sizeof(struct node));
        new_node->str = (char *) malloc(sizeof(char));
        *(new_node->str) = (char) (sum + 48);
        new_node->data = (int *) malloc(sizeof(int));
        *(int *)(new_node->data) = sum;
        if (!res)
            res = temp;
        else
            prev->next = temp;
        prev = temp;
        if (list1)
            list1 = list1->next;
        if (list2)
            list2 = list2->next;
    }
    if (carry > 0) {
        struct node *new_node = (struct node *) malloc(sizeof(struct node));
        new_node->str = (char *) malloc(sizeof(char));
        *(new_node->str) = (char) (carry + 48);
        new_node->data = (int *) malloc(sizeof(int));
        *(int *)(new_node->data) = carry;
        new_node->next = NULL;
        temp->next = new_node;
    }
    return res;
}

void append_fwdlist(struct fwd_list *list, const char *str, void *data) {
    struct node *new_node = (struct node*) malloc(sizeof(struct node));
    new_node->str = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(new_node->str, str);
    new_node->data = data;
    new_node->next = NULL;
    if (!list->head) {
        list->head = new_node;
        list->tail = list->head;
    } else {
        list->tail->next = new_node;
        list->tail = list->tail->next;
        // list->tail->next = NULL;  new_node->next == NULL
    }
}

void append_list(struct node** head_ref, const char *str, void *data) {
    struct node *new_node = (struct node*) malloc(sizeof(struct node));
    struct node *last = *head_ref;
    new_node->str = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(new_node->str, str);
    new_node->data = data;
    new_node->next = NULL;
    if (!*head_ref) {
        *head_ref = new_node;
    } else {
        while (last->next)
            last = last->next;
        last->next = new_node;
    }
}

void append_node(struct node** prev_ref, const char *str, void *data) {
    struct node *new_node = (struct node*) malloc(sizeof(struct node));
    new_node->str = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(new_node->str, str);
    new_node->data = data;
    new_node->next = NULL;
    if (!*prev_ref)
        *prev_ref = new_node;
    else
        (*prev_ref)->next = new_node;
}

void *at(struct node *head, size_t pos) {
    struct node* current = head;
    size_t i = 0;
    while (current) {
        if (i == pos) {
            return (current->data);
        } else {
            i++;
            current = current->next;
        }
    }
    return NULL;
}

void clear(struct node **head_ref) {
    struct node *current = *head_ref, *next;
    while (current) {
        next = current->next;
        free(current->str);
        free(current->data);
        free(current);
        current = next;
    }
    *head_ref = NULL;
}

void clear_list(struct fwd_list *list) {
    struct node *current = list->head, *next;
    if (list->type == 2) {
        while (current) {
            next = current->next;
            free(current->str);
            free(current->data);  // Assume listDst's memory is allocated by malloc()
            free(current);
            current = next;
        }
    } else {
        while (current) {
            next = current->next;
            free(current->str);
            free(current);
            current = next;
        }
    }
    list->head = NULL;
    list->tail = NULL;
}

size_t count(struct node* head, void *data) {
    size_t i = 0;
    struct node *current = head;
    while (current) {
        if (current->data == data)
            i++;
        current = current->next;
    }
    return i;
}

void erase_at(struct node **head_ref, size_t pos) {
    if (*head_ref) {
        struct node *temp = *head_ref;
        if (!pos) {
            *head_ref = temp->next;
            free(temp->str);
            free(temp->data);
            free(temp);
        } else {
            for (size_t i = 0; temp && i < pos - 1; i++) {
                temp = temp->next;
            }
            if (temp && temp->next) {
                struct node *next = temp->next->next;
                free(temp->next->str);
                free(temp->next->data);
                free(temp->next);
                temp->next = next;
            }
        }
    }
}

void insert_after(struct node *prev_node, char *str, void *data) {
    if (prev_node) {
        struct node *new_node = (struct node *) malloc(sizeof(struct node));
        new_node->str = str;
        new_node->data = data;
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }
}

size_t length(struct node *head) {
    size_t len = 0;
    struct node *current = head;
    while (current) {
        len++;
        current = current->next;
    }
    return len;
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

bool loop(struct node *head) {
    struct node *slow = head, *fast = head;
    while (slow && fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }
    return false;
}

struct node *merge_list_recursive(struct node *subl1, struct node *subl2) {
    struct node *merged = NULL;
    if (!subl1) {
        return subl2;
    } else if (!subl2) {
        return subl1;
    } else if (subl1->data <= subl2->data) {
        merged = subl1;
        merged->next = merge_list_recursive(subl1->next, subl2);
    } else {
        merged = subl2;
        merged->next = merge_list_recursive(subl1, subl2->next);
    }
    return merged;
}

void merge_sort_list(struct node **head_ref) {
    struct node *head = *head_ref, *subl1, *subl2;
    if (head && head->next) {
        split_list(head, &subl1, &subl2);
        merge_sort_list(&subl1);
        merge_sort_list(&subl2);
        *head_ref = merge_list_recursive(subl1, subl2);
    }
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
        if (listDst->head)
            clear_list(listDst);
        struct node *new_node = (struct node*) malloc(sizeof(struct node));
        new_node->str = (char *) malloc(sizeof(char) * (strlen(l_temp[0].str)) + 1);
        strcpy(new_node->str, l_temp[0].str);
        new_node->data = l_temp[i].data;
        new_node->next = NULL;
        listDst->head = new_node;
        listDst->tail = listDst->head;
        for (i = 1; i < len; i++) {
            // temp->str = l_temp[i].str;  Dangling pointer
            struct node *new_node = (struct node*) malloc(sizeof(struct node));
            new_node->str = (char *) malloc(sizeof(char) * (strlen(l_temp[i].str)) + 1);
            strcpy(new_node->str, l_temp[i].str);
            new_node->data = l_temp[i].data;
            new_node->next = NULL;
            listDst->tail->next = new_node;
            listDst->tail = listDst->tail->next;
        }
        free(l_temp);
    }
}

void msort_list(struct node **head_ref, int (*const cmp)(const void *, const void *)) {
    size_t len = length(*head_ref);
    if (len) {
        size_t i;
        struct node *l_temp = malloc(sizeof(struct node) * len);
        struct node *temp = *head_ref;
        for (i = 0; i < len; i++) {
            memcpy(l_temp + i, temp, sizeof(struct node));
            temp = temp->next;
        }
        msort(l_temp, len, sizeof(struct node), cmp);
        temp = *head_ref;
        for (i = 0; i < len; i++) {
            temp->str = l_temp[i].str;
            temp->data = l_temp[i].data;
            temp = temp->next;
        }
        free(l_temp);
    }
}

void print_forward(struct node *head) {
    struct node *current = head;
    while (current) {
        printf("%s %d\n", current->str, *(int *)(current->data));
        current = current->next;
    }
}

void print_middle_fast(struct node *head) {
    struct node *mid = head, *last = head;
    if (head) {
        while (last && last->next) {
            last = last->next->next;
            mid = mid->next;
        }
        printf("%s %d\n", mid->str, *(int *)(mid->data));
    }
}

void print_middle_slow(struct node* head) {
    size_t i = 0;
    struct node *mid = head, *last = head;
    while (last) {
        if (i & 1) {
            mid = mid->next;
        } else {
            i++;
            head = head->next;
        }
    }
    if (mid)
        printf("%s %d\n", mid->str, *(int *)(mid->data));
}

void print_reverse_at(struct node* head, size_t pos) {
    static size_t i = 0;
    struct node *current = head;
    if (current) {
        print_reverse_at(current->next, pos);
        if (++i == pos)
            printf("%s %d\n", current->str, *(int *)(current->data));
    }
}

void push_end(struct node **head_ref, char *str, void *data) {
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    struct node *last = *head_ref;
    new_node->str = str;
    new_node->data = data;
    new_node->next = NULL;
    if (!*head_ref) {
        *head_ref = new_node;
    } else {
        while (last->next) {
            last = last->next;
        }
        last->next = new_node;
    }
}

void push_front(struct node **head_ref, char *str, void *data) {
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    new_node->str = str;
    new_node->data = data;
    new_node->next = *head_ref;
    *head_ref = new_node;
}

/* qsort is unstable! https://stackoverflow.com/a/6105590/13087142
 * The canonical solution is to make (i.e. allocate memory for and fill) an array of pointers to the elements of
 * the original array, and qsort this new array, using an extra level of indirection and falling back to
 * comparing pointer values when the things they point to are equal. This approach has the potential side benefit that
 * you don't modify the original array at all - but if you want the original array to be sorted in the end,
 * you'll have to permute it to match the order in the array of pointers after qsort returns.  */
void qsort_list(struct node **head_ref, int (*const cmp)(const void *, const void *)) {
    size_t len = length(*head_ref);
    if (len) {
        size_t i;
        struct node *l_temp = malloc(sizeof(struct node) * len);
        struct node *temp = *head_ref;
        for (i = 0; i < len; i++) {
            memcpy(l_temp + i, temp, sizeof(struct node));
            temp = temp->next;
        }
        qsort(l_temp, len, sizeof(struct node), cmp);
        temp = *head_ref;
        for (i = 0; i < len; i++) {
            temp->str = l_temp[i].str;
            temp->data = l_temp[i].data;
            temp = temp->next;
        }
        free(l_temp);
    }
}

void remove_data(struct node **head_ref, void *data) {
    struct node *prev_node, *temp = *head_ref;
    if (temp && temp->data == data) {
        *head_ref = temp->next;
        free(temp->str);
        free(temp->data);
        free(temp);
    } else {
        while (temp && temp->data != data) {
            prev_node = temp;
            temp = temp->next;
        }
        if (temp) {
            prev_node->next = temp->next;
            free(temp->str);
            free(temp->data);
            free(temp);
        }
    }
}

void remove_loop(struct node *slow, struct node *head) {
    struct node *ptr1 = slow, *ptr2 = slow;
    size_t i, k = 1;
    while (ptr1->next != ptr2) {
        ptr1 = ptr1->next;
        k++;
    }
    ptr1 = head;
    ptr2 = head;
    for (i = 0; i < k; i++) {
        ptr2 = ptr2->next;
    }
    while (ptr2 != ptr1) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    while (ptr2->next != ptr1) {
        ptr2 = ptr2->next;
    }
    ptr2->next = NULL;
}

bool search(struct node *head, void *data) {
    struct node* current = head;
    while (current) {
        if (current->data == data)
            return true;
        current = current->next;
    }
    return false;
}

bool search_recursive(struct node *head, void *data) {
    if (!head)
        return false;
    else if (head->data == data)
        return true;
    else
        return search(head->next, data);
}

void split_list(struct node *src, struct node **head_ref, struct node **tail_ref) {
    struct node *fast = src->next, *slow = src;
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *head_ref = src;
    *tail_ref = slow->next;
    slow->next = NULL;
}

//
// Created by Yihua on 2021/10/26.
//

#include "h3.h"
#include "lab5_dlist.h"

/* Why do we use struct node *fwd_list rather than struct node **head_ref?  */

dlist createDlist(dlistValueType type) {
    // struct node **head_ref = (struct node **) malloc(sizeof(struct node *));
    // head_ref = 0x20ea480, *head_ref = 0x21
    // *head_ref = 0x4030c9
    // struct node **list = (struct node **) calloc(1, sizeof(struct node *));
    /* switch (type) {
        case 1:
            list = (int *) calloc(1, sizeof(int));
            break;
        case 2:
            list = (char **) calloc(1, sizeof(char *));
            break;
        case 3:
            list = (double *) calloc(1, sizeof(double));
            break;
        default:
            break;
    }  */
    struct fwd_list *list = (struct fwd_list *) malloc(sizeof(struct fwd_list));
    list->head = NULL;
    list->tail = NULL;
    list->type = (int)type;
    return list;
}

int dlistIsEmpty(dlist_const this) {
    return !((struct fwd_list *)this)->head;
}

void dlistAppend(dlist this, const char* key, dlistValue value) {
    switch (((struct fwd_list *) this)->type) {
        case DLIST_STR:
            append_fwdlist(this, key, value.strValue);
            break;
        case DLIST_INT:
            append_fwdlist(this, key, &value.intValue);
            break;
        case DLIST_DOUBLE:
            append_fwdlist(this, key, &value.doubleValue);
            break;
        default:
            fprintf(stderr, "Invalid data type.\n");
            break;
    }
}

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method) {
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

void dlistPrint(dlist_const this) {
    struct node *current = ((struct fwd_list *)this)->head;
    switch (((struct fwd_list *) this)->type) {
        case DLIST_STR:
            while (current) {
                printf("%s=%s\n", current->str, ((char *) (current->data)));
                current = current->next;
            }
            break;
        case DLIST_INT:
            while (current) {
                printf("%s=%d\n", current->str, *((int *)(current->data)));
                current = current->next;
            }
            break;
        case DLIST_DOUBLE:
            while (current) {
                printf("%s=%lf\n", current->str, *((double *)(current->data)));
                current = current->next;
            }
            break;
        default:
            fprintf(stderr, "Invalid data type.\n");
            break;
    }
}

void dlistFree(dlist this) {
    clear_list(this);
    free(this);
}

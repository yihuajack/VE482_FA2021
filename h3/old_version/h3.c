#include "h3.h"

int get_datatype(char *ifn) {
    if (!strcmp(ifn, "rand_string.txt")) {
        return 0;
    } else if (!strcmp(ifn, "rand_int.txt")) {
        return 1;
    } else if (!strcmp(ifn, "rand_double.txt")) {
        return 2;
    } else {
        return -1;
    }
}

int get_sortingtype(char *sortingtype) {
    if (!strcmp(sortingtype, "rand")) {
        return 0;
    } else if (!strcmp(sortingtype, "inc")) {
        return 1;
    } else if (!strcmp(sortingtype, "dec")) {
        return 2;
    } else {
        return -1;
    }
}

void insert_after_list(struct list *l, char *str, void *data) {
    /* if (list) {
     *     struct node *new_node = (struct node *) malloc(sizeof(struct node));
     *     new_node->str = str;
     *     new_node->data = data;
     *     new_node->next = list->next;
     *     list->next = new_node;
     * } else {
     *     list = (struct node *) malloc(sizeof(struct node));
     *     list->str = str;
     *     list->data = data;
     *     list->next = NULL;
     * }
     * return list;  */
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    new_node->str = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(new_node->str, str);
    new_node->data = data;
    new_node->next = (l->first) ? l->first : NULL;
    // new_node->next = l->first->next;
    l->first = new_node;
    l->length++;
}

struct list *read_file(char *ifn, int datatype) {
    char line[1024];
    FILE *ifile = fopen(ifn, "r");
    printf("reading %s\n", ifn);
    // struct node *head_node = (struct node *) malloc(sizeof(struct node));
    // struct node *head_node = NULL;
    // head_ref = &head_node;
    struct list *l;
    list_initializer(&l);
    while (fgets(line, 1024, ifile)) {
        char *str, *raw_data;
        void *data = NULL;
        str = strtok(line, "=");
        raw_data = strtok(NULL, "\n");
        switch (datatype) {
            case 0:
                data = malloc(sizeof(char) * (strlen(raw_data) + 1));
                strcpy((char *)data, raw_data);
                break;
            case 1:
                data = malloc(sizeof(int));
                *(int *)(data) = atoi(raw_data);
                break;
            case 2:
                data = malloc(sizeof(double));
                *(double *)(data) = strtod(raw_data, NULL);
                break;
            default:
                break;
        }
        insert_after_list(l, str, data);
        memset(line, 0, 1024);
    }
    fclose(ifile);
    return l;
}

void fprint_forward(struct node *head, FILE *fp, int datatype) {
    struct node *current = head;
    while (current) {
        switch (datatype) {
            case 0:
                fprintf(fp, "%s=%s\n", current->str, ((char *)(current->data)));
                break;
            case 1:
                fprintf(fp, "%s=%d\n", current->str, *((int *)(current->data)));
                break;
            case 2:
                fprintf(fp, "%s=%lf\n", current->str, *((double *)(current->data)));
                break;
            default:
                break;
        }
        current = current->next;
    }
}

void write_file(struct list *l, int datatype, int sortingtype) {
    char ofn[15];
    const char *sorting_name[3] = {"rand", "inc", "dec"};
    // `typename` is not a keyword of C, but is a keyword of C++:
    // https://en.cppreference.com/w/cpp/keyword
    // Using `g++` will force an error: "expected unqualified-id before ‘typename’".
    const char *type_name[3] = {"string", "int", "double"};
    sprintf(ofn, "%s_%s.txt", sorting_name[sortingtype], type_name[datatype]);
    printf("writing %s\n", ofn);
    FILE *ofile = fopen(ofn, "w");
    fprint_forward(l, ofile, datatype);
    fclose(ofile);
}

int main(int argc, char *argv[]) {
    if (argc != 3)
        return 0;
    int datatype = get_datatype(argv[1]), sortingtype = get_sortingtype(argv[2]);
    struct list *l = read_file(argv[1], datatype);
    printf("sorting elements\n");
    sort_list(l, cmp[datatype][sortingtype]);
    write_file(l, datatype, sortingtype);
    clear(head_ref);
    return 0;
}

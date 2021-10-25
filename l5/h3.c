#include "h3.h"

int get_datatype(char *ifn) {
    if (!strcmp(ifn, "rand_char*.txt")) {
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

/* void insert_after_list(struct list *l, char *str, void *data) {
     * if (list) {
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
     * return list;
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    new_node->str = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(new_node->str, str);
    new_node->data = data;
    new_node->next = (l->first) ? l->first : NULL;
    // new_node->next = l->first->next;
    l->first = new_node;
    l->length++;
}  */

void read_file(struct node **head_ref, const char *ifn, const int datatype) {
    char line[1024];
    FILE *ifile = fopen(ifn, "r");
    printf("reading %s\n", ifn);
    struct node *prev_node = NULL;
    char *str, *raw_data;
    if (datatype == 0) {
        if (fgets(line, 1024, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(char) * (strlen(raw_data) + 1));
            strcpy((char *)data, raw_data);
            append_node(head_ref, str, data);
            prev_node = *head_ref;
        }
        while (fgets(line, 1024, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(char) * (strlen(raw_data) + 1));
            strcpy((char *)data, raw_data);
            append_node(&prev_node, str, data);
            prev_node = prev_node->next;
        }
    } else if (datatype == 1) {
        if (fgets(line, 1024, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(int));
            *(int *)(data) = atoi(raw_data);
            append_node(head_ref, str, data);
            prev_node = *head_ref;
        }
        while (fgets(line, 1024, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(int));
            *(int *)(data) = atoi(raw_data);
            append_node(&prev_node, str, data);
            prev_node = prev_node->next;
        }
    } else if (datatype == 2) {
        if (fgets(line, 1024, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(double));
            *(double *)(data) = strtod(raw_data, NULL);
            append_node(head_ref, str, data);
            prev_node = *head_ref;
        }
        while (fgets(line, 1024, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(double));
            *(double *)(data) = strtod(raw_data, NULL);
            append_node(&prev_node, str, data);
            prev_node = prev_node->next;
        }
    }
    fclose(ifile);
}

void fprint_forward(FILE *fp, struct node *head, int datatype) {
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

void write_file(struct node **head_ref, int datatype, int sortingtype) {
    char ofn[15];
    const char *sorting_name[3] = {"rand", "inc", "dec"};
    // `typename` is not a keyword of C, but is a keyword of C++:
    // https://en.cppreference.com/w/cpp/keyword
    // Using `g++` will force an error: "expected unqualified-id before ‘typename’".
    const char *type_name[3] = {"string", "int", "double"};
    sprintf(ofn, "%s_%s.txt", sorting_name[sortingtype], type_name[datatype]);
    printf("writing %s\n", ofn);
    FILE *ofile = fopen(ofn, "w");
    fprint_forward(ofile, *head_ref, datatype);
    fclose(ofile);
}

int main(int argc, char *argv[]) {
    if (argc != 3)
        return 0;
    int datatype = get_datatype(argv[1]), sortingtype = get_sortingtype(argv[2]);
    // struct list *l = read_file(argv[1], datatype);
    struct node *head_node = NULL;
    read_file(&head_node, argv[1], datatype);
    printf("sorting elements\n");
    sort_list(&head_node, cmp[datatype][sortingtype]);
    write_file(&head_node, datatype, sortingtype);
    clear(&head_node);
    return 0;
}

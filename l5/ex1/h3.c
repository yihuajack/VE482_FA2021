#include "h3.h"

int (*const cmp[3][3])(const void *, const void *) = {
    {any_rand, string_inc, string_dec},
    {any_rand, int_inc, int_dec},
    {any_rand, double_inc, double_dec}
};

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

void read_file(struct node **head_ref, const char *ifn, const int datatype) {
    char line[MAX_LINE_LEN];
    FILE *ifile = fopen(ifn, "r");
    printf("reading %s\n", ifn);
    struct node *prev_node = NULL;
    char *str, *raw_data;
    if (datatype == 0) {
        if (fgets(line, MAX_LINE_LEN, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(char) * (strlen(raw_data) + 1));
            strcpy((char *)data, raw_data);
            append_node(head_ref, str, data);
            prev_node = *head_ref;
        }
        while (fgets(line, MAX_LINE_LEN, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(char) * (strlen(raw_data) + 1));
            strcpy((char *)data, raw_data);
            append_node(&prev_node, str, data);
            prev_node = prev_node->next;
        }
    } else if (datatype == 1) {
        if (fgets(line, MAX_LINE_LEN, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(int));
            *(int *)(data) = atoi(raw_data);
            append_node(head_ref, str, data);
            prev_node = *head_ref;
        }
        while (fgets(line, MAX_LINE_LEN, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(int));
            *(int *)(data) = atoi(raw_data);
            append_node(&prev_node, str, data);
            prev_node = prev_node->next;
        }
    } else if (datatype == 2) {
        if (fgets(line, MAX_LINE_LEN, ifile)) {
            str = strtok(line, "=");
            raw_data = strtok(NULL, "\n");
            void *data = malloc(sizeof(double));
            *(double *)(data) = strtod(raw_data, NULL);
            append_node(head_ref, str, data);
            prev_node = *head_ref;
        }
        while (fgets(line, MAX_LINE_LEN, ifile)) {
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
    char ofn[16];
    const char *sorting_name[3] = {"rand", "inc", "dec"};
    // `typename` is not a keyword of C, but is a keyword of C++:
    // https://en.cppreference.com/w/cpp/keyword
    // Using `g++` will force an error: "expected unqualified-id before ‘typename’".
    const char *type_name[3] = {"char*", "int", "double"};
    sprintf(ofn, "%s_%s.txt", sorting_name[sortingtype], type_name[datatype]);
    printf("writing %s\n", ofn);
    FILE *ofile = fopen(ofn, "w");
    fprint_forward(ofile, *head_ref, datatype);
    fclose(ofile);
}

void h3(char *ifn, int datatype, int sortingtype) {
    struct node *head_node = NULL;
    read_file(&head_node, ifn, datatype);
    printf("sorting elements\n");
    msort_list(&head_node, cmp[datatype][sortingtype]);
    write_file(&head_node, datatype, sortingtype);
    clear(&head_node);
}

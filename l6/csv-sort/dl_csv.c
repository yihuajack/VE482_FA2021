//
// Created by Yihua Liu on November 4, 2021.
//

#include <stdio.h>
#include "util.h"
#include "plugin_manager.h"

struct columns {
    struct node *head_node;
    int data_type;
    int sorting_type;
};

size_t strcnt(const char *str, int c) {
    size_t i, ret = 0;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == c)
            ++ret;
    }
    return ret;
}

void read_csv(FILE *ifile, struct columns *cols, size_t n_cols) {
    char line[MAX_LINE_LEN];
    struct node **head_ref = NULL;
    struct node **prev_node = (struct node **) malloc(sizeof(struct node *) * n_cols);
    char *str, *raw_data, *element;
    void *data = NULL;
    size_t i;
    // size_t i_row = 0;
    if (fgets(line, MAX_LINE_LEN, ifile)) {
        char *save_ptr = line;
        for (i = 0; i < n_cols; i++) {
            element = strtok_r(save_ptr, ",\n", &save_ptr);
            str = strtok(element, "=");
            raw_data = strtok(NULL, ",\n");
            head_ref = &cols[i].head_node;
            // You cannot define a variable below a label without a block, otherwise
            // "error: a label can only be part of a statement and a declaration is not a statement"
            // https://stackoverflow.com/questions/46341364/label-can-only-be-used-as-part-of-a-statement-error
            switch (cols[i].data_type) {
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
            append_node(head_ref, str, data);
            prev_node[i] = *head_ref;
        }
        // i_row++;
    }
    while (fgets(line, MAX_LINE_LEN, ifile)) {
        char *save_ptr = line;
        for (i = 0; i < n_cols; i++) {
            element = strtok_r(save_ptr, ",\n", &save_ptr);
            str = strtok(element, "=");
            raw_data = strtok(NULL, ",\n");
            switch (cols[i].data_type) {
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
            append_node(&prev_node[i], str, data);
            prev_node[i] = prev_node[i]->next;
        }
        // i_row++;
    }
    free(prev_node);
}

void dl_csv(argument *arg_list){
    char line[MAX_LINE_LEN];
    FILE *ifile = fopen(arg_list->file_name, "r");
    fprintf(stderr, "reading %s\n", arg_list->file_name);
    fgets(line, MAX_LINE_LEN, ifile);
    size_t i, n_cols = strcnt(line, ',') + 1;
    struct columns *cols  = (struct columns *) malloc(sizeof(struct columns) * n_cols);
    memset(cols, 0, sizeof(struct columns) * n_cols);
    char *save_ptr, *col_head = strtok_r(line, ",\n", &save_ptr);
    cols[0].data_type = get_datatype(strdup(col_head));
    cols[0].sorting_type = get_sortingtype(strtok(col_head, "_"));
    for (i = 1; i < n_cols; i++) {
        col_head = strtok_r(save_ptr, ",\n", &save_ptr);
        cols[i].data_type = get_datatype(strdup(col_head));
        cols[i].sorting_type = get_sortingtype(strtok(col_head, "_"));
    }
    read_csv(ifile, cols, n_cols);
    fclose(ifile);
    fprintf(stderr, "sorting elements\n");
    for (i = 0; i < n_cols; i++) {
        msort_list(&cols[i].head_node, cmp[cols[i].data_type][cols[i].sorting_type]);
        write_file(&cols[i].head_node, cols[i].data_type, cols[i].sorting_type);
        clear(&cols[i].head_node);
    }
    free(cols);
}

void dl_csv_init(manager *m){
    // EXPOSING
    register_hook(m, "csv", dl_csv);
    fprintf(stderr, "dl_csv: Registration Succeed!\n");
}

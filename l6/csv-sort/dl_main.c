//
// Created by Yihua Liu on November 4, 2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "plugin_manager.h"

typedef struct plugin_node_t {
    void *handle;
    struct plugin_node_t *next;
} plugin_node;

typedef struct plugin_list_t {
    plugin_node *head_node;
} plugin_list;

int main(int argc, char *argv[]) {
    typedef void(*regfn_t)(manager *);
    manager *m = new_manager();
    DIR *dir = opendir(".");
    if (!dir) {
        fprintf(stderr, "Error: opendir failed.\n");
        free_manager(m);
        return 0;
    }
    plugin_list *l_plugin = (plugin_list *) malloc(sizeof(plugin_list));
    l_plugin->head_node = NULL;
    struct dirent *dir_ent;
    while ((dir_ent = readdir(dir))) {
        char *ent_name = strdup(dir_ent->d_name);
        char *suffix = strrchr(ent_name, '.');
        if (suffix && !strcmp(suffix, ".so")) {
            char *body = (char *) malloc(sizeof(char) * (suffix - ent_name + 1));
            strncpy(body, ent_name, suffix - ent_name);
            body[suffix - ent_name] = '\0';
            char *rel_path = (char *) malloc(sizeof(char) * (strlen(ent_name) + 3));
            strcpy(rel_path, "./");
            strcat(rel_path, ent_name);
            void *handle = dlopen(rel_path, RTLD_NOW);
            char *handle_name = (char *) malloc(sizeof(char) * (strlen(body) + 6));
            strcpy(handle_name, body);
            strcat(handle_name, "_init");
            ((regfn_t) dlsym(handle, handle_name))(m);
            plugin_node *new_dl_node = (plugin_node *) malloc(sizeof(plugin_node));
            new_dl_node->handle = handle;
            new_dl_node->next = l_plugin->head_node;
            l_plugin->head_node = new_dl_node;
            free(handle_name);
            free(body);
            free(rel_path);
        }
        free(ent_name);
    }
    closedir(dir);
    if (!l_plugin->head_node) {
        free(l_plugin);
        free_manager(m);
        return 0;
    }

    argument arg_list;
    arg_list.file_name = strdup(argv[1]);
    strtok(argv[1], ".");
    char *file_type = strtok(NULL, " \n");
    if (!strcmp(file_type, "txt")) {
        if (argc != 3) {
            fprintf(stderr, "Error: invalid arguments for processing text file.\n");
            free(arg_list.file_name);
            return 0;
        } else {
            strtok(argv[1], "_");
            char *data_type = strtok(NULL, ".");
            if (!strcmp(data_type, "char*") || !strcmp(data_type, "string")) {
                arg_list.data_type = 0;
            } else if (!strcmp(data_type, "int")) {
                arg_list.data_type = 1;
            } else if (!strcmp(data_type, "double")) {
                arg_list.data_type = 2;
            } else {
                arg_list.data_type = -1;
            }
            if (!strcmp(argv[2], "rand")) {
                arg_list.sorting_type = 0;
            } else if (!strcmp(argv[2], "inc")) {
                arg_list.sorting_type = 1;
            } else if (!strcmp(argv[2], "dec")) {
                arg_list.sorting_type = 2;
            } else {
                arg_list.sorting_type = -1;
            }
        }
    }
    if (!apply_hook(m, &arg_list, file_type)) {
        fprintf(stderr, "main: Fail to finish %s sort.\n", file_type);
        free(arg_list.file_name);
        exit(0);
    }
    free(arg_list.file_name);

    plugin_node *node = l_plugin->head_node, *next;
    while (node) {
        next = node->next;
        dlclose(node->handle);
        free(node);
        node = next;
    }
    free(l_plugin);
    free_manager(m);
    return 0;
}

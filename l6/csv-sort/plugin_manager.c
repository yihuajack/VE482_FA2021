//
// Created by Yihua Liu on November 4, 2021.
//

#include "plugin_manager.h"

struct hook_list_t {
    char *file_type;
    hook_t hook;
    struct hook_list_t *next;
};

struct manager_t {
    struct hook_list_t *hook_list;
};

manager *new_manager() {
    manager *m = (manager *) malloc(sizeof(manager));
    m->hook_list = NULL;
    return m;
}

void free_manager(manager *m) {
    struct hook_list_t *hook_list_node = m->hook_list;
    while (hook_list_node) {
        struct hook_list_t *next = hook_list_node->next;
        free(hook_list_node->file_type);
        free(hook_list_node);
        hook_list_node = next;
    }
    free(m);
}

void register_hook(manager *m, char *file_type, hook_t hook) {
    struct hook_list_t *hook_list_node = (struct hook_list_t *) malloc(sizeof(struct hook_list_t));
    hook_list_node->file_type = strdup(file_type);
    hook_list_node->hook = hook;
    hook_list_node->next = m->hook_list;
    m->hook_list = hook_list_node;
}

bool apply_hook(manager *m, argument *arg_list, char *file_type) {
    struct hook_list_t *hook_list_node = m->hook_list;
    while (hook_list_node) {
        if (!strcmp(hook_list_node->file_type, file_type)) {
            hook_list_node->hook(arg_list);
            return true;
        }
        hook_list_node = hook_list_node->next;
    }
    printf("manager: Fail to find suitable plugin to do %s sort.\n", file_type);
    return false;
}

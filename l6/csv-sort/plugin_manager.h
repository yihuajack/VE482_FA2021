//
// Created by Yihua Liu on November 4, 2021.
//

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct argument_t {
    char *file_name;
    int data_type;
    int sorting_type;
};

typedef struct manager_t manager;
typedef struct argument_t argument;

typedef void(*hook_t)(argument *arg_list);
typedef int (*plugin_init)(manager *);

manager *new_manager();

void free_manager(manager *m);

void register_hook(manager *m, char *file_type, hook_t hook);

bool apply_hook(manager *m, argument *arg_list, char *file_type);

#endif // PLUGIN_MANAGER_H

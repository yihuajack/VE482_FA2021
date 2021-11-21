//
// Created by Yihua Liu on November 4, 2021.
//

#include <stdio.h>
#include "util.h"
#include "plugin_manager.h"

void dl_text(argument *arg_list){
    h3(arg_list->file_name, arg_list->data_type, arg_list->sorting_type);
}

void dl_text_init(manager *m){
    // EXPOSING
    register_hook(m, "txt", dl_text);
    fprintf(stderr, "dl_text: Registration Succeed!\n");
}

#include "h3.h"
#include "interface.h"

int cli(int argc, char *argv[]) {
    if (argc != 3)
        fprintf(stderr, "error: invalid arguments\n");
    else
        h3(argv[1], get_datatype(argv[1]), get_sortingtype(argv[2]));
    return 0;
}

int menui(int argc, char *argv[]) {
    printf("Welcome to the Menu Interface written by Yihua Liu for the second time!\n");
    int c, datatype, sortingtype;
    while (1) {
        if (argc == 3) {
            printf("Do you want to proceed with arguments %s and %s? [y/n]\n", argv[1], argv[2]);
            c = getc(stdin);
            while (c) {
                if (c == 'y') {
                    h3(argv[1], get_datatype(argv[1]), get_sortingtype(argv[2]));
                    printf("Sorting completed. Do you want to proceed or exit? Please input 0 or 1\n");
                    printf("0. Proceed\n");
                    printf("1. Exit\n");
                    c = getc(stdin);
                    if (c == '1')
                        return 0;
                    else if (c != '0')
                        printf("Invalid option, continuing...\n");
                    break;
                } else if (c == 'n') {
                    break;
                } else {
                    printf("Invalid option. Please input y or n> \n");
                    c = getc(stdin);
                }
            }
        } else if (argc != 1) {
            printf("Invalid arguments, continuing...\n");
        }
        printf("Which file do you want to process? Please input 0, 1, or 2\n");
        printf("0. rand_char*.txt\n");
        printf("1. rand_int.txt\n");
        printf("2. rand_double.txt\n");
        printf("> ");
        c = getc(stdin);
        while (c) {
            if (c == '0' || c == '1' || c == '2') {
                datatype = c - 48;
            } else {
                printf("Invalid option. Please input 0, 1, or 2\n> ");
                c = getc(stdin);
            }
        }
        printf("How do you want to sort your file? Please input 0, 1, or 2\n");
        printf("0. random\n");
        printf("1. increasing\n");
        printf("2. decreasing\n");
        printf("> ");
        c = getc(stdin);
        while (c) {
            if (c == '0' || c == '1' || c == '2') {
                sortingtype = c - 48;
            } else {
                printf("Invalid option. Please input 0, 1, or 2> \n");
                c = getc(stdin);
            }
        }
        switch (datatype) {
            case 0:
                h3("rand_char*.txt", datatype, sortingtype);
                break;
            case 1:
                h3("rand_int.txt", datatype, sortingtype);
                break;
            case 2:
                h3("rand_double.txt", datatype, sortingtype);
                break;
            default:
                break;
        }
        printf("Sorting completed. Do you want to proceed or exit? Please input 0 or 1\n");
        printf("0. Proceed\n");
        printf("1. Exit\n");
        c = getc(stdin);
        while (c) {
            if (c == '0')
                continue;
            else if (c == '1')
                return 0;
            else
                printf("Invalid option. Please input 0, 1, or 2\n");
        }
    }
}

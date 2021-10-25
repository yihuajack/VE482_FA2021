#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* RAND_MAX defined in /usr/include/stdlib.h:86:9:
 * #define RAND_MAX 2147483647  */
#define RAND_MAX_TEST 10000000

int main() {
    srand((unsigned int)time(NULL));
    FILE *fp_string = fopen("rand_string.txt", "w");
    FILE *fp_int = fopen("rand_int.txt", "w");
    FILE *fp_double = fopen("rand_double.txt", "w");
    for (int i = 0; i < 10000; i++) {
        fprintf(fp_string, "%d=", i);
        unsigned int len_str = rand() % 10 + 10;
        // For simplicity, assume all words are lower case.
        for (unsigned int j = 0; j < len_str; j++) {
            fprintf(fp_string, "%c", 97 + rand() % 26);
        }
        fprintf(fp_string, "\n");
        fprintf(fp_int, "%d=%d\n", i, (int)((rand() % RAND_MAX_TEST - RAND_MAX_TEST / 2)));
        fprintf(fp_double, "%d=%lf\n", i, (double)((rand() % RAND_MAX_TEST - RAND_MAX_TEST / 2)) / 1000000);
    }
    fclose(fp_string);
    fclose(fp_int);
    fclose(fp_double);
    return 0;
}

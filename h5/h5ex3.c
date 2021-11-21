#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* error: conflicting types for 'vprintf'
void vprintf(int *vector, unsigned int size) {
     ^
/usr/include/x86_64-linux-gnu/bits/stdio.h:39:1: note: previous definition is here  */
void vecprintf(int *vector, unsigned int size) {
    for (unsigned int i = 0; i < size; i++)
        printf("% d", vector[i]);
    printf("\n");
}

void mprintf(int **matrix, unsigned int n_row, unsigned int n_col) {
    for (unsigned int i = 0; i < n_row; i++) {
        for (unsigned int j = 0; j < n_col; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

int main() {
    unsigned int i, j, k, n_res, n_proc, i_seq = 0;
    printf("The number of resource types: ");
    scanf("%d", &n_res);
    printf("The number of available resources of each type as a vector (separate by space): ");
    int *r_avail = (int *) malloc(sizeof(int) * n_res);
    for (i = 0; i < n_res; i++) {
        scanf("%d", &r_avail[i]);
    }
    printf("The number of processes: ");
    scanf("%d", &n_proc);
    printf("The maximum demand of each process as a matrix (row by process, column by resource):\n");
    int **m_max = (int **) malloc(sizeof(int*) * n_proc);
    for (i = 0; i < n_proc; i++) {
        m_max[i] = (int *) malloc(sizeof(int) * n_res);
        for (j = 0; j < n_res; j++) {
            scanf("%d", &m_max[i][j]);
        }
    }
    printf("The number of resources of each type currently allocated to each process as a matrix (row by process, column by resource):\n");
    int **m_alloc = (int **) malloc(sizeof(int*) * n_proc), **m_need = (int **) malloc(sizeof(int*) * n_proc);
    for (i = 0; i < n_proc; i++) {
        m_alloc[i] = (int *) malloc(sizeof(int) * n_res);
        m_need[i] = (int *) malloc(sizeof(int) * n_res);
        for (j = 0; j < n_res; j++) {
            scanf("%d", &m_alloc[i][j]);
            m_need[i][j] = m_max[i][j] - m_alloc[i][j];
        }
    }
    printf("The remaining resource need of each process as a matrix (row by process, column by resource):\n");
    mprintf(m_need, n_proc, n_res);
    bool f_enough, *p_finished = (bool *) calloc(n_proc, sizeof(bool));
    unsigned int *seq_safe = (unsigned int *) calloc(n_proc, sizeof(unsigned int));
    for (k = 0; k < n_proc; k++) {
        for (i = 0; i < n_proc; i++) {
            if (p_finished[i] == false) {
                f_enough = true;
                for (j = 0; j < n_res; j++) {
                    if (m_need[i][j] > r_avail[j]){
                        f_enough = false;
                        break;
                    }
                }
                if (f_enough == true) {
                    printf("Execute Process (process number starting from 1): %d\n", i + 1);
                    seq_safe[i_seq++] = i + 1;
                    for (j = 0; j < n_res; j++)
                        r_avail[j] += m_alloc[i][j];
                    printf("Current number of available resources of each type:");
                    vecprintf(r_avail, n_res);
                    p_finished[i] = true;
                }
            }
        }
    }
    if (!seq_safe[0]) {
        printf("The processes cannot be completed without the system being in an unsafe state at any stage.\n");
    } else {
        printf("One possible sequence of processes that the system is in a safe state (process number starting from 1):\n");
        for (i = 0; i < n_proc - 1; i++)
            printf("P%d -> ", seq_safe[i]);
        printf("P%d\n", seq_safe[n_proc - 1]);
    }
    free(seq_safe);
    free(p_finished);
    for (i = 0; i < n_proc; i++) {
        free(m_need[i]);
        free(m_alloc[i]);
        free(m_max[i]);
    }
    free(m_need);
    free(m_alloc);
    free(m_max);
    free(r_avail);
    return 0;
}

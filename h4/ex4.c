#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000

int count = 0;

void *thread_count(void *a) {
    int i, tmp;
    sem_t *sem = a;
    for (i = 0; i < N; i++) {
        sem_wait(&sem);
        tmp = count;
        tmp = tmp + 1;
        count = tmp;
        sem_post(&sem);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int i;
    sem_t sem;
    pthread_t *t = (pthread_t *) malloc(sizeof(pthread_t) * 2);
    if (sem_init(&sem, 0, 1)) {
        fprintf(stderr, "ERROR initializing semaphore\n");
        exit(1);
    }
    for (i = 0; i < 2; i++) {
        if (pthread_create(t + i, NULL, thread_count, &sem)) {
            fprintf(stderr, "ERROR creating thread %d\n", i);
            exit(1);
        }
    }
    for (i = 0; i < 2 ; i++) {
        if (pthread_join(*(t + i), NULL)) {
            fprintf(stderr, "ERROR joining thread\n");
            exit(1);
        }
    }
    if (count < 2 * N) printf("Count is %d, but should be %d\n", count, 2*N);
    else printf("Count is [%d]\n", count);
    pthread_exit(NULL);
    free(t);
    sem_destroy(&sem);
    return 0;
}

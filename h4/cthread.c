#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N 1000000
int count = 0;
void * thread_count(void *a) {
    int i, tmp;
    for(i = 0; i < N; i++) {
        tmp = count;
        tmp = tmp+1;
        count = tmp;
    }
}
int main(int argc, char * argv[]) {
    int i;
    pthread_t *t=malloc(2*sizeof(pthread_t));
    for(i=0;i<2;i++) {
    if(pthread_create(t+i, NULL, thread_count, NULL)) {
        fprintf(stderr,"ERROR creating thread %d\n", i);
        exit(1);
    }
    }
    for(i=0;i<2;i++) {
    if(pthread_join(*(t+i), NULL)) {
        fprintf(stderr,"ERROR joining thread\n");
        exit(1);
    }
    }
    if (count < 2 * N) printf("Count is %d, but should be %d\n", count, 2*N);
    else printf("Count is [%d]\n", count);
    pthread_exit(NULL);
    free(t);
}
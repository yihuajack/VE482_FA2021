#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
int deadline = 1;
int exitflag = 0;
pid_t pid = 0;
unsigned long counter = 0;
clock_t start;
int i = 0;
void sigint_cb(int signal)
{
    exitflag = 1;
    if (i!=10) // parent process only for timer purpose
        printf("pid:%d\t the %d most tickets \ttime:%d\n", getpid(), i, clock() - start);
}

int main(int argc, char *argv[])
{
    int nice_val=2;
    printf("Lottery testcase\n");
    start = clock();
    for (;i<10;i++){
    pid = fork();
    if (pid)
    {
        nice(deadline = nice_val);
        
    }else{
        printf("proc:%d\t the %d most tickets is created\n", getpid(),i);
        //child
        break;
    }
    }
    signal(SIGINT, sigint_cb);

    if (i==10){ 
        //parent
        sleep(5);      
        //signal(SIGINT,SIG_IGN);  
        kill(0,SIGINT);
    }
    while (!exitflag)
    {
        counter++;
    }


}

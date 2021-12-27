#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <lib.h>
#include <time.h>

void proc(int id)
{
  int loop;
  switch (id)
  {
  case 1:
    // child process 1, deadline = 15
    chddl(15);
    printf("proc1 started\n");
    sleep(1);
    break;
  case 2:
    // child process 2, deadline = 25
    chddl(25);
    printf("proc2 started\n");
    sleep(1);
    break;
  case 3:
    // child process 3, deadline = 35
    chddl(35);
    printf("proc3 started\n");
    sleep(1);
    break;
  case 4: 
    // child process 4, no deadline
    chddl(0);
    printf("proc4 started\n");

    break;
  }
  for (loop = 1; loop < 30; loop++)
  {
    // 0 - 5
    //4 1:0+15 2:0+25 3:0+35
    // 5 - 10
    //4 1:0+15 3:5+15 2:0+25
    // 10 - 15
    //4 3:5+15 2:0+25 1:10+30
    // 15 - 20
    //4 2:0+25 1:10+30 3:15+30
    // set deadline = 15 for child process 3 at loop = 5 (5s)
    if (id == 3 && loop == 5)
    {
      chddl(15);
      printf("Change proc3 deadline to 5+15s\n");
    }
    // set deadline = 30 for child process 1 at loop = 10 (10s)
    if (id == 1 && loop == 10)
    {
      chddl(30);
      printf("Change proc1 deadline to 10+30s\n");
    }
    // set deadline = 30 for child process 3 at loop = 15 
    if (id == 3 && loop == 15)
    {
      chddl(30);
      printf("Change proc3 deadline to 15+30s\n");
    }
    // sleep for 1s
    sleep(1);
    printf("prc%d tick %d\n", id, loop);
  }
  exit(0);
}
int main(void)
{
  // create five child process
  for (int i = 1; i < 5; i++)
  {
    if (fork() == 0)
    {
      proc(i);
    }
  }
  return 0;
}

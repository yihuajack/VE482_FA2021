# Banker's Algorithm

For the Banker's algorithm to work, it needs to know three things:

- How much of each resource each process could possibly request[MAX]
- How much of each resource each process is currently holding[ALLOCATED]
- How much of each resource the system currently has available[AVAILABLE]

Resources may be allocated to a process only if the amount of resources requested is less than or equal to the amount available; otherwise, the process waits until resources are available.

Some of the resources that are tracked in real systems are memory, semaphores and interface access.

The Banker's Algorithm derives its name from the fact that this algorithm could be used in a banking system to ensure that the bank does not run out of resources, because the bank would never allocate its money in such a way that it can no longer satisfy the needs of all its customers. By using the Banker's algorithm, the bank ensures that when customers request money the bank never leaves a safe state. If the customer's request does not cause the bank to leave a safe state, the cash will be allocated, otherwise the customer must wait until some other customer deposits enough.

Basic data structures to be maintained to implement the Banker's Algorithm:

Let $n$ be the number of processes in the system and $m$ be the number of resource types. Then we need the following data structures:

- Available: A vector of length $m$ indicates the number of available resources of each type. If Available[$j$] = $k$, there are $k$ instances of resource type $R_j$ available.
- Max: An $n\times m$ matrix defines the maximum demand of each process. If Max[$i,j$] = $k$, then $P_i$ may request at most $k$ instances of resource type $R_j$.
- Allocation: An $n\times m$ matrix defines the number of resources of each type currently allocated to each process. If Allocation[$i,j$] = $k$, then process $P_i$ is currently allocated $k$ instances of resource type $R_j$.
- Need: An $n\times m$ matrix indicates the remaining resource need of each process. If Need[$i,j$] = $k$, then $P_i$ may need $k$ more instances of resource type $R_j$ to complete the task.

Note: Need[$i,j$] = Max[$i,j$] - Allocation[$i,j$]. $n=m-a$.

Example:

```
Total system resources are:
A B C D
6 5 7 6
Available system resources are:
A B C D
3 1 1 2
Processes (currently allocated resources):
   A B C D
P1 1 2 2 1
P2 1 0 3 3
P3 1 2 1 0
Processes (maximum resources):
   A B C D
P1 3 3 2 2
P2 1 2 3 4
P3 1 3 5 0
Need = maximum resources - currently allocated resources
Processes (possibly needed resources):
   A B C D
P1 2 1 0 1
P2 0 2 0 1
P3 0 1 4 0
```

A state (as in the above example) is considered safe if it is possible for all processes to finish executing (terminate). Since the system cannot know when a process will terminate, or how many resources it will have requested by then, the system assumes that all processes will eventually attempt to acquire their stated maximum resources and terminate soon afterward. This is a reasonable assumption in most cases since the system is not particularly concerned with how long each process runs (at least not from a deadlock avoidance perspective). Also, if a process terminates without acquiring its maximum resource it only makes it easier on the system. A safe state is considered to be the decision maker if it's going to process ready queue.

Given that assumption, the algorithm determines if a state is **safe** by trying to find a hypothetical set of requests by the processes that would allow each to acquire its maximum resources and then terminate (returning its resources to the system). Any state where no such set exists is an **unsafe** state.

We can show that the state given in the previous example is a safe state by showing that it is possible for each process to acquire its maximum resources and then terminate.

1. P1 needs 2 A, 1 B and 1 D more resources, achieving its maximum
   - [available resource: <3 1 1 2> - <2 1 0 1> = <1 0 1 1>]
   - The system now still has 1 A, no B, 1 C and 1 D resource available
2. P1 terminates, returning 3 A, 3 B, 2 C and 2 D resources to the system
   - [available resource: <1 0 1 1> + <3 3 2 2> = <4 3 3 3>]
   - The system now has 4 A, 3 B, 3 C and 3 D resources available
3. P2 acquires 2 B and 1 D extra resources, then terminates, returning all its resources
   - [available resource: <4 3 3 3> - <0 2 0 1> + <1 2 3 4> = <5 3 6 6>]
   - The system now has 5 A, 3 B, 6 C and 6 D resources
4. P3 acquires 1 B and 4 C resources and terminates.
   - [available resource: <5 3 6 6> - <0 1 4 0> + <1 3 5 0> = <6 5 7 6>]
   - The system now has all resources: 6 A, 5 B, 7 C and 6 D
5. Because all processes were able to terminate, this state is safe

For an example of an unsafe state, consider what would happen if process 2 was holding 2 units of resource B at the beginning.

When the system receives a request for resources, it runs the Banker's algorithm to determine if it is safe to grant the request. The algorithm is fairly straightforward once the distinction between safe and unsafe states is understood.

1. Can the request be granted?
   - If not, the request is impossible and must either be denied or put on a waiting list
2. Assume that the request is granted
3. Is the new state safe?
   - If so grant the request
   - If not, either deny the request or put it on a waiting list

*Whether the system denies or postpones an impossible or unsafe request is a decision specific to the operating system.*

Starting in the same state as the previous example started in, assume process 1 requests 2 units of resource C.

1. There is not enough of resource C available to grant the request
2. The request is denied

On the other hand, assume process 3 requests 1 unit of resource C.

1. There are enough resources to grant the request
2. Assume the request is granted
   - The new state of the system would be:

```
    Available system resources
     A B C D
Free 3 1 0 2
    Processes (currently allocated resources):
     A B C D
P1   1 2 2 1
P2   1 0 3 3
P3   1 2 2 0
    Processes (maximum resources):
     A B C D
P1   3 3 2 2
P2   1 2 3 4
P3   1 3 5 0
```

1. Determine if this new state is safe
   1. P1 can acquire 2 A, 1 B and 1 D resources and terminate
   2. Then, P2 can acquire 2 B and 1 D resources and terminate
   3. Finally, P3 can acquire 1 B and 3 C resources and terminate
   4. Therefore, this new state is safe
2. Since the new state is safe, grant the request


Final example: from the state we started at, assume that process 2 requests 1 unit of resource B.

1. There are enough resources
2. Assuming the request is granted, the new state would be:

```
    Available system resources:
     A B C D
Free 3 0 1 2
    Processes (currently allocated resources):
     A B C D
P1   1 2 5 1
P2   1 1 3 3
P3   1 2 1 0
    Processes (maximum resources):
     A B C D
P1   3 3 2 2
P2   1 2 3 4
P3   1 3 5 0
```

1. Is this state safe? Assuming P1, P2, and P3 request more of resource B and C.
   - P1 is unable to acquire enough B resources
   - P2 is unable to acquire enough B resources
   - P3 is unable to acquire enough B resources
   - No process can acquire enough resources to terminate, so this state is not safe
2. Since the state is unsafe, deny the request

```python
import numpy as np

n_processes = int(input('Number of processes? '))
n_resources = int(input('Number of resources? '))

available_resources = [int(x) for x in input('Claim vector? ').split(' ')]

currently_allocated = np.array([[int(x) for x in input('Currently allocated for process ' + str(i + 1) + '? ').split(' ')] for i in range(n_processes)])
max_demand = np.array([[int(x) for x in input('Maximum demand from process ' + str(i + 1) + '? ').split(' ')] for i in range(n_processes)])

total_available = available_resources - np.sum(currently_allocated, axis=0)

running = np.ones(n_processes)  # An array with n_processes 1's to indicate if process is yet to run

while np.count_nonzero(running) > 0:
    at_least_one_allocated = False
    for p in range(n_processes):
        if running[p]:
            if all(i >= 0 for i in total_available - (max_demand[p] - currently_allocated[p])):
                at_least_one_allocated = True
                print(str(p) + ' is running')
                running[p] = 0
                total_available += currently_allocated[p]
    if not at_least_one_allocated:
        print('Unsafe')
        exit()
                
print('Safe')
```

**Safety Algorithm**
The algorithm for finding out whether or not a system is in a safe state can be described as follows: 


> 1) Let Work and Finish be vectors of length ‘m’ and ‘n’ respectively. 
>    Initialize: Work = Available 
>    Finish[i] = false; for i=1, 2, 3, 4….n
> 2) Find an i such that both 
>    a) Finish[i] = false 
>    b) $Need_i <= Work$
>    if no such i exists goto step (4)
> 3) Work = Work + Allocation[i]
>    Finish[i] = true 
>    goto step (2)
> 4) if Finish [i] = true for all i 
>    then the system is in a safe state 

**Resource-Request Algorithm**
Let $Request_i$ be the request array for process $P_i$. $Request_i [j] = k$ means process $P_i$ wants $k$ instances of resource type $R_j$. When a request for resources is made by process $P_i$, the following actions are taken:


> 1) If $Request_i <= Need_i$
>    Goto step (2) ; otherwise, raise an error condition, since the process has exceeded its maximum claim.
> 2) If $Request_i <= Available$
>    Goto step (3); otherwise, $P_i$ must wait, since the resources are not available.
> 3) Have the system pretend to have allocated the requested resources to process $P_i$ by modifying the state as 
>    follows: 
>    $Available = Available – Request_i$
>    $Allocation_i = Allocation_i + Request_i$
>    $Need_i = Need_i– Request_i$

```c
// Banker's Algorithm
#include <stdio.h>
int main()
{
    // P0, P1, P2, P3, P4 are the Process names here
  
    int n, m, i, j, k;
    n = 5; // Number of processes
    m = 3; // Number of resources
    int alloc[5][3] = { { 0, 1, 0 }, // P0    // Allocation Matrix
                        { 2, 0, 0 }, // P1
                        { 3, 0, 2 }, // P2
                        { 2, 1, 1 }, // P3
                        { 0, 0, 2 } }; // P4
  
    int max[5][3] = { { 7, 5, 3 }, // P0    // MAX Matrix
                      { 3, 2, 2 }, // P1
                      { 9, 0, 2 }, // P2
                      { 2, 2, 2 }, // P3
                      { 4, 3, 3 } }; // P4
  
    int avail[3] = { 3, 3, 2 }; // Available Resources
  
    int f[n], ans[n], ind = 0;
    for (k = 0; k < n; k++) {
        f[k] = 0;
    }
    int need[n][m];
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    }
    int y = 0;
    for (k = 0; k < 5; k++) {
        for (i = 0; i < n; i++) {
            if (f[i] == 0) {
  
                int flag = 0;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]){
                        flag = 1;
                         break;
                    }
                }
  
                if (flag == 0) {
                    ans[ind++] = i;
                    for (y = 0; y < m; y++)
                        avail[y] += alloc[i][y];
                    f[i] = 1;
                }
            }
        }
    }
  
    printf("Following is the SAFE Sequence\n");
    for (i = 0; i < n - 1; i++)
        printf(" P%d ->", ans[i]);
    printf(" P%d", ans[n - 1]);
  
    return (0);
  
    // This code is contributed by Deep Baldha (CandyZack)
}
```

- Limitations of Banker Algorithm:

- As the processes enter the system, they must predict the maximum number of resources needed which is not impractical to determine.
- In this algorithm, the number of processes remain fixed which is not possible in interactive systems.
- This algorithm requires that there should be a fixed number of resources to allocate. If a device breaks and becomes suddenly unavailable the algorithm would not work.
- Overhead cost incurred by the algorithm can be high when there are many processes and resources because it has to be invoked for every processes.
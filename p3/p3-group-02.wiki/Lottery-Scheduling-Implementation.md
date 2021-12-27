# Lottery Scheduling Implementation

In our implementation of lottery scheduling algorithm, several parts are revised.

## Kernel Space

The major changes is done in `/usr/src/kernel/proc.c`. Two parts are changed here:
- `ticket_rand(unsigned int seed)`
- `pick_proc(void)`

### `ticket_rand`
A self-written random number generator `ticket_rand(unsigned int seed)` is implemented so that we can generate a lucky number for lottery scheduling algorithm. Basically, this random number generator has the same principle as the `rand()` function in C. The input parameter `seed` of this random number generator function is supposed to be the `lo` parameter of function `read_tsc(u32_t *hi, u32_t *lo)`.

### `pick_proc`
The original `pick_proc(void)` function is revised to support lottery scheduling. If the system environment parameter `sched_t` equals to 1, `pick_proc(void)` will choose the next process to run according to lottery scheduling algorithm. In that case, `pick_proc()` will first sum up the tickets of all the processes with priority greater than or equal to `USER_Q` (since the lottery algorithm should not block the system processes, we do not put the processes with priority smaller than `USER_Q` into the lottery pool). With this total number of tickets, a lucky number will be generated. Then the function will traverse the processes with priority greater than or equal to `USER_Q` again to see which one is the lucky process, pick it out and return.

Note that we define a macro `#define PRIO_2_TICKET(prio) MIN_USER_Q - prio + 1` to calculate the tickets for each process by using its priority. As a result, there is no need for us to add an extra member in `struct proc` under `/usr/src/kernel/proc.h`, needless to say the initialization for this member of each process. Similarly, we can also change the tickets number of each process by using the original `nice()` function, we do not have to implement an extra `nice()` system call for lottery scheduling algorithm.